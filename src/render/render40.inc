// -----------------------------------------------------------------------------
// 0.40 expanded vehicle fleet presentation.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy30();}
static void draw_roads(void){draw_roads_legacy30();}
static void draw_buildings(void){draw_buildings_legacy30();}
static void draw_hud(float fps){draw_hud_legacy30(fps);if(g_settings_menu_open)return;settings_draw_2d_begin();hud_panel(20,20,820,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 0.40.0 // BRAZIL FLEET",.92f,.95f,1);if(g_debug){char b[180];int loaded=0;for(int i=0;i<8;i++)if(g_traffic_car_assets40[i]>=0)loaded++;sprintf(b,"traffic fleet: %d/8 glTF bodies loaded",loaded);gl_text(34,148,b,.72f,.88f,.80f);}settings_draw_2d_end();}
