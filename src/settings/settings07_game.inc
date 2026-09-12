// 0.7.1 public game loop wrapper. 0.7 gameplay remains untouched underneath.
static void game_update(float dt){
  if(g_settings_menu_open){settings_menu_update();return;}
  if(g_started&&!g_camera_captured&&key_down(VK_ESCAPE)&&!g_key_prev[VK_ESCAPE]){g_key_prev[VK_ESCAPE]=1;settings_open_menu();return;}
  game_update_legacy07(dt);
}
