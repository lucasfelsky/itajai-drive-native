// -----------------------------------------------------------------------------
// 0.14 render presentation.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy13();}
static void draw_roads(void){draw_roads_legacy13();renderer14_draw_wet_reflections();}
static void draw_buildings(void){draw_buildings_legacy13();}
static void draw_hud(float fps){renderer14_post_overlay();draw_hud_legacy13(fps);if(g_settings_menu_open)return;settings_draw_2d_begin();hud_panel(20,20,590,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 0.14.0 // CINEMATIC FX",.92f,.95f,1);if(g_debug){hud_panel(835,(float)g_h-255,420,31);gl_text(850,(float)g_h-232,"FX: SOFT SHADOWS / WET REFLECTION / POST",.66f,.88f,1);}settings_draw_2d_end();}
