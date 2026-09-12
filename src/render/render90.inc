// -----------------------------------------------------------------------------
// 0.90 final content pass presentation.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy80();}
static void draw_roads(void){draw_roads_legacy80();}
static void draw_buildings(void){draw_buildings_legacy80();content90_draw();}
static void draw_hud(float fps){draw_hud_legacy80(fps);if(g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();hud_panel(20,20,930,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 0.90.0 // FINAL CONTENT",.92f,.95f,1);if(g_debug){char b[200];sprintf(b,"curated district props in range: %d // sector %s",g_content90_props,urban10_sector_name());gl_text(34,306,b,.72f,.90f,.78f);}settings_draw_2d_end();}
