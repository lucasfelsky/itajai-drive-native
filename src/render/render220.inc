// 2.20 Performance & Streaming 3.0 renderer wrapper.
static void draw_ground(void){draw_ground_legacy215();}
static void draw_roads(void){draw_roads_legacy215();}
static void draw_buildings(void){draw_buildings_legacy215();}
static void draw_hud(float fps){
  draw_hud_legacy215(fps);if(!g_debug||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;
  settings_draw_2d_begin();char b[360];sprintf(b,"2.20 STREAMING 3.0 // regions %d loaded %d | roads %d buildings %d | cache build %lu ms (%d) | direct lookups %d",g_region_count08,g_stream_loaded_regions08,g_stream_road_count08,g_stream_building_count08,(unsigned long)g_stream220_cache_build_ms,g_stream220_cache_builds,g_stream220_direct_lookups);hud_panel(20,941,980,30);gl_text(35,963,b,.56f,.86f,.94f);settings_draw_2d_end();
}
