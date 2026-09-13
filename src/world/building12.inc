#ifndef ITAJAI_BUILDING12_INCLUDED
#define ITAJAI_BUILDING12_INCLUDED 1
// -----------------------------------------------------------------------------
// 0.12 exact OSM building footprint sidecar.
// 4.0 also uses this exact-footprint store for whole-municipality GeoItajai
// cadastral polygons. Capacity is doubled so outer neighborhoods do not lose
// collision/render footprints when the municipal layer is loaded in full.
// -----------------------------------------------------------------------------
#define MAX_BUILDING_VERTS12 960000
#define BUILDING_HASH12 262144
static V3 g_building_verts12[MAX_BUILDING_VERTS12];
static int g_building_vert_count12=0;
static int g_building_poly_start12[MAX_BUILDINGS];
static unsigned short g_building_poly_count12[MAX_BUILDINGS];
static int g_building_seed_hash12[BUILDING_HASH12];
static int g_building_polys12=0;
static int g_building12_ready=0;

static u32 building12_hash(u32 s){s^=s>>16;s*=0x7feb352du;s^=s>>15;return s&(BUILDING_HASH12-1);}
static void building12_hash_build(void){for(int i=0;i<BUILDING_HASH12;i++)g_building_seed_hash12[i]=-1;for(int i=0;i<g_world.buildingCount;i++){u32 h=building12_hash(g_world.buildings[i].seed);for(int k=0;k<BUILDING_HASH12;k++){int q=(h+k)&(BUILDING_HASH12-1);if(g_building_seed_hash12[q]<0){g_building_seed_hash12[q]=i;break;}}}}
static int building12_find_seed(u32 s){u32 h=building12_hash(s);for(int k=0;k<BUILDING_HASH12;k++){int q=(h+k)&(BUILDING_HASH12-1),i=g_building_seed_hash12[q];if(i<0)return -1;if(i<g_world.buildingCount&&g_world.buildings[i].seed==s)return i;}return -1;}

static void building12_capture_way(const char*obj,const char*end,int*bi){char building[32]={0};if(!key_string(obj,end,"building",building,32))return;static V3 pts[2048];int n=parse_geometry_points(obj,end,pts,2048);if(n<3)return;float minx=pts[0].x,maxx=pts[0].x,minz=pts[0].z,maxz=pts[0].z;for(int i=1;i<n;i++){minx=minf(minx,pts[i].x);maxx=maxf(maxx,pts[i].x);minz=minf(minz,pts[i].z);maxz=maxf(maxz,pts[i].z);}float sx=maxx-minx,sz=maxz-minz;if(sx<1.5f||sz<1.5f||sx>220||sz>220)return;if(*bi>=g_world.buildingCount)return;int id=(*bi)++;if(n>1){V3 d=vsub(pts[n-1],pts[0]);if(vdot(d,d)<.30f*.30f)n--;}if(n<3)return;int keep=n;if(keep>96)keep=96;if(g_building_vert_count12+keep>MAX_BUILDING_VERTS12)return;g_building_poly_start12[id]=g_building_vert_count12;g_building_poly_count12[id]=(unsigned short)keep;for(int i=0;i<keep;i++)g_building_verts12[g_building_vert_count12++]=pts[i];g_building_polys12++;}

static void building12_prepare(void){if(g_building12_ready)return;g_building12_ready=1;g_building_vert_count12=0;g_building_polys12=0;memset(g_building_poly_start12,0,sizeof(g_building_poly_start12));memset(g_building_poly_count12,0,sizeof(g_building_poly_count12));building12_hash_build();char*json=NULL;DWORD size=0;if(!file_read_all(g_cache_path,&json,&size))return;const char*end=json+size,*el=find_range(json,end,"\"elements\"");if(!el){VirtualFree(json,0,MEM_RELEASE);return;}const char*p=find_range(el,end,"[");if(!p){VirtualFree(json,0,MEM_RELEASE);return;}p++;int inString=0,esc=0,depth=0,bi=0;char*objStart=NULL;for(char*c=(char*)p;c<(char*)end;c++){char ch=*c;if(inString){if(esc)esc=0;else if(ch=='\\')esc=1;else if(ch=='\"')inString=0;continue;}if(ch=='\"'){inString=1;continue;}if(ch=='{'){if(depth==0)objStart=c;depth++;}else if(ch=='}'){depth--;if(depth==0&&objStart){char save=c[1];c[1]=0;char typ[12]={0};if(key_string(objStart,c+1,"type",typ,12)&&str_eq(typ,"way"))building12_capture_way(objStart,c+1,&bi);c[1]=save;objStart=NULL;}}else if(ch==']'&&depth==0)break;}VirtualFree(json,0,MEM_RELEASE);}
#endif