// -----------------------------------------------------------------------------
// 2.11 Vehicle Visual 2.0.
// Resolves the named body/glass/trim asset contract introduced in 2.10 and layers
// those independent meshes on player/traffic. Procedural lights/wheels remain as
// animation/detail fallback until hand-authored assets replace generated parts.
// -----------------------------------------------------------------------------
static const char*g_vehicle211_stem[CAR160_COUNT]={
  "fiat_uno_way_2014","vw_gol_g6_2016","hyundai_hb20_2024","fiat_strada_2024",
  "toyota_corolla_2020","jeep_renegade_2021","chevrolet_onix_2024","chevrolet_celta_2015"};
static int g_vehicle211_glass[CAR160_COUNT]={-2,-2,-2,-2,-2,-2,-2,-2};
static int g_vehicle211_trim[CAR160_COUNT]={-2,-2,-2,-2,-2,-2,-2,-2};
static int g_vehicle211_parts_ready=0,g_vehicle211_part_draws=0;
static void vehicle211_resolve(void){char n[64];g_vehicle211_parts_ready=0;for(int i=0;i<CAR160_COUNT;i++){if(g_vehicle211_glass[i]==-2){sprintf(n,"glass_%s",g_vehicle211_stem[i]);g_vehicle211_glass[i]=asset08_find(n);}if(g_vehicle211_trim[i]==-2){sprintf(n,"trim_%s",g_vehicle211_stem[i]);g_vehicle211_trim[i]=asset08_find(n);}if(g_vehicle211_glass[i]>=0&&g_vehicle211_trim[i]>=0)g_vehicle211_parts_ready++;}}
static void vehicle211_parts_local(int model){vehicle211_resolve();if(model<0||model>=CAR160_COUNT)return;if(g_vehicle211_glass[model]>=0){renderer09_set_material(MAT09_GLASS);asset08_draw_mesh(g_vehicle211_glass[model],.075f,.13f,.17f);g_vehicle211_part_draws++;}if(g_vehicle211_trim[model]>=0){renderer09_set_material(MAT09_RUBBER);asset08_draw_mesh(g_vehicle211_trim[model],.09f,.095f,.10f);g_vehicle211_part_draws++;}renderer09_set_material(MAT09_GENERIC);}
static void vehicle211_draw_player_parts(void){if(!g_started||g_player.camera==2)return;int model=g_player_model160[g_player.car&3];glPushMatrix();glTranslatef(g_player.body.p.x,g_player.body.p.y-.66f,g_player.body.p.z);glRotatef(g_player.body.yaw*57.29578f,0,1,0);glRotatef(g_player.pitch*57.29578f,1,0,0);glRotatef(g_player.roll*57.29578f,0,0,1);vehicle211_parts_local(model);glPopMatrix();}
static void vehicle211_draw_traffic_parts(void){vehicle211_resolve();float maxD=minf(g_lod20_traffic_dist,95.0f);maxD*=maxD;for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active)continue;float dx=t->p.x-g_player.body.p.x,dz=t->p.z-g_player.body.p.z;if(dx*dx+dz*dz>maxD)continue;int model=(i*5+i/3)&7;glPushMatrix();glTranslatef(t->p.x,t->p.y-.55f,t->p.z);glRotatef(t->yaw*57.29578f,0,1,0);vehicle211_parts_local(model);glPopMatrix();}}
