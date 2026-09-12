// -----------------------------------------------------------------------------
// 1.6 Real Car Model Pass presentation wrapper.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy150();}
static void draw_roads(void){draw_roads_legacy150();}
static void draw_buildings(void){draw_buildings_legacy150();}
static void draw_hud(float fps){
  draw_hud_legacy150(fps);if(g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();
  hud_panel(20,20,1180,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 1.6.0 // REAL CAR MODEL PASS",1.0f,.94f,.76f);
  hud_panel(20,282,420,45);char b[220];sprintf(b,"CARRO: %s",vehicle160_player_name());gl_text(34,308,b,.86f,.92f,1.0f);
  if(g_debug){hud_panel(590,282,550,68);sprintf(b,"VEHICLES: %d meshes | player model %d",CAR160_COUNT,g_player_model160[g_player.car&3]);gl_text(605,307,b,.72f,.90f,.82f);gl_text(605,329,"loft body + vidro + luzes + rodas 12 lados",.72f,.78f,.90f);}settings_draw_2d_end();
}
