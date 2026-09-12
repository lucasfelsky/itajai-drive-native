// -----------------------------------------------------------------------------
// Simulation 0.6: lane model + vehicle profiles + traffic state
// -----------------------------------------------------------------------------
typedef struct {
  float engineAccel,maxSpeed,mass,wheelbase,track,maxSteer,brakeAccel,baseGrip,aero,rolling;
} VehicleProfile;

static const VehicleProfile g_vehicle_profiles[4]={
  {7.2f,40.0f,1010.0f,2.37f,1.43f,.54f,14.5f,1.00f,.018f,.68f},
  {8.2f,45.0f,1125.0f,2.48f,1.47f,.52f,15.0f,1.04f,.018f,.70f},
  {9.5f,52.0f,1260.0f,2.58f,1.51f,.50f,15.8f,1.08f,.019f,.72f},
  {8.7f,47.0f,1470.0f,2.57f,1.55f,.49f,15.5f,.98f,.021f,.78f}
};

static int g_traffic_lane[MAX_TRAFFIC];
static float g_traffic_cruise[MAX_TRAFFIC];
static float g_traffic_brake_amount[MAX_TRAFFIC];
static float g_traffic_gap[MAX_TRAFFIC];
static float g_traffic_avg_speed=0;

static int road_total_lanes(int road){
  if(road<0||road>=g_world.roadCount)return 1;
  RoadWay*r=&g_world.roads[road];
  int total=(int)(r->width/3.0f+.5f);
  if(total<1)total=1;if(total>6)total=6;
  if(!r->oneway){if(total<2)total=2;if(total&1)total--;if(total<2)total=2;}
  return total;
}
static int edge_lane_count(int eid){
  if(eid<0||eid>=g_world.edgeCount)return 1;Edge*e=&g_world.edges[eid];RoadWay*r=&g_world.roads[e->road];int total=road_total_lanes(e->road);return r->oneway?maxf(1,(float)total):(total/2);
}
static int edge_clamp_lane(int eid,int lane){int n=edge_lane_count(eid);if(lane<0)return 0;if(lane>=n)return n-1;return lane;}
static float edge_lane_width(int eid){if(eid<0||eid>=g_world.edgeCount)return 3.0f;Edge*e=&g_world.edges[eid];RoadWay*r=&g_world.roads[e->road];int n=edge_lane_count(eid);float usable=r->oneway?r->width:r->width*.5f;return clampf(usable/maxf(1,(float)n),2.15f,3.45f);}
static float edge_lane_offset(int eid,int lane){
  if(eid<0||eid>=g_world.edgeCount)return 0;Edge*e=&g_world.edges[eid];RoadWay*r=&g_world.roads[e->road];int n=edge_lane_count(eid);lane=edge_clamp_lane(eid,lane);float lw=edge_lane_width(eid);float half=r->width*.5f;
  if(r->oneway)return half-lw*(lane+.5f);
  return half-lw*(lane+.5f);
}
static V3 edge_lane_position(int eid,float t,int lane,float y){
  if(eid<0||eid>=g_world.edgeCount)return v3(0,y,0);Edge*e=&g_world.edges[eid];V3 a=g_world.nodes[e->from].p,b=g_world.nodes[e->to].p,d=vsub(b,a);float len=vlen(d);V3 right=len>.01f?v3(d.z/len,0,-d.x/len):v3(1,0,0);V3 p=vadd(vadd(a,vmul(d,clampf(t,0,1))),vmul(right,edge_lane_offset(eid,lane)));p.y=y;return p;
}
static float edge_lane_yaw(int eid){if(eid<0||eid>=g_world.edgeCount)return 0;Edge*e=&g_world.edges[eid];V3 d=vsub(g_world.nodes[e->to].p,g_world.nodes[e->from].p);return (float)atan2((double)d.x,(double)d.z);}
static float sim_surface_grip(int surface){return surface==SURFACE_ASPHALT?1.0f:surface==SURFACE_CURB?.72f:.48f;}
static float sim_surface_power(int surface){return surface==SURFACE_ASPHALT?1.0f:surface==SURFACE_CURB?.78f:.58f;}
static float sim_surface_height(int surface){return surface==SURFACE_CURB?.065f:surface==SURFACE_GRASS?-.025f:0.0f;}
static float sim_sign(float x){return x>0?1.0f:x<0?-1.0f:0.0f;}
static float sim_move_toward(float current,float target,float maxDelta){float d=target-current;if(d>maxDelta)d=maxDelta;else if(d<-maxDelta)d=-maxDelta;return current+d;}

static float traffic_gap_ahead(int self,int eid,int lane,float t){
  float best=9999.0f;if(eid<0||eid>=g_world.edgeCount)return best;float len=maxf(.1f,g_world.edges[eid].length);
  for(int j=0;j<g_traffic_count;j++){if(j==self)continue;Traffic*o=&g_traffic[j];if(!o->active||o->routeN<=0)continue;int oe=o->route[o->ri];int ol=edge_clamp_lane(oe,g_traffic_lane[j]);if(oe==eid&&ol==lane&&o->t>t){float gap=(o->t-t)*len-4.2f;if(gap<best)best=gap;continue;}
    Traffic*s=&g_traffic[self];if(s->ri+1<s->routeN&&s->route[s->ri+1]==oe&&ol==edge_clamp_lane(oe,lane)){float gap=(1.0f-t)*len+o->t*maxf(.1f,g_world.edges[oe].length)-4.2f;if(gap<best)best=gap;}
  }
  return best;
}
