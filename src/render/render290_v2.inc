// 2.90 Release Candidate diagnostics wrapper.
static void draw_ground(void){draw_ground_legacy280v2();}
static void draw_roads(void){draw_roads_legacy280v2();}
static void draw_buildings(void){draw_buildings_legacy280v2();}
static void draw_hud(float fps){
  draw_hud_legacy280v2(fps);rc290_update();if(!g_debug||g_photo70_hide_hud)return;settings_draw_2d_begin();char a[190],b[220];sprintf(a,"RC 2.90 // HEALTH %d/%d%s",g_rc290_score,g_rc290_total,g_rc290_stream_pressure?"  // STREAM PRESSURE":"  // NOMINAL");sprintf(b,"regions %d  roads %d/%d  buildings %d/%d  props %d/%d",g_stream_loaded_regions08,g_stream_road_count08,MAX_STREAM_ROADS08,g_stream_building_count08,MAX_STREAM_BUILDINGS08,g_asset_instance_count08,MAX_ASSET_INSTANCES08);gl_text(35,921,a,g_rc290_score==g_rc290_total?.64f:1.0f,g_rc290_score==g_rc290_total?.92f:.70f,g_rc290_score==g_rc290_total?.70f:.36f);gl_text(35,903,b,.58f,.72f,.82f);settings_draw_2d_end();
}
