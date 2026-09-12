static void traffic_update(float dt,double time,V3 player){
  float speedSum=0;
  for(int i=0;i<g_traffic_count;i++){
    Traffic*t=&g_traffic[i];if(!t->active||t->routeN<=0)continue;if(g_traffic_cruise[i]<=0)g_traffic_cruise[i]=.72f+rng01()*.25f;
    int eid=t->route[t->ri];if(eid<0||eid>=g_world.edgeCount)continue;Edge*e=&g_world.edges[eid];g_traffic_lane[i]=edge_clamp_lane(eid,g_traffic_lane[i]);int lane=g_traffic_lane[i];float len=maxf(.1f,e->length);
    float target=minf(e->speed*g_traffic_cruise[i],16.5f);float remaining=(1.0f-t->t)*len;float stopDist=3.0f+t->speed*.85f+t->speed*t->speed/11.0f;
    if(signal_red_for_node(e->to,time)&&remaining<stopDist)target=minf(target,maxf(0,(remaining-1.8f)*.65f));
    float gap=traffic_gap_ahead(i,eid,lane,t->t);g_traffic_gap[i]=gap;float safe=4.5f+t->speed*1.35f;if(gap<safe){float factor=clampf((gap-1.8f)/maxf(1.0f,safe-1.8f),0,1);target=minf(target,target*factor);}
    V3 cur=edge_lane_position(eid,t->t,lane,.6f),ed=vsub(g_world.nodes[e->to].p,g_world.nodes[e->from].p),dir=vnorm(ed),right=v3(dir.z,0,-dir.x),toPlayer=vsub(player,cur);float ahead=vdot(toPlayer,dir),side=absf(vdot(toPlayer,right));if(ahead>0&&ahead<7.5f+t->speed*.9f&&side<2.4f)target=minf(target,maxf(0,(ahead-2.4f)*.7f));
    float oldSpeed=t->speed;float accel=target>t->speed?2.35f:5.8f;t->speed=sim_move_toward(t->speed,target,accel*dt);g_traffic_brake_amount[i]=clampf((oldSpeed-t->speed)/maxf(.01f,dt*5.0f),0,1);t->t+=t->speed*dt/len;
    while(t->t>=1.0f&&t->active){t->t-=1.0f;t->ri++;if(t->ri>=t->routeN){traffic_new_route(t,e->to);if(!t->active)break;t->ri=0;}if(t->active){eid=t->route[t->ri];if(eid<0||eid>=g_world.edgeCount){t->active=0;break;}e=&g_world.edges[eid];len=maxf(.1f,e->length);g_traffic_lane[i]=edge_clamp_lane(eid,g_traffic_lane[i]);lane=g_traffic_lane[i];}}
    if(!t->active)continue;eid=t->route[t->ri];lane=edge_clamp_lane(eid,g_traffic_lane[i]);t->p=edge_lane_position(eid,t->t,lane,.6f);float desiredYaw=edge_lane_yaw(eid),dy=desiredYaw-t->yaw;while(dy>3.14159265f)dy-=6.2831853f;while(dy<-3.14159265f)dy+=6.2831853f;t->yaw+=dy*clampf(dt*7.0f,0,1);speedSum+=t->speed;
  }
  g_traffic_avg_speed=g_traffic_count>0?speedSum/g_traffic_count:0;
}

