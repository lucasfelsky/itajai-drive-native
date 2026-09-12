// -----------------------------------------------------------------------------
// 2.12 Traffic AI 2.0.
// Adds stable driver personalities, turn/lane-change intent and more varied
// following behaviour on top of the proven lane/connector simulation.
// -----------------------------------------------------------------------------
static unsigned char g_ai212_profile[MAX_TRAFFIC]; // 1 cautious, 2 normal, 3 assertive, 4 utility
static signed char g_ai212_indicator[MAX_TRAFFIC]; // -1 left, +1 right
static float g_ai212_stop_timer[MAX_TRAFFIC];
static int g_ai212_indicating=0,g_ai212_cautious=0,g_ai212_assertive=0;

static void traffic212_init_profile(int i){
  if(i<0||i>=MAX_TRAFFIC||g_ai212_profile[i])return;
  unsigned h=(unsigned)(i*1103515245u+12345u);int r=(int)((h>>16)&7u);
  g_ai212_profile[i]=(unsigned char)(r<2?1:r<6?2:r==6?3:4);
}

static void traffic212_post(float dt){
  g_ai212_indicating=0;g_ai212_cautious=0;g_ai212_assertive=0;
  for(int i=0;i<g_traffic_count;i++){
    Traffic*t=&g_traffic[i];traffic212_init_profile(i);g_ai212_indicator[i]=0;
    if(!t->active||t->routeN<=0)continue;
    int p=(int)g_ai212_profile[i];if(p==1)g_ai212_cautious++;if(p==3)g_ai212_assertive++;
    float targetCruise=p==1?.76f:p==2?.88f:p==3?.99f:.72f;
    // Rush hour keeps assertive/normal drivers moving while utility traffic remains slower.
    if((g_hour>=7&&g_hour<=9)||(g_hour>=17&&g_hour<=19)){if(p==2||p==3)targetCruise+=.03f;}
    g_traffic_cruise[i]=lerpf(g_traffic_cruise[i],targetCruise,clampf(dt*.35f,0,1));

    int eid=t->route[t->ri];if(eid<0||eid>=g_world.edgeCount)continue;Edge*e=&g_world.edges[eid];
    float remaining=(1.0f-t->t)*maxf(.1f,e->length);
    // Signal planned route turns before the connector. Cross sign follows the road graph convention.
    if(t->ri+1<t->routeN&&remaining<42.0f){float dot=1,cross=sim07_turn_cross(eid,t->route[t->ri+1],&dot);if(dot<.90f&&absf(cross)>.10f)g_ai212_indicator[i]=(signed char)(cross>0?-1:1);}
    // Lane-change indicator overrides turn intent while the visual lane is still transitioning.
    float laneDelta=(float)g_traffic_lane[i]-g_lane_visual17[i];if(absf(laneDelta)>.12f)g_ai212_indicator[i]=(signed char)(laneDelta>0?1:-1);
    if(g_ai212_indicator[i])g_ai212_indicating++;

    // Personality affects following margin without bypassing the base collision-safe gap logic.
    float gap=g_traffic_gap[i];float safe=4.0f+t->speed*(p==1?1.65f:p==3?1.05f:1.30f);
    if(gap<safe&&p==1)t->speed=sim_move_toward(t->speed,maxf(0,(gap-2.0f)*.55f),4.8f*dt);
    if(t->speed<.35f)g_ai212_stop_timer[i]+=dt;else g_ai212_stop_timer[i]=0;
  }
}

static void game_update(float dt){
  game_update_legacy211(dt);
  if(!g_started||g_settings_menu_open||g_editor19||g_photo70)return;
  traffic212_post(dt);
}
