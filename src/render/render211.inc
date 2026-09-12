// 2.11 Vehicle Visual 2.0 wrapper.
static void draw_ground(void){draw_ground_legacy290();}
static void draw_roads(void){draw_roads_legacy290();}
static void draw_buildings(void){draw_buildings_legacy290();}
static void draw_hud(float fps){draw_hud_legacy290(fps);if(!g_debug||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();char b[300];sprintf(b,"2.11 VEHICLE VISUAL // named part sets %d/8 | part draws %d | PAK assets %d | GPU VBO %s",g_vehicle211_parts_ready,g_vehicle211_part_draws,g_asset_count08,g_renderer26_vbo_ready?"ON":"FALLBACK");hud_panel(20,805,790,30);gl_text(35,827,b,.77f,.89f,1.0f);settings_draw_2d_end();}
