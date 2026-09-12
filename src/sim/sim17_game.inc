// -----------------------------------------------------------------------------
// 0.17 advanced traffic layer: opportunistic lane changes, local yielding,
// sector/time personality, and smoother lateral lane transitions.
// -----------------------------------------------------------------------------
static float g_lane_visual17[MAX_TRAFFIC];
static float g_lane_cooldown17[MAX_TRAFFIC];
static int g_lane_changes17=0,g_yielding17=0;
static float g_traffic_density17=1.0f;

static float traffic17_lane_gap(int self,int eid,int lane,float t){
  float ahead=traffic_gap_ahead(self,eid,lane,t),behind=9999.0f;if(eid<0||eid>=g_world.edgeCount)return ahead;
  float len=maxf(.1f,g_world.edges[eid].length);for(int j=0;j<g_traffic_count;j++){if(j==self)continue;Traffic*o=&g_traffic[j];if(!o->active||o->routeN<=0)continue;int oe=o->route[o->ri];if(oe!=eid||edge_clamp_lane(oe,g_traffic_lane[j])!=lane||o->t>=t)continue;float gap=(t-o->t)*len-4.2f;if(gap<behind)behind=gap;}return minf(ahead,behind*.82f);
}
static void traffic17_post(float dt){
  g_yielding17=0;float hourFactor=(g_hour>=7&&g_hour<=9)||(g_hour>=17&&g_hour<=19)?1.0f:(g_hour>=23||g_hour<=5?.42f:.72f);float sectorFactor=(g_sector10==0||g_sector10==2)?1.0f:(g_sector10==6?.82f:.70f);g_traffic_density17=hourFactor*sectorFactor;
  for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active||t->routeN<=0)continue;g_lane_cooldown17[i]-=dt;int eid=t->route[t->ri];if(eid<0||eid>=g_world.edgeCount)continue;int lanes=edge_lane_count(eid),cur=edge_clamp_lane(eid,g_traffic_lane[i]);float currentGap=traffic_gap_ahead(i,eid,cur,t->t);
    if(lanes>1&&g_lane_cooldown17[i]<=0&&currentGap<12.0f+t->speed*.85f){int best=cur;float bestGap=currentGap;for(int d=-1;d<=1;d+=2){int cand=cur+d;if(cand<0||cand>=lanes)continue;float gap=traffic17_lane_gap(i,eid,cand,t->t);if(gap>bestGap+8.0f&&gap>8.0f){bestGap=gap;best=cand;}}if(best!=cur){g_traffic_lane[i]=best;g_lane_cooldown17[i]=4.0f+rng01()*3.5f;g_lane_changes17++;}}
    float desired=(float)g_traffic_lane[i];if(g_lane_visual17[i]<-.5f||g_lane_visual17[i]>lanes+.5f)g_lane_visual17[i]=desired;g_lane_visual17[i]=lerpf(g_lane_visual17[i],desired,clampf(dt*1.8f,0,1));
    Edge*e=&g_world.edges[eid];Node*to=&g_world.nodes[e->to];float remaining=(1.0f-t->t)*maxf(.1f,e->length);if(to->outCount>2&&remaining<13.0f&&i%3!=0){t->speed=minf(t->speed,6.0f+(i%4)*.65f);g_yielding17++;}
    // Smooth visual lane interpolation without altering the route's longitudinal t.
    V3 a=g_world.nodes[e->from].p,b=g_world.nodes[e->to].p,d=vsub(b,a);float len=vlen(d);if(len>.01f){V3 right=v3(d.z/len,0,-d.x/len);float lw=edge_lane_width(eid),half=g_world.roads[e->road].width*.5f;float off=half-lw*(g_lane_visual17[i]+.5f);V3 center=vadd(a,vmul(d,clampf(t->t,0,1)));V3 target=vadd(center,vmul(right,off));target.y=.6f;if(!g_traffic_prev_valid[i]||t->t>SIM07_CONNECT_T)t->p=target;}
  }
}
static void game_update(float dt){game_update_legacy15(dt);if(!g_started||g_settings_menu_open)return;traffic17_post(dt);}
