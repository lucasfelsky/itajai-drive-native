// Final rendering chain from 0.60 onward.
#include "../render/weather60_render.inc"
#define draw_ground draw_ground_legacy60
#define draw_roads draw_roads_legacy60
#define draw_buildings draw_buildings_legacy60
#define draw_hud draw_hud_legacy60
#include "../render/render60.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#include "../ui/ui70_render.inc"
#define draw_ground draw_ground_legacy70
#define draw_roads draw_roads_legacy70
#define draw_buildings draw_buildings_legacy70
#define draw_hud draw_hud_legacy70
#include "../render/render70.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#define draw_ground draw_ground_legacy80
#define draw_roads draw_roads_legacy80
#define draw_buildings draw_buildings_legacy80
#define draw_hud draw_hud_legacy80
#include "../render/render80.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#include "../render/content90.inc"
#define draw_ground draw_ground_legacy90
#define draw_roads draw_roads_legacy90
#define draw_buildings draw_buildings_legacy90
#define draw_hud draw_hud_legacy90
#include "../render/render90.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#include "../ui/ui100_render.inc"
#define draw_ground draw_ground_legacy100
#define draw_roads draw_roads_legacy100
#define draw_buildings draw_buildings_legacy100
#define draw_hud draw_hud_legacy100
#include "../render/render100.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#define draw_ground draw_ground_legacy110
#define draw_roads draw_roads_legacy110
#define draw_buildings draw_buildings_legacy110
#define draw_hud draw_hud_legacy110
#include "../render/render110.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#define draw_ground draw_ground_legacy120
#define draw_roads draw_roads_legacy120
#define draw_buildings draw_buildings_legacy120
#define draw_hud draw_hud_legacy120
#include "../render/render120.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#define draw_ground draw_ground_legacy130
#define draw_roads draw_roads_legacy130
#define draw_buildings draw_buildings_legacy130
#define draw_hud draw_hud_legacy130
#include "../render/render130.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#include "../render/identity150.inc"
#include "../render/render150.inc"
