// -----------------------------------------------------------------------------
// Simulation 0.7: mouse camera, intersection lane connectors, dynamic GPS,
// and active-cell streaming working set. 1.8 refines cockpit eye placement.
// -----------------------------------------------------------------------------
#define SIM07_CONNECT_T 0.22f
#define MAX_STREAM_SEGMENTS 12000
#define MAX_STREAM_BUILDINGS 4000
#define STREAM_CELL_RADIUS 18

// Mouse-look / orbit camera state.
static int g_camera_captured=0;
static float g_camera_yaw=0.0f,g_camera_yaw_target=0.0f;
static float g_camera_pitch=0.0f,g_camera_pitch_target=0.0f;
static float g_camera_sensitivity=.0031f;
static int g_camera_cockpit_slot180=0;

// Intersection transition state for each traffic vehicle.
static int g_traffic_prev_edge[MAX_TRAFFIC];
static int g_traffic_prev_lane[MAX_TRAFFIC];
static int g_traffic_prev_valid[MAX_TRAFFIC];
static int g_sim07_turning_cars=0;

// Cell-based render/processing working set. World data remains resident in 0.7.
static int g_stream_segments[MAX_STREAM_SEGMENTS],g_stream_segment_count=0;
static int g_stream_buildings[MAX_STREAM_BUILDINGS],g_stream_building_count=0;
static u32 g_stream_seg_seen[MAX_SEGMENTS],g_stream_build_seen[MAX_BUILDINGS],g_stream_stamp=1;
static int g_stream_cx=0x7fffffff,g_stream_cz=0x7fffffff,g_stream_active_cells=0;
static float g_stream_refresh_timer=0;

static float sim07_wrap_angle(float a){while(a>3.14159265f)a-=6.2831853f;while(a<-3.14159265f)a+=6.2831853f;return a;}
static void camera_recenter(HWND h){RECT r;if(!GetClientRect(h,&r))return;POINT p;p.x=(r.right-r.left)/2;p.y=(r.bottom-r.top)/2;if(ClientToScreen(h,&p))SetCursorPos(p.x,p.y);}
static void camera_capture_mouse(HWND h){if(g_camera_captured)return;g_camera_captured=1;SetCapture(h);ShowCursor(FALSE);camera_recenter(h);}
static void camera_release_mouse(void){if(!g_camera_captured)return;g_camera_captured=0;ReleaseCapture();ShowCursor(TRUE);}
static void camera_mouse_move(HWND h,LPARAM l){
  if(!g_camera_captured)return;RECT r;if(!GetClientRect(h,&r))return;int cx=(r.right-r.left)/2,cy=(r.bottom-r.top)/2;int x=(int)(short)(l&0xffff),y=(int)(short)((l>>16)&0xffff);int dx=x-cx,dy=y-cy;if(dx==0&&dy==0)return;
  g_camera_yaw_target=sim07_wrap_angle(g_camera_yaw_target+dx*g_camera_sensitivity);g_camera_pitch_target=clampf(g_camera_pitch_target+dy*g_camera_sensitivity*.78f,-.55f,.58f);camera_recenter(h);
}
static void camera_update(float dt){
  float dy=sim07_wrap_angle(g_camera_yaw_target-g_camera_yaw);g_camera_yaw=sim07_wrap_angle(g_camera_yaw+dy*clampf(dt*14.0f,0,1));g_camera_pitch=lerpf(g_camera_pitch,g_camera_pitch_target,clampf(dt*12.0f,0,1));
}
static void camera_reset_orbit(void){g_camera_yaw_target=0;g_camera_pitch_target=0;}
static void sim07_camera_pose(int camera,V3 carPos,float carYaw,V3*eye,V3*target){
  float yaw=carYaw+g_camera_yaw;
  if(camera==2){
    // Driver-eye offsets for the four player slots: Uno, Gol, HB20, Renegade.
    // The slot is mirrored by the final gameplay layer because this legacy camera
    // is compiled before Player itself is declared in the historical translation unit.
    static const float eyeY[4]={.51f,.48f,.49f,.61f};
    static const float eyeZ[4]={-.10f,-.13f,-.11f,-.16f};
    static const float eyeX[4]={-.31f,-.32f,-.33f,-.35f};
    int ci=g_camera_cockpit_slot180&3;float pitch=clampf(g_camera_pitch,-.48f,.48f);float cp=(float)cos((double)pitch),sp=(float)sin((double)pitch);V3 dir=v3((float)sin((double)yaw)*cp,-sp,(float)cos((double)yaw)*cp);V3 carF=v3((float)sin((double)carYaw),0,(float)cos((double)carYaw)),carR=v3(carF.z,0,-carF.x);*eye=vadd(carPos,vadd(vadd(vmul(carF,eyeZ[ci]),vmul(carR,eyeX[ci])),v3(0,eyeY[ci],0)));*target=vadd(*eye,vmul(dir,28.0f));return;
  }
  float dist=camera==0?8.7f:camera==1?5.0f:39.0f;float basePitch=camera==0?.38f:camera==1?.31f:1.18f;float pitch=clampf(basePitch+g_camera_pitch,camera==3?.72f:.08f,camera==3?1.42f:1.02f);float cp=(float)cos((double)pitch),sp=(float)sin((double)pitch);V3 orbit=v3(-(float)sin((double)yaw)*cp,sp,-(float)cos((double)yaw)*cp);*target=vadd(carPos,v3(0,camera==3?.35f:.92f,0));*eye=vadd(*target,vmul(orbit,dist));
}

