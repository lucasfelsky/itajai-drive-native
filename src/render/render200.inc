// -----------------------------------------------------------------------------
// 2.3 Foundation Pass presentation wrapper.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy190();}
static void draw_roads(void){draw_roads_legacy190();}
static void draw_buildings(void){draw_buildings_legacy190();}
static void draw_hud(float fps){
  draw_hud_legacy190(fps);if(g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();
  hud_panel(20,20,1180,31);gl_text(34,43,"ITAJAI DRIVE // 2.3.0 // FOUNDATION PASS",1.0f,.96f,.79f);
  if(g_debug){char b[320];int cars=ui200_vehicle_asset_count();int visual=(g_asset_count08>=16)+(cars==CAR160_COUNT)+(g_renderer09_init?1:0)+(g_stream_road_count08>0);int total=g_selfcheck100+visual;hud_panel(20,548,615,105);sprintf(b,"SELF-CHECK: %d/14 | GLSL %s | PAK %d assets",total,g_renderer09_shader?"ON":"FALLBACK",g_asset_count08);gl_text(35,573,b,total==14?.72f:1.0f,total==14?.94f:.72f,total==14?.78f:.38f);sprintf(b,"FLEET %d/8 | footprint %s | hull r %.2fm span %.2fm",cars,g_building_polys12>0?"ON":"AABB FALLBACK",g_vehicle23_last_hull_radius,g_vehicle23_last_hull_span);gl_text(35,595,b,.70f,.84f,.94f);sprintf(b,"PHYSICS substeps %d | hull hits %d | sweep %d | traffic %d",g_vehicle23_substeps,g_vehicle23_static_hits,g_vehicle23_sweep_hits,g_vehicle23_dynamic_hits);gl_text(35,617,b,.66f,.92f,.84f);gl_text(35,639,"2.3 // vehicle-sized 3-circle hull + continuous sweep + adaptive substeps",.72f,.78f,.86f);}settings_draw_2d_end();
}
