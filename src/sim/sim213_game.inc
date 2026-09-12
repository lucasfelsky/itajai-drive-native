// 2.13 Living City 2.0 gameplay wrapper.
static void game_update(float dt){
  game_update_legacy212(dt);
  if(!g_started||g_settings_menu_open||g_editor19||g_photo70)return;
  life213_update(dt);
}
