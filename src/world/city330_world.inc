// -----------------------------------------------------------------------------
// 3.3 City Rebuild: municipal cadastral building enrichment.
//
// Roads/signals continue to come from OSM, while building footprints prefer the
// public GeoItajai Base_Cadastral_Edificacao FeatureServer. The municipal layer
// provides denser polygon coverage plus numpav (floors). Data is cached into the
// existing World cache and a small exact-footprint sidecar so normal launches do
// not depend on the municipal service.
//
// If GeoItajai is unavailable or incomplete, the existing OSM building path is
// retained unchanged. No proprietary map source is required.
// -----------------------------------------------------------------------------
#define CITY330_FOOT_MAGIC 0x33464449u /* IDF3 */
#define CITY330_FOOT_VERSION 1u
#define CITY330_PAGE_SIZE 2000
#define CITY330_MIN_MUNICIPAL_BUILDINGS 500
#define CITY330_HTTP_CAP (32u*1024u*1024u)
#define CITY330_RING_CAP 1024
#define CITY330_KEEP_VERTS 96

typedef struct {
  u32 magic,version,buildingCount,vertCount,municipalCount;
} City330FootHeader;

static Building g_city330_temp_buildings[MAX_BUILDINGS];
static int g_city330_temp_poly_start[MAX_BUILDINGS];
static unsigned short g_city330_temp_poly_count[MAX_BUILDINGS];
static V3 g_city330_temp_verts[MAX_BUILDING_VERTS12];
static int g_city330_temp_building_count=0;
static int g_city330_temp_vert_count=0;
static int g_city330_municipal_buildings=0;
static int g_city330_municipal_pages=0;
static int g_city330_municipal_cached=0;
static int g_city330_municipal_live=0;
static char g_city330_foot_path[520];

static u32 city330_mix(u32 x){x^=x>>16;x*=0x7feb352du;x^=x>>15;x*=0x846ca68bu;x^=x>>16;return x;}
static char city330_lower(char c){return c>='A'&&c<='Z'?(char)(c-'A'+'a'):c;}
static int city330_contains_ci(const char*s,const char*n){if(!s||!n||!n[0])return 0;for(int i=0;s[i];i++){int j=0;while(n[j]&&s[i+j]&&city330_lower(s[i+j])==city330_lower(n[j]))j++;if(!n[j])return 1;}return 0;}
static int city330_ascii_wide(const char*s,u16*out,int cap){int i=0;if(!s||cap<=0)return 0;while(s[i]&&i<cap-1){out[i]=(u16)(u8)s[i];i++;}out[i]=0;return i;}

static void city330_temp_reset(void){g_city330_temp_building_count=0;g_city330_temp_vert_count=0;memset(g_city330_temp_poly_start,0,sizeof(g_city330_temp_poly_start));memset(g_city330_temp_poly_count,0,sizeof(g_city330_temp_poly_count));}
static void city330_exact_reset(void){g_building_vert_count12=0;g_building_polys12=0;g_building12_ready=0;memset(g_building_poly_start12,0,sizeof(g_building_poly_start12));memset(g_building_poly_count12,0,sizeof(g_building_poly_count12));}
static void city330_make_paths(void){sprintf(g_city330_foot_path,"%s\\itajai_buildings_v330.bin",g_exe_dir);}

static int city330_sidecar_write(void){
  city330_make_paths();HANDLE f=CreateFileA(g_city330_foot_path,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);if(f==INVALID_HANDLE_VALUE)return 0;
  City330FootHeader h={CITY330_FOOT_MAGIC,CITY330_FOOT_VERSION,(u32)g_world.buildingCount,(u32)g_building_vert_count12,(u32)g_city330_municipal_buildings};DWORD wrote=0;BOOL ok=WriteFile(f,&h,sizeof(h),&wrote,NULL)&&wrote==sizeof(h);
  DWORD a=(DWORD)(g_world.buildingCount*sizeof(g_building_poly_start12[0]));if(ok&&a)ok=WriteFile(f,g_building_poly_start12,a,&wrote,NULL)&&wrote==a;
  DWORD b=(DWORD)(g_world.buildingCount*sizeof(g_building_poly_count12[0]));if(ok&&b)ok=WriteFile(f,g_building_poly_count12,b,&wrote,NULL)&&wrote==b;
  DWORD c=(DWORD)(g_building_vert_count12*sizeof(g_building_verts12[0]));if(ok&&c)ok=WriteFile(f,g_building_verts12,c,&wrote,NULL)&&wrote==c;CloseHandle(f);return ok?1:0;
}

