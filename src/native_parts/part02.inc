__declspec(dllimport) void APIENTRY glEnable(u32);
__declspec(dllimport) void APIENTRY glDisable(u32);
__declspec(dllimport) void APIENTRY glDepthFunc(u32);
__declspec(dllimport) void APIENTRY glBlendFunc(u32,u32);
__declspec(dllimport) void APIENTRY glViewport(int,int,int,int);
__declspec(dllimport) void APIENTRY glMatrixMode(u32);
__declspec(dllimport) void APIENTRY glLoadIdentity(void);
__declspec(dllimport) void APIENTRY glMultMatrixf(const float*);
__declspec(dllimport) void APIENTRY glOrtho(double,double,double,double,double,double);
__declspec(dllimport) void APIENTRY glFrustum(double,double,double,double,double,double);
__declspec(dllimport) void APIENTRY glBegin(u32);
__declspec(dllimport) void APIENTRY glEnd(void);
__declspec(dllimport) void APIENTRY glVertex2f(float,float);
__declspec(dllimport) void APIENTRY glVertex3f(float,float,float);
__declspec(dllimport) void APIENTRY glColor3f(float,float,float);
__declspec(dllimport) void APIENTRY glColor4f(float,float,float,float);
__declspec(dllimport) void APIENTRY glPushMatrix(void);
__declspec(dllimport) void APIENTRY glPopMatrix(void);
__declspec(dllimport) void APIENTRY glTranslatef(float,float,float);
__declspec(dllimport) void APIENTRY glRotatef(float,float,float,float);
__declspec(dllimport) void APIENTRY glScalef(float,float,float);
__declspec(dllimport) void APIENTRY glLineWidth(float);
__declspec(dllimport) void APIENTRY glRasterPos2f(float,float);
__declspec(dllimport) void APIENTRY glListBase(u32);
__declspec(dllimport) void APIENTRY glCallLists(int,u32,const void*);
__declspec(dllimport) u32 APIENTRY glGenLists(int);

// constants
#define CS_OWNDC 0x0020
#define WS_OVERLAPPEDWINDOW 0x00CF0000L
#define WS_VISIBLE 0x10000000L
#define WM_DESTROY 0x0002
#define WM_CLOSE 0x0010
#define WM_QUIT 0x0012
#define WM_SIZE 0x0005
#define PM_REMOVE 0x0001
#define SW_SHOW 5
#define PFD_DRAW_TO_WINDOW 0x00000004
#define PFD_SUPPORT_OPENGL 0x00000020
#define PFD_DOUBLEBUFFER 0x00000001
#define PFD_TYPE_RGBA 0
#define PFD_MAIN_PLANE 0
#define MEM_COMMIT 0x1000
#define MEM_RESERVE 0x2000
#define MEM_RELEASE 0x8000
#define GENERIC_READ 0x80000000u
#define GENERIC_WRITE 0x40000000u
#define FILE_SHARE_READ 0x00000001
#define OPEN_EXISTING 3
#define CREATE_ALWAYS 2
#define FILE_ATTRIBUTE_NORMAL 0x80
#define INVALID_HANDLE_VALUE ((HANDLE)(i64)-1)
#define INTERNET_DEFAULT_HTTPS_PORT 443
#define WINHTTP_FLAG_SECURE 0x00800000
#define WINHTTP_ACCESS_TYPE_DEFAULT_PROXY 0
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_DEPTH_TEST 0x0B71
#define GL_LEQUAL 0x0203
#define GL_BLEND 0x0BE2
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_PROJECTION 0x1701
#define GL_MODELVIEW 0x1700
#define GL_QUADS 0x0007
#define GL_LINES 0x0001
#define GL_LINE_STRIP 0x0003
#define GL_UNSIGNED_BYTE 0x1401
#define SYSTEM_FONT 13

#define VK_TAB 0x09
#define VK_SPACE 0x20
#define VK_ESCAPE 0x1B

int _fltused=0;

