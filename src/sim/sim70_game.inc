// -----------------------------------------------------------------------------
// 0.70 photo mode gameplay wrapper. P toggles; H hides photo UI.
// Simulation freezes while photo mode is active, camera orbit keeps updating.
// -----------------------------------------------------------------------------
static void game_update(float dt){
  if(g_photo70){update_key_edges();if(key_tap(VK_ESCAPE))PostQuitMessage(0);if(key_tap('P')){g_photo70=0;g_photo70_hide_hud=0;camera_reset_orbit();return;}if(key_tap('H'))g_photo70_hide_hud=!g_photo70_hide_hud;camera_update(dt);return;}
  game_update_legacy60(dt);if(!g_started||g_settings_menu_open||g_editor19)return;if(key_tap('P')){g_photo70=1;g_photo70_hide_hud=0;g_player.camera=0;g_camera_yaw_target=.45f;g_camera_pitch_target=.18f;}
}
