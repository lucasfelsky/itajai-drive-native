// -----------------------------------------------------------------------------
// 2.20 Performance & Streaming 3.0.
// Replaces the original region-cache O(regions * world_objects) writer with a
// hash-indexed two-pass builder and refreshes the 5x5 working set by direct region
// lookup rather than scanning every cached region every 280 ms.
// Disk format remains IDREG08 v1, so old valid caches stay compatible.
// -----------------------------------------------------------------------------
__declspec(dllimport) DWORD WINAPI GetTickCount(void);
#define STREAM220_HASH 4096
static int g_stream220_hash[STREAM220_HASH];
static u32 g_stream220_road_write[MAX_REGIONS08],g_stream220_build_write[MAX_REGIONS08];
static DWORD g_stream220_cache_build_ms=0;
static int g_stream220_cache_builds=0,g_stream220_direct_lookups=0;

static void stream220_hash_clear(void){for(int i=0;i<STREAM220_HASH;i++)g_stream220_hash[i]=-1;}
static int stream220_region_lookup(int rx,int rz,int create){
  u32 h=stream08_hash(rx,rz,0x2205f17u)&(STREAM220_HASH-1);
  for(int probe=0;probe<STREAM220_HASH;probe++){
    int s=(int)((h+(u32)probe)&(STREAM220_HASH-1)),ri=g_stream220_hash[s];
    if(ri<0){if(!create||g_region_count08>=MAX_REGIONS08)return -1;ri=g_region_count08++;memset(&g_regions08[ri],0,sizeof(g_regions08[ri]));g_regions08[ri].rx=rx;g_regions08[ri].rz=rz;g_stream220_hash[s]=ri;return ri;}
    if(g_regions08[ri].rx==rx&&g_regions08[ri].rz==rz)return ri;
  }return -1;
}
static void stream220_index_loaded_regions(void){stream220_hash_clear();for(int ri=0;ri<g_region_count08;ri++){u32 h=stream08_hash(g_regions08[ri].rx,g_regions08[ri].rz,0x2205f17u)&(STREAM220_HASH-1);for(int p=0;p<STREAM220_HASH;p++){int s=(int)((h+(u32)p)&(STREAM220_HASH-1));if(g_stream220_hash[s]<0){g_stream220_hash[s]=ri;break;}}}}

