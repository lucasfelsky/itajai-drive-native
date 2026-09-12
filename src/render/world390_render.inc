// 3.9 scope diagnostics, layered onto the existing map draw call.
static void map380_draw(void){
  map380_draw_legacy380();
  if(!g_debug||g_map380_open||g_photo70_hide_hud)return;
  settings_draw_2d_begin();char b[320];const char*src=g_world390_boundary_ready>0?(g_world390_boundary_live?"GEOITAJAI LIVE":"GEOITAJAI CACHE"):(g_world390_boundary_ready<0?"BOUNDARY FALLBACK":"BOUNDARY LOADING");
  sprintf(b,"WORLD 3.9 // %s // limite %d verts // roads %d seg %d nodes %d edges %d // stream regions %d/%d",src,g_world390_boundary_count,g_world.roadCount,g_world.segCount,g_world.nodeCount,g_world.edgeCount,g_region_count08,MAX_REGIONS08);
  gl_text(18,(float)g_h-48,b,.62f,.78f,.90f);settings_draw_2d_end();
}
