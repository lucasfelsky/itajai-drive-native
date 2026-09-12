// -----------------------------------------------------------------------------
// 0.13 PBR-compat presentation layer.
// World geometry stays in 0.12; renderer09 backend is now physically-inspired.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy12();}
static void draw_roads(void){draw_roads_legacy12();}
static void draw_buildings(void){draw_buildings_legacy12();}
static void draw_hud(float fps){draw_hud_legacy12(fps);if(g_settings_menu_open)return;settings_draw_2d_begin();hud_panel(20,20,560,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 0.13.0 // PBR-COMPAT",.92f,.95f,1);if(g_debug){hud_panel(835,(float)g_h-220,420,31);gl_text(850,(float)g_h-197,g_renderer09_shader?"SHADING: MICROFACET + FRESNEL + TONEMAP":"SHADING: FIXED-FUNCTION FALLBACK",g_renderer09_shader?.62f:1.0f,g_renderer09_shader?.92f:.70f,.72f);}settings_draw_2d_end();}
