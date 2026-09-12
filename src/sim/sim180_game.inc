// -----------------------------------------------------------------------------
// 1.8 Vehicle Polish gameplay glue.
// Keeps the early camera module independent from the later Player declaration.
// -----------------------------------------------------------------------------
static void game_update(float dt){game_update_legacy130(dt);if(g_started)g_camera_cockpit_slot180=g_player.car&3;}
