static void traffic_update(float dt,double time,V3 player){for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active||t->routeN<=0)continue;int eid=t->route[t->ri];if(eid<0||eid>=g_world.edgeCount)continue;Edge*e=&g_world.edges[eid];V3 a=g_world.nodes[e->from].p,b=g_world.nodes[e->to].p,d=vsub(b,a);float len=maxf(.1f,e->length);float target=minf(e->speed,10.5f);if(signal_red_for_node(e->to,time)&&t->t>.70f)target=0;V3 cur=vadd(a,vmul(d,t->t));float pd=vlen(vsub(cur,player));if(pd<7)target=0;t->speed=lerpf(t->speed,target,clampf(dt*2,0,1));t->t+=t->speed*dt/len;if(t->t>=1){t->t-=1;t->ri++;if(t->ri>=t->routeN){traffic_new_route(t,e->to);if(!t->active)continue;e=&g_world.edges[t->route[0]];a=g_world.nodes[e->from].p;b=g_world.nodes[e->to].p;d=vsub(b,a);}else{e=&g_world.edges[t->route[t->ri]];a=g_world.nodes[e->from].p;b=g_world.nodes[e->to].p;d=vsub(b,a);}}float dl=vlen(d);V3 right=dl>.01f?v3(d.z/dl,0,-d.x/dl):v3(1,0,0);float off=minf(1.9f,maxf(.65f,e->width*.17f));t->p=vadd(vadd(a,vmul(d,t->t)),vmul(right,off));t->p.y=.6f;t->yaw=(float)atan2((double)d.x,(double)d.z);}}

