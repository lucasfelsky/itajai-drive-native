// -----------------------------------------------------------------------------
// 4.0 Single-player polish gameplay layer.
// - GPS 2.0 telemetry: current road, next road/turn, remaining distance and ETA.
// - Conservative forward handbrake settling (reverse 3.0.1 behavior untouched).
// - Road-class-aware traffic cruise discipline.
// - Lightweight Drive Hub state for the production HUD.
// -----------------------------------------------------------------------------
#define POLISH400_VK_F2 0x71
static char g_nav400_current_road[64]="";
static char g_nav400_next_road[64]="";
static char g_nav400_turn[24]="";
static float g_nav400_remaining=0.0f,g_nav400_eta=0.0f,g_nav400_next_distance=0.0f;
static int g_nav400_route_index=-1,g_nav400_active=0;
static float g_nav400_timer=0.0f;
static int g_drivehub400_open=0;
static int g_drive400_handbrake_settles=0;
static int g_traffic400_capped=0;

static void polish400_copy_road_name(char*out,int road,const char*fallback){
  if(road>=0&&road<g_world.roadCount&&g_world.roads[road].name[0])str_copy(out,g_world.roads[road].name,64);else str_copy(out,fallback,64);
}
static int polish400_nearest_route_index(void){
  if(g_gps_n<=0)return -1;float best=1e30f;int bi=-1;
  // Full-route scan is intentionally throttled by g_nav400_timer. Even a 9k-edge
  // municipality-crossing route is cheap at 5 Hz and keeps the HUD exact.
  for(int i=0;i<g_gps_n;i++){int eid=g_gps_route[i];if(eid<0||eid>=g_world.edgeCount)continue;Edge*e=&g_world.edges[eid];V3 a=g_world.nodes[e->from].p,b=g_world.nodes[e->to].p;float d=point_segment_distance_xz(g_player.body.p,a,b);if(d<best){best=d;bi=i;}}
  return bi;
}
static void polish400_navigation(void){
  g_nav400_active=0;g_nav400_route_index=-1;g_nav400_remaining=g_nav400_eta=g_nav400_next_distance=0;g_nav400_current_road[0]=g_nav400_next_road[0]=g_nav400_turn[0]=0;
  if(g_gps_n<=0)return;int idx=polish400_nearest_route_index();if(idx<0)return;g_nav400_route_index=idx;g_nav400_active=1;
  int eid=g_gps_route[idx];if(eid<0||eid>=g_world.edgeCount)return;Edge*cur=&g_world.edges[eid];int currentRoad=cur->road;polish400_copy_road_name(g_nav400_current_road,currentRoad,"VIA SEM NOME");
  V3 curEnd=g_world.nodes[cur->to].p;float first=vlen(vsub(curEnd,g_player.body.p));g_nav400_remaining=first;g_nav400_eta=first/maxf(4.0f,cur->speed);
  int nextIdx=-1;float beforeNext=first;
  for(int i=idx+1;i<g_gps_n;i++){int qid=g_gps_route[i];if(qid<0||qid>=g_world.edgeCount)continue;Edge*q=&g_world.edges[qid];if(nextIdx<0&&q->road!=currentRoad){nextIdx=i;g_nav400_next_distance=beforeNext;}g_nav400_remaining+=q->length;g_nav400_eta+=q->length/maxf(4.0f,q->speed);if(nextIdx<0)beforeNext+=q->length;}
  if(nextIdx<0){str_copy(g_nav400_next_road,"DESTINO",64);str_copy(g_nav400_turn,"SIGA",24);g_nav400_next_distance=g_nav400_remaining;return;}
  int neid=g_gps_route[nextIdx];Edge*ne=&g_world.edges[neid];polish400_copy_road_name(g_nav400_next_road,ne->road,"PROXIMA VIA");
  V3 a=vnorm(vsub(g_world.nodes[cur->to].p,g_world.nodes[cur->from].p));V3 b=vnorm(vsub(g_world.nodes[ne->to].p,g_world.nodes[ne->from].p));float dot=clampf(a.x*b.x+a.z*b.z,-1,1),cross=a.z*b.x-a.x*b.z;
  if(dot>.86f)str_copy(g_nav400_turn,"SIGA",24);else if(dot<-.58f)str_copy(g_nav400_turn,"RETORNO",24);else if(cross>.10f)str_copy(g_nav400_turn,"ESQUERDA",24);else if(cross<-.10f)str_copy(g_nav400_turn,"DIREITA",24);else str_copy(g_nav400_turn,"SIGA",24);
}

static void polish400_handbrake(float dt){
  // The validated 3.0.1 reverse model is deliberately excluded. This only damps
  // the endless forward lateral/yaw tail reported during runtime testing.
  if(!key_down(VK_SPACE)||g_player.speed<=2.8f||g_settings_menu_open||g_map380_open||g_garage215_open)return;
  V3 f=v3((float)sin((double)g_player.body.yaw),0,(float)cos((double)g_player.body.yaw)),r=v3(f.z,0,-f.x);float longitudinal=vdot(g_player.body.vel,f),lateral=vdot(g_player.body.vel,r);if(longitudinal<=2.8f)return;
  float lateralDamp=clampf(dt*(.52f+absf(longitudinal)*.015f),0,.115f),yawDamp=clampf(dt*(.36f+absf(longitudinal)*.010f),0,.085f);lateral*=1.0f-lateralDamp;g_player.body.angularVel*=1.0f-yawDamp;g_player.body.vel=vadd(vmul(f,longitudinal),vmul(r,lateral));g_player.speed=longitudinal;g_player.lateralSpeed=lateral;g_player.slip=absf(longitudinal)>.8f?clampf((float)atan2((double)lateral,(double)maxf(1.0f,absf(longitudinal))),-1,1):0;g_drive400_handbrake_settles++;
}
static float polish400_traffic_cap(const RoadWay*r){
  if(!r)return .88f;if(str_eq(r->type,"motorway")||str_eq(r->type,"trunk"))return .98f;if(str_eq(r->type,"primary"))return .94f;if(str_eq(r->type,"secondary")||str_eq(r->type,"tertiary"))return .91f;if(str_eq(r->type,"residential")||str_eq(r->type,"living_street"))return .82f;if(str_eq(r->type,"service"))return .70f;return .86f;
}
static void polish400_traffic(float dt){
  g_traffic400_capped=0;for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active||t->ri<0||t->ri>=t->routeN)continue;int eid=t->route[t->ri];if(eid<0||eid>=g_world.edgeCount)continue;Edge*e=&g_world.edges[eid];RoadWay*r=(e->road>=0&&e->road<g_world.roadCount)?&g_world.roads[e->road]:NULL;float cap=polish400_traffic_cap(r);if(g_weather>=2)cap-=.06f;if(g_weather>=3)cap-=.05f;cap=clampf(cap,.56f,.99f);if(g_traffic_cruise[i]>cap){g_traffic_cruise[i]=lerpf(g_traffic_cruise[i],cap,clampf(dt*1.7f,0,1));g_traffic400_capped++;}}
}
static void polish400_input(void){if(!g_started)return;if(key_tap(POLISH400_VK_F2)&&!g_settings_menu_open&&!g_map380_open&&!g_net321_menu_open&&!g_garage215_open)g_drivehub400_open=!g_drivehub400_open;}

static void game_update(float dt){
  game_update_legacy390(dt);if(!g_started)return;polish400_input();polish400_handbrake(dt);polish400_traffic(dt);g_nav400_timer-=dt;if(g_nav400_timer<=0){g_nav400_timer=.20f;polish400_navigation();}
}
