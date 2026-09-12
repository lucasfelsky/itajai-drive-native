// -----------------------------------------------------------------------------
// 0.60 advanced climate presentation.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy50();}
static void draw_roads(void){draw_roads_legacy50();weather60_draw_road_sheen();}
static void draw_buildings(void){draw_buildings_legacy50();}
static void draw_hud(float fps){weather60_draw_flash();draw_hud_legacy50(fps);if(g_settings_menu_open)return;settings_draw_2d_begin();hud_panel(20,20,870,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 0.60.0 // DYNAMIC CLIMATE",.92f,.95f,1);if(g_debug){char b[210];sprintf(b,"climate wet %.2f cloud %.2f wind %.2f lightning %.2f",g_wetness60,g_cloud60,g_wind60,g_lightning60);gl_text(34,192,b,.72f,.88f,.86f);}settings_draw_2d_end();}
