// 2.70 Coast & River 3.0 wrapper.
static void draw_ground(void){draw_ground_legacy260();coast270_draw();}
static void draw_roads(void){draw_roads_legacy260();}
static void draw_buildings(void){draw_buildings_legacy260();}
static void draw_hud(float fps){draw_hud_legacy260(fps);if(g_debug&&!g_photo70_hide_hud){settings_draw_2d_begin();char b[120];sprintf(b,"COAST 3.0 // water draws %d",g_coast270_draws);gl_text(35,957,b,.62f,.80f,.92f);settings_draw_2d_end();}}
