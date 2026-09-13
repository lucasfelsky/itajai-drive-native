// -----------------------------------------------------------------------------
// 2.5 Collision & Physics World 2.0.
// Five-circle rounded-box hull approximates the full vehicle footprint much more
// closely than the 2.3 longitudinal capsule while keeping the broad-phase cheap.
// ----------------------------------------------------------------------------
static int g_vehicle25_static_hits=0,g_vehicle25_sweep_hits=0,g_vehicle25_dynamic_hits=0;
static float g_vehicle25_radius=.42f,g_vehicle25_half_x=.55f,g_vehicle25_half_z=1.35f;

static void vehicle25_centers(V3 p,float yaw,V3 out[5]){
  const VehicleHull23*h=&g_vehicle_hull23[g_player.car&3];float r=minf(h->width*.27f,.49f);float hx=maxf(.18f,h->width*.5f-r*.78f),hz=maxf(.32f,h->length*.5f-r*1.05f);
  V3 f=v3((float)sin((double)yaw),0,(float)cos((double)yaw)),rr=v3(f.z,0,-f.x);g_vehicle25_radius=r;g_vehicle25_half_x=hx;g_vehicle25_half_z=hz;
  out[0]=vadd(vadd(p,vmul(f,hz)),vmul(rr,-hx));out[1]=vadd(vadd(p,vmul(f,hz)),vmul(rr,hx));
  out[2]=p;out[3]=vadd(vadd(p,vmul(f,-hz)),vmul(rr,-hx));out[4]=vadd(vadd(p,vmul(f,-hz)),vmul(rr,hx));
}

static int vehicle25_contact_pose(V3 p,float yaw,Contact*out,V3*leverOut){
  static int ids[FOUNDATION_QUERY_MAX];V3 pts[5];vehicle25_centers(p,yaw,pts);const VehicleHull23*h=&g_vehicle_hull23[g_player.car&3];
  int n=foundation_query_colliders(p,h->length*.58f+1.0f,ids,FOUNDATION_QUERY_MAX),found=0;float best=-1;Contact bestCt;V3 bestLever=v3(0,0,0);
  for(int i=0;i<n;i++){int id=ids[i];if(id<0||id>=g_static_collider_count)continue;StaticCollider*c=&g_static_colliders[id];for(int k=0;k<5;k++){
    Contact ct;int hit=-1;if(c->type==COLLIDER_BUILDING&&(g_building12_ready||c->source<=-100))hit=collision22_building_contact(c->source,pts[k],g_vehicle25_radius,&ct,id);if(hit<0)hit=circle_aabb_contact(pts[k],g_vehicle25_radius,c,&ct,id);if(!hit)continue;
    if(!found||ct.penetration>best){found=1;best=ct.penetration;bestCt=ct;bestLever=vsub(pts[k],p);}
  }}
  if(found){if(out)*out=bestCt;if(leverOut)*leverOut=bestLever;}return found;
}

static int vehicle25_resolve_static(void){
  int hits=0;for(int pass=0;pass<6;pass++){Contact ct;V3 lever;if(!vehicle25_contact_pose(g_player.body.p,g_player.body.yaw,&ct,&lever))break;
    g_player.body.p=vadd(g_player.body.p,vmul(ct.normal,ct.penetration+.0025f));float vn=vdot(g_player.body.vel,ct.normal);
    if(vn<0){float restitution=.035f;g_player.body.vel=vsub(g_player.body.vel,vmul(ct.normal,(1.0f+restitution)*vn));float arm=lever.x*ct.normal.z-lever.z*ct.normal.x;float iz=maxf(520.0f,g_player.body.mass*(g_vehicle_hull23[g_player.car&3].length*g_vehicle_hull23[g_player.car&3].length+g_vehicle_hull23[g_player.car&3].width*g_vehicle_hull23[g_player.car&3].width)/12.0f);float impulse=absf(vn)*g_player.body.mass*.22f;g_player.body.angularVel+=clampf(arm*impulse/iz,-.45f,.45f);}
    g_player.body.angularVel=clampf(g_player.body.angularVel,-2.5f,2.5f)*.91f;hits++;}
  if(hits){V3 f=v3((float)sin((double)g_player.body.yaw),0,(float)cos((double)g_player.body.yaw)),r=v3(f.z,0,-f.x);g_player.speed=vdot(g_player.body.vel,f);g_player.lateralSpeed=vdot(g_player.body.vel,r);g_last_static_contacts+=hits;g_foundation_contacts+=hits;}g_vehicle25_static_hits+=hits;return hits;
}

