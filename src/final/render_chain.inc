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
#include "../render/render90.inc"
