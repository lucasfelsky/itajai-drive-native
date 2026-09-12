// -----------------------------------------------------------------------------
// 0.60 climate state: persistent wetness, wind, cloud response, lightning and
// a gently advancing clock. Manual T/Y controls remain authoritative.
// -----------------------------------------------------------------------------
static float g_wetness60=0.0f,g_cloud60=.12f,g_wind60=.18f,g_lightning60=0.0f;
static float g_clock_accum60=0.0f,g_lightning_timer60=4.5f;
static int g_last_weather60=-1;
static void weather60_update(float dt){
  float wetTarget=g_weather==0?0.0f:g_weather==1?.10f:g_weather==2?.65f:1.0f;
  float wetRate=wetTarget>g_wetness60?.16f:.018f;g_wetness60=sim_move_toward(g_wetness60,wetTarget,wetRate*dt);
  float cloudTarget=g_weather==0?.08f:g_weather==1?.48f:g_weather==2?.76f:.96f;g_cloud60=lerpf(g_cloud60,cloudTarget,clampf(dt*.45f,0,1));
  float windTarget=(g_weather==3?.88f:g_weather==2?.58f:g_weather==1?.32f:.18f)+.08f*(float)sin(g_time*.19);g_wind60=lerpf(g_wind60,clampf(windTarget,.08f,1.0f),clampf(dt*.6f,0,1));
  if(g_weather==3){g_lightning_timer60-=dt;if(g_lightning_timer60<=0){g_lightning60=1.0f;float q=.5f+.5f*(float)sin(g_time*1.731);g_lightning_timer60=5.5f+q*8.5f;}}else g_lightning_timer60=3.5f;
  g_lightning60=maxf(0,g_lightning60-dt*3.1f);
  g_clock_accum60+=dt;if(g_clock_accum60>=60.0f){g_clock_accum60-=60.0f;g_hour=(g_hour+1)%24;}
  g_last_weather60=g_weather;
}