// -----------------------------------------------------------------------------
// player/game state
// -----------------------------------------------------------------------------
typedef struct { PhysicsBody body; float speed,steer,lateralSpeed,slip,roll,pitch,rideHeight; int car,camera; int wheelSurface[4]; float wheelCompression[4]; } Player;
static Player g_player;
static int g_weather=0;
static int g_hour=17;
static int g_debug=0;
static double g_time=0;
static int g_started=0;
static int g_gps_route[MAX_ROUTE],g_gps_n=0;
static char g_nearest_road[64]="ITAJAI";
static float g_road_scan_timer=0;
static int g_key_prev[256];
static int g_last_static_contacts=0;
static int g_last_dynamic_contacts=0;
static int key_down(int vk){return (GetAsyncKeyState(vk)&0x8000)!=0;}
static int key_tap(int vk){int d=key_down(vk),r=d&&!g_key_prev[vk];g_key_prev[vk]=d;return r;}
static void update_key_edges(void){for(int i=0;i<256;i++){int d=key_down(i);if(!d)g_key_prev[i]=0;}}
static void player_apply_profile(void){const VehicleProfile*p=&g_vehicle_profiles[g_player.car];g_player.body.mass=p->mass;g_player.body.radius=g_player.car==3?1.12f:1.05f;g_player.body.restitution=.07f;}
static void player_reset(void){int n=nearest_node(v3(0,0,0));V3 p=n>=0?vadd(g_world.nodes[n].p,v3(0,1.0f,0)):v3(0,1,0);physics_body_init(&g_player.body,p,1.05f,g_vehicle_profiles[g_player.car].mass);player_apply_profile();g_player.body.yaw=0;g_player.speed=0;g_player.steer=0;g_player.lateralSpeed=0;g_player.slip=0;g_player.roll=0;g_player.pitch=0;g_player.rideHeight=0;for(int i=0;i<4;i++){g_player.wheelSurface[i]=SURFACE_ASPHALT;g_player.wheelCompression[i]=.5f;}}
static void gps_random(void){int s=nearest_node(g_player.body.p);if(s<0)return;int comp=g_world.nodes[s].component;for(int k=0;k<20;k++){int goal=random_node_component(comp);if(goal<0)continue;g_gps_n=astar_route(s,goal,g_gps_route,MAX_ROUTE);if(g_gps_n>4)return;}g_gps_n=0;}
static void update_nearest_road(float dt){g_road_scan_timer-=dt;if(g_road_scan_timer>0)return;g_road_scan_timer=.18f;int br=-1;float center=9999;foundation_nearest_road(g_player.body.p,&br,&center);if(br>=0&&g_world.roads[br].name[0])str_copy(g_nearest_road,g_world.roads[br].name,64);else str_copy(g_nearest_road,"ITAJAI",64);}
static void player_sample_wheels(float dt,float throttle,float brake,int handbrake){
  const VehicleProfile*vp=&g_vehicle_profiles[g_player.car];V3 f=v3((float)sin(g_player.body.yaw),0,(float)cos(g_player.body.yaw)),r=v3(f.z,0,-f.x);float hz=vp->wheelbase*.5f,hx=vp->track*.5f;V3 wp[4];wp[0]=vadd(vadd(g_player.body.p,vmul(f,hz)),vmul(r,-hx));wp[1]=vadd(vadd(g_player.body.p,vmul(f,hz)),vmul(r,hx));wp[2]=vadd(vadd(g_player.body.p,vmul(f,-hz)),vmul(r,-hx));wp[3]=vadd(vadd(g_player.body.p,vmul(f,-hz)),vmul(r,hx));float grip=0,h[4];int counts[3]={0,0,0};
  for(int i=0;i<4;i++){int road=-1;float edge=0;int s=foundation_surface_at(wp[i],&road,&edge);g_player.wheelSurface[i]=s;counts[s]++;grip+=sim_surface_grip(s);h[i]=sim_surface_height(s);float target=.50f+h[i]*2.5f;g_player.wheelCompression[i]=lerpf(g_player.wheelCompression[i],clampf(target,.20f,.82f),clampf(dt*9,0,1));}
  int surface=counts[SURFACE_ASPHALT]>=2?SURFACE_ASPHALT:counts[SURFACE_CURB]>=2?SURFACE_CURB:SURFACE_GRASS;g_player.body.surface=surface;g_player.body.grip=(grip*.25f)*vp->baseGrip;
  float speedNorm=clampf(absf(g_player.speed)/maxf(1,vp->maxSpeed),0,1);float left=(h[0]+h[2])*.5f,right=(h[1]+h[3])*.5f,front=(h[0]+h[1])*.5f,rear=(h[2]+h[3])*.5f;float targetRoll=-g_player.steer*speedNorm*.075f+(left-right)*.42f;float targetPitch=(brake*.035f-throttle*.018f)+(rear-front)*.35f;if(handbrake)targetRoll*=1.18f;g_player.roll=lerpf(g_player.roll,targetRoll,clampf(dt*5.5f,0,1));g_player.pitch=lerpf(g_player.pitch,targetPitch,clampf(dt*5.0f,0,1));g_player.rideHeight=lerpf(g_player.rideHeight,(h[0]+h[1]+h[2]+h[3])*.25f,clampf(dt*7,0,1));
}
static int player_traffic_collisions(void){
  int hits=0;float pr=g_player.body.radius;for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active)continue;float dx=g_player.body.p.x-t->p.x,dz=g_player.body.p.z-t->p.z;float minDist=pr+1.05f,ds=dx*dx+dz*dz;if(ds>=minDist*minDist)continue;float d=(float)sqrt((double)maxf(ds,.0001f));V3 n=d>.01f?v3(dx/d,0,dz/d):v3(1,0,0);float pen=minDist-d;float tm=1200.0f,total=maxf(1,g_player.body.mass+tm);g_player.body.p=vadd(g_player.body.p,vmul(n,pen*(tm/total)+.005f));t->p=vsub(t->p,vmul(n,pen*(g_player.body.mass/total)));
    V3 tf=v3((float)sin(t->yaw),0,(float)cos(t->yaw)),tv=vmul(tf,t->speed),rel=vsub(g_player.body.vel,tv);float vn=vdot(rel,n);if(vn<0){float j=-(1.0f+.12f)*vn/(1.0f/g_player.body.mass+1.0f/tm);g_player.body.vel=vadd(g_player.body.vel,vmul(n,j/g_player.body.mass));float trafficImpulse=vdot(vmul(n,-j/tm),tf);t->speed=maxf(0,t->speed+trafficImpulse);}g_traffic_brake_amount[i]=1;hits++;}
  V3 pf=v3((float)sin(g_player.body.yaw),0,(float)cos(g_player.body.yaw));g_player.speed=vdot(g_player.body.vel,pf);g_foundation_dynamic_contacts+=hits;return hits;
}
static void game_update(float dt){
  g_time+=dt;update_key_edges();if(key_tap(VK_ESCAPE))PostQuitMessage(0);if(!g_started)return;if(key_tap(VK_TAB))g_debug=!g_debug;if(key_tap('T'))g_hour=(g_hour+3)%24;if(key_tap('Y'))g_weather=(g_weather+1)%4;if(key_tap('C'))g_player.camera=(g_player.camera+1)%4;if(key_tap('E')){g_player.car=(g_player.car+1)%4;player_apply_profile();}if(key_tap('R'))player_reset();if(key_tap('M'))gps_random();if(key_tap('U')){DeleteFileA(g_cache_path);DeleteFileA(g_cache_bin_path);DeleteFileA(g_cache_bin_legacy_path);}
  float throttle=key_down('W')?1.0f:0,brake=key_down('S')?1.0f:0,steerInput=(key_down('A')?1.0f:0)-(key_down('D')?1.0f:0);int handbrake=key_down(VK_SPACE);g_player.steer=lerpf(g_player.steer,steerInput,clampf(dt*6.5f,0,1));player_sample_wheels(dt,throttle,brake,handbrake);const VehicleProfile*vp=&g_vehicle_profiles[g_player.car];
  V3 f=v3((float)sin(g_player.body.yaw),0,(float)cos(g_player.body.yaw)),right=v3(f.z,0,-f.x);float longV=vdot(g_player.body.vel,f),latV=vdot(g_player.body.vel,right);float powerMul=sim_surface_power(g_player.body.surface);float driveAccel=throttle*vp->engineAccel*powerMul;if(brake){if(longV>.45f)driveAccel-=vp->brakeAccel;else driveAccel-=5.8f*powerMul;}longV+=driveAccel*dt;float rollMul=g_player.body.surface==SURFACE_ASPHALT?1.0f:g_player.body.surface==SURFACE_CURB?1.75f:3.2f;float drag=vp->rolling*rollMul+vp->aero*longV*longV;longV=sim_move_toward(longV,0,drag*dt);if(handbrake)longV=sim_move_toward(longV,0,5.5f*dt);float surfaceMax=g_player.body.surface==SURFACE_GRASS?20.0f:g_player.body.surface==SURFACE_CURB?32.0f:vp->maxSpeed;longV=clampf(longV,-7.0f,minf(vp->maxSpeed,surfaceMax));
  g_player.body.vel=vadd(vmul(f,longV),vmul(right,latV));float steerFalloff=clampf(1.0f-absf(longV)/(vp->maxSpeed*1.8f),.48f,1.0f);float steerAngle=g_player.steer*vp->maxSteer*steerFalloff;float targetYawRate=(longV/vp->wheelbase)*steerAngle*g_player.body.grip;if(handbrake)targetYawRate*=1.28f;g_player.body.angularVel=lerpf(g_player.body.angularVel,targetYawRate,clampf(dt*(handbrake?5.0f:8.0f),0,1));g_player.body.yaw+=g_player.body.angularVel*dt;
  f=v3((float)sin(g_player.body.yaw),0,(float)cos(g_player.body.yaw));right=v3(f.z,0,-f.x);longV=vdot(g_player.body.vel,f);latV=vdot(g_player.body.vel,right);float lateralDamp=(handbrake?1.35f:7.4f)*g_player.body.grip/(1.0f+absf(longV)*.025f);latV*=clampf(1.0f-lateralDamp*dt,0,1);g_player.lateralSpeed=latV;g_player.slip=absf(longV)>.4f?clampf(latV/maxf(1.0f,absf(longV)),-1,1):0;g_player.body.vel=vadd(vmul(f,longV),vmul(right,latV));V3 old=g_player.body.p;g_player.body.p=vadd(g_player.body.p,vmul(g_player.body.vel,dt));g_player.body.p.y=1.0f+g_player.rideHeight;g_player.speed=longV;
  g_last_static_contacts=physics_resolve_static(&g_player.body,&g_player.speed);if(g_last_static_contacts){f=v3((float)sin(g_player.body.yaw),0,(float)cos(g_player.body.yaw));g_player.speed=vdot(g_player.body.vel,f);}traffic_update(dt,g_time,g_player.body.p);g_last_dynamic_contacts=player_traffic_collisions();if(g_last_dynamic_contacts)g_last_static_contacts+=physics_resolve_static(&g_player.body,&g_player.speed);if(dt>.00001f&&vlen(g_player.body.vel)<.001f)g_player.body.vel=vmul(vsub(g_player.body.p,old),1.0f/dt);update_nearest_road(dt);
}

