// -----------------------------------------------------------------------------
// 0.8 regional render/object streaming.
// The routing graph remains resident; render geometry is cached by 320m regions
// and read on demand from itajai_regions_v08.bin. 0.10 raises capacities only;
// the disk format itself stays compatible.
// -----------------------------------------------------------------------------
#define REGION_SIZE08 320.0f
#define REGION_RADIUS08 2
#define MAX_REGIONS08 1024
#define MAX_STREAM_ROADS08 30000
#define MAX_STREAM_BUILDINGS08 12000
#define MAX_ASSET_INSTANCES08 5000

#pragma pack(push,1)
typedef struct { char magic[8]; u32 version,regionCount,worldSegCount,worldBuildingCount; } RegionHeader08;
typedef struct { i32 rx,rz; u32 roadCount,buildingCount; u64 roadOffset,buildingOffset; } RegionMeta08;
typedef struct { V3 a,b; i32 road; } StreamRoad08;
typedef struct { Building b; } StreamBuilding08;
#pragma pack(pop)

typedef struct { int asset; V3 p; float yaw,scale; u32 seed; } AssetInstance08;

static RegionMeta08 g_regions08[MAX_REGIONS08];static int g_region_count08=0;
static HANDLE g_region_file08=INVALID_HANDLE_VALUE;static char g_region_path08[520];
static StreamRoad08 g_stream_roads08[MAX_STREAM_ROADS08];static int g_stream_road_count08=0;
static StreamBuilding08 g_stream_buildings08[MAX_STREAM_BUILDINGS08];static int g_stream_building_count08=0;
static AssetInstance08 g_asset_instances08[MAX_ASSET_INSTANCES08];static int g_asset_instance_count08=0;
static int g_stream_region_x08=0x7fffffff,g_stream_region_z08=0x7fffffff,g_stream_loaded_regions08=0;
static float g_stream_refresh08=0;static int g_stream_asset_contacts08=0;

static int stream08_coord(float x){int q=(int)(x/REGION_SIZE08);if(x<0&&((float)q*REGION_SIZE08)>x)q--;return q;}
static int stream08_find_region(int rx,int rz){for(int i=0;i<g_region_count08;i++)if(g_regions08[i].rx==rx&&g_regions08[i].rz==rz)return i;return -1;}
static int stream08_add_region(int rx,int rz){int i=stream08_find_region(rx,rz);if(i>=0)return i;if(g_region_count08>=MAX_REGIONS08)return -1;i=g_region_count08++;memset(&g_regions08[i],0,sizeof(g_regions08[i]));g_regions08[i].rx=rx;g_regions08[i].rz=rz;return i;}
static u32 stream08_hash(i32 a,i32 b,u32 salt){u32 x=(u32)a*0x9e3779b1u^(u32)b*0x85ebca6bu^salt;x^=x>>16;x*=0x7feb352du;x^=x>>15;x*=0x846ca68bu;x^=x>>16;return x;}

static int stream08_build_cache(void){
  g_region_count08=0;
  for(int i=0;i<g_world.segCount;i++){V3 m=vmul(vadd(g_world.segs[i].a,g_world.segs[i].b),.5f);int ri=stream08_add_region(stream08_coord(m.x),stream08_coord(m.z));if(ri>=0)g_regions08[ri].roadCount++;}
  for(int i=0;i<g_world.buildingCount;i++){Building*b=&g_world.buildings[i];int ri=stream08_add_region(stream08_coord(b->c.x),stream08_coord(b->c.z));if(ri>=0)g_regions08[ri].buildingCount++;}
  RegionHeader08 h;memset(&h,0,sizeof(h));memcpy(h.magic,"IDREG08",7);h.version=1;h.regionCount=(u32)g_region_count08;h.worldSegCount=(u32)g_world.segCount;h.worldBuildingCount=(u32)g_world.buildingCount;
  u64 cursor=sizeof(h)+(u64)g_region_count08*sizeof(RegionMeta08);for(int i=0;i<g_region_count08;i++){g_regions08[i].roadOffset=cursor;cursor+=(u64)g_regions08[i].roadCount*sizeof(StreamRoad08);g_regions08[i].buildingOffset=cursor;cursor+=(u64)g_regions08[i].buildingCount*sizeof(StreamBuilding08);}if(cursor>16ull*1024ull*1024ull)return 0;
  char*mem=(char*)VirtualAlloc(NULL,(SIZE_T)cursor,MEM_COMMIT|MEM_RESERVE,4);if(!mem)return 0;memset(mem,0,(SIZE_T)cursor);memcpy(mem,&h,sizeof(h));memcpy(mem+sizeof(h),g_regions08,(SIZE_T)g_region_count08*sizeof(RegionMeta08));
  for(int r=0;r<g_region_count08;r++){u32 rc=0,bc=0;for(int i=0;i<g_world.segCount;i++){RoadSegment*s=&g_world.segs[i];V3 m=vmul(vadd(s->a,s->b),.5f);if(stream08_coord(m.x)!=g_regions08[r].rx||stream08_coord(m.z)!=g_regions08[r].rz)continue;StreamRoad08 sr;sr.a=s->a;sr.b=s->b;sr.road=s->road;memcpy(mem+g_regions08[r].roadOffset+(u64)rc*sizeof(sr),&sr,sizeof(sr));rc++;}
    for(int i=0;i<g_world.buildingCount;i++){Building*b=&g_world.buildings[i];if(stream08_coord(b->c.x)!=g_regions08[r].rx||stream08_coord(b->c.z)!=g_regions08[r].rz)continue;StreamBuilding08 sb;sb.b=*b;memcpy(mem+g_regions08[r].buildingOffset+(u64)bc*sizeof(sb),&sb,sizeof(sb));bc++;}}
  int ok=file_write_all(g_region_path08,mem,(DWORD)cursor);VirtualFree(mem,0,MEM_RELEASE);return ok;
}

