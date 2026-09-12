// -----------------------------------------------------------------------------
// 2.2 QA Fix Pass presentation wrapper.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy190();}
static void draw_roads(void){draw_roads_legacy190();}
static void draw_buildings(void){draw_buildings_legacy190();}
static void draw_hud(float fps){
  draw_hud_legacy190(fps);if(g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();
  hud_panel(20,20,1180,31);gl_text(34,43,"ITAJAI DRIVE // 2.2.0 // QA FIX PASS",1.0f,.96f,.79f);
  if(g_debug){char b[300];int cars=ui200_vehicle_asset_count();int visual=(g_asset_count08>=16)+(cars==CAR160_COUNT)+(g_renderer09_init?1:0)+(g_stream_road_count08>0);int total=g_selfcheck100+visual;hud_panel(20,548,575,83);sprintf(b,"SELF-CHECK: %d/14 | GLSL %s | PAK %d assets",total,g_renderer09_shader?"ON":"FALLBACK",g_asset_count08);gl_text(35,573,b,total==14?.72f:1.0f,total==14?.94f:.72f,total==14?.78f:.38f);sprintf(b,"FLEET: %d/8 | OSM footprint collision %s | polygons %d",cars,g_building_polys12>0?"ON":"AABB FALLBACK",g_building_polys12);gl_text(35,595,b,.70f,.84f,.94f);gl_text(35,617,"2.2 // handbrake retune + exact building narrow-phase + silhouette rev22",.72f,.78f,.86f);}settings_draw_2d_end();
}
