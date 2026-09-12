// -----------------------------------------------------------------------------
// 2.90 Release Candidate health model.
// The RC pass does not change simulation behavior. It continuously evaluates the
// core native stack so TAB can reveal whether a fallback or capacity condition is
// active before the 3.0 release is cut.
// -----------------------------------------------------------------------------
static int g_rc290_score=0,g_rc290_total=12,g_rc290_stream_pressure=0;
static void rc290_update(void){
  int score=0;score+=g_world.roadCount>0;score+=g_world.edgeCount>0;score+=g_world.buildingCount>0;score+=g_static_collider_count>0;score+=g_asset_pack_ok08!=0;score+=g_asset_count08>=32;score+=g_renderer09_init!=0;score+=g_stream_ready08!=0;score+=g_stream_road_count08>0;score+=g_building_polys12>0;score+=g_life50_init!=0;score+=g_audio_started18!=0;g_rc290_score=score;
  g_rc290_stream_pressure=(g_stream_road_count08>MAX_STREAM_ROADS08*9/10)||(g_stream_building_count08>MAX_STREAM_BUILDINGS08*9/10)||(g_asset_instance_count08>MAX_ASSET_INSTANCES08*9/10);
}
