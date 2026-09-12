// -----------------------------------------------------------------------------
// 1.3 Roads 2.0 shared state/helpers.
// -----------------------------------------------------------------------------
static float g_road_height130=0,g_road_grade130=0,g_bump130=0,g_bump_cooldown130=0;
static int g_road_detail_segments130=0,g_road_bumps_visible130=0,g_road_bike_segments130=0;

static u32 roads130_hash(const StreamRoad08*s){V3 m=vmul(vadd(s->a,s->b),.5f);u32 x=(u32)((int)(m.x*.25f)),z=(u32)((int)(m.z*.25f));u32 h=(u32)(s->road+1)*0x9e3779b1u^x*0x85ebca6bu^z*0xc2b2ae35u;h^=h>>16;h*=0x7feb352du;return h^(h>>15);}
static int roads130_is_local(int road){if(road<0||road>=g_world.roadCount)return 0;RoadWay*r=&g_world.roads[road];return str_eq(r->type,"residential")||str_eq(r->type,"living_street")||str_eq(r->type,"unclassified")||str_eq(r->type,"tertiary");}
static int roads130_has_bump(const StreamRoad08*s){if(!roads130_is_local(s->road))return 0;float len=vlen(vsub(s->b,s->a));if(len<18.0f||len>85.0f)return 0;return (roads130_hash(s)%23u)==0u;}
static int roads130_has_patch(const StreamRoad08*s){return (roads130_hash(s)%7u)==0u;}
static int roads130_has_drain(const StreamRoad08*s){return roads130_is_local(s->road)&&((roads130_hash(s)>>5)%5u)==0u;}
static int roads130_has_bike_lane(const StreamRoad08*s){if(s->road<0||s->road>=g_world.roadCount)return 0;RoadWay*r=&g_world.roads[s->road];if(r->width<8.5f)return 0;V3 m=vmul(vadd(s->a,s->b),.5f);int sector=urban10_sector_at(m,NULL);return sector==1||sector==4||sector==5||sector==6;}
static float roads130_height(float x,float z){return terrain11_height(x,z)+.022f;}
static float roads130_segment_distance(V3 p,const StreamRoad08*s){return point_segment_distance_xz(p,s->a,s->b);}
