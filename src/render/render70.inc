// -----------------------------------------------------------------------------
// 0.70 minimap / UI / photo-mode presentation.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy60();}
static void draw_roads(void){draw_roads_legacy60();}
static void draw_buildings(void){draw_buildings_legacy60();}
static void draw_hud(float fps){if(g_photo70&&g_photo70_hide_hud)return;draw_hud_legacy60(fps);if(g_settings_menu_open)return;settings_draw_2d_begin();ui70_draw_minimap();ui70_draw_photo_overlay();hud_panel(20,20,900,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 0.70.0 // MAP + PHOTO MODE",.92f,.95f,1);settings_draw_2d_end();}
