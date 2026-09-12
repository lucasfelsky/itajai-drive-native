// -----------------------------------------------------------------------------
// 1.0 vertical-slice integration state, guided landmark tour and self-check.
// -----------------------------------------------------------------------------
static int g_help100=1,g_tour100_index=-1,g_selfcheck100=0;
static char g_tour100_name[64]="LIVRE";
typedef struct { const char*name; V3 p; } TourStop100;
static const TourStop100 g_tour100[]={
  {"CENTRO",{-298,0,0}},
  {"BEIRA-RIO / FAZENDA",{-99,0,1002}},
  {"PORTO",{-1489,0,-1336}},
  {"MOLHES",{1290,0,-891}},
  {"ATALAIA",{2482,0,334}},
  {"CABECUDAS",{2680,0,2004}},
  {"PRAIA BRAVA",{2283,0,5343}}
};
#define TOUR100_COUNT ((int)(sizeof(g_tour100)/sizeof(g_tour100[0])))
static int final100_system_check(void){int n=0;if(g_world.roadCount>0&&g_world.segCount>0)n++;if(g_world.nodeCount>0&&g_world.edgeCount>0)n++;if(g_static_collider_count>0)n++;if(g_asset_pack_ok08)n++;if(g_glrc!=NULL)n++;if(g_audio_started18)n++;if(g_landmark_count19>0)n++;if(g_life50_init)n++;if(g_stream_ready08)n++;if(g_building_polys12>0)n++;return n;}
static void final100_route_next(void){
  if(g_world.nodeCount<=0)return;int start=nearest_node(g_player.body.p);if(start<0)return;
  for(int tries=0;tries<TOUR100_COUNT;tries++){g_tour100_index=(g_tour100_index+1)%TOUR100_COUNT;int goal=nearest_node(g_tour100[g_tour100_index].p);if(goal<0||g_world.nodes[goal].component!=g_world.nodes[start].component)continue;int n=astar_route(start,goal,g_gps_route,MAX_ROUTE);if(n>0){g_gps_n=n;g_gps_goal07=goal;g_gps_offroute07=0;g_gps_recalc_timer07=.4f;g_gps_length07=sim07_route_length(g_gps_route,g_gps_n);str_copy(g_tour100_name,g_tour100[g_tour100_index].name,64);return;}}
  str_copy(g_tour100_name,"SEM ROTA CONECTADA",64);
}
