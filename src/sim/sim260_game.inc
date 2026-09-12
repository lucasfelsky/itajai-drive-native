// -----------------------------------------------------------------------------
// 2.60 Traffic 3.0.
// Sector/time/weather-aware traffic rhythm layered on the collision-safe AI.
// It never disables the base routing logic; it only nudges cruise behavior and
// exposes queue/hazard telemetry for presentation and debugging.
// -----------------------------------------------------------------------------
static float g_traffic260_pressure=.65f,g_traffic260_avg_speed=0;
static int g_traffic260_queue=0,g_traffic260_hazard[MAX_TRAFFIC];

static float traffic260_hour_factor(void){
  if((g_hour>=7&&g_hour<10)||(g_hour>=16&&g_hour<20))return 1.18f;
  if(g_hour>=22||g_hour<5)return .58f;
  if(g_hour>=10&&g_hour<16)return .92f;
  return .78f;
}
static void traffic260_update(float dt){
  const UrbanSector10*s=&g_sectors10[g_sector10];float weather=g_weather==3?.70f:g_weather==2?.84f:1.0f;
  float target=clampf((.44f+s->urban*.58f+s->port*.24f)*traffic260_hour_factor()*weather,.28f,1.28f);
  g_traffic260_pressure=lerpf(g_traffic260_pressure,target,clampf(dt*.32f,0,1));g_traffic260_queue=0;float sum=0;int active=0;
  for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];g_traffic260_hazard[i]=0;if(!t->active)continue;active++;sum+=absf(t->speed);traffic212_init_profile(i);int p=(int)g_ai212_profile[i];float base=p==1?.76f:p==2?.88f:p==3?.99f:.72f;
    float sector=1.0f+s->coastal*.025f-s->port*.055f;float congestion=1.04f-.15f*clampf((g_traffic260_pressure-.45f)/.75f,0,1);if(g_weather>=2)sector-=.035f;float wanted=clampf(base*sector*congestion,.56f,1.03f);g_traffic_cruise[i]=lerpf(g_traffic_cruise[i],wanted,clampf(dt*.22f,0,1));
    if(g_ai212_stop_timer[i]>1.6f)g_traffic260_queue++;if(p==4&&g_ai212_stop_timer[i]>4.2f)g_traffic260_hazard[i]=1;
  }
  g_traffic260_avg_speed=active?sum/(float)active:0;
}
static void game_update(float dt){game_update_legacy250feel(dt);if(!g_started||g_settings_menu_open||g_editor19||g_photo70||g_garage215_open)return;traffic260_update(dt);}
