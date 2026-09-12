// -----------------------------------------------------------------------------
// 3.9 canonical world scope.
// Primary world = official Itajai municipal polygon (GeoItajai).
// Secondary playable extension = Navegantes / airport / BR-470 / BR-101 corridor.
// A small south exit remains visible context only and is not a GPS destination.
// -----------------------------------------------------------------------------
#define WORLD390_SCOPE_OUTSIDE 0
#define WORLD390_SCOPE_ITAJAI 1
#define WORLD390_SCOPE_NAVEGANTES 2
#define WORLD390_SCOPE_CONNECTOR 3
#define WORLD390_SCOPE_EXIT 4
#define WORLD390_BOUNDARY_MAGIC 0x39424449u /* IDB9 */
#define WORLD390_BOUNDARY_VERSION 1u
#define WORLD390_BOUNDARY_CAP 32768
#define WORLD390_HTTP_CAP (8u*1024u*1024u)

typedef struct { u32 magic,version,count,reserved; } World390BoundaryHeader;
static V3 g_world390_boundary[WORLD390_BOUNDARY_CAP];
static int g_world390_boundary_count=0;
static volatile LONG g_world390_boundary_ready=0; // 0 loading, 1 official/cache, -1 fallback
static int g_world390_boundary_live=0,g_world390_boundary_cached=0,g_world390_scope_thread_started=0;
static char g_world390_boundary_path[520];
static char g_world390_query_path[1200];
static u16 g_world390_http_wpath[1400];

static int world390_ascii_wide(const char*s,u16*out,int cap){int i=0;if(!s||cap<=0)return 0;while(s[i]&&i<cap-1){out[i]=(u16)(u8)s[i];i++;}out[i]=0;return i;}
static void world390_make_path(void){sprintf(g_world390_boundary_path,"%s\\itajai_boundary_v390.bin",g_exe_dir);}
static int world390_boundary_write(void){world390_make_path();HANDLE f=CreateFileA(g_world390_boundary_path,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);if(f==INVALID_HANDLE_VALUE)return 0;World390BoundaryHeader h={WORLD390_BOUNDARY_MAGIC,WORLD390_BOUNDARY_VERSION,(u32)g_world390_boundary_count,0};DWORD a=0,b=0;BOOL ok=WriteFile(f,&h,sizeof(h),&a,NULL)&&a==sizeof(h);DWORD bytes=(DWORD)(g_world390_boundary_count*sizeof(V3));if(ok&&bytes)ok=WriteFile(f,g_world390_boundary,bytes,&b,NULL)&&b==bytes;CloseHandle(f);return ok?1:0;}
static int world390_boundary_read(void){world390_make_path();HANDLE f=CreateFileA(g_world390_boundary_path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);if(f==INVALID_HANDLE_VALUE)return 0;World390BoundaryHeader h;DWORD got=0;BOOL ok=ReadFile(f,&h,sizeof(h),&got,NULL)&&got==sizeof(h)&&h.magic==WORLD390_BOUNDARY_MAGIC&&h.version==WORLD390_BOUNDARY_VERSION&&h.count>=4&&h.count<=WORLD390_BOUNDARY_CAP;if(!ok){CloseHandle(f);return 0;}DWORD bytes=(DWORD)(h.count*sizeof(V3));ok=ReadFile(f,g_world390_boundary,bytes,&got,NULL)&&got==bytes;CloseHandle(f);if(!ok)return 0;g_world390_boundary_count=(int)h.count;g_world390_boundary_cached=1;return 1;}

static int world390_http_get(const char*path,char**out,DWORD*outSize){if(!path||!out||!outSize)return 0;world390_ascii_wide(path,g_world390_http_wpath,1400);HINTERNET ses=WinHttpOpen(L"ItajaiDriveNative/3.9.0",WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,NULL,NULL,0);if(!ses)return 0;WinHttpSetTimeouts(ses,6000,10000,18000,45000);HINTERNET con=WinHttpConnect(ses,L"arcgis.itajai.sc.gov.br",INTERNET_DEFAULT_HTTPS_PORT,0);if(!con){WinHttpCloseHandle(ses);return 0;}HINTERNET req=WinHttpOpenRequest(con,L"GET",(LPCWSTR)g_world390_http_wpath,NULL,NULL,NULL,WINHTTP_FLAG_SECURE);if(!req){WinHttpCloseHandle(con);WinHttpCloseHandle(ses);return 0;}BOOL ok=WinHttpSendRequest(req,NULL,0,NULL,0,0,0)&&WinHttpReceiveResponse(req,NULL);if(!ok){WinHttpCloseHandle(req);WinHttpCloseHandle(con);WinHttpCloseHandle(ses);return 0;}char*data=(char*)VirtualAlloc(NULL,(SIZE_T)WORLD390_HTTP_CAP+1,MEM_COMMIT|MEM_RESERVE,4);if(!data){WinHttpCloseHandle(req);WinHttpCloseHandle(con);WinHttpCloseHandle(ses);return 0;}DWORD used=0;while(1){DWORD avail=0;if(!WinHttpQueryDataAvailable(req,&avail)){ok=0;break;}if(!avail){ok=1;break;}if(used+avail>WORLD390_HTTP_CAP){ok=0;break;}DWORD got=0;if(!WinHttpReadData(req,data+used,avail,&got)){ok=0;break;}used+=got;if(!got)break;}data[used]=0;WinHttpCloseHandle(req);WinHttpCloseHandle(con);WinHttpCloseHandle(ses);if(!ok||used<64){VirtualFree(data,0,MEM_RELEASE);return 0;}*out=data;*outSize=used;return 1;}

