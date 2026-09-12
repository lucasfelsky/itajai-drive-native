// Rendering tail for 2.11+ checkpoints. Kept separate so late-stage polish no
// longer rewrites the historical 0.60-2.9 render chain.
#include "../render/vehicle211.inc"
#define draw_ground draw_ground_legacy211
#define draw_roads draw_roads_legacy211
#define draw_buildings draw_buildings_legacy211
#define draw_hud draw_hud_legacy211
#include "../render/render211.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#include "../render/traffic212_render.inc"
#define draw_ground draw_ground_legacy212
#define draw_roads draw_roads_legacy212
#define draw_buildings draw_buildings_legacy212
#define draw_hud draw_hud_legacy212
#include "../render/render212.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#include "../render/life213_render.inc"
#define draw_ground draw_ground_legacy213
#define draw_roads draw_roads_legacy213
#define draw_buildings draw_buildings_legacy213
#define draw_hud draw_hud_legacy213
#include "../render/render213.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#define draw_ground draw_ground_legacy214
#define draw_roads draw_roads_legacy214
#define draw_buildings draw_buildings_legacy214
#define draw_hud draw_hud_legacy214
#include "../render/render214.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#include "../render/render215.inc"
