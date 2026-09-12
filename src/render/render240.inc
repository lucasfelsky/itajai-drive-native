// 2.40 Navegantes Gateway renderer wrapper.
static void draw_ground(void){draw_ground_legacy230();}
static void draw_roads(void){draw_roads_legacy230();}
static void draw_buildings(void){draw_buildings_legacy230();navegantes240_draw();}
static void draw_hud(float fps){
  draw_hud_legacy230(fps);if(g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;
  if(g_navegantes240_active){settings_draw_2d_begin();hud_panel(20,1009,520,30);gl_text(35,1031,"NAVEGANTES GATEWAY // NORTH BANK + LOGISTICS/AIRPORT CORRIDOR",.72f,.88f,1.0f);settings_draw_2d_end();}
}
