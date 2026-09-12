// 2.12 Traffic AI 2.0 renderer wrapper.
static void draw_ground(void){draw_ground_legacy211();}
static void draw_roads(void){draw_roads_legacy211();}
static void draw_buildings(void){draw_buildings_legacy211();}
static void draw_hud(float fps){
  draw_hud_legacy211(fps);if(!g_debug||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;
  settings_draw_2d_begin();char b[300];sprintf(b,"2.12 TRAFFIC AI // indicators %d | cautious %d | assertive %d | lane changes %d | yielding %d",g_ai212_indicating,g_ai212_cautious,g_ai212_assertive,g_lane_changes17,g_yielding17);hud_panel(20,839,790,30);gl_text(35,861,b,.86f,.78f,.36f);settings_draw_2d_end();
}
