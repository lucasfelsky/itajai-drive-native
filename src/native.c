// Itajai Drive Native source bundle.
#include "native_parts/part01.inc"
#include "platform/audio18.inc"
#include "platform/extra20.inc"
#include "native_parts/part02.inc"
#include "platform/extra08.inc"
#include "platform/extra09.inc"
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
static int landmark506_replaces(const Building*b);
static void landmark506_colliders(void);
#include "foundation/foundation.inc"
static int landmark506_contact(int source,V3 p,float radius,Contact*out,int collider);
#include "sim/sim06.inc"
#include "settings/settings07_state.inc"
#define camera_mouse_move camera_mouse_move_legacy07
#include "sim/sim07.inc"
#undef camera_mouse_move
#define camera_mouse_move camera_mouse_move_legacy071
#include "settings/camera071.inc"
#undef camera_mouse_move
#include "settings/camera211.inc"
#include "settings/settings07.inc"
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
#include "urban/urban10_core.inc"
#include "sim/reverse211.inc"
#define game_update game_update_legacy07
#include "sim/sim07_game.inc"
#undef game_update
#define game_update game_update_legacy071
#include "settings/settings07_game.inc"
#undef game_update
#include "stream/lod20_state.inc"
#include "assets/assets08.inc"
#define stream08_build_cache stream08_build_cache_legacy08
#define stream08_prepare stream08_prepare_legacy08
#define stream08_refresh stream08_refresh_legacy08
#include "stream/stream08.inc"
#undef stream08_build_cache
#undef stream08_prepare
#undef stream08_refresh
#include "stream/stream220.inc"
#define game_update game_update_legacy08
#include "sim/sim08_game.inc"
#undef game_update
#include "urban/urban10_stream.inc"
#define game_update game_update_legacy10
#include "sim/sim10_game.inc"
#undef game_update
#include "world/terrain11.inc"
#define game_update game_update_legacy11
#include "sim/sim11_game.inc"
#undef game_update
#include "world/building12.inc"
#include "world/landmarks506.inc"
#define game_update game_update_legacy12
#include "sim/sim12_game.inc"
#undef game_update
#define game_update game_update_legacy15
#include "sim/sim15_game.inc"
#undef game_update
#define game_update game_update_legacy17
#include "sim/sim17_game.inc"
#undef game_update
#define game_update game_update_legacy18
#include "sim/audio18.inc"
#undef game_update
#include "world/editor19.inc"
#define game_update game_update_legacy19
#include "sim/sim19_game.inc"
#undef game_update
#include "world/life50.inc"
#define game_update game_update_legacy50
#include "sim/sim50_game.inc"
#undef game_update
#include "final/game_chain.inc"

#define setup_camera setup_camera_legacy06
#define draw_ground draw_ground_legacy06
#define draw_lane_markings draw_lane_markings_legacy06
#define draw_roads draw_roads_legacy06
#define draw_buildings draw_buildings_legacy06
#define draw_hud draw_hud_legacy06
#define draw_player_vehicle draw_player_vehicle_legacy06
#define draw_traffic draw_traffic_legacy06
#include "native_parts/part07.inc"
#undef setup_camera
#undef draw_ground
#undef draw_lane_markings
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#undef draw_player_vehicle
#undef draw_traffic
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
#define draw_hud draw_hud_legacy071
#include "settings/settings07_ui.inc"
#undef draw_hud
#define draw_roads draw_roads_legacy08
#define draw_buildings draw_buildings_legacy08
#define draw_hud draw_hud_legacy08
#include "render/render08.inc"
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#include "render/renderer09.inc"
#include "world/district30.inc"
#define renderer09_draw_sky renderer09_draw_sky_legacy09
#define renderer09_draw_shadows renderer09_draw_shadows_legacy09
#define renderer09_draw_rain renderer09_draw_rain_legacy09
#include "render/effects09.inc"
#undef renderer09_draw_sky
#undef renderer09_draw_shadows
#undef renderer09_draw_rain
#include "render/effects14.inc"
#define draw_ground draw_ground_legacy09
#define draw_roads draw_roads_legacy09
#define draw_buildings draw_buildings_legacy09
#define draw_hud draw_hud_legacy09
#include "render/render09.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#define draw_roads draw_roads_legacy10
#define draw_buildings draw_buildings_legacy10
#define draw_hud draw_hud_legacy10
#include "render/render10.inc"
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#define draw_ground draw_ground_legacy11
#define draw_roads draw_roads_legacy11
#define draw_buildings draw_buildings_legacy11
#define draw_hud draw_hud_legacy11
#include "render/render11.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#define draw_ground draw_ground_legacy12
#define draw_roads draw_roads_legacy12
#define draw_buildings draw_buildings_legacy12
#define draw_hud draw_hud_legacy12
#include "render/render12.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#define draw_ground draw_ground_legacy13
#define draw_roads draw_roads_legacy13
#define draw_buildings draw_buildings_legacy13
#define draw_hud draw_hud_legacy13
#include "render/render13.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#define draw_ground draw_ground_legacy14
#define draw_roads draw_roads_legacy14
#define draw_buildings draw_buildings_legacy14
#define draw_hud draw_hud_legacy14
#include "render/render14.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#define draw_ground draw_ground_legacy15
#define draw_roads draw_roads_legacy15
#define draw_buildings draw_buildings_legacy15
#define draw_hud draw_hud_legacy15
#include "render/render15.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#include "render/vehicle16.inc"
#define draw_traffic draw_traffic_legacy17
#include "render/traffic17.inc"
#undef draw_traffic
#include "render/traffic40.inc"
#include "render/life50_render.inc"
#define draw_ground draw_ground_legacy17
#define draw_roads draw_roads_legacy17
#define draw_buildings draw_buildings_legacy17
#define draw_hud draw_hud_legacy17
#include "render/render17.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#define draw_ground draw_ground_legacy18
#define draw_roads draw_roads_legacy18
#define draw_buildings draw_buildings_legacy18
#define draw_hud draw_hud_legacy18
#include "render/render18.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#define draw_ground draw_ground_legacy19
#define draw_roads draw_roads_legacy19
#define draw_buildings draw_buildings_legacy19
#define draw_hud draw_hud_legacy19
#include "render/render19.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#define draw_ground draw_ground_legacy20
#define draw_roads draw_roads_legacy20
#define draw_buildings draw_buildings_legacy20
#define draw_hud draw_hud_legacy20
#include "render/render20.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#define draw_ground draw_ground_legacy30
#define draw_roads draw_roads_legacy30
#define draw_buildings draw_buildings_legacy30
#define draw_hud draw_hud_legacy30
#include "render/render30.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#define draw_ground draw_ground_legacy40
#define draw_roads draw_roads_legacy40
#define draw_buildings draw_buildings_legacy40
#define draw_hud draw_hud_legacy40
#include "render/render40.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#define draw_ground draw_ground_legacy50
#define draw_roads draw_roads_legacy50
#define draw_buildings draw_buildings_legacy50
#define draw_hud draw_hud_legacy50
#include "render/render50.inc"
#undef draw_ground
#undef draw_roads
#undef draw_buildings
#undef draw_hud
#include "final/render_chain.inc"

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
#define draw_loading draw_loading_legacy10
#define render render_legacy10
#define wndproc wndproc_legacy10
#define init_window init_window_legacy10
#define WinMainCRTStartup WinMainCRTStartup_legacy10
#include "sim/sim10_win32.inc"
#undef draw_loading
#undef render
#undef wndproc
#undef init_window
#undef WinMainCRTStartup
#include "sim/sim11_win32.inc"
