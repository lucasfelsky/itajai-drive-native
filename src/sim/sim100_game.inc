// -----------------------------------------------------------------------------
// 1.0 final integration controls: F1 help, Shift+F2 guided landmark tour.
// Plain F2 is intentionally left untouched for the 4.0 Drive Hub layer.
// -----------------------------------------------------------------------------
static void game_update(float dt){game_update_legacy80(dt);if(!g_started)return;if(key_tap(0x70))g_help100=!g_help100;if(key_down(0x10)&&key_tap(0x71))final100_route_next();g_selfcheck100=final100_system_check();}
