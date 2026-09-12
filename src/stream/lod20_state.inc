// -----------------------------------------------------------------------------
// 0.20 adaptive LOD state. Render code samples FPS and adjusts only expensive
// decorative ranges; gameplay/collision/navigation remain deterministic.
// -----------------------------------------------------------------------------
static float g_lod20_fps=60.0f;
static float g_lod20_prop_dist=620.0f;
static float g_lod20_traffic_dist=360.0f;
static int g_lod20_tier=2;
static int g_lod20_mesh_lists_built=0;

static void lod20_sample_fps(float fps){
  if(fps<1.0f||fps>500.0f)return;
  g_lod20_fps=lerpf(g_lod20_fps,fps,.06f);
  int tier=g_lod20_tier;
  if(g_lod20_fps<38.0f)tier=0;else if(g_lod20_fps<52.0f)tier=1;else if(g_lod20_fps>58.0f)tier=2;
  g_lod20_tier=tier;
  float pd=tier==0?300.0f:tier==1?460.0f:620.0f;
  float td=tier==0?220.0f:tier==1?300.0f:360.0f;
  g_lod20_prop_dist=lerpf(g_lod20_prop_dist,pd,.08f);
  g_lod20_traffic_dist=lerpf(g_lod20_traffic_dist,td,.08f);
}
