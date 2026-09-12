// -----------------------------------------------------------------------------
// 1.0 final vertical-slice presentation.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy90();}
static void draw_roads(void){draw_roads_legacy90();}
static void draw_buildings(void){draw_buildings_legacy90();}
static void draw_hud(float fps){draw_hud_legacy90(fps);if(g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();hud_panel(20,20,930,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 1.0.0 // VERTICAL SLICE",1.0f,.94f,.76f);ui100_draw_help();settings_draw_2d_end();}
