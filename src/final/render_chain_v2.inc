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
#define draw_ground draw_ground_legacy215
#define draw_roads draw_roads_legacy215
#define draw_buildings draw_buildings_legacy215
#define draw_hud draw_hud_legacy215
#include "../render/render215.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#define draw_ground draw_ground_legacy220
#define draw_roads draw_roads_legacy220
#define draw_buildings draw_buildings_legacy220
#define draw_hud draw_hud_legacy220
#include "../render/render220.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#include "../render/fidelity230.inc"
#define draw_ground draw_ground_legacy230
#define draw_roads draw_roads_legacy230
#define draw_buildings draw_buildings_legacy230
#define draw_hud draw_hud_legacy230
#include "../render/render230.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#include "../render/navegantes240.inc"
#define draw_ground draw_ground_legacy240
#define draw_roads draw_roads_legacy240
#define draw_buildings draw_buildings_legacy240
#define draw_hud draw_hud_legacy240
#include "../render/render240.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#include "../render/feel250_render.inc"
#define draw_ground draw_ground_legacy250
#define draw_roads draw_roads_legacy250
#define draw_buildings draw_buildings_legacy250
#define draw_hud draw_hud_legacy250
#include "../render/render250.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#include "../render/traffic260_render.inc"
#define draw_ground draw_ground_legacy260
#define draw_roads draw_roads_legacy260
#define draw_buildings draw_buildings_legacy260
#define draw_hud draw_hud_legacy260
#include "../render/render260.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#include "../render/coast270.inc"
#define draw_ground draw_ground_legacy270v2
#define draw_roads draw_roads_legacy270v2
#define draw_buildings draw_buildings_legacy270v2
#define draw_hud draw_hud_legacy270v2
#include "../render/render270_v2.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#include "../render/atmos280.inc"
#define draw_ground draw_ground_legacy280v2
#define draw_roads draw_roads_legacy280v2
#define draw_buildings draw_buildings_legacy280v2
#define draw_hud draw_hud_legacy280v2
#include "../render/render280_v2.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#include "rc290.inc"
#include "../render/render290_v2.inc"
#include "../render/net301_render.inc"
#define draw_hud draw_hud_legacy320
#include "../render/net320_render.inc"
#undef draw_hud
#include "../render/net321_render.inc"
#include "../render/city330_render.inc"
#include "../render/city340_identity.inc"
#include "../render/city340_streets.inc"
#include "../render/city350_buildings.inc"
#include "../render/city360_urban.inc"
#include "../render/city370_final.inc"
#define map380_draw map380_draw_legacy380
#include "../ui/map380_render.inc"
#undef map380_draw
#include "../render/world390_render.inc"
#include "../render/city400_world.inc"
#include "../render/vehicle400.inc"
#include "../render/render400.inc"
#include "../ui/ui400_render.inc"