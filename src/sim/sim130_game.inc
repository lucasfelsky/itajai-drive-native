// -----------------------------------------------------------------------------
// 1.3 Roads 2.0 gameplay layer: road grade, traffic elevation and lightweight
// procedural speed-bump response.
// -----------------------------------------------------------------------------
static void roads130_update_traffic_height(void){for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active)continue;t->p.y=terrain11_height(t->p.x,t->p.z)+.60f;}}
static void roads130_update_player_height(float dt){
  float h=terrain11_height(g_player.body.p.x,g_player.body.p.z);g_road_height130=h;V3 f=v3((float)sin((double)g_player.body.yaw),0,(float)cos((double)g_player.body.yaw));float h0=terrain11_height(g_player.body.p.x-f.x*3.0f,g_player.body.p.z-f.z*3.0f),h1=terrain11_height(g_player.body.p.x+f.x*3.0f,g_player.body.p.z+f.z*3.0f);g_road_grade130=lerpf(g_road_grade130,(h1-h0)/6.0f,clampf(dt*5.0f,0,1));g_player.body.p.y=h+1.0f+g_player.rideHeight;
}
static void roads130_update_bumps(float dt){
  g_bump130=maxf(0,g_bump130-dt*3.0f);g_bump_cooldown130=maxf(0,g_bump_cooldown130-dt);if(g_bump_cooldown130>0||absf(g_player.speed)<2.0f||!g_stream_ready08)return;
  for(int i=0;i<g_stream_road_count08;i++){StreamRoad08*s=&g_stream_roads08[i];if(!roads130_has_bump(s))continue;V3 m=vmul(vadd(s->a,s->b),.5f);float dx=m.x-g_player.body.p.x,dz=m.z-g_player.body.p.z;if(dx*dx+dz*dz>2.4f*2.4f)continue;g_bump130=1.0f;g_bump_cooldown130=.55f;g_player.body.vel=vmul(g_player.body.vel,.94f);g_player.speed*=.94f;g_player.pitch=clampf(g_player.pitch+(g_player.speed>=0?.055f:-.055f),-.16f,.16f);for(int w=0;w<4;w++)g_player.wheelCompression[w]=clampf(g_player.wheelCompression[w]+(w<2?.10f:.05f),0,1);break;}
}
static void game_update(float dt){game_update_legacy110(dt);if(!g_started)return;if(g_settings_menu_open||g_editor19||g_photo70)return;roads130_update_player_height(dt);roads130_update_traffic_height();roads130_update_bumps(dt);}
