// -----------------------------------------------------------------------------
// 1.8 Vehicle Polish presentation wrapper.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy170();}
static void draw_roads(void){draw_roads_legacy170();}
static void draw_buildings(void){draw_buildings_legacy170();}
static void draw_hud(float fps){
  draw_hud_legacy170(fps);if(g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();hud_panel(20,20,1180,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 1.8.0 // VEHICLE POLISH",1.0f,.94f,.76f);if(g_player.camera==2){hud_panel(20,334,365,39);gl_text(34,359,"COCKPIT // mouse-look livre",.72f,.90f,1.0f);}if(g_debug){char b[260];hud_panel(590,450,550,88);sprintf(b,"COCKPIT %s | AUDIO %s",g_vehicle180_cockpit_drawn?"ATIVO":"externo",g_audio_available18?g_audio_voice180[g_player.car&3]:"FALLBACK SILENT");gl_text(605,475,b,.74f,.91f,.82f);sprintf(b,"engine lvl %.3f | road lvl %.3f | gear %d | rpm %.0f",g_audio_engine_level180,g_audio_road_level180,g_gear15,g_engine_rpm15);gl_text(605,497,b,.72f,.82f,.94f);gl_text(605,519,"interior + volante + mirrors + plates + engine voice",.72f,.76f,.84f);}settings_draw_2d_end();
}
