// -----------------------------------------------------------------------------
// 0.60 climate/time gameplay wrapper.
// -----------------------------------------------------------------------------
static void game_update(float dt){game_update_legacy50(dt);if(!g_started||g_settings_menu_open)return;weather60_update(dt);}
