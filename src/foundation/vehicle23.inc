// -----------------------------------------------------------------------------
// 2.3 Foundation Pass: vehicle-sized collision hull + continuous static sweep.
//
// Legacy physics uses one center circle because it predates production-size cars.
// 2.3 keeps that circle as a cheap compatibility core, but adds a three-circle
// capsule-like hull whose dimensions match each playable vehicle. The same hull
// is used for exact OSM building narrow phase, signals and traffic nose/tail hits.
// -----------------------------------------------------------------------------
typedef struct { float width,length; } VehicleHull23;
static const VehicleHull23 g_vehicle_hull23[4]={
  {1.64f,3.81f}, // Uno Way
  {1.66f,3.90f}, // Gol G6
  {1.72f,4.02f}, // HB20
  {1.81f,4.23f}  // Renegade
};

static int g_vehicle23_substeps=1;
static int g_vehicle23_static_hits=0;
static int g_vehicle23_sweep_hits=0;
static int g_vehicle23_dynamic_hits=0;
static float g_vehicle23_last_hull_radius=.75f;
static float g_vehicle23_last_hull_span=1.10f;

static float vehicle23_wrap(float a){while(a>3.14159265f)a-=6.2831853f;while(a<-3.14159265f)a+=6.2831853f;return a;}
static float vehicle23_radius_for(int car){const VehicleHull23*h=&g_vehicle_hull23[car&3];return h->width*.46f;}
static float vehicle23_span_for(int car){const VehicleHull23*h=&g_vehicle_hull23[car&3];float r=vehicle23_radius_for(car);return maxf(.18f,h->length*.5f-r-.05f);}

static void vehicle23_sync_body_radius(void){
  float r=vehicle23_radius_for(g_player.car);g_player.body.radius=r;g_vehicle23_last_hull_radius=r;g_vehicle23_last_hull_span=vehicle23_span_for(g_player.car);
}

static void vehicle23_circle_centers(V3 p,float yaw,V3 out[3]){
  float span=vehicle23_span_for(g_player.car);V3 f=v3((float)sin((double)yaw),0,(float)cos((double)yaw));
  out[0]=vadd(p,vmul(f,-span));out[1]=p;out[2]=vadd(p,vmul(f,span));
}

// Broad phase is one query around the whole vehicle. Narrow phase tests all three
// body circles and keeps the deepest contact so one stable correction is applied.
static int vehicle23_contact_pose(V3 p,float yaw,Contact*out,V3*leverOut){
  static int ids[FOUNDATION_QUERY_MAX];V3 circles[3];vehicle23_circle_centers(p,yaw,circles);float radius=vehicle23_radius_for(g_player.car);const VehicleHull23*h=&g_vehicle_hull23[g_player.car&3];
  int n=foundation_query_colliders(p,h->length*.56f+.8f,ids,FOUNDATION_QUERY_MAX),found=0;float best=-1.0f;Contact bestCt;V3 bestLever=v3(0,0,0);
  for(int i=0;i<n;i++){
    int id=ids[i];if(id<0||id>=g_static_collider_count)continue;StaticCollider*c=&g_static_colliders[id];
    for(int k=0;k<3;k++){
      Contact ct;int hit=-1;if(c->type==COLLIDER_BUILDING&&(g_building12_ready||c->source<=-100))hit=collision22_building_contact(c->source,circles[k],radius,&ct,id);if(hit<0)hit=circle_aabb_contact(circles[k],radius,c,&ct,id);if(!hit)continue;
      if(!found||ct.penetration>best){found=1;best=ct.penetration;bestCt=ct;bestLever=vsub(circles[k],p);}
    }
  }
  if(found){if(out)*out=bestCt;if(leverOut)*leverOut=bestLever;}return found;
}

static int vehicle23_resolve_static(void){
  int hits=0;for(int pass=0;pass<5;pass++){
    Contact ct;V3 lever;if(!vehicle23_contact_pose(g_player.body.p,g_player.body.yaw,&ct,&lever))break;
    g_player.body.p=vadd(g_player.body.p,vmul(ct.normal,ct.penetration+.003f));float vn=vdot(g_player.body.vel,ct.normal);
    if(vn<0){g_player.body.vel=vsub(g_player.body.vel,vmul(ct.normal,1.025f*vn));float arm=lever.x*ct.normal.z-lever.z*ct.normal.x;float kick=clampf(absf(vn)*.035f,0,.30f);g_player.body.angularVel+=arm*kick;}
    g_player.body.angularVel=clampf(g_player.body.angularVel,-2.7f,2.7f)*.88f;hits++;
  }
  if(hits){V3 f=v3((float)sin((double)g_player.body.yaw),0,(float)cos((double)g_player.body.yaw));V3 r=v3(f.z,0,-f.x);g_player.speed=vdot(g_player.body.vel,f);g_player.lateralSpeed=vdot(g_player.body.vel,r);g_last_static_contacts+=hits;g_foundation_contacts+=hits;}
  g_vehicle23_static_hits+=hits;return hits;
}

