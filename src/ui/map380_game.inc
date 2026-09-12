// -----------------------------------------------------------------------------
// 3.8 Interactive Map & GPS
// Full-city destination selection layered on the existing A* GPS/reroute system.
// M opens/closes the map. Arrows move the cursor, +/- zoom, Enter routes,
// Delete clears the route and Home recenters on the player. Mouse click routing
// is implemented by map380_render.inc / the final Win32 window procedure.
// 3.8.1 makes destination snapping component-aware and retries nearby routable
// nodes before declaring a point unreachable.
// -----------------------------------------------------------------------------
#define MAP380_VK_OEM_PLUS 0xBB
#define MAP380_VK_ADD 0x6B
#define MAP380_VK_OEM_MINUS 0xBD
#define MAP380_VK_SUBTRACT 0x6D
#define MAP380_VK_HOME 0x24
#define MAP380_VK_DELETE 0x2E
#define MAP380_VK_BACK 0x08
static int g_map380_open=0,g_map380_m_prev=0;
static V3 g_map380_center={0,0,0},g_map380_cursor={0,0,0};
static float g_map380_range=1800.0f;
static char g_map380_status[160]="SELECIONE UM DESTINO";
static int g_map380_routes_set=0,g_map380_failed=0;

static void map380_status(const char*s){int i=0;if(!s){g_map380_status[0]=0;return;}for(;s[i]&&i<158;i++)g_map380_status[i]=s[i];g_map380_status[i]=0;}
static void map380_center_player(void){g_map380_center=g_player.body.p;g_map380_cursor=g_player.body.p;}
static void map380_zoom(int dir){if(dir>0)g_map380_range*=.78f;else if(dir<0)g_map380_range*=1.28f;g_map380_range=clampf(g_map380_range,220.0f,5200.0f);}
static void map380_keep_cursor_visible(void){
  float usableH=maxf(240.0f,(float)g_h-150.0f),usableW=maxf(320.0f,(float)g_w-70.0f),scale=usableH/(2.0f*g_map380_range),halfX=usableW/(2.0f*scale),mx=halfX*.72f,mz=g_map380_range*.72f;
  if(g_map380_cursor.x>g_map380_center.x+mx)g_map380_center.x=g_map380_cursor.x-mx;
  if(g_map380_cursor.x<g_map380_center.x-mx)g_map380_center.x=g_map380_cursor.x+mx;
  if(g_map380_cursor.z>g_map380_center.z+mz)g_map380_center.z=g_map380_cursor.z-mz;
  if(g_map380_cursor.z<g_map380_center.z-mz)g_map380_center.z=g_map380_cursor.z+mz;
}
static void map380_clear_route(void){g_gps_n=0;g_gps_goal07=-1;g_gps_offroute07=0;g_gps_length07=0;map380_status("ROTA CANCELADA");}
static int map380_seen_node(int n,const int*seen,int count){for(int i=0;i<count;i++)if(seen[i]==n)return 1;return 0;}
static int map380_route_near_target(V3 target,int start,int*outGoal,float*outSnap){
  if(start<0||start>=g_world.nodeCount)return 0;int comp=g_world.nodes[start].component,raw=nearest_node(target);float rawDist=1e20f,maxSnap=220.0f;
  if(raw>=0&&raw<g_world.nodeCount){rawDist=vlen(vsub(g_world.nodes[raw].p,target));if(g_world.nodes[raw].component==comp)maxSnap=minf(360.0f,maxf(90.0f,rawDist+180.0f));else maxSnap=minf(220.0f,maxf(90.0f,rawDist+110.0f));}
  int seen[18],seenN=0;for(int attempt=0;attempt<18;attempt++){
    int best=-1;float bestD2=1e30f;for(int i=0;i<g_world.nodeCount;i++){Node*n=&g_world.nodes[i];if(n->component!=comp||(n->head<0&&n->headIn<0)||map380_seen_node(i,seen,seenN))continue;float dx=n->p.x-target.x,dz=n->p.z-target.z,d2=dx*dx+dz*dz;if(d2<bestD2){bestD2=d2;best=i;}}
    if(best<0)break;float snap=(float)sqrt((double)bestD2);if(snap>maxSnap)break;seen[seenN++]=best;int n=astar_route(start,best,g_gps_route,MAX_ROUTE);if(n>0){if(outGoal)*outGoal=best;if(outSnap)*outSnap=snap;return n;}
  }return 0;
}
static int map380_set_destination(V3 target){
  int start=nearest_node(g_player.body.p);if(start<0){map380_status("SEM VIA PROXIMA AO JOGADOR");g_map380_failed++;return 0;}
  int goal=-1;float snap=0;int n=map380_route_near_target(target,start,&goal,&snap);if(n<=0||goal<0){map380_status("SEM ROTA VIARIA ACESSIVEL PERTO DO DESTINO");g_map380_failed++;return 0;}
  g_gps_n=n;g_gps_goal07=goal;g_gps_offroute07=0;g_gps_recalc_timer07=.15f;g_gps_length07=sim07_route_length(g_gps_route,g_gps_n);g_map380_cursor=g_world.nodes[goal].p;g_map380_routes_set++;
  int road=-1;if(n>0){int eid=g_gps_route[n-1];if(eid>=0&&eid<g_world.edgeCount)road=g_world.edges[eid].road;}if(road>=0&&road<g_world.roadCount&&g_world.roads[road].name[0]){if(snap>24.0f)sprintf(g_map380_status,"DESTINO: %s // %.1f KM // AJUSTE %.0f M",g_world.roads[road].name,g_gps_length07/1000.0f,snap);else sprintf(g_map380_status,"DESTINO: %s // %.1f KM",g_world.roads[road].name,g_gps_length07/1000.0f);}else{if(snap>24.0f)sprintf(g_map380_status,"DESTINO DEFINIDO // %.1f KM // AJUSTE %.0f M",g_gps_length07/1000.0f,snap);else sprintf(g_map380_status,"DESTINO DEFINIDO // %.1f KM",g_gps_length07/1000.0f);}return 1;
}
static void map380_open_now(void){g_map380_open=1;map380_center_player();if(g_gps_goal07>=0&&g_gps_goal07<g_world.nodeCount){g_map380_cursor=g_world.nodes[g_gps_goal07].p;map380_status("ROTA ATUAL // CLIQUE OU ENTER PARA TROCAR O DESTINO");}else map380_status("CLIQUE NO MAPA OU USE AS SETAS + ENTER");if(g_camera_captured)camera_release_mouse();}
static void map380_close_now(void){g_map380_open=0;}
static void map380_input(float dt){
  if(!g_map380_open)return;float step=maxf(55.0f,g_map380_range*.58f)*dt;
  if(key_down(VK_LEFT))g_map380_cursor.x-=step;if(key_down(VK_RIGHT))g_map380_cursor.x+=step;if(key_down(VK_UP))g_map380_cursor.z+=step;if(key_down(VK_DOWN))g_map380_cursor.z-=step;
  if(key_tap(MAP380_VK_OEM_PLUS)||key_tap(MAP380_VK_ADD))map380_zoom(1);if(key_tap(MAP380_VK_OEM_MINUS)||key_tap(MAP380_VK_SUBTRACT))map380_zoom(-1);
  if(key_tap(MAP380_VK_HOME)){map380_center_player();map380_status("CENTRALIZADO NO JOGADOR");}
  if(key_tap(VK_RETURN))map380_set_destination(g_map380_cursor);
  if(key_tap(MAP380_VK_DELETE)||key_tap(MAP380_VK_BACK))map380_clear_route();
  map380_keep_cursor_visible();g_player.body.vel=v3(0,0,0);g_player.speed=0;g_player.lateralSpeed=0;g_player.body.angularVel=0;
}

static void game_update(float dt){
  int md=key_down('M'),mtap=md&&!g_map380_m_prev;g_map380_m_prev=md;
  // Prevent the historical 0.7 M=random-route shortcut from firing. The new map
  // owns M while keeping gps_random07 available internally for regression/debug.
  if(mtap)g_key_prev['M']=1;
  Player frozen=g_player;int freeze=g_map380_open;
  game_update_legacy321(dt);
  if(freeze){g_player=frozen;g_player.body.vel=v3(0,0,0);g_player.speed=0;g_player.lateralSpeed=0;g_player.body.angularVel=0;}
  if(!g_started)return;
  if(mtap&&!g_settings_menu_open&&!g_net321_menu_open){if(g_map380_open)map380_close_now();else map380_open_now();}
  map380_input(dt);
}
