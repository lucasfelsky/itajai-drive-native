// -----------------------------------------------------------------------------
// 0.18 native audio presentation.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy17();}
static void draw_roads(void){draw_roads_legacy17();}
static void draw_buildings(void){draw_buildings_legacy17();}
static void draw_hud(float fps){draw_hud_legacy17(fps);if(g_settings_menu_open)return;settings_draw_2d_begin();hud_panel(20,20,680,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 0.18.0 // NATIVE AUDIO",.92f,.95f,1);if(g_debug){char b[200];hud_panel(835,(float)g_h-365,430,31);sprintf(b,"AUDIO: %s | RPM %.0f | ROAD/RAIN/COAST MIX",g_audio_available18?"WINMM PCM":"FALLBACK SILENT",g_engine_rpm15);gl_text(850,(float)g_h-342,b,g_audio_available18?.68f:1.0f,g_audio_available18?.94f:.65f,.74f);}settings_draw_2d_end();}
