// 2.14 Audio 2.0 renderer wrapper.
static void draw_ground(void){draw_ground_legacy213();}
static void draw_roads(void){draw_roads_legacy213();}
static void draw_buildings(void){draw_buildings_legacy213();}
static void draw_hud(float fps){
  draw_hud_legacy213(fps);if(!g_debug||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;
  settings_draw_2d_begin();char b[340];sprintf(b,"2.14 AUDIO 2.0 // %s | engine %.2f road %.2f tyre %.2f wind %.2f surface %.2f | device %s",g_audio_voice180[g_player.car&3],g_audio_engine_level180,g_audio_road_level180,g_audio214_tyre_level,g_audio214_wind_level,g_audio214_surface_level,g_audio_available18?"ON":"FALLBACK");hud_panel(20,907,900,30);gl_text(35,929,b,.74f,.82f,.95f);settings_draw_2d_end();
}
