// -----------------------------------------------------------------------------
// 0.10 urban sectors and runtime policy.
// Coordinates are projected into the engine's local Itajai frame. These are
// art-direction regions, not administrative neighborhood boundaries.
// -----------------------------------------------------------------------------
typedef struct { const char*name; V3 c; float coastal,green,port,urban; } UrbanSector10;
static const UrbanSector10 g_sectors10[]={
  {"CENTRO",                 {-298,0,0},      .05f,.22f,.12f,1.00f},
  {"BEIRA-RIO / FAZENDA",    {-99,0,1002},    .18f,.72f,.05f,.72f},
  {"PORTO",                  {-1489,0,-1336}, .02f,.08f,1.00f,.62f},
  {"MOLHES",                 {1290,0,-891},   .82f,.42f,.18f,.38f},
  {"ATALAIA",                {2482,0,334},    .90f,.68f,.02f,.38f},
  {"CABECUDAS",              {2680,0,2004},   1.00f,.76f,.01f,.32f},
  {"PRAIA BRAVA",            {2283,0,5343},   1.00f,.70f,.01f,.56f}
};
#define URBAN10_SECTOR_COUNT ((int)(sizeof(g_sectors10)/sizeof(g_sectors10[0])))
static int g_sector10=0;static float g_sector_distance10=0;static float g_traffic_recycle_timer10=0;

static int urban10_sector_at(V3 p,float*outDistance){float best=1e30f;int bi=0;for(int i=0;i<URBAN10_SECTOR_COUNT;i++){float dx=p.x-g_sectors10[i].c.x,dz=p.z-g_sectors10[i].c.z,d=dx*dx+dz*dz;if(d<best){best=d;bi=i;}}if(outDistance)*outDistance=(float)sqrt((double)best);return bi;}
static const UrbanSector10* urban10_sector_info(V3 p){return &g_sectors10[urban10_sector_at(p,NULL)];}
static const char* urban10_sector_name(void){return g_sectors10[g_sector10].name;}
static void urban10_update_sector(void){g_sector10=urban10_sector_at(g_player.body.p,&g_sector_distance10);}

static void init_paths(void){char exe[520];DWORD n=GetModuleFileNameA(NULL,exe,520);if(n>=519)n=519;exe[n]=0;int cut=(int)n-1;while(cut>=0&&exe[cut]!='\\'&&exe[cut]!='/')cut--;if(cut<0)str_copy(g_exe_dir,".",480);else{if(cut>=479)cut=479;for(int i=0;i<cut;i++)g_exe_dir[i]=exe[i];g_exe_dir[cut]=0;}sprintf(g_cache_path,"%s\\itajai_osm_cache_v10.json",g_exe_dir);sprintf(g_cache_bin_path,"%s\\itajai_world_v10.bin",g_exe_dir);sprintf(g_cache_bin_legacy_path,"%s\\itajai_world_v05.bin",g_exe_dir);}

static void urban10_recycle_traffic(float dt){g_traffic_recycle_timer10-=dt;if(g_traffic_recycle_timer10>0)return;g_traffic_recycle_timer10=1.8f;int base=nearest_node(g_player.body.p);int comp=base>=0?g_world.nodes[base].component:-1;for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];float dx=t->p.x-g_player.body.p.x,dz=t->p.z-g_player.body.p.z;int recycle=!t->active||(dx*dx+dz*dz>1250.0f*1250.0f);if(!recycle)continue;int s=-1;for(int k=0;k<500;k++){if(g_world.nodeCount<=0)break;int n=(int)(rng01()*g_world.nodeCount);if(n>=g_world.nodeCount)n=g_world.nodeCount-1;Node*nn=&g_world.nodes[n];if(nn->outCount<=0||(comp>=0&&nn->component!=comp))continue;float qx=nn->p.x-g_player.body.p.x,qz=nn->p.z-g_player.body.p.z,floatD=qx*qx+qz*qz;if(floatD>140.0f*140.0f&&floatD<900.0f*900.0f){s=n;break;}}if(s<0)s=base;if(s>=0){memset(t,0,sizeof(*t));traffic_new_route(t,s);if(t->active){t->speed=4.0f+rng01()*6.5f;t->p=g_world.nodes[s].p;}}}}
