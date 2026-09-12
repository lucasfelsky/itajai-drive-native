// 2.7 Lighting & Weather 2.0 presentation wrapper.
static void draw_ground(void){draw_ground_legacy200();}
static void draw_roads(void){draw_roads_legacy200();lighting27_draw_wet_strips();lighting27_draw_traffic_headlights();lighting27_draw_spray();}
static void draw_buildings(void){draw_buildings_legacy200();}
static void draw_hud(float fps){draw_hud_legacy200(fps);if(!g_debug||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();char b[280];sprintf(b,"2.7 LIGHTING // traffic lamps %d | wet strips %d | spray rays %d | wet %.2f | lightning %.2f",g_light27_headlights,g_light27_wet_strips,g_light27_spray,g_wetness60,g_lightning60);hud_panel(20,700,760,30);gl_text(35,722,b,.72f,.86f,1.0f);settings_draw_2d_end();}