// -----------------------------------------------------------------------------
// player/game state
// -----------------------------------------------------------------------------
typedef struct { PhysicsBody body; float speed,steer; int car,camera; } Player;
static Player g_player;
static int g_weather=0; // clear/cloud/rain/heavy
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
static void player_reset(void){int n=nearest_node(v3(0,0,0));V3 p=n>=0?vadd(g_world.nodes[n].p,v3(0,1.0f,0)):v3(0,1,0);physics_body_init(&g_player.body,p,1.05f,1180.0f);g_player.body.yaw=0;g_player.speed=0;g_player.steer=0;}
static void gps_random(void){int s=nearest_node(g_player.body.p);if(s<0)return;int comp=g_world.nodes[s].component;for(int k=0;k<20;k++){int goal=random_node_component(comp);if(goal<0)continue;g_gps_n=astar_route(s,goal,g_gps_route,MAX_ROUTE);if(g_gps_n>4)return;}g_gps_n=0;}
static void update_nearest_road(float dt){g_road_scan_timer-=dt;if(g_road_scan_timer>0)return;g_road_scan_timer=.18f;int br=-1;float center=9999;foundation_nearest_road(g_player.body.p,&br,&center);if(br>=0&&g_world.roads[br].name[0])str_copy(g_nearest_road,g_world.roads[br].name,64);else str_copy(g_nearest_road,"ITAJAI",64);}
static int player_traffic_collisions(void){int hits=0;float pr=g_player.body.radius;for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active)continue;float dx=g_player.body.p.x-t->p.x,dz=g_player.body.p.z-t->p.z;float minDist=pr+1.05f,ds=dx*dx+dz*dz;if(ds>=minDist*minDist)continue;float d=(float)sqrt((double)maxf(ds,.0001f));V3 n=d>.01f?v3(dx/d,0,dz/d):v3(1,0,0);float pen=minDist-d;g_player.body.p=vadd(g_player.body.p,vmul(n,pen*.82f+.01f));t->p=vsub(t->p,vmul(n,pen*.18f));float fwdDot=absf(vdot(v3((float)sin(g_player.body.yaw),0,(float)cos(g_player.body.yaw)),n));g_player.speed*=clampf(.55f-fwdDot*.42f,.08f,.55f);t->speed*=.38f;hits++;}g_foundation_dynamic_contacts+=hits;return hits;}
static void game_update(float dt){g_time+=dt;update_key_edges();if(key_tap(VK_ESCAPE))PostQuitMessage(0);if(!g_started)return;if(key_tap(VK_TAB))g_debug=!g_debug;if(key_tap('T'))g_hour=(g_hour+3)%24;if(key_tap('Y'))g_weather=(g_weather+1)%4;if(key_tap('C'))g_player.camera=(g_player.camera+1)%4;if(key_tap('E'))g_player.car=(g_player.car+1)%4;if(key_tap('R'))player_reset();if(key_tap('M'))gps_random();if(key_tap('U')){DeleteFileA(g_cache_path);DeleteFileA(g_cache_bin_path);DeleteFileA(g_cache_bin_legacy_path);}float throttle=key_down('W')?1:0,brake=key_down('S')?1:0,target=(key_down('A')?1.0f:0)-(key_down('D')?1.0f:0);g_player.steer=lerpf(g_player.steer,target,clampf(dt*5,0,1));
  int road=-1;float edgeDist=9999;int surface=foundation_surface_at(g_player.body.p,&road,&edgeDist);g_player.body.surface=surface;g_player.body.grip=surface==SURFACE_ASPHALT?1.0f:surface==SURFACE_CURB?.76f:.54f;
  float power[4]={7.2f,8.2f,9.5f,8.7f},maxs[4]={40,45,52,47};float powerMul=surface==SURFACE_ASPHALT?1.0f:surface==SURFACE_CURB?.78f:.60f;float rolling=surface==SURFACE_ASPHALT?.70f:surface==SURFACE_CURB?1.55f:2.75f;float quad=surface==SURFACE_ASPHALT?.018f:surface==SURFACE_CURB?.026f:.038f;float drag=g_player.speed==0?0:(g_player.speed>0?1:-1)*(rolling+quad*g_player.speed*g_player.speed);float accel=throttle*power[g_player.car]*powerMul-brake*(g_player.speed>1?15:6)-drag;if(key_down(VK_SPACE))accel-=(g_player.speed>=0?1:-1)*16;g_player.speed+=accel*dt;float surfaceMax=surface==SURFACE_GRASS?22.0f:maxs[g_player.car];g_player.speed=clampf(g_player.speed,-7,minf(maxs[g_player.car],surfaceMax));if(absf(g_player.speed)<.04f&&!throttle&&!brake)g_player.speed=0;
  float steerScale=clampf(1-absf(g_player.speed)/70,.28f,1)*g_player.body.grip;float targetYawRate=g_player.steer*steerScale*g_player.speed*.030f;g_player.body.angularVel=lerpf(g_player.body.angularVel,targetYawRate,clampf(dt*8,0,1));g_player.body.yaw+=g_player.body.angularVel*dt;V3 f=v3((float)sin(g_player.body.yaw),0,(float)cos(g_player.body.yaw));V3 old=g_player.body.p;g_player.body.p.x+=f.x*g_player.speed*dt;g_player.body.p.z+=f.z*g_player.speed*dt;g_player.body.p.y=surface==SURFACE_CURB?1.04f:1.0f;if(dt>.00001f)g_player.body.vel=vmul(vsub(g_player.body.p,old),1.0f/dt);
  g_last_static_contacts=physics_resolve_static(&g_player.body,&g_player.speed);traffic_update(dt,g_time,g_player.body.p);g_last_dynamic_contacts=player_traffic_collisions();if(g_last_dynamic_contacts)g_last_static_contacts+=physics_resolve_static(&g_player.body,&g_player.speed);update_nearest_road(dt);}

// -----------------------------------------------------------------------------
// OpenGL renderer
// -----------------------------------------------------------------------------
static HWND g_hwnd;static HDC g_hdc;static HGLRC g_glrc;static int g_w=1280,g_h=720;static u32 g_font_base=0;static int g_draw_calls=0;
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