static int stream08_open_cache(void){
  if(g_region_file08!=INVALID_HANDLE_VALUE){CloseHandle(g_region_file08);g_region_file08=INVALID_HANDLE_VALUE;}g_region_file08=CreateFileA(g_region_path08,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);if(g_region_file08==INVALID_HANDLE_VALUE)return 0;
  RegionHeader08 h;DWORD got=0;if(!ReadFile(g_region_file08,&h,sizeof(h),&got,NULL)||got!=sizeof(h)||h.magic[0]!='I'||h.magic[1]!='D'||h.magic[2]!='R'||h.magic[3]!='E'||h.magic[4]!='G'||h.magic[5]!='0'||h.magic[6]!='8'||h.version!=1||h.regionCount>MAX_REGIONS08||h.worldSegCount!=(u32)g_world.segCount||h.worldBuildingCount!=(u32)g_world.buildingCount){CloseHandle(g_region_file08);g_region_file08=INVALID_HANDLE_VALUE;return 0;}
  g_region_count08=(int)h.regionCount;DWORD bytes=(DWORD)(g_region_count08*sizeof(RegionMeta08));if(bytes&&(!ReadFile(g_region_file08,g_regions08,bytes,&got,NULL)||got!=bytes)){CloseHandle(g_region_file08);g_region_file08=INVALID_HANDLE_VALUE;return 0;}return 1;
}

static int stream08_prepare(void){sprintf(g_region_path08,"%s\\itajai_regions_v08.bin",g_exe_dir);if(!stream08_open_cache()){DeleteFileA(g_region_path08);if(!stream08_build_cache()||!stream08_open_cache())return 0;}g_stream_region_x08=0x7fffffff;g_stream_region_z08=0x7fffffff;return 1;}
static int stream08_seek_read(u64 off,void*out,DWORD bytes){if(g_region_file08==INVALID_HANDLE_VALUE||!bytes)return bytes==0;LARGE_INTEGER li;li.QuadPart=(i64)off;if(!SetFilePointerEx(g_region_file08,li,NULL,FILE_BEGIN))return 0;DWORD got=0;return ReadFile(g_region_file08,out,bytes,&got,NULL)&&got==bytes;}

static void stream08_add_asset(int asset,V3 p,float yaw,float scale,u32 seed){if(asset<0||g_asset_instance_count08>=MAX_ASSET_INSTANCES08)return;AssetInstance08*x=&g_asset_instances08[g_asset_instance_count08++];x->asset=asset;x->p=p;x->yaw=yaw;x->scale=scale;x->seed=seed;}
static void stream08_generate_assets(void){
  g_asset_instance_count08=0;if(!g_asset_pack_ok08)return;int tree=asset08_find("urban_tree"),lamp=asset08_find("street_lamp"),container=asset08_find("port_container");
  for(int i=0;i<g_stream_road_count08;i++){StreamRoad08*s=&g_stream_roads08[i];if(s->road<0||s->road>=g_world.roadCount)continue;RoadWay*r=&g_world.roads[s->road];V3 d=vsub(s->b,s->a);float len=vlen(d);if(len<8)continue;V3 dir=vnorm(d),right=v3(dir.z,0,-dir.x);int steps=(int)(len/34.0f);if(steps<1)steps=1;for(int k=0;k<steps;k++){float t=(k+.5f)/steps;V3 p=vadd(s->a,vmul(d,t));i32 hx=(i32)(p.x*2),hz=(i32)(p.z*2);u32 h=stream08_hash(hx,hz,(u32)(s->road*31+k));float side=(h&1)?1.0f:-1.0f;
      if(tree>=0&&r->width<11.5f&&(h%100)<44){V3 tp=vadd(p,vmul(right,side*(r->width*.5f+3.2f)));stream08_add_asset(tree,tp,0,.82f+.28f*((h>>8)&255)/255.0f,h);}
      if(lamp>=0&&r->width>=5.5f&&((h>>5)%100)<30){V3 lp=vadd(p,vmul(right,side*(r->width*.5f+1.25f)));stream08_add_asset(lamp,lp,(float)atan2((double)dir.x,(double)dir.z),1.0f,h^0x55aa55aa);}
      if(container>=0&&str_eq(r->type,"service")&&((h>>11)%100)<8){V3 cp=vadd(p,vmul(right,side*(r->width*.5f+5.2f)));stream08_add_asset(container,cp,(float)atan2((double)dir.x,(double)dir.z),1.0f,h^0xa53c9e21);}
    }}
}

