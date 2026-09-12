// -----------------------------------------------------------------------------
// 2.3 Foundation Pass gameplay wrapper.
// Adaptive semi-fixed substeps keep legacy movement stable. From 2.5 onward the
// precise compound hull owns the post-step collision pass; the 2.3 hull remains
// available as a compatibility fallback/debug representation.
// -----------------------------------------------------------------------------
static void game_update(float dt){
  if(dt<=0)return;int steps=1;while(steps<4&&dt/(float)steps>.018f)steps++;float h=dt/(float)steps;g_vehicle23_substeps=steps;g_vehicle23_static_hits=0;g_vehicle23_sweep_hits=0;g_vehicle23_dynamic_hits=0;
  for(int i=0;i<steps;i++){
    if(g_started)vehicle23_sync_body_radius();V3 oldPos=g_player.body.p;float oldYaw=g_player.body.yaw;game_update_legacy180(h);if(!g_started)continue;vehicle23_sync_body_radius();
    if(g_settings_menu_open||g_editor19||g_photo70)continue;if(!g_vehicle25_enabled)vehicle23_post_step(oldPos,oldYaw);
  }
}
