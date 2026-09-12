// Itajai Drive Native source bundle.
// Split into include parts only to keep connector-sized commits manageable.
#include "native_parts/part01.inc"
#include "native_parts/part02.inc"
#include "platform/extra08.inc"
#include "native_parts/part03.inc"
#include "native_parts/part04.inc"
#include "native_parts/part05.inc"
#include "foundation/globals.inc"
#include "foundation/foundation.inc"
#include "sim/sim06.inc"
#include "settings/settings07_state.inc"

// Keep the original 0.7 mouse handler internally; 0.7.1 provides persistent
// sensitivity/invert-Y without disturbing the rest of the intersection code.
#define camera_mouse_move camera_mouse_move_legacy07
#include "sim/sim07.inc"
#undef camera_mouse_move
#include "settings/camera071.inc"
#include "settings/settings07.inc"

// Keep the 0.6 game loop/traffic code available as an internal fallback.
#define traffic_update traffic_update_legacy06
#define game_update game_update_legacy06
#include "native_parts/part06.inc"
#undef traffic_update
#undef game_update

// 0.7 gameplay remains internally available; 0.7.1 wraps it with the pause menu.
#define game_update game_update_legacy07
#include "sim/sim07_game.inc"
#undef game_update
#define game_update game_update_legacy071
#include "settings/settings07_game.inc"
#undef game_update

// 0.8 source-asset runtime and disk-backed regional streaming.
#include "assets/assets08.inc"
#include "stream/stream08.inc"
#include "sim/sim08_game.inc"

// Keep original renderers as explicit fallbacks.
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

#define draw_lane_markings draw_lane_markings_legacy07
#define draw_roads draw_roads_legacy07
#define draw_buildings draw_buildings_legacy07
#define draw_hud draw_hud_legacy07
#include "sim/sim07_render.inc"
#undef draw_lane_markings
#undef draw_roads
#undef draw_buildings
#undef draw_hud

// Preserve settings HUD/menu underneath the 0.8 telemetry/version layer.
#define draw_hud draw_hud_legacy071
#include "settings/settings07_ui.inc"
#undef draw_hud
#include "render/render08.inc"

// Preserve old Win32 bootstrap/loading path internally and expose the 0.7.1
// bootstrap; it now calls the public 0.8 game/render functions above.
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
