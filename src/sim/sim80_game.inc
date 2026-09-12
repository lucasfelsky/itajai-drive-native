// -----------------------------------------------------------------------------
// 0.80 performance-budget gameplay wrapper.
// -----------------------------------------------------------------------------
static void game_update(float dt){game_update_legacy70(dt);if(!g_started)return;perf80_update(dt);}
