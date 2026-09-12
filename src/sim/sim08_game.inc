// -----------------------------------------------------------------------------
// 0.8 gameplay wrapper: initialize asset/runtime region caches, refresh streamed
// regions and resolve collisions against streamed urban props.
// -----------------------------------------------------------------------------
static int g_sim08_initialized=0;
static int g_stream_ready08=0;

static void sim08_initialize(void){
  if(g_sim08_initialized)return;g_sim08_initialized=1;asset08_load_pack();g_stream_ready08=stream08_prepare();if(g_stream_ready08)stream08_refresh(g_player.body.p,1.0f);
}

static void game_update(float dt){
  game_update_legacy071(dt);if(!g_started)return;if(!g_sim08_initialized)sim08_initialize();if(g_settings_menu_open)return;
  if(g_stream_ready08){stream08_refresh(g_player.body.p,dt);int h=stream08_resolve_asset_collisions(&g_player.body,&g_player.speed);if(h){V3 f=v3((float)sin((double)g_player.body.yaw),0,(float)cos((double)g_player.body.yaw));g_player.speed=vdot(g_player.body.vel,f);g_last_static_contacts+=h;}}
}
