// -----------------------------------------------------------------------------
// 3.8 Interactive Map & GPS
// Full-city destination selection layered on the existing A* GPS/reroute system.
// M opens/closes the map. Arrows move the cursor, +/- zoom, Enter routes,
// Delete clears the route and Home recenters on the player. Mouse click routing
// is implemented by map380_render.inc / the final Win32 window procedure.
// -----------------------------------------------------------------------------
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
static int map380_set_destination(V3 target){
  int start=nearest_node(g_player.body.p),goal=nearest_node(target);if(start<0||goal<0){map380_status("SEM VIA PROXIMA AO DESTINO");g_map380_failed++;return 0;}
  if(g_world.nodes[start].component!=g_world.nodes[goal].component){map380_status("DESTINO FORA DA MALHA VIARIA CONECTADA");g_map380_failed++;return 0;}
  int n=astar_route(start,goal,g_gps_route,MAX_ROUTE);if(n<=0){map380_status("NAO FOI POSSIVEL CALCULAR A ROTA");g_map380_failed++;return 0;}
  g_gps_n=n;g_gps_goal07=goal;g_gps_offroute07=0;g_gps_recalc_timer07=.15f;g_gps_length07=sim07_route_length(g_gps_route,g_gps_n);g_map380_cursor=g_world.nodes[goal].p;g_map380_routes_set++;
  int road=-1;float center=9999.0f;foundation_nearest_road(g_world.nodes[goal].p,&road,&center);if(road>=0&&road<g_world.roadCount&&g_world.roads[road].name[0])sprintf(g_map380_status,"DESTINO: %s // %.1f KM",g_world.roads[road].name,g_gps_length07/1000.0f);else sprintf(g_map380_status,"DESTINO DEFINIDO // %.1f KM",g_gps_length07/1000.0f);return 1;
}
static void map380_open_now(void){g_map380_open=1;map380_center_player();if(g_gps_goal07>=0&&g_gps_goal07<g_world.nodeCount){g_map380_cursor=g_world.nodes[g_gps_goal07].p;map380_status("ROTA ATUAL // CLIQUE OU ENTER PARA TROCAR O DESTINO");}else map380_status("CLIQUE NO MAPA OU USE AS SETAS + ENTER");if(g_camera_captured)camera_release_mouse();}
static void map380_close_now(void){g_map380_open=0;}
static void map380_input(float dt){
  if(!g_map380_open)return;float step=maxf(55.0f,g_map380_range*.58f)*dt;
  if(key_down(VK_LEFT))g_map380_cursor.x-=step;if(key_down(VK_RIGHT))g_map380_cursor.x+=step;if(key_down(VK_UP))g_map380_cursor.z+=step;if(key_down(VK_DOWN))g_map380_cursor.z-=step;
  if(key_tap(VK_OEM_PLUS)||key_tap(VK_ADD))map380_zoom(1);if(key_tap(VK_OEM_MINUS)||key_tap(VK_SUBTRACT))map380_zoom(-1);
  if(key_tap(VK_HOME)){map380_center_player();map380_status("CENTRALIZADO NO JOGADOR");}
  if(key_tap(VK_RETURN))map380_set_destination(g_map380_cursor);
  if(key_tap(VK_DELETE)||key_tap(VK_BACK))map380_clear_route();
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