static int world390_parse_boundary(char*json,DWORD size){const char*end=json+size,*r=find_range(json,end,"\"rings\"");if(!r)return 0;const char*p=find_range(r,end,"[");if(!p)return 0;int depth=0,n=0;for(;p<end;p++){if(*p=='['){depth++;if(depth==3){const char*q=skip_ws(p+1);if(q>=end)break;double lon=atof(q);const char*comma=find_range(q,end,",");if(!comma)break;double lat=atof(comma+1);if(n<WORLD390_BOUNDARY_CAP)g_world390_boundary[n++]=project_geo(lat,lon);}}else if(*p==']'){if(depth==2)break;depth--;if(depth<0)break;}}if(n>1){V3 d=vsub(g_world390_boundary[n-1],g_world390_boundary[0]);if(vdot(d,d)<1.0f)n--;}if(n<4)return 0;g_world390_boundary_count=n;return 1;}

static int world390_point_in_boundary(V3 p){if(g_world390_boundary_count<4)return 0;int inside=0;for(int i=0,j=g_world390_boundary_count-1;i<g_world390_boundary_count;j=i++){V3 a=g_world390_boundary[i],b=g_world390_boundary[j];int cross=((a.z>p.z)!=(b.z>p.z));if(cross){float den=b.z-a.z;if(absf(den)<.0001f)den=den<0?-.0001f:.0001f;float x=a.x+(p.z-a.z)*(b.x-a.x)/den;if(p.x<x)inside=!inside;}}return inside;}
static int world390_geo_box(V3 p,double minLat,double minLon,double maxLat,double maxLon){V3 a=project_geo(minLat,minLon),b=project_geo(maxLat,maxLon);float minx=minf(a.x,b.x),maxx=maxf(a.x,b.x),minz=minf(a.z,b.z),maxz=maxf(a.z,b.z);return p.x>=minx&&p.x<=maxx&&p.z>=minz&&p.z<=maxz;}
static int world390_scope_at(V3 p){
  if(g_world390_boundary_count>=4&&world390_point_in_boundary(p))return WORLD390_SCOPE_ITAJAI;
  // Preserve the known Itajai urban core while the official polygon is loading.
  if(g_world390_boundary_count<4&&world390_geo_box(p,-26.9580,-48.6860,-26.8840,-48.6250))return WORLD390_SCOPE_ITAJAI;
  if(world390_geo_box(p,-26.9250,-48.6850,-26.8350,-48.6200))return WORLD390_SCOPE_NAVEGANTES;
  if(world390_geo_box(p,-26.9350,-48.7450,-26.8350,-48.6750))return WORLD390_SCOPE_CONNECTOR;
  if(world390_geo_box(p,-27.0200,-48.6550,-26.9500,-48.6100))return WORLD390_SCOPE_EXIT;
  // Offline fallback: official service extent keeps the municipality usable, but
  // explicit Navegantes/BR corridors above retain their own identity.
  if(g_world390_boundary_ready<0&&world390_geo_box(p,-27.1000,-48.8740,-26.8360,-48.6170))return WORLD390_SCOPE_ITAJAI;
  return WORLD390_SCOPE_OUTSIDE;
}
static int world390_destination_allowed(V3 p){int s=world390_scope_at(p);return s==WORLD390_SCOPE_ITAJAI||s==WORLD390_SCOPE_NAVEGANTES||s==WORLD390_SCOPE_CONNECTOR;}
static const char*world390_scope_name(int s){if(s==WORLD390_SCOPE_ITAJAI)return "ITAJAI // AREA PRINCIPAL";if(s==WORLD390_SCOPE_NAVEGANTES)return "NAVEGANTES // EXTENSAO JOGAVEL";if(s==WORLD390_SCOPE_CONNECTOR)return "BR-470 / BR-101 // CORREDOR";if(s==WORLD390_SCOPE_EXIT)return "SAIDA PARA BC // FORA DO MAPA";return "FORA DA AREA JOGAVEL";}

static DWORD WINAPI world390_scope_worker(LPVOID unused){(void)unused;g_world390_boundary_ready=0;g_world390_boundary_live=0;g_world390_boundary_cached=0;if(world390_boundary_read()){g_world390_boundary_ready=1;return 0;}sprintf(g_world390_query_path,"/server/rest/services/limite_municipal/FeatureServer/0/query?where=1%%3D1&outFields=id,text&returnGeometry=true&returnZ=false&outSR=4326&f=json");char*data=NULL;DWORD size=0;if(world390_http_get(g_world390_query_path,&data,&size)){if(world390_parse_boundary(data,size)){g_world390_boundary_live=1;world390_boundary_write();g_world390_boundary_ready=1;VirtualFree(data,0,MEM_RELEASE);return 0;}VirtualFree(data,0,MEM_RELEASE);}g_world390_boundary_ready=-1;return 0;}
static void world390_scope_start_async(void){if(g_world390_scope_thread_started)return;g_world390_scope_thread_started=1;HANDLE th=CreateThread(NULL,0,world390_scope_worker,NULL,0,NULL);if(th)CloseHandle(th);else g_world390_boundary_ready=-1;}
