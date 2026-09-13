// -----------------------------------------------------------------------------
// 0.80 / 5.0 measurable frame-budget manager. It never changes physics or
// navigation; only decorative density, far-building fidelity and map coverage
// adapt. 5.0 treats a stable 60 fps on the reference Ryzen 5 4600G / RTX 3060
// class machine as the quality target instead of waiting for severe slowdown.
// -----------------------------------------------------------------------------
static int g_perf80_quality=2;
static int g_perf80_ped_stride=1,g_perf80_parked_stride=1;
static int g_perf80_exact_buildings=0,g_perf80_lod_buildings=0;
static float g_perf80_budget=1.0f,g_perf80_exact_building_range=360.0f,g_perf80_pressure=0.0f;
static void perf80_update(float dt){
  float fps=g_lod20_fps;int q=g_perf80_quality;float pressure=0.0f;
  if(q==2){if(fps<55.5f)pressure=1.0f;else if(fps>59.5f)pressure=-1.0f;}
  else if(q==1){if(fps<43.0f)pressure=1.0f;else if(fps>60.5f)pressure=-1.0f;}
  else {if(fps>49.0f)pressure=-1.0f;}
  if(pressure==0.0f)g_perf80_pressure=lerpf(g_perf80_pressure,0.0f,clampf(dt*3.0f,0,1));else g_perf80_pressure+=pressure*dt;
  if(g_perf80_pressure>.75f){if(q>0)q--;g_perf80_pressure=0;}else if(g_perf80_pressure<-.95f){if(q<2)q++;g_perf80_pressure=0;}
  g_perf80_quality=q;float target=q==0?.46f:q==1?.70f:1.0f;g_perf80_budget=lerpf(g_perf80_budget,target,clampf(dt*5.0f,0,1));
  g_perf80_ped_stride=q==0?4:q==1?2:1;g_perf80_parked_stride=q==0?4:q==1?2:1;
  g_perf80_exact_building_range=q==0?175.0f:q==1?255.0f:360.0f;
  g_minimap_range70=lerpf(g_minimap_range70,q==0?150.0f:q==1?185.0f:220.0f,clampf(dt*5.0f,0,1));
}
