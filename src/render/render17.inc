// -----------------------------------------------------------------------------
// 0.17 traffic presentation.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy15();}
static void draw_roads(void){draw_roads_legacy15();}
static void draw_buildings(void){draw_buildings_legacy15();}
static void draw_hud(float fps){draw_hud_legacy15(fps);if(g_settings_menu_open)return;settings_draw_2d_begin();hud_panel(20,20,650,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 0.17.0 // ADVANCED TRAFFIC",.92f,.95f,1);if(g_debug){char b[220];hud_panel(835,(float)g_h-330,430,46);sprintf(b,"LANE CHANGES %d | YIELDING %d | DENSITY %.2f",g_lane_changes17,g_yielding17,g_traffic_density17);gl_text(850,(float)g_h-305,b,.66f,.90f,.76f);}settings_draw_2d_end();}
