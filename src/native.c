// Itajai Drive Native source bundle.
// Split into include parts only to keep connector-sized commits manageable.
#include "native_parts/part01.inc"
#include "native_parts/part02.inc"
#include "platform/extra08.inc"
#include "platform/extra09.inc"

// Keep the original small-map cache/parser/network path compiled for regression,
// while 0.10 exposes a larger world implementation under the original names.
#define file_read_all file_read_all_legacy64
#define world_cache_write world_cache_write_legacy05
#define world_cache_read world_cache_read_legacy05
#include "native_parts/part03.inc"
#define parse_way parse_way_legacy05
#define parse_osm parse_osm_legacy05
#include "native_parts/part04.inc"
#define fetch_host fetch_host_legacy05
#define map_worker map_worker_legacy05
#define traffic_spawn traffic_spawn_legacy05
#include "native_parts/part05.inc"
#undef file_read_all
#undef world_cache_write
#undef world_cache_read
#undef parse_way
#undef parse_osm
#undef fetch_host
#undef map_worker
#undef traffic_spawn
#include "world/urban10_world.inc"

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

// Keep the 0.6 game loop and box primitives as explicit fallbacks. 0.9 replaces
// the public box primitive with normals/UVs required by lighting/materials.
#define traffic_update traffic_update_legacy06
#define game_update game_update_legacy06
#define draw_unit_box draw_unit_box_legacy06
#define draw_box draw_box_legacy06
#include "native_parts/part06.inc"
#undef traffic_update
#undef game_update
#undef draw_unit_box
#undef draw_box
#include "render/primitives09.inc"

// g_player is now declared, so the 0.10 sector/traffic policy can safely bind.
#include "urban/urban10_core.inc"

// 0.7 gameplay remains internally available; 0.7.1 wraps it with the pause menu.
#define game_update game_update_legacy07
#include "sim/sim07_game.inc"
#undef game_update
#define game_update game_update_legacy071
#include "settings/settings07_game.inc"
#undef game_update

// 0.8 asset/runtime regional streaming.
#include "assets/assets08.inc"
#include "stream/stream08.inc"
#define game_update game_update_legacy08
#include "sim/sim08_game.inc"
#undef game_update

// sim08_game declares the stream-ready state consumed by the 0.10 prop layer.
#include "urban/urban10_stream.inc"
#include "sim/sim10_game.inc"

// Keep original renderers as explicit fallbacks.
#define setup_camera setup_camera_legacy06
#define draw_ground draw_ground_legacy06
#define draw_lane_markings draw_lane_markings_legacy06
#define draw_roads draw_roads_legacy06
#define draw_buildings draw_buildings_legacy06
#define draw_hud draw_hud_legacy06
#include "native_parts/part07.inc"
#undef setup_camera
#undef draw_ground
#undef draw_lane_markings
#undef draw_roads
#undef draw_buildings
#undef draw_hud

#define setup_camera setup_camera_legacy07
#define draw_lane_markings draw_lane_markings_legacy07
#define draw_roads draw_roads_legacy07
#define draw_buildings draw_buildings_legacy07
#define draw_hud draw_hud_legacy07
#include "sim/sim07_render.inc"
#undef setup_camera
#undef draw_lane_markings
#undef draw_roads
#undef draw_buildings
#undef draw_hud

// Preserve settings HUD/menu underneath the later version layers.
#define draw_hud draw_hud_legacy071
#include "settings/settings07_ui.inc"
#undef draw_hud

// Preserve the 0.8 disk-streamed renderer as a complete visual fallback.
#define draw_roads draw_roads_legacy08
#define draw_buildings draw_buildings_legacy08
#define draw_hud draw_hud_legacy08
#include "render/render08.inc"
#undef draw_roads
#undef draw_buildings
#undef draw_hud

// 0.9 programmable renderer + atmosphere/effects remain the graphics backend.
#include "render/renderer09.inc"
#include "render/effects09.inc"
#define draw_roads draw_roads_legacy09
#define draw_buildings draw_buildings_legacy09
#define draw_hud draw_hud_legacy09
#include "render/render09.inc"
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#include "render/render10.inc"

// Historical bootstraps remain compiled under private names for regression.
#define draw_loading draw_loading_legacy06
#define render render_legacy06
#define wndproc wndproc_legacy06
#define init_window init_window_legacy06
#define init_paths init_paths_legacy05
#define WinMainCRTStartup WinMainCRTStartup_legacy06
#include "native_parts/part08.inc"
#undef draw_loading
#undef render
#undef wndproc
#undef init_window
#undef init_paths
#undef WinMainCRTStartup

#define draw_loading draw_loading_legacy071
#define render render_legacy071
#define wndproc wndproc_legacy071
#define init_window init_window_legacy071
#define WinMainCRTStartup WinMainCRTStartup_legacy071
#include "sim/sim07_win32.inc"
#undef draw_loading
#undef render
#undef wndproc
#undef init_window
#undef WinMainCRTStartup

#define draw_loading draw_loading_legacy09
#define render render_legacy09
#define wndproc wndproc_legacy09
#define init_window init_window_legacy09
#define WinMainCRTStartup WinMainCRTStartup_legacy09
#include "sim/sim09_win32.inc"
#undef draw_loading
#undef render
#undef wndproc
#undef init_window
#undef WinMainCRTStartup
#include "sim/sim10_win32.inc"