static int city330_sidecar_read(void){
  city330_make_paths();HANDLE f=CreateFileA(g_city330_foot_path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);if(f==INVALID_HANDLE_VALUE)return 0;
  City330FootHeader h;DWORD got=0;BOOL ok=ReadFile(f,&h,sizeof(h),&got,NULL)&&got==sizeof(h)&&h.magic==CITY330_FOOT_MAGIC&&h.version==CITY330_FOOT_VERSION&&h.buildingCount==(u32)g_world.buildingCount&&h.buildingCount<=MAX_BUILDINGS&&h.vertCount<=MAX_BUILDING_VERTS12&&h.municipalCount>=CITY330_MIN_MUNICIPAL_BUILDINGS;
  if(!ok){CloseHandle(f);return 0;}city330_exact_reset();
  DWORD a=(DWORD)(h.buildingCount*sizeof(g_building_poly_start12[0]));if(a)ok=ReadFile(f,g_building_poly_start12,a,&got,NULL)&&got==a;
  DWORD b=(DWORD)(h.buildingCount*sizeof(g_building_poly_count12[0]));if(ok&&b)ok=ReadFile(f,g_building_poly_count12,b,&got,NULL)&&got==b;
  DWORD c=(DWORD)(h.vertCount*sizeof(g_building_verts12[0]));if(ok&&c)ok=ReadFile(f,g_building_verts12,c,&got,NULL)&&got==c;CloseHandle(f);if(!ok){city330_exact_reset();return 0;}
  g_building_vert_count12=(int)h.vertCount;g_building_polys12=0;for(int i=0;i<g_world.buildingCount;i++)if(g_building_poly_count12[i]>=3)g_building_polys12++;
  g_city330_municipal_buildings=(int)h.municipalCount;g_city330_municipal_cached=1;g_building12_ready=1;building12_hash_build();return 1;
}

static int city330_http_get(const char*path,char**out,DWORD*outSize){
  if(!path||!out||!outSize)return 0;u16 wpath[1600];city330_ascii_wide(path,wpath,1600);
  HINTERNET ses=WinHttpOpen(L"ItajaiDriveNative/3.3.0",WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,NULL,NULL,0);if(!ses)return 0;WinHttpSetTimeouts(ses,7000,12000,25000,70000);
  HINTERNET con=WinHttpConnect(ses,L"arcgis.itajai.sc.gov.br",INTERNET_DEFAULT_HTTPS_PORT,0);if(!con){WinHttpCloseHandle(ses);return 0;}
  HINTERNET req=WinHttpOpenRequest(con,L"GET",(LPCWSTR)wpath,NULL,NULL,NULL,WINHTTP_FLAG_SECURE);if(!req){WinHttpCloseHandle(con);WinHttpCloseHandle(ses);return 0;}
  BOOL ok=WinHttpSendRequest(req,NULL,0,NULL,0,0,0)&&WinHttpReceiveResponse(req,NULL);if(!ok){WinHttpCloseHandle(req);WinHttpCloseHandle(con);WinHttpCloseHandle(ses);return 0;}
  char*data=(char*)VirtualAlloc(NULL,(SIZE_T)CITY330_HTTP_CAP+1,MEM_COMMIT|MEM_RESERVE,4);if(!data){WinHttpCloseHandle(req);WinHttpCloseHandle(con);WinHttpCloseHandle(ses);return 0;}DWORD used=0;
  while(1){DWORD avail=0;if(!WinHttpQueryDataAvailable(req,&avail)){ok=0;break;}if(!avail){ok=1;break;}if(used+avail>CITY330_HTTP_CAP){ok=0;break;}DWORD got=0;if(!WinHttpReadData(req,data+used,avail,&got)){ok=0;break;}used+=got;if(!got)break;}
  data[used]=0;WinHttpCloseHandle(req);WinHttpCloseHandle(con);WinHttpCloseHandle(ses);if(!ok||used<32){VirtualFree(data,0,MEM_RELEASE);return 0;}*out=data;*outSize=used;return 1;
}

static int city330_parse_first_ring(const char*obj,const char*end,V3*pts,int cap){
  const char*r=find_range(obj,end,"\"rings\"");if(!r)return 0;const char*p=find_range(r,end,"[");if(!p)return 0;int depth=0,n=0;
  for(;p<end;p++){
    if(*p=='['){depth++;if(depth==3){const char*q=skip_ws(p+1);if(q>=end)break;double lon=atof(q);const char*comma=find_range(q,end,",");if(!comma)break;double lat=atof(comma+1);if(n<cap)pts[n++]=project_geo(lat,lon);}}
    else if(*p==']'){if(depth==2)break;depth--;if(depth<0)break;}
  }
  if(n>1){V3 d=vsub(pts[n-1],pts[0]);if(vdot(d,d)<.25f*.25f)n--;}return n;
}

