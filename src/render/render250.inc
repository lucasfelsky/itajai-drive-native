// 2.50 Game Feel & Polish wrapper.
static void draw_ground(void){draw_ground_legacy240();feel250_draw_skids();}
static void draw_roads(void){draw_roads_legacy240();}
static void draw_buildings(void){draw_buildings_legacy240();feel250_draw_spray();}
static void draw_hud(float fps){draw_hud_legacy240(fps);feel250_draw_impact();if(g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;if(g_debug){settings_draw_2d_begin();char b[160];sprintf(b,"FEEL 2.50  // skid %d  spray %.2f  impact %.2f",g_skid250_live,g_spray250,g_impact250);gl_text(35,993,b,.70f,.82f,.90f);settings_draw_2d_end();}}
