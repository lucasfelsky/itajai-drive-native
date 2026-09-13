// -----------------------------------------------------------------------------
// 5.0 Horizon-style activity/progression foundation.
// Lightweight route events, speed trap, drift zone, credits/reputation and a
// persistent local profile. No multiplayer dependency.
// -----------------------------------------------------------------------------
#define EVENT500_COUNT 5
#define EVENT500_VK_F4 0x73
#define EVENT500_MAGIC 0x35545645u
#define EVENT500_VERSION 1u
typedef struct{const char*name;double slat,slon,flat,flon;float targetSpeed;int reward;}Event500Def;
static const Event500Def g_event500_defs[EVENT500_COUNT]={
 {"BEIRA-RIO SPRINT",-26.9105556,-48.6536111,-26.92051,-48.65538,16.0f,3200},
 {"CENTRO -> SAO VICENTE",-26.9074694,-48.6619194,-26.90776,-48.67914,15.0f,4200},
 {"PORTO RUN",-26.9063889,-48.6541667,-26.88489,-48.69623,17.0f,5200},
 {"ATALAIA COAST RUN",-26.92051,-48.65538,-26.9244,-48.6416,14.0f,3600},
 {"INTERIOR CHALLENGE",-26.90776,-48.67914,-26.87810,-48.74889,18.0f,6800}
};
typedef struct{u32 magic,version;int credits,rep;float best[EVENT500_COUNT];float speedBest,driftBest;}Event500Save;
static Event500Save g_event500_save;
static char g_event500_path[640];static int g_event500_loaded=0,g_event500_active=-1,g_event500_near=-1,g_event500_last_result=0;
static float g_event500_time=0,g_event500_target=0,g_event500_route_length=0,g_event500_result_timer=0;
static float g_speed500_cooldown=0,g_drift500_score=0,g_drift500_live=0;static int g_drift500_inside=0;
static V3 g_event500_speedtrap,g_event500_drift_a,g_event500_drift_b;
static void event500_save_now(void){HANDLE f=CreateFileA(g_event500_path,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);if(f==INVALID_HANDLE_VALUE)return;DWORD n=0;WriteFile(f,&g_event500_save,sizeof(g_event500_save),&n,NULL);CloseHandle(f);}
static void event500_init(void){if(g_event500_loaded)return;g_event500_loaded=1;sprintf(g_event500_path,"%s\\itajai_profile_v500.bin",g_exe_dir);memset(&g_event500_save,0,sizeof(g_event500_save));g_event500_save.magic=EVENT500_MAGIC;g_event500_save.version=EVENT500_VERSION;g_event500_save.credits=12000;HANDLE f=CreateFileA(g_event500_path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);if(f!=INVALID_HANDLE_VALUE){Event500Save s;DWORD n=0;if(ReadFile(f,&s,sizeof(s),&n,NULL)&&n==sizeof(s)&&s.magic==EVENT500_MAGIC&&s.version==EVENT500_VERSION)g_event500_save=s;CloseHandle(f);}g_event500_speedtrap=project_geo(-26.9117,-48.6545);g_event500_drift_a=project_geo(-26.9230,-48.6438);g_event500_drift_b=project_geo(-26.9258,-48.6400);}
static float event500_dist(V3 a,V3 b){float x=a.x-b.x,z=a.z-b.z;return (float)sqrt((double)(x*x+z*z));}
static int event500_start(int id){if(id<0||id>=EVENT500_COUNT)return 0;V3 goalP=project_geo(g_event500_defs[id].flat,g_event500_defs[id].flon);int start=nearest_node(g_player.body.p),goal=nearest_node(goalP);if(start<0||goal<0)return 0;int n=astar_route(start,goal,g_gps_route,MAX_ROUTE);if(n<2)return 0;g_gps_n=n;g_gps_goal07=goal;g_gps_offroute07=0;g_gps_recalc_timer07=.15f;g_gps_length07=sim07_route_length(g_gps_route,g_gps_n);g_event500_route_length=g_gps_length07;g_event500_target=maxf(35.0f,g_event500_route_length/maxf(8.0f,g_event500_defs[id].targetSpeed));g_event500_time=0;g_event500_active=id;g_event500_last_result=0;g_event500_result_timer=0;return 1;}
static void event500_finish(void){int id=g_event500_active;if(id<0)return;float t=g_event500_time;float old=g_event500_save.best[id];int personal=old<=0||t<old;int reward=g_event500_defs[id].reward;if(t<=g_event500_target)reward+=(int)(g_event500_defs[id].reward*.55f);if(personal)reward+=(int)(g_event500_defs[id].reward*.25f);g_event500_save.credits+=reward;g_event500_save.rep+=maxf(100,(float)(reward/8));if(personal)g_event500_save.best[id]=t;event500_save_now();g_event500_last_result=reward;g_event500_result_timer=7.0f;g_event500_active=-1;g_gps_n=0;g_gps_goal07=-1;}
static void event500_abort(void){g_event500_active=-1;g_event500_time=0;g_gps_n=0;g_gps_goal07=-1;g_event500_last_result=-1;g_event500_result_timer=3.0f;}
static void event500_update_activities(float dt){g_event500_near=-1;float best=1e30f;for(int i=0;i<EVENT500_COUNT;i++){V3 p=project_geo(g_event500_defs[i].slat,g_event500_defs[i].slon);float d=event500_dist(g_player.body.p,p);if(d<best){best=d;if(d<42.0f)g_event500_near=i;}}if(key_tap(EVENT500_VK_F4)&&!g_map380_open&&!g_settings_menu_open&&!g_garage215_open){if(g_event500_active>=0)event500_abort();else if(g_event500_near>=0)event500_start(g_event500_near);}
 if(g_event500_active>=0){g_event500_time+=dt;V3 f=project_geo(g_event500_defs[g_event500_active].flat,g_event500_defs[g_event500_active].flon);if(event500_dist(g_player.body.p,f)<19.0f)event500_finish();}
 if(g_event500_result_timer>0)g_event500_result_timer-=dt;if(g_speed500_cooldown>0)g_speed500_cooldown-=dt;float sd=event500_dist(g_player.body.p,g_event500_speedtrap);if(sd<13.0f&&g_speed500_cooldown<=0){float kph=absf(g_player.speed)*3.6f;if(kph>g_event500_save.speedBest){g_event500_save.speedBest=kph;g_event500_last_result=(int)kph;g_event500_result_timer=4.0f;event500_save_now();}g_speed500_cooldown=5.0f;}
 float dz=point_segment_distance_xz(g_player.body.p,g_event500_drift_a,g_event500_drift_b);int inside=dz<34.0f;if(inside&&absf(g_player.speed)>7.0f&&absf(g_player.slip)>.10f){g_drift500_score+=absf(g_player.slip)*absf(g_player.speed)*dt*185.0f;g_drift500_live=2.0f;}if(g_drift500_live>0)g_drift500_live-=dt;if(g_drift500_inside&&!inside&&g_drift500_score>25){if(g_drift500_score>g_event500_save.driftBest){g_event500_save.driftBest=g_drift500_score;event500_save_now();}g_event500_last_result=(int)g_drift500_score;g_event500_result_timer=4.0f;g_drift500_score=0;}if(!inside&&g_drift500_live<=0&&g_drift500_score>0)g_drift500_score=maxf(0,g_drift500_score-dt*25.0f);g_drift500_inside=inside;}
static void game_update(float dt){game_update_legacy500(dt);if(!g_started)return;event500_init();event500_update_activities(dt);}