static int vehicle25_sweep(V3 oldPos,float oldYaw,V3 targetPos,float targetYaw){
  V3 d=vsub(targetPos,oldPos);d.y=0;float dist=vlen(d);if(dist<.18f||dist>9.0f)return 0;int steps=1+(int)(dist/.24f);if(steps>24)steps=24;float dy=vehicle23_wrap(targetYaw-oldYaw);
  for(int s=1;s<=steps;s++){float t=(float)s/(float)steps;V3 p=vadd(oldPos,vmul(d,t));p.y=targetPos.y;float yaw=vehicle23_wrap(oldYaw+dy*t);Contact ct;V3 lever;if(vehicle25_contact_pose(p,yaw,&ct,&lever)){g_player.body.p=p;g_player.body.yaw=yaw;vehicle25_resolve_static();g_vehicle25_sweep_hits++;return 1;}}
  return 0;
}

static int vehicle25_resolve_traffic(void){
  V3 pts[5];vehicle25_centers(g_player.body.p,g_player.body.yaw,pts);int hits=0;float tr=.88f;
  for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active)continue;float best=0;V3 nbest=v3(0,0,0),lever=v3(0,0,0);int found=0;
    for(int k=0;k<5;k++){float dx=pts[k].x-t->p.x,dz=pts[k].z-t->p.z,minD=g_vehicle25_radius+tr,ds=dx*dx+dz*dz;if(ds>=minD*minD)continue;float d=(float)sqrt((double)maxf(ds,.0001f));float pen=minD-d;V3 n=d>.01f?v3(dx/d,0,dz/d):v3(1,0,0);if(!found||pen>best){found=1;best=pen;nbest=n;lever=vsub(pts[k],g_player.body.p);}}
    if(!found)continue;float tm=1200.0f,total=maxf(1.0f,g_player.body.mass+tm);g_player.body.p=vadd(g_player.body.p,vmul(nbest,best*(tm/total)+.002f));
    V3 tf=v3((float)sin((double)t->yaw),0,(float)cos((double)t->yaw)),tv=vmul(tf,t->speed),rel=vsub(g_player.body.vel,tv);float vn=vdot(rel,nbest);
    if(vn<0){float j=-(1.0f+.06f)*vn/(1.0f/g_player.body.mass+1.0f/tm);g_player.body.vel=vadd(g_player.body.vel,vmul(nbest,j/g_player.body.mass));float iz=maxf(520.0f,g_player.body.mass*1.3f);float arm=lever.x*nbest.z-lever.z*nbest.x;g_player.body.angularVel+=clampf(arm*j/iz,-.34f,.34f);float ti=vdot(vmul(nbest,-j/tm),tf);t->speed=maxf(0,t->speed+ti);}g_traffic_brake_amount[i]=1.0f;hits++;}
  if(hits){V3 f=v3((float)sin((double)g_player.body.yaw),0,(float)cos((double)g_player.body.yaw));g_player.speed=vdot(g_player.body.vel,f);g_last_dynamic_contacts+=hits;g_foundation_dynamic_contacts+=hits;}g_vehicle25_dynamic_hits+=hits;return hits;
}

static void vehicle25_post(V3 oldPos,float oldYaw){V3 target=g_player.body.p;float targetYaw=g_player.body.yaw;if(!vehicle25_sweep(oldPos,oldYaw,target,targetYaw))vehicle25_resolve_static();vehicle25_resolve_traffic();}

static void vehicle25_draw_debug(void){V3 c[5];vehicle25_centers(g_player.body.p,g_player.body.yaw,c);float y=g_player.body.p.y-.70f;glColor4f(1.0f,.46f,.10f,.92f);glLineWidth(2.0f);glBegin(GL_LINES);for(int q=0;q<5;q++)for(int k=0;k<18;k++){float a=(float)k*6.2831853f/18.0f,b=(float)(k+1)*6.2831853f/18.0f;glVertex3f(c[q].x+(float)cos((double)a)*g_vehicle25_radius,y,c[q].z+(float)sin((double)a)*g_vehicle25_radius);glVertex3f(c[q].x+(float)cos((double)b)*g_vehicle25_radius,y,c[q].z+(float)sin((double)b)*g_vehicle25_radius);}glEnd();g_draw_calls++;}
