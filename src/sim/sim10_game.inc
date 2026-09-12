// -----------------------------------------------------------------------------
// 0.10 gameplay wrapper: keep 0.8 physics/streaming behavior, then apply urban
// sector policy, regenerate regional props and recycle distant traffic locally.
// -----------------------------------------------------------------------------
static void game_update(float dt){game_update_legacy08(dt);if(!g_started||g_settings_menu_open)return;urban10_update_sector();urban10_refresh_props_if_needed();urban10_recycle_traffic(dt);}
