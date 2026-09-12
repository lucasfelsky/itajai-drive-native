// -----------------------------------------------------------------------------
// 0.8 runtime asset pack. Source assets are glTF/GLB, compiled by
// tools/compile_assets.py into itajai_assets_v08.pak during CI.
// -----------------------------------------------------------------------------
#define MAX_ASSETS08 32

typedef struct {
  char name[32];
  u32 vertexCount,indexCount;
  float colliderRadius;
  V3 bmin,bmax;
  const float* vertices; // interleaved pos3 normal3 uv2
  const u32* indices;
} AssetMesh08;

static AssetMesh08 g_assets08[MAX_ASSETS08];
static int g_asset_count08=0;
static char* g_asset_pack_mem08=NULL;
static DWORD g_asset_pack_size08=0;
static char g_asset_pack_path08[520];
static int g_asset_pack_ok08=0;

static u32 asset08_u32(const u8*p){u32 v;memcpy(&v,p,4);return v;}
static u64 asset08_u64(const u8*p){u64 v;memcpy(&v,p,8);return v;}
static float asset08_f32(const u8*p){float v;memcpy(&v,p,4);return v;}

static void asset08_build_path(void){sprintf(g_asset_pack_path08,"%s\\itajai_assets_v08.pak",g_exe_dir);}
static int asset08_find(const char*name){for(int i=0;i<g_asset_count08;i++)if(str_eq(g_assets08[i].name,name))return i;return -1;}

static int asset08_load_pack(void){
  asset08_build_path();char*data=NULL;DWORD size=0;g_asset_pack_ok08=0;g_asset_count08=0;
  if(!file_read_all(g_asset_pack_path08,&data,&size)||size<16)return 0;
  if(memcpy==NULL){VirtualFree(data,0,MEM_RELEASE);return 0;} // keeps freestanding compiler happy about import use
  if(!(data[0]=='I'&&data[1]=='D'&&data[2]=='A'&&data[3]=='P'&&data[4]=='A'&&data[5]=='K'&&data[6]=='0'&&data[7]=='8')){VirtualFree(data,0,MEM_RELEASE);return 0;}
  const u8*p=(const u8*)data;u32 version=asset08_u32(p+8),count=asset08_u32(p+12);if(version!=1||count>MAX_ASSETS08){VirtualFree(data,0,MEM_RELEASE);return 0;}
  const u32 recSize=84;u64 recEnd=16ull+(u64)count*recSize;if(recEnd>size){VirtualFree(data,0,MEM_RELEASE);return 0;}
  for(u32 i=0;i<count;i++){
    const u8*r=p+16+i*recSize;AssetMesh08*a=&g_assets08[i];memset(a,0,sizeof(*a));for(int k=0;k<31&&r[k];k++)a->name[k]=(char)r[k];
    a->vertexCount=asset08_u32(r+32);a->indexCount=asset08_u32(r+36);a->colliderRadius=asset08_f32(r+40);
    a->bmin=v3(asset08_f32(r+44),asset08_f32(r+48),asset08_f32(r+52));a->bmax=v3(asset08_f32(r+56),asset08_f32(r+60),asset08_f32(r+64));
    u64 vo=asset08_u64(r+68),io=asset08_u64(r+76),vb=(u64)a->vertexCount*8ull*4ull,ib=(u64)a->indexCount*4ull;if(vo+vb>size||io+ib>size){VirtualFree(data,0,MEM_RELEASE);g_asset_count08=0;return 0;}
    a->vertices=(const float*)(data+vo);a->indices=(const u32*)(data+io);
  }
  g_asset_pack_mem08=data;g_asset_pack_size08=size;g_asset_count08=(int)count;g_asset_pack_ok08=1;return 1;
}

static void asset08_draw_mesh(int id,float r,float g,float b){
  if(id<0||id>=g_asset_count08)return;AssetMesh08*a=&g_assets08[id];glColor3f(r,g,b);glBegin(GL_TRIANGLES);
  for(u32 ii=0;ii<a->indexCount;ii++){u32 vi=a->indices[ii];if(vi>=a->vertexCount)continue;const float*v=a->vertices+vi*8;glNormal3f(v[3],v[4],v[5]);glTexCoord2f(v[6],v[7]);glVertex3f(v[0],v[1],v[2]);}
  glEnd();g_draw_calls++;
}

static void asset08_draw_instance(int id,V3 p,float yaw,float scale,float r,float g,float b){
  if(id<0||id>=g_asset_count08)return;glPushMatrix();glTranslatef(p.x,p.y,p.z);glRotatef(yaw*57.29578f,0,1,0);glScalef(scale,scale,scale);asset08_draw_mesh(id,r,g,b);glPopMatrix();
}
