// -----------------------------------------------------------------------------
// 0.30 quality district presentation.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy20();district30_surface();}
static void draw_roads(void){draw_roads_legacy20();}
static void draw_buildings(void){draw_buildings_legacy20();district30_props();}
static void draw_hud(float fps){draw_hud_legacy20(fps);if(g_settings_menu_open)return;settings_draw_2d_begin();hud_panel(20,20,790,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 0.30.0 // QUALITY DISTRICT",.92f,.95f,1);if(g_debug&&district30_near(1050.0f)){char b[160];sprintf(b,"BEIRA-RIO quality layer // props %d",g_district30_drawn_props);gl_text(34,126,b,.76f,.90f,.76f);}settings_draw_2d_end();}
