// -----------------------------------------------------------------------------
// 0.12 gameplay wrapper: lazily prepare exact building polygons once world data
// and raw OSM cache are available.
// -----------------------------------------------------------------------------
static void game_update(float dt){game_update_legacy11(dt);if(!g_started)return;if(!g_building12_ready)building12_prepare();}
