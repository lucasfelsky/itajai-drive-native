// 2.13 Living City 2.0 renderer wrapper.
static void draw_ground(void){draw_ground_legacy212();}
static void draw_roads(void){draw_roads_legacy212();}
static void draw_buildings(void){draw_buildings_legacy212();}
static void draw_hud(float fps){
  draw_hud_legacy212(fps);if(!g_debug||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;
  settings_draw_2d_begin();char b[320];sprintf(b,"2.13 LIVING CITY // peds %d wait %d | cyclists %d | boats %d + tugs %d | parked %d",g_life50_active_peds,g_life213_waiting_peds,g_life213_active_cyclists,g_life50_active_boats,g_life213_active_tugs,LIFE50_PARKED);hud_panel(20,873,820,30);gl_text(35,895,b,.61f,.88f,.66f);settings_draw_2d_end();
}