static void stream08_refresh(V3 p,float dt){
  g_stream_refresh08-=dt;int rx=stream08_coord(p.x),rz=stream08_coord(p.z);if(rx==g_stream_region_x08&&rz==g_stream_region_z08&&g_stream_refresh08>0)return;g_stream_refresh08=.28f;g_stream_region_x08=rx;g_stream_region_z08=rz;g_stream_road_count08=0;g_stream_building_count08=0;g_stream_loaded_regions08=0;
  if(g_region_file08==INVALID_HANDLE_VALUE)return;for(int i=0;i<g_region_count08;i++){RegionMeta08*m=&g_regions08[i];if(m->rx<rx-REGION_RADIUS08||m->rx>rx+REGION_RADIUS08||m->rz<rz-REGION_RADIUS08||m->rz>rz+REGION_RADIUS08)continue;g_stream_loaded_regions08++;
    u32 rn=m->roadCount;if(g_stream_road_count08+(int)rn>MAX_STREAM_ROADS08)rn=(u32)(MAX_STREAM_ROADS08-g_stream_road_count08);if(rn){if(stream08_seek_read(m->roadOffset,g_stream_roads08+g_stream_road_count08,rn*sizeof(StreamRoad08)))g_stream_road_count08+=(int)rn;}
    u32 bn=m->buildingCount;if(g_stream_building_count08+(int)bn>MAX_STREAM_BUILDINGS08)bn=(u32)(MAX_STREAM_BUILDINGS08-g_stream_building_count08);if(bn){if(stream08_seek_read(m->buildingOffset,g_stream_buildings08+g_stream_building_count08,bn*sizeof(StreamBuilding08)))g_stream_building_count08+=(int)bn;}
  }stream08_generate_assets();
}

static int stream08_resolve_asset_collisions(PhysicsBody*b,float*speedScalar){
  int hits=0;for(int i=0;i<g_asset_instance_count08;i++){AssetInstance08*x=&g_asset_instances08[i];if(x->asset<0||x->asset>=g_asset_count08)continue;float rr=g_assets08[x->asset].colliderRadius*x->scale;if(rr<.18f)continue;float dx=b->p.x-x->p.x,dz=b->p.z-x->p.z,minD=b->radius+rr,ds=dx*dx+dz*dz;if(ds>=minD*minD)continue;float d=(float)sqrt((double)maxf(ds,.0001f));V3 n=d>.01f?v3(dx/d,0,dz/d):v3(1,0,0);float pen=minD-d;b->p=vadd(b->p,vmul(n,pen+.01f));float vn=vdot(b->vel,n);if(vn<0)b->vel=vsub(b->vel,vmul(n,vn*1.08f));if(speedScalar)*speedScalar*=.72f;hits++;}
  g_stream_asset_contacts08=hits;return hits;
}

static void stream08_draw_assets(void){for(int i=0;i<g_asset_instance_count08;i++){AssetInstance08*x=&g_asset_instances08[i];float dx=x->p.x-g_player.body.p.x,dz=x->p.z-g_player.body.p.z;if(dx*dx+dz*dz>420*420)continue;if(x->asset==asset08_find("urban_tree"))asset08_draw_instance(x->asset,x->p,x->yaw,x->scale,.20f,.48f,.18f);else if(x->asset==asset08_find("street_lamp"))asset08_draw_instance(x->asset,x->p,x->yaw,x->scale,.28f,.30f,.32f);else asset08_draw_instance(x->asset,x->p,x->yaw,x->scale,.63f,.20f,.10f);}}
