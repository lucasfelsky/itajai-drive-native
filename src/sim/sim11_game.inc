// -----------------------------------------------------------------------------
// 0.11 gameplay wrapper: terrain awareness and ocean drag.
// -----------------------------------------------------------------------------
static void game_update(float dt){
  game_update_legacy10(dt);if(!g_started||g_settings_menu_open)return;
  float th=terrain11_raw_height(g_player.body.p.x,g_player.body.p.z);g_terrain_height11=th;
  g_water_depth11=terrain11_water_depth(g_player.body.p.x,g_player.body.p.z);
  g_player_in_water11=terrain11_is_ocean(g_player.body.p.x,g_player.body.p.z)&&g_water_depth11>.20f;
  if(g_player_in_water11){
    float drag=clampf(dt*(2.2f+g_water_depth11*.75f),0,1);g_player.body.vel=vmul(g_player.body.vel,1.0f-drag);g_player.speed*=1.0f-drag;
    if(g_water_depth11>2.8f){int n=nearest_node(g_player.body.p);if(n>=0&&absf(g_player.speed)<.8f){g_player.body.p=vadd(g_world.nodes[n].p,v3(0,1,0));g_player.body.vel=v3(0,0,0);g_player.speed=0;}}
  }
}
