// Itajai Drive Native source bundle.
// Split into include parts only to keep connector-sized commits manageable.
#include "native_parts/part01.inc"
#include "native_parts/part02.inc"
#include "native_parts/part03.inc"
#include "native_parts/part04.inc"
#include "native_parts/part05.inc"
#include "foundation/globals.inc"
#include "foundation/foundation.inc"
#include "sim/sim06.inc"
#include "sim/sim07.inc"

// Keep the 0.6 game loop/traffic code available as an internal fallback,
// while 0.7 provides the public implementations used by the entry point.
#define traffic_update traffic_update_legacy06
#define game_update game_update_legacy06
#include "native_parts/part06.inc"
#undef traffic_update
#undef game_update
#include "sim/sim07_game.inc"

// Same strategy for renderer functions evolved by 0.7.
#define setup_camera setup_camera_legacy06
#define draw_lane_markings draw_lane_markings_legacy06
#define draw_roads draw_roads_legacy06
#define draw_buildings draw_buildings_legacy06
#define draw_hud draw_hud_legacy06
#include "native_parts/part07.inc"
#undef setup_camera
#undef draw_lane_markings
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#include "sim/sim07_render.inc"

// Preserve old Win32 bootstrap/loading path internally and expose the 0.7 one.
#define draw_loading draw_loading_legacy06
#define render render_legacy06
#define wndproc wndproc_legacy06
#define init_window init_window_legacy06
#define WinMainCRTStartup WinMainCRTStartup_legacy06
#include "native_parts/part08.inc"
#undef draw_loading
#undef render
#undef wndproc
#undef init_window
#undef WinMainCRTStartup
#include "sim/sim07_win32.inc"
