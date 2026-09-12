// 2.30 Itajai Fidelity Pass renderer wrapper.
static void draw_ground(void){draw_ground_legacy220();}
static void draw_roads(void){draw_roads_legacy220();}
static void draw_buildings(void){draw_buildings_legacy220();fidelity230_draw();}
static void draw_hud(float fps){
  draw_hud_legacy220(fps);if(!g_debug||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;
  settings_draw_2d_begin();char b[260];sprintf(b,"2.30 ITAJAI FIDELITY // curated nearby accents %d | Centro + Beira-Rio + Porto",g_fidelity230_draws);hud_panel(20,975,760,30);gl_text(35,997,b,.92f,.76f,.46f);settings_draw_2d_end();
}
