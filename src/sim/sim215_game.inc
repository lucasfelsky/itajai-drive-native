// -----------------------------------------------------------------------------
// 2.15 Production UI gameplay wrapper: native garage/vehicle selector.
// F3 opens/closes; arrows browse; Enter applies. Vehicle motion is held while open.
// -----------------------------------------------------------------------------
static void game_update(float dt){
  game_update_legacy213(dt);
  if(!g_started||g_settings_menu_open||g_editor19||g_photo70)return;
  if(key_tap(VK_F3)){g_garage215_open=!g_garage215_open;if(g_garage215_open){g_garage215_selected=g_player.car&3;if(g_camera_captured)camera_release_mouse();}}
  if(!g_garage215_open)return;
  g_player.body.vel=v3(0,0,0);g_player.body.angularVel=0;g_player.speed=0;g_player.lateralSpeed=0;
  if(key_tap(VK_LEFT)||key_tap('A')){g_garage215_selected=(g_garage215_selected+3)&3;}
  if(key_tap(VK_RIGHT)||key_tap('D')){g_garage215_selected=(g_garage215_selected+1)&3;}
  if(key_tap(VK_RETURN)){g_player.car=g_garage215_selected&3;g_camera_cockpit_slot180=g_player.car;player_apply_profile();g_garage215_open=0;camera_reset_orbit();}
}
