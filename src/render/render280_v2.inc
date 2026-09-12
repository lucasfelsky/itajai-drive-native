// 2.80 Night & Wet City 3.0 wrapper.
static void draw_ground(void){draw_ground_legacy270v2();}
static void draw_roads(void){draw_roads_legacy270v2();atmos280_draw_wet_roads();}
static void draw_buildings(void){draw_buildings_legacy270v2();atmos280_draw_windows();}
static void draw_hud(float fps){draw_hud_legacy270v2(fps);if(g_debug&&!g_photo70_hide_hud){settings_draw_2d_begin();char b[150];sprintf(b,"ATMOS 3.0 // windows %d  wet glints %d  wet %.2f",g_atmos280_windows,g_atmos280_glints,g_wetness60);gl_text(35,939,b,.66f,.80f,.92f);settings_draw_2d_end();}}
