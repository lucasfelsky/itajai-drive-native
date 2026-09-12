// -----------------------------------------------------------------------------
// 1.7 Graphics Uplift presentation wrapper.
// -----------------------------------------------------------------------------
static void draw_ground(void){graphics170_sky_overlay();draw_ground_legacy160();}
static void draw_roads(void){draw_roads_legacy160();graphics170_road_light_pass();}
static void draw_buildings(void){draw_buildings_legacy160();graphics170_vegetation_volume();}
static void draw_hud(float fps){
  draw_hud_legacy160(fps);if(g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();hud_panel(20,20,1180,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 1.7.0 // GRAPHICS UPLIFT",1.0f,.94f,.76f);if(g_debug){char b[240];hud_panel(590,356,550,88);sprintf(b,"GFX170 lamps %d | pools %d | beams %d",g_graphics170_lamps,g_graphics170_pools,g_graphics170_beams);gl_text(605,381,b,.74f,.91f,.82f);sprintf(b,"wetness %.2f | puddles %d | night %.2f",g_wetness60,g_graphics170_puddles,graphics170_night());gl_text(605,403,b,.72f,.82f,.94f);gl_text(605,425,"persistent sheen / local lights / foliage volume",.72f,.76f,.84f);}settings_draw_2d_end();
}
