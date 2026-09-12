// 2.9 Brazilian Streets 3.0 wrapper.
static void draw_ground(void){draw_ground_legacy280();}
static void draw_roads(void){draw_roads_legacy280();roads29_draw();}
static void draw_buildings(void){draw_buildings_legacy280();}
static void draw_hud(float fps){draw_hud_legacy280(fps);if(!g_debug||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();char b[280];sprintf(b,"2.9 BR STREETS // crosswalks %d | stop bars %d | arrows %d | roadside signs %d",g_roads29_crosswalks,g_roads29_stopbars,g_roads29_arrows,g_roads29_signs);hud_panel(20,770,760,30);gl_text(35,792,b,.90f,.83f,.58f);settings_draw_2d_end();}
