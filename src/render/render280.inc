// 2.8 World Geometry 2.0 wrapper.
static void draw_ground(void){draw_ground_legacy270();}
static void draw_roads(void){draw_roads_legacy270();}
static void draw_buildings(void){draw_buildings_legacy270();world28_draw();}
static void draw_hud(float fps){draw_hud_legacy270(fps);if(!g_debug||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();char b[260];sprintf(b,"2.8 WORLD GEO // roofs %d | rooftop units %d | podiums %d | curb greenery %d",g_world28_roofs,g_world28_mech,g_world28_podium,g_world28_green);hud_panel(20,735,720,30);gl_text(35,757,b,.72f,.86f,.72f);settings_draw_2d_end();}