// -----------------------------------------------------------------------------
// OpenGL renderer
// -----------------------------------------------------------------------------
static HWND g_hwnd;static HDC g_hdc;static HGLRC g_glrc;static int g_w=1280,g_h=720;static u32 g_font_base=0;
static void gl_text(float x,float y,const char* s,float r,float g,float b){if(!s||!s[0]||!g_font_base)return;glColor3f(r,g,b);glRasterPos2f(x,y);glListBase(g_font_base-32);glCallLists((int)strlen(s),GL_UNSIGNED_BYTE,s);}
static void draw_unit_box(void){glBegin(GL_QUADS);
  glVertex3f(-.5f,-.5f,.5f);glVertex3f(.5f,-.5f,.5f);glVertex3f(.5f,.5f,.5f);glVertex3f(-.5f,.5f,.5f);
  glVertex3f(.5f,-.5f,-.5f);glVertex3f(-.5f,-.5f,-.5f);glVertex3f(-.5f,.5f,-.5f);glVertex3f(.5f,.5f,-.5f);
  glVertex3f(-.5f,-.5f,-.5f);glVertex3f(-.5f,-.5f,.5f);glVertex3f(-.5f,.5f,.5f);glVertex3f(-.5f,.5f,-.5f);
  glVertex3f(.5f,-.5f,.5f);glVertex3f(.5f,-.5f,-.5f);glVertex3f(.5f,.5f,-.5f);glVertex3f(.5f,.5f,.5f);
  glVertex3f(-.5f,.5f,.5f);glVertex3f(.5f,.5f,.5f);glVertex3f(.5f,.5f,-.5f);glVertex3f(-.5f,.5f,-.5f);
  glVertex3f(-.5f,-.5f,-.5f);glVertex3f(.5f,-.5f,-.5f);glVertex3f(.5f,-.5f,.5f);glVertex3f(-.5f,-.5f,.5f);
 glEnd();g_draw_calls++;}
static void draw_box(V3 p,float sx,float sy,float sz,float yaw,float r,float g,float b){glPushMatrix();glTranslatef(p.x,p.y,p.z);glRotatef(yaw*57.29578f,0,1,0);glScalef(sx,sy,sz);glColor3f(r,g,b);draw_unit_box();glPopMatrix();}
