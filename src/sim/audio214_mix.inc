// -----------------------------------------------------------------------------
// 2.14 Audio 2.0 supplemental procedural mix.
// Keeps the dependency-free WinMM backend but adds tyre, wind, surface, shift and
// impact layers so the vehicle communicates grip and motion instead of only RPM.
// -----------------------------------------------------------------------------
static float g_audio214_tyre_level=0,g_audio214_wind_level=0,g_audio214_surface_level=0,g_audio214_transient=0;
static float g_audio214_squeal_phase=0,g_audio214_rumble_phase=0;
static int g_audio214_last_gear=0,g_audio214_last_contacts=0;

static float audio214_extra(float n,float speed,float throttle){
  float slip=absf(g_player.slip);for(int i=0;i<4;i++)slip=maxf(slip,absf(g_wheels15[i].slipLat)*.72f+absf(g_wheels15[i].slipLong)*.38f);
  float tyre=clampf((slip-.055f)*3.6f,0,.72f)*clampf((speed-2.0f)/18.0f,0,1);if(key_down(VK_SPACE))tyre=maxf(tyre,clampf(speed/24.0f,0,.58f));
  g_audio214_tyre_level=lerpf(g_audio214_tyre_level,tyre,.08f);
  float wind=clampf(speed/55.0f,0,1);wind=wind*wind*.045f;g_audio214_wind_level=lerpf(g_audio214_wind_level,wind,.025f);
  float surface=g_player.body.surface==SURFACE_CURB?.065f:g_player.body.surface==SURFACE_GRASS?.095f:0.0f;g_audio214_surface_level=lerpf(g_audio214_surface_level,surface,.06f);

  if(g_gear15!=g_audio214_last_gear){g_audio214_last_gear=g_gear15;g_audio214_transient=maxf(g_audio214_transient,.16f+.05f*throttle);}
  int contacts=g_last_static_contacts+g_last_dynamic_contacts;if(contacts>g_audio214_last_contacts)g_audio214_transient=maxf(g_audio214_transient,.20f);g_audio214_last_contacts=contacts;
  g_audio214_transient*=.9962f;

  g_audio214_squeal_phase=audio180_wrap(g_audio214_squeal_phase+6.2831853f*(620.0f+speed*7.0f)/AUDIO18_RATE);
  g_audio214_rumble_phase=audio180_wrap(g_audio214_rumble_phase+6.2831853f*(34.0f+speed*.85f)/AUDIO18_RATE);
  float squeal=((float)sin((double)g_audio214_squeal_phase)*.72f+n*.28f)*g_audio214_tyre_level*.10f;
  float rumble=((float)sin((double)g_audio214_rumble_phase)*.68f+n*.32f)*g_audio214_surface_level;
  float windNoise=n*g_audio214_wind_level;
  float transient=n*g_audio214_transient;
  return squeal+rumble+windNoise+transient;
}
