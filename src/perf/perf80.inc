// -----------------------------------------------------------------------------
// 0.80 measurable frame-budget manager. It never changes physics/navigation;
// only decorative density, far-building fidelity and minimap coverage adapt.
// -----------------------------------------------------------------------------
static int g_perf80_quality=2;
static int g_perf80_ped_stride=1,g_perf80_parked_stride=1;
static int g_perf80_exact_buildings=0,g_perf80_lod_buildings=0;
static float g_perf80_budget=1.0f,g_perf80_exact_building_range=360.0f;
static void perf80_update(float dt){
  (void)dt;float fps=g_lod20_fps;int q=g_perf80_quality;
  if(fps<34.0f)q=0;else if(fps<48.0f)q=1;else if(fps>57.0f)q=2;
  g_perf80_quality=q;float target=q==0?.48f:q==1?.72f:1.0f;g_perf80_budget=lerpf(g_perf80_budget,target,.08f);
  g_perf80_ped_stride=q==0?3:q==1?2:1;g_perf80_parked_stride=q==0?3:q==1?2:1;
  g_perf80_exact_building_range=q==0?190.0f:q==1?275.0f:360.0f;
  g_minimap_range70=lerpf(g_minimap_range70,q==0?150.0f:q==1?185.0f:220.0f,.08f);
}
