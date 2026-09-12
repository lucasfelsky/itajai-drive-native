// -----------------------------------------------------------------------------
// 0.11 Terrain & Coastline, upgraded by Roads 2.0.
// Terrain remains analytic, but the road flattening plane now follows the same
// low-frequency terrain height instead of forcing every street to global Y=0.
// -----------------------------------------------------------------------------
static float g_water_depth11=0.0f;
static float g_terrain_height11=0.0f;
static int g_player_in_water11=0;

static float terrain11_shore_x(float z){
  // Art-directed east coast approximation, not a surveyed shoreline.
  float wave=(float)sin((double)(z*.00072f))*165.0f+(float)sin((double)(z*.00183f+1.7f))*72.0f;
  float north=clampf((z-1800.0f)/3600.0f,0,1);
  return 2350.0f+wave-210.0f*north;
}

static float terrain11_raw_height(float x,float z){
  float shore=terrain11_shore_x(z), inland=shore-x;
  if(inland<-35.0f)return -2.4f;
  float coastal=clampf(inland/520.0f,0,1);
  float base=.15f+coastal*.55f;
  // Rolling terrain farther inland; keep Centro/Porto comparatively flat.
  float west=clampf((-x-800.0f)/2600.0f,0,1);
  float north=clampf((z-1200.0f)/4700.0f,0,1);
  float hills=(float)(sin((double)(x*.00145f))+sin((double)(z*.00110f+1.2f)))*7.0f;
  hills+=(float)sin((double)((x+z)*.00062f))*11.0f;
  float ridge=maxf(0.0f,hills)*west*west*1.7f;
  float coastalHill=maxf(0.0f,(float)sin((double)(z*.00135f-.8f))*18.0f)*north*clampf((inland-120.0f)/900.0f,0,1);
  return base+ridge+coastalHill;
}

static float terrain11_road_grade(float x,float z){
  // The art terrain is intentionally low-frequency enough for road grades.
  // Clamp extremes so a routing segment never turns into a wall.
  return clampf(terrain11_raw_height(x,z),-.08f,16.0f);
}

static float terrain11_height(float x,float z){
  V3 p=v3(x,0,z);int road=-1;float center=9999.0f;
  foundation_nearest_road(p,&road,&center);
  if(road>=0){float flatten=g_world.roads[road].width*.5f+7.0f;if(center<flatten)return terrain11_road_grade(x,z);}
  return terrain11_raw_height(x,z);
}

static int terrain11_is_ocean(float x,float z){return x>terrain11_shore_x(z)+4.0f;}
static float terrain11_water_depth(float x,float z){float d=x-terrain11_shore_x(z);return d>0?clampf(d*.025f,0,4.5f):0;}
