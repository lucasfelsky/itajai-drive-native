// -----------------------------------------------------------------------------
// 2.5 Collision & Physics World 2.0 gameplay wrapper.
// ----------------------------------------------------------------------------
static void game_update(float dt){
  g_vehicle25_enabled=1;g_vehicle25_static_hits=0;g_vehicle25_sweep_hits=0;g_vehicle25_dynamic_hits=0;
  V3 oldPos=g_player.body.p;float oldYaw=g_player.body.yaw;game_update_legacy240(dt);
  if(!g_started||g_settings_menu_open||g_editor19||g_photo70)return;vehicle25_post(oldPos,oldYaw);
}
