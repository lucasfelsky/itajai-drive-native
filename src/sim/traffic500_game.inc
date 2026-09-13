// -----------------------------------------------------------------------------
// Traffic 5.0: final single-player stabilization pass.
// Removes late lane changes, snap-yaw connectors and permanent deadlocks while
// keeping the existing route/lane/signal simulation authoritative.
// -----------------------------------------------------------------------------
static float g_ai500_stuck[MAX_TRAFFIC],g_ai500_prev_yaw[MAX_TRAFFIC];static unsigned char g_ai500_yaw_init[MAX_TRAFFIC];static int g_ai500_recovered=0,g_ai500_player_yields=0;
static float ai500_angle_delta(float a,float b){float d=a-b;while(d>3.14159265f)d-=6.2831853f;while(d<-3.14159265f)d+=6.2831853f;return d;}
static void traffic500_post(float dt){g_ai500_recovered=g_ai500_player_yields=0;for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active||t->routeN<=0)continue;int eid=t->route[t->ri];if(eid<0||eid>=g_world.edgeCount)continue;Edge*e=&g_world.edges[eid];RoadWay*r=(e->road>=0&&e->road<g_world.roadCount)?&g_world.roads[e->road]:NULL;float len=maxf(.1f,e->length),rem=(1.0f-t->t)*len;Node*to=&g_world.nodes[e->to];
  if(rem<26.0f&&to->outCount>2&&r&&!str_eq(r->type,"motorway")&&!str_eq(r->type,"trunk")){int keep=(int)(g_lane_visual17[i]+.5f);g_traffic_lane[i]=edge_clamp_lane(eid,keep);t->speed=minf(t->speed,9.2f);}
  V3 f=v3((float)sin(t->yaw),0,(float)cos(t->yaw)),rt=v3(f.z,0,-f.x),rel=vsub(g_player.body.p,t->p);float ahead=vdot(rel,f),side=absf(vdot(rel,rt));if(ahead>0&&ahead<22.0f+t->speed*.65f&&side<2.5f){float safe=maxf(0,(ahead-3.2f)*.52f);t->speed=sim_move_toward(t->speed,minf(t->speed,safe),7.2f*dt);g_traffic_brake_amount[i]=maxf(g_traffic_brake_amount[i],.55f);g_ai500_player_yields++;}
  if(!g_ai500_yaw_init[i]){g_ai500_prev_yaw[i]=t->yaw;g_ai500_yaw_init[i]=1;}float dy=ai500_angle_delta(t->yaw,g_ai500_prev_yaw[i]),lim=(2.25f+clampf(t->speed/18.0f,0,1)*1.15f)*dt;dy=clampf(dy,-lim,lim);t->yaw=g_ai500_prev_yaw[i]+dy;g_ai500_prev_yaw[i]=t->yaw;
  int red=signal_red_for_node(e->to,g_time)&&rem<12.0f;float pdx=t->p.x-g_player.body.p.x,pdz=t->p.z-g_player.body.p.z;int blockedByPlayer=pdx*pdx+pdz*pdz<45.0f;if(t->speed<.22f&&!red&&!blockedByPlayer)g_ai500_stuck[i]+=dt;else g_ai500_stuck[i]=maxf(0,g_ai500_stuck[i]-dt*2.0f);if(g_ai500_stuck[i]>10.0f){int start=e->to;traffic_new_route(t,start);if(t->active){t->p=g_world.nodes[start].p;t->p.y=.6f;t->speed=2.4f;g_traffic_lane[i]=0;}g_ai500_stuck[i]=0;g_ai500_recovered++;}
 }}
static void game_update(float dt){game_update_legacy400(dt);if(!g_started||g_settings_menu_open||g_map380_open||g_garage215_open)return;traffic500_post(dt);}
