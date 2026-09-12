// -----------------------------------------------------------------------------
// 1.5 Itajai Identity presentation wrapper.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy130();}
static void draw_roads(void){draw_roads_legacy130();}
static void draw_buildings(void){draw_buildings_legacy130();identity150_draw();}
static void draw_hud(float fps){
  draw_hud_legacy130(fps);if(g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();
  hud_panel(20,20,1140,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 1.5.0 // ITAJAI IDENTITY PASS",1.0f,.94f,.76f);
  if(g_debug){char b[240];hud_panel(590,166,550,112);sprintf(b,"IDENTITY: %s",urban10_sector_name());gl_text(605,190,b,.72f,.92f,.78f);sprintf(b,"lojas/placas %d | varandas %d | muros %d",g_identity_signs150,g_identity_balconies150,g_identity_walls150);gl_text(605,212,b,.72f,.82f,.94f);sprintf(b,"postos/servicos %d | porto %d | marcos %d",g_identity_service150,g_identity_port150,g_identity_landmarks150);gl_text(605,234,b,.80f,.78f,.92f);gl_text(605,256,"fachadas PT-BR / costa / porto / residencial",.72f,.76f,.84f);}settings_draw_2d_end();
}