// -----------------------------------------------------------------------------
// math
// -----------------------------------------------------------------------------
typedef struct { float x,y,z; } V3;
static V3 v3(float x,float y,float z){V3 v={x,y,z};return v;}
static V3 vadd(V3 a,V3 b){return v3(a.x+b.x,a.y+b.y,a.z+b.z);}
static V3 vsub(V3 a,V3 b){return v3(a.x-b.x,a.y-b.y,a.z-b.z);}
static V3 vmul(V3 a,float s){return v3(a.x*s,a.y*s,a.z*s);}
static float vdot(V3 a,V3 b){return a.x*b.x+a.y*b.y+a.z*b.z;}
static V3 vcross(V3 a,V3 b){return v3(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);}
static float vlen(V3 a){return (float)sqrt((double)vdot(a,a));}
static V3 vnorm(V3 a){float l=vlen(a);return l>.0001f?vmul(a,1.0f/l):v3(0,0,0);}
static float clampf(float x,float a,float b){return x<a?a:(x>b?b:x);}
static float lerpf(float a,float b,float t){return a+(b-a)*t;}
static float absf(float a){return a<0?-a:a;}
static float minf(float a,float b){return a<b?a:b;}
static float maxf(float a,float b){return a>b?a:b;}

static u32 rng_state=0x91e10da5u;
static u32 rng_u32(void){rng_state^=rng_state<<13;rng_state^=rng_state>>17;rng_state^=rng_state<<5;return rng_state;}
static float rng01(void){return (rng_u32()&0xffff)/65535.0f;}

// -----------------------------------------------------------------------------
// world data - 3.3 capacity bump for municipal cadastral building coverage
// -----------------------------------------------------------------------------
#define MAX_ROAD_WAYS 8000
#define MAX_SEGMENTS 60000
#define MAX_BUILDINGS 40000
#define MAX_SIGNALS 2048
#define MAX_NODES 40000
#define MAX_EDGES 120000
#define MAX_TRAFFIC 48
#define MAX_ROUTE 1200
#define NODE_HASH_SIZE 131072

typedef struct { char name[64]; char type[24]; float width,speed; int oneway; } RoadWay;
typedef struct { V3 a,b; int road; } RoadSegment;
typedef struct { V3 c; float sx,sz,h; u32 seed; } Building;
typedef struct { V3 p; int node; } Signal;
typedef struct { V3 p; int head,headIn; int component; int outCount; int hashNext; } Node;
typedef struct { int from,to,next,nextIn,road; float length,speed,width; } Edge;

typedef struct {
  RoadWay roads[MAX_ROAD_WAYS]; int roadCount;
  RoadSegment segs[MAX_SEGMENTS]; int segCount;
  Building buildings[MAX_BUILDINGS]; int buildingCount;
  Signal signals[MAX_SIGNALS]; int signalCount;
  Node nodes[MAX_NODES]; int nodeCount;
  Edge edges[MAX_EDGES]; int edgeCount;
  int componentCount;
  int mapFromOSM;
} World;
static World g_world;
static volatile LONG g_world_state=0; // 0 load,1 ready,2 fallback
static volatile LONG g_load_stage=0;  // 0 cache,1 network,2 parse,3 graph,4 ready,5 fallback,7 municipal enrichment
static char g_cache_path[520];
static char g_cache_bin_path[520];
static char g_exe_dir[480];
static char g_post_body[8192];
static int g_node_hash[NODE_HASH_SIZE];

// -----------------------------------------------------------------------------
// string/file helpers
// -----------------------------------------------------------------------------
static void str_copy(char* d,const char* s,int cap){int i=0;if(cap<=0)return;while(s&&s[i]&&i<cap-1){d[i]=s[i];i++;}d[i]=0;}
static int str_eq(const char*a,const char*b){if(!a||!b)return 0;while(*a&&*b&&*a==*b){a++;b++;}return *a==*b;}
static const char* find_range(const char* a,const char* end,const char* needle){int n=(int)strlen(needle);if(n<=0)return a;for(const char*p=a;p+n<=end;p++){int ok=1;for(int i=0;i<n;i++)if(p[i]!=needle[i]){ok=0;break;}if(ok)return p;}return NULL;}
static const char* skip_ws(const char* p){while(*p==' '||*p=='\t'||*p=='\r'||*p=='\n')p++;return p;}
