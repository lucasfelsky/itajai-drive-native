// -----------------------------------------------------------------------------
// Production presentation wrapper. Release identity is centralized from 2.4.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy190();}
static void draw_roads(void){draw_roads_legacy190();}
static void draw_buildings(void){draw_buildings_legacy190();}
static void draw_hud(float fps){
  draw_hud_legacy190(fps);if(g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();
  hud_panel(20,20,1180,31);gl_text(34,43,"ITAJAI DRIVE // " ITAJAI_VERSION " // " ITAJAI_STAGE,1.0f,.96f,.79f);
  if(g_debug){char b[360];int cars=ui200_vehicle_asset_count();int visual=(g_asset_count08>=16)+(cars==CAR160_COUNT)+(g_renderer09_init?1:0)+(g_stream_road_count08>0);int total=g_selfcheck100+visual;hud_panel(20,548,700,126);sprintf(b,"SELF-CHECK: %d/14 | GLSL %s | PAK %d assets",total,g_renderer09_shader?"ON":"FALLBACK",g_asset_count08);gl_text(35,573,b,total==14?.72f:1.0f,total==14?.94f:.72f,total==14?.78f:.38f);sprintf(b,"FLEET %d/8 | footprint %s | hull r %.2fm span %.2fm",cars,g_building_polys12>0?"ON":"AABB FALLBACK",g_vehicle23_last_hull_radius,g_vehicle23_last_hull_span);gl_text(35,595,b,.70f,.84f,.94f);sprintf(b,"COLLISION substeps %d | hull %d | sweep %d | traffic %d",g_vehicle23_substeps,g_vehicle23_static_hits,g_vehicle23_sweep_hits,g_vehicle23_dynamic_hits);gl_text(35,617,b,.66f,.92f,.84f);sprintf(b,"TYRES 120Hz ticks %d | slip F %.3f R %.3f | traction %.2f brake %.2f",g_dyn24_ticks,g_dyn24_front_slip,g_dyn24_rear_slip,g_dyn24_traction_use,g_dyn24_brake_use);gl_text(35,639,b,.76f,.90f,.73f);sprintf(b,"AXLE LOAD F %.0fN R %.0fN | lateral force F %.0f R %.0f",g_dyn24_load_front,g_dyn24_load_rear,g_dyn24_front_force,g_dyn24_rear_force);gl_text(35,661,b,.72f,.78f,.86f);}settings_draw_2d_end();
}
