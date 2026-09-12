// -----------------------------------------------------------------------------
// 1.9 Production Presentation wrapper.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy170();}
static void draw_roads(void){draw_roads_legacy170();graphics190_vehicle_reflections();graphics190_road_studs();}
static void draw_buildings(void){draw_buildings_legacy170();}
static void draw_hud(float fps){
  draw_hud_legacy170(fps);if(g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();hud_panel(20,20,1180,31);gl_text(34,43,"ITAJAI DRIVE // 1.9.0 // PRODUCTION PRESENTATION",1.0f,.95f,.78f);if(g_debug){char b[256];hud_panel(590,451,550,83);sprintf(b,"MATERIALS: paint/glass/rubber/chrome | GLSL %s",g_renderer09_shader?"ON":"FALLBACK");gl_text(605,476,b,.76f,.91f,.84f);sprintf(b,"ROAD: %d reflective studs | %d car reflections",g_graphics190_studs,g_graphics190_car_reflections);gl_text(605,498,b,.74f,.84f,.96f);gl_text(605,520,"clearcoat Fresnel + tinted glass + wet local reflection",.72f,.78f,.86f);}settings_draw_2d_end();
}