static float sim07_turn_cross(int a,int b,float*dotOut){if(a<0||b<0||a>=g_world.edgeCount||b>=g_world.edgeCount){if(dotOut)*dotOut=1;return 0;}Edge*ea=&g_world.edges[a],*eb=&g_world.edges[b];V3 da=vnorm(vsub(g_world.nodes[ea->to].p,g_world.nodes[ea->from].p)),db=vnorm(vsub(g_world.nodes[eb->to].p,g_world.nodes[eb->from].p));float dot=clampf(vdot(da,db),-1,1);if(dotOut)*dotOut=dot;return da.z*db.x-da.x*db.z;}
static int sim07_choose_next_lane(int prevE,int prevLane,int nextE){int n=edge_lane_count(nextE);if(n<=1)return 0;float dot=1,cross=sim07_turn_cross(prevE,nextE,&dot);if(dot>.82f)return edge_clamp_lane(nextE,prevLane);if(cross>.12f)return 0;if(cross<-.12f)return n-1;return edge_clamp_lane(nextE,prevLane);}
static float sim07_turn_target_speed(int curE,int nextE){float dot=1;sim07_turn_cross(curE,nextE,&dot);if(dot>.90f)return 99.0f;if(dot>.62f)return 11.5f;if(dot>.12f)return 8.0f;return 5.8f;}
static void sim07_sample_traffic(int i,int eid,float t,int lane,V3*outPos,float*outYaw){
  if(i>=0&&i<MAX_TRAFFIC&&g_traffic_prev_valid[i]&&t<SIM07_CONNECT_T){int pe=g_traffic_prev_edge[i],pl=g_traffic_prev_lane[i];if(pe>=0&&pe<g_world.edgeCount&&g_world.edges[pe].to==g_world.edges[eid].from){float u=clampf(t/SIM07_CONNECT_T,0,1),v=1.0f-u;V3 p0=edge_lane_position(pe,1.0f,pl,.6f),p1=g_world.nodes[g_world.edges[eid].from].p,p2=edge_lane_position(eid,SIM07_CONNECT_T,lane,.6f);p1.y=.6f;V3 p=vadd(vadd(vmul(p0,v*v),vmul(p1,2*v*u)),vmul(p2,u*u));V3 d=vadd(vmul(vsub(p1,p0),2*v),vmul(vsub(p2,p1),2*u));p.y=.6f;*outPos=p;*outYaw=(float)atan2((double)d.x,(double)d.z);return;}}
  *outPos=edge_lane_position(eid,t,lane,.6f);*outYaw=edge_lane_yaw(eid);
}

static void sim07_stream_update(V3 p,float dt){
  g_stream_refresh_timer-=dt;int cx=grid_coord(p.x),cz=grid_coord(p.z);if(cx==g_stream_cx&&cz==g_stream_cz&&g_stream_refresh_timer>0)return;g_stream_refresh_timer=.30f;g_stream_cx=cx;g_stream_cz=cz;g_stream_segment_count=0;g_stream_building_count=0;g_stream_active_cells=0;g_stream_stamp++;if(g_stream_stamp==0){memset(g_stream_seg_seen,0,sizeof(g_stream_seg_seen));memset(g_stream_build_seen,0,sizeof(g_stream_build_seen));g_stream_stamp=1;}u32 stamp=g_stream_stamp;
  for(int z=cz-STREAM_CELL_RADIUS;z<=cz+STREAM_CELL_RADIUS;z++)for(int x=cx-STREAM_CELL_RADIUS;x<=cx+STREAM_CELL_RADIUS;x++){int ci=grid_find_cell(x,z,0);if(ci<0)continue;g_stream_active_cells++;
    for(int li=g_grid_cells[ci].roadHead;li>=0;li=g_grid_road_links[li].next){int si=g_grid_road_links[li].item;if(si<0||si>=g_world.segCount||g_stream_seg_seen[si]==stamp)continue;g_stream_seg_seen[si]=stamp;if(g_stream_segment_count<MAX_STREAM_SEGMENTS)g_stream_segments[g_stream_segment_count++]=si;}
    for(int li=g_grid_cells[ci].colliderHead;li>=0;li=g_grid_collider_links[li].next){int co=g_grid_collider_links[li].item;if(co<0||co>=g_static_collider_count)continue;StaticCollider*c=&g_static_colliders[co];if(c->type!=COLLIDER_BUILDING||c->source<0||c->source>=g_world.buildingCount)continue;int bi=c->source;if(g_stream_build_seen[bi]==stamp)continue;g_stream_build_seen[bi]=stamp;if(g_stream_building_count<MAX_STREAM_BUILDINGS)g_stream_buildings[g_stream_building_count++]=bi;}
  }
}

static float sim07_distance_to_route(V3 p,const int*route,int n){float best=1e30f;for(int i=0;i<n;i++){int eid=route[i];if(eid<0||eid>=g_world.edgeCount)continue;Edge*e=&g_world.edges[eid];float d=point_segment_distance_xz(p,g_world.nodes[e->from].p,g_world.nodes[e->to].p);if(d<best)best=d;}return best<1e29f?best:9999.0f;}
static float sim07_route_length(const int*route,int n){float s=0;for(int i=0;i<n;i++){int e=route[i];if(e>=0&&e<g_world.edgeCount)s+=g_world.edges[e].length;}return s;}