// Sample the travelled segment before accepting the final position. This is a
// lightweight continuous-collision guard for thin facades/signals at high speed.
static int vehicle23_sweep(V3 oldPos,float oldYaw,V3 targetPos,float targetYaw){
  V3 d=vsub(targetPos,oldPos);d.y=0;float dist=vlen(d);if(dist<.28f||dist>6.0f)return 0;int steps=1+(int)(dist/.34f);if(steps>10)steps=10;float dy=vehicle23_wrap(targetYaw-oldYaw);
  for(int s=1;s<=steps;s++){
    float t=(float)s/(float)steps;V3 p=vadd(oldPos,vmul(d,t));p.y=targetPos.y;float yaw=vehicle23_wrap(oldYaw+dy*t);Contact ct;V3 lever;
    if(vehicle23_contact_pose(p,yaw,&ct,&lever)){
      g_player.body.p=p;g_player.body.yaw=yaw;vehicle23_resolve_static();g_vehicle23_sweep_hits++;return 1;
    }
  }
  return 0;
}

// Legacy dynamic collision still handles center-to-center impacts. This catches
// contacts at the physical nose/tail that the old center circle cannot see yet.
static int vehicle23_resolve_traffic(void){
  V3 circles[3];vehicle23_circle_centers(g_player.body.p,g_player.body.yaw,circles);float pr=vehicle23_radius_for(g_player.car),tr=.92f;int hits=0;
  for(int i=0;i<g_traffic_count;i++){
    Traffic*t=&g_traffic[i];if(!t->active)continue;float bestPen=0;V3 bestN=v3(0,0,0);int found=0;
    for(int q=0;q<2;q++){
      int k=q?2:0;float dx=circles[k].x-t->p.x,dz=circles[k].z-t->p.z,minDist=pr+tr,ds=dx*dx+dz*dz;if(ds>=minDist*minDist)continue;float dd=(float)sqrt((double)maxf(ds,.0001f));float pen=minDist-dd;V3 n=dd>.01f?v3(dx/dd,0,dz/dd):v3(1,0,0);if(!found||pen>bestPen){found=1;bestPen=pen;bestN=n;}
    }
    if(!found)continue;
    float tm=1200.0f,total=maxf(1.0f,g_player.body.mass+tm);g_player.body.p=vadd(g_player.body.p,vmul(bestN,bestPen*(tm/total)+.003f));
    V3 tf=v3((float)sin((double)t->yaw),0,(float)cos((double)t->yaw)),tv=vmul(tf,t->speed),rel=vsub(g_player.body.vel,tv);float vn=vdot(rel,bestN);
    if(vn<0){float j=-(1.0f+.08f)*vn/(1.0f/g_player.body.mass+1.0f/tm);g_player.body.vel=vadd(g_player.body.vel,vmul(bestN,j/g_player.body.mass));float trafficImpulse=vdot(vmul(bestN,-j/tm),tf);t->speed=maxf(0,t->speed+trafficImpulse);}
    g_player.body.angularVel*=.86f;g_traffic_brake_amount[i]=1.0f;hits++;
  }
  if(hits){V3 f=v3((float)sin((double)g_player.body.yaw),0,(float)cos((double)g_player.body.yaw));g_player.speed=vdot(g_player.body.vel,f);g_last_dynamic_contacts+=hits;g_foundation_dynamic_contacts+=hits;}
  g_vehicle23_dynamic_hits+=hits;return hits;
}

static void vehicle23_post_step(V3 oldPos,float oldYaw){
  V3 target=g_player.body.p;float targetYaw=g_player.body.yaw;if(!vehicle23_sweep(oldPos,oldYaw,target,targetYaw))vehicle23_resolve_static();vehicle23_resolve_traffic();
}

// Debug overlay: cyan circles show the actual playable collision hull.
static void vehicle23_draw_debug(void){
  V3 c[3];vehicle23_circle_centers(g_player.body.p,g_player.body.yaw,c);float r=vehicle23_radius_for(g_player.car),y=g_player.body.p.y-.72f;glColor4f(.12f,1.0f,.88f,.90f);glLineWidth(2.0f);glBegin(GL_LINES);
  for(int q=0;q<3;q++)for(int k=0;k<20;k++){float a=(float)k*6.2831853f/20.0f,b=(float)(k+1)*6.2831853f/20.0f;glVertex3f(c[q].x+(float)cos((double)a)*r,y,c[q].z+(float)sin((double)a)*r);glVertex3f(c[q].x+(float)cos((double)b)*r,y,c[q].z+(float)sin((double)b)*r);}glEnd();g_draw_calls++;
}