static int stream08_build_cache(void){
  DWORD begin=GetTickCount();g_region_count08=0;stream220_hash_clear();
  for(int i=0;i<g_world.segCount;i++){RoadSegment*s=&g_world.segs[i];V3 m=vmul(vadd(s->a,s->b),.5f);int ri=stream220_region_lookup(stream08_coord(m.x),stream08_coord(m.z),1);if(ri>=0)g_regions08[ri].roadCount++;}
  for(int i=0;i<g_world.buildingCount;i++){Building*b=&g_world.buildings[i];int ri=stream220_region_lookup(stream08_coord(b->c.x),stream08_coord(b->c.z),1);if(ri>=0)g_regions08[ri].buildingCount++;}
  RegionHeader08 h;memset(&h,0,sizeof(h));memcpy(h.magic,"IDREG08",7);h.version=1;h.regionCount=(u32)g_region_count08;h.worldSegCount=(u32)g_world.segCount;h.worldBuildingCount=(u32)g_world.buildingCount;
  u64 cursor=sizeof(h)+(u64)g_region_count08*sizeof(RegionMeta08);for(int i=0;i<g_region_count08;i++){g_regions08[i].roadOffset=cursor;cursor+=(u64)g_regions08[i].roadCount*sizeof(StreamRoad08);g_regions08[i].buildingOffset=cursor;cursor+=(u64)g_regions08[i].buildingCount*sizeof(StreamBuilding08);}if(cursor>16ull*1024ull*1024ull)return 0;
  char*mem=(char*)VirtualAlloc(NULL,(SIZE_T)cursor,MEM_COMMIT|MEM_RESERVE,4);if(!mem)return 0;memset(mem,0,(SIZE_T)cursor);memcpy(mem,&h,sizeof(h));memcpy(mem+sizeof(h),g_regions08,(SIZE_T)g_region_count08*sizeof(RegionMeta08));memset(g_stream220_road_write,0,sizeof(g_stream220_road_write));memset(g_stream220_build_write,0,sizeof(g_stream220_build_write));
  for(int i=0;i<g_world.segCount;i++){RoadSegment*s=&g_world.segs[i];V3 m=vmul(vadd(s->a,s->b),.5f);int ri=stream220_region_lookup(stream08_coord(m.x),stream08_coord(m.z),0);if(ri<0)continue;u32 w=g_stream220_road_write[ri]++;if(w>=g_regions08[ri].roadCount)continue;StreamRoad08 sr;sr.a=s->a;sr.b=s->b;sr.road=s->road;memcpy(mem+g_regions08[ri].roadOffset+(u64)w*sizeof(sr),&sr,sizeof(sr));}
  for(int i=0;i<g_world.buildingCount;i++){Building*b=&g_world.buildings[i];int ri=stream220_region_lookup(stream08_coord(b->c.x),stream08_coord(b->c.z),0);if(ri<0)continue;u32 w=g_stream220_build_write[ri]++;if(w>=g_regions08[ri].buildingCount)continue;StreamBuilding08 sb;sb.b=*b;memcpy(mem+g_regions08[ri].buildingOffset+(u64)w*sizeof(sb),&sb,sizeof(sb));}
  int ok=file_write_all(g_region_path08,mem,(DWORD)cursor);VirtualFree(mem,0,MEM_RELEASE);g_stream220_cache_build_ms=GetTickCount()-begin;g_stream220_cache_builds++;return ok;
}

static int stream08_prepare(void){
  sprintf(g_region_path08,"%s\\itajai_regions_v08.bin",g_exe_dir);if(!stream08_open_cache()){DeleteFileA(g_region_path08);if(!stream08_build_cache()||!stream08_open_cache())return 0;}stream220_index_loaded_regions();g_stream_region_x08=0x7fffffff;g_stream_region_z08=0x7fffffff;return 1;
}

static void stream08_refresh(V3 p,float dt){
  g_stream_refresh08-=dt;int rx=stream08_coord(p.x),rz=stream08_coord(p.z);if(rx==g_stream_region_x08&&rz==g_stream_region_z08&&g_stream_refresh08>0)return;g_stream_refresh08=.28f;g_stream_region_x08=rx;g_stream_region_z08=rz;g_stream_road_count08=0;g_stream_building_count08=0;g_stream_loaded_regions08=0;if(g_region_file08==INVALID_HANDLE_VALUE)return;
  for(int z=rz-REGION_RADIUS08;z<=rz+REGION_RADIUS08;z++)for(int x=rx-REGION_RADIUS08;x<=rx+REGION_RADIUS08;x++){g_stream220_direct_lookups++;int ri=stream220_region_lookup(x,z,0);if(ri<0)continue;RegionMeta08*m=&g_regions08[ri];g_stream_loaded_regions08++;
    u32 rn=m->roadCount;if(g_stream_road_count08+(int)rn>MAX_STREAM_ROADS08)rn=(u32)(MAX_STREAM_ROADS08-g_stream_road_count08);if(rn&&stream08_seek_read(m->roadOffset,g_stream_roads08+g_stream_road_count08,rn*sizeof(StreamRoad08)))g_stream_road_count08+=(int)rn;
    u32 bn=m->buildingCount;if(g_stream_building_count08+(int)bn>MAX_STREAM_BUILDINGS08)bn=(u32)(MAX_STREAM_BUILDINGS08-g_stream_building_count08);if(bn&&stream08_seek_read(m->buildingOffset,g_stream_buildings08+g_stream_building_count08,bn*sizeof(StreamBuilding08)))g_stream_building_count08+=(int)bn;
  }stream08_generate_assets();
}