static float city330_polygon_area(const V3*p,int n){double a=0;for(int i=0,j=n-1;i<n;j=i++)a+=(double)p[j].x*p[i].z-(double)p[i].x*p[j].z;return (float)(a<0?-a*.5:a*.5);}
static int city330_building_class(const char*name,int floors,float area){
  if(name&&(city330_contains_ci(name,"igreja")||city330_contains_ci(name,"hospital")||city330_contains_ci(name,"escola")||city330_contains_ci(name,"colegio")||city330_contains_ci(name,"universidade")||city330_contains_ci(name,"prefeitura")||city330_contains_ci(name,"museu")))return BLD10_CIVIC;
  if(floors>=4)return BLD10_APARTMENT;if(area>750.0f&&floors<=2)return BLD10_INDUSTRIAL;if(area>260.0f)return BLD10_COMMERCIAL;return BLD10_HOUSE;
}
static float city330_height_for(int cls,int floors,u32 id){
  float q=(city330_mix(id)&65535u)/65535.0f;if(floors>0)return clampf(floors*3.05f+.45f,3.2f,160.0f);
  if(cls==BLD10_HOUSE)return 4.6f+q*3.0f;if(cls==BLD10_APARTMENT)return 13.0f+q*22.0f;if(cls==BLD10_COMMERCIAL)return 6.2f+q*10.0f;if(cls==BLD10_INDUSTRIAL)return 5.8f+q*6.5f;if(cls==BLD10_CIVIC)return 8.0f+q*16.0f;return 6.0f+q*8.0f;
}

static int city330_parse_feature(const char*obj,const char*end){
  if(g_city330_temp_building_count>=MAX_BUILDINGS)return 0;static V3 pts[CITY330_RING_CAP];int n=city330_parse_first_ring(obj,end,pts,CITY330_RING_CAP);if(n<3)return 0;
  float minx=pts[0].x,maxx=pts[0].x,minz=pts[0].z,maxz=pts[0].z,cx=0,cz=0;for(int i=0;i<n;i++){minx=minf(minx,pts[i].x);maxx=maxf(maxx,pts[i].x);minz=minf(minz,pts[i].z);maxz=maxf(maxz,pts[i].z);cx+=pts[i].x;cz+=pts[i].z;}cx/=n;cz/=n;float sx=maxx-minx,sz=maxz-minz;if(sx<1.2f||sz<1.2f||sx>320.0f||sz>320.0f)return 0;
  float area=city330_polygon_area(pts,n);if(area<3.0f)return 0;u32 oid=(u32)key_number(obj,end,"objectid",(double)(g_city330_temp_building_count+1));char name[64]={0},fs[24]={0};key_string(obj,end,"nome",name,64);key_string(obj,end,"numpav",fs,24);int floors=fs[0]?(int)(atof(fs)+.5):0;if(floors<0||floors>80)floors=0;int cls=city330_building_class(name,floors,area);float h=city330_height_for(cls,floors,oid);
  int keep=n;if(keep>CITY330_KEEP_VERTS)keep=CITY330_KEEP_VERTS;if(g_city330_temp_vert_count+keep>MAX_BUILDING_VERTS12)return 0;int id=g_city330_temp_building_count++;Building*b=&g_city330_temp_buildings[id];b->c=v3(cx,h*.5f,cz);b->sx=sx;b->sz=sz;b->h=h;b->seed=(city330_mix(oid)&0x0fffffffu)|(((u32)cls&15u)<<28);
  g_city330_temp_poly_start[id]=g_city330_temp_vert_count;g_city330_temp_poly_count[id]=(unsigned short)keep;for(int i=0;i<keep;i++){int src=(i*n)/keep;if(src>=n)src=n-1;g_city330_temp_verts[g_city330_temp_vert_count++]=pts[src];}return 1;
}

