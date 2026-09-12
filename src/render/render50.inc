// -----------------------------------------------------------------------------
// 0.50 living world presentation.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy40();}
static void draw_roads(void){draw_roads_legacy40();}
static void draw_buildings(void){draw_buildings_legacy40();life50_draw();}
static void draw_hud(float fps){draw_hud_legacy40(fps);if(g_settings_menu_open)return;settings_draw_2d_begin();hud_panel(20,20,840,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 0.50.0 // LIVING WORLD",.92f,.95f,1);if(g_debug){char b[180];sprintf(b,"life: peds %d/%d  boats %d/%d  parked %d",g_life50_active_peds,LIFE50_PEDS,g_life50_active_boats,LIFE50_BOATS,LIFE50_PARKED);gl_text(34,170,b,.74f,.90f,.78f);}settings_draw_2d_end();}
