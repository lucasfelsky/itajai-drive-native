// -----------------------------------------------------------------------------
// 0.50 living world gameplay wrapper.
// -----------------------------------------------------------------------------
static void game_update(float dt){game_update_legacy19(dt);if(!g_started||g_settings_menu_open)return;life50_update(dt);}