static int city330_parse_page(char*json,DWORD size,int*seenOut){
  const char*end=json+size,*f=find_range(json,end,"\"features\"");if(!f)return 0;const char*p=find_range(f,end,"[");if(!p)return 0;p++;int inString=0,esc=0,depth=0,accepted=0,seen=0;char*objStart=NULL;
  for(char*c=(char*)p;c<(char*)end;c++){char ch=*c;if(inString){if(esc)esc=0;else if(ch=='\\')esc=1;else if(ch=='\"')inString=0;continue;}if(ch=='\"'){inString=1;continue;}if(ch=='{'){if(depth==0)objStart=c;depth++;}else if(ch=='}'){depth--;if(depth==0&&objStart){seen++;char save=c[1];c[1]=0;accepted+=city330_parse_feature(objStart,c+1);c[1]=save;objStart=NULL;}}else if(ch==']'&&depth==0)break;}
  if(seenOut)*seenOut=seen;return accepted;
}

static int city330_fetch_municipal_temp(void){
  city330_temp_reset();g_city330_municipal_pages=0;int complete=0;
  for(int page=0;page<32&&g_city330_temp_building_count<MAX_BUILDINGS;page++){
    int offset=page*CITY330_PAGE_SIZE;char path[1400];sprintf(path,"/server/rest/services/Base_Cadastral_Edificacao/FeatureServer/0/query?where=1=1&geometry=-48.6860,-26.9580,-48.6250,-26.8840&geometryType=esriGeometryEnvelope&inSR=4326&spatialRel=esriSpatialRelIntersects&outFields=objectid,nome,tipouso,numpav&returnGeometry=true&returnZ=false&outSR=4326&orderByFields=objectid&resultOffset=%d&resultRecordCount=%d&f=json",offset,CITY330_PAGE_SIZE);
    char*data=NULL;DWORD size=0;if(!city330_http_get(path,&data,&size))break;int seen=0;city330_parse_page(data,size,&seen);VirtualFree(data,0,MEM_RELEASE);g_city330_municipal_pages++;if(seen<CITY330_PAGE_SIZE){complete=1;break;}
  }
  return complete&&g_city330_temp_building_count>=CITY330_MIN_MUNICIPAL_BUILDINGS;
}

static void city330_commit_municipal(void){
  g_world.buildingCount=g_city330_temp_building_count;memcpy(g_world.buildings,g_city330_temp_buildings,(SIZE_T)g_world.buildingCount*sizeof(Building));city330_exact_reset();
  g_building_vert_count12=g_city330_temp_vert_count;memcpy(g_building_poly_start12,g_city330_temp_poly_start,(SIZE_T)g_world.buildingCount*sizeof(g_building_poly_start12[0]));memcpy(g_building_poly_count12,g_city330_temp_poly_count,(SIZE_T)g_world.buildingCount*sizeof(g_building_poly_count12[0]));memcpy(g_building_verts12,g_city330_temp_verts,(SIZE_T)g_building_vert_count12*sizeof(g_building_verts12[0]));
  g_building_polys12=g_world.buildingCount;g_building12_ready=1;g_city330_municipal_buildings=g_world.buildingCount;g_city330_municipal_live=1;building12_hash_build();
}

static int city330_try_enrich(void){if(!city330_fetch_municipal_temp())return 0;city330_commit_municipal();world_cache_write();city330_sidecar_write();return 1;}

static DWORD WINAPI city330_map_worker(LPVOID unused){
  (void)unused;char*data=NULL;DWORD size=0;g_city330_municipal_buildings=0;g_city330_municipal_cached=0;g_city330_municipal_live=0;g_load_stage=0;
  if(world_cache_read()){
    if(!city330_sidecar_read())city330_try_enrich();g_load_stage=4;g_world_state=g_world.mapFromOSM?1:2;return 0;
  }
  if(file_read_all(g_cache_path,&data,&size)){
    g_load_stage=2;if(parse_osm(data,size)){g_load_stage=3;compute_components();bind_signals();if(!city330_try_enrich())world_cache_write();VirtualFree(data,0,MEM_RELEASE);g_load_stage=4;g_world_state=1;return 0;}VirtualFree(data,0,MEM_RELEASE);
  }
  g_load_stage=1;if(fetch_host(L"overpass-api.de",&data,&size)||fetch_host(L"overpass.kumi.systems",&data,&size)){
    file_write_all(g_cache_path,data,size);g_load_stage=2;if(parse_osm(data,size)){g_load_stage=3;compute_components();bind_signals();if(!city330_try_enrich())world_cache_write();VirtualFree(data,0,MEM_RELEASE);g_load_stage=4;g_world_state=1;return 0;}VirtualFree(data,0,MEM_RELEASE);
  }
  g_load_stage=5;build_fallback_world();world_cache_write();g_world_state=2;return 0;
}
