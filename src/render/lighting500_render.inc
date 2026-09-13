// -----------------------------------------------------------------------------
// Renderer 5.0 urban lighting. HDR bulbs and ground pools are driven by actual
// visible street-lamp instances: explicit city/landmark poles registered by 3.6
// plus streamed props. Nearby duplicate sites are suppressed. Wet asphalt gains
// source-linked warm reflection streaks that extend toward the camera, so the
// reflection belongs to the visible lamp instead of being a generic screen glow.
// A compact road fallback remains for missing asset packs. One additive batch
// covers bulbs, pools and wet reflections.
// -----------------------------------------------------------------------------
static int g_lighting500_lamps=0,g_lighting500_pools=0,g_lighting500_streamed=0,g_lighting500_registered=0,g_lighting500_wet_streaks=0;
static float g_lighting500_strength=1.0f;

static void lighting500_head_emit(V3 p,float scale,float strength){
  float y=terrain11_height(p.x,p.z)+3.52f*scale;
  V3 c=v3(p.x,y,p.z),to=vsub(g_renderer_camera_eye09,c);to.y=0;
  float tl=vlen(to);V3 side=tl>.01f?v3(to.z/tl,0,-to.x/tl):v3(1,0,0);
  for(int layer=0;layer<3;layer++){
    float r=layer==0?.12f:layer==1?.34f:.78f;
    float a=(layer==0?.90f:layer==1?.31f:.072f)*strength;
    glColor4f(1.0f,.73f,.34f,a);
    glVertex3f(c.x-side.x*r,c.y-r,c.z-side.z*r);
    glVertex3f(c.x+side.x*r,c.y-r,c.z+side.z*r);
    glVertex3f(c.x+side.x*r,c.y+r,c.z+side.z*r);
    glVertex3f(c.x-side.x*r,c.y+r,c.z-side.z*r);
  }
  g_lighting500_lamps++;
}

static void lighting500_pool_emit(V3 p,float radius,float strength){
  for(int ring=0;ring<3;ring++){
    float k=ring==0?.34f:ring==1?.66f:1.0f,r=radius*k;
    float a=(ring==0?.052f:ring==1?.026f:.010f)*strength;
    V3 q[4]={v3(p.x-r,0,p.z-r),v3(p.x+r,0,p.z-r),v3(p.x+r,0,p.z+r),v3(p.x-r,0,p.z+r)};
    for(int i=0;i<4;i++)q[i].y=terrain11_height(q[i].x,q[i].z)+.054f;
    glColor4f(1.0f,.56f,.20f,a);
    for(int i=0;i<4;i++)glVertex3f(q[i].x,q[i].y,q[i].z);
  }
  g_lighting500_pools++;
}

static void lighting500_wet_streak_emit(V3 p,float scale,float strength,float d2){
  float wet=clampf(g_wetness60,0,1);if(wet<.16f||d2>175.0f*175.0f)return;
  V3 toward=vsub(g_renderer_camera_eye09,p);toward.y=0;float tl=vlen(toward);if(tl<1.0f)return;toward=vmul(toward,1.0f/tl);V3 rt=v3(toward.z,0,-toward.x);
  float nearFade=1.0f-clampf(tl/190.0f,0,1),len=(2.8f+wet*8.2f)*scale*(.72f+.28f*nearFade),halfW=(.10f+wet*.17f)*scale;
  V3 a=vadd(p,vmul(toward,.35f)),b=vadd(p,vmul(toward,len));V3 q0=vadd(a,vmul(rt,-halfW)),q1=vadd(a,vmul(rt,halfW)),q2=vadd(b,vmul(rt,halfW*.34f)),q3=vadd(b,vmul(rt,-halfW*.34f));
  q0.y=terrain11_height(q0.x,q0.z)+.061f;q1.y=terrain11_height(q1.x,q1.z)+.061f;q2.y=terrain11_height(q2.x,q2.z)+.061f;q3.y=terrain11_height(q3.x,q3.z)+.061f;
  float a0=(.040f+.060f*wet)*strength*nearFade;glColor4f(1.0f,.58f,.20f,a0);glVertex3f(q0.x,q0.y,q0.z);glVertex3f(q1.x,q1.y,q1.z);glColor4f(1.0f,.39f,.12f,.004f*strength);glVertex3f(q2.x,q2.y,q2.z);glVertex3f(q3.x,q3.y,q3.z);g_lighting500_wet_streaks++;
}

static int lighting500_emit_site(V3 p,float scale,float maxD2,float strength,int*cap){
  if(*cap<=0)return 0;float dx=p.x-g_player.body.p.x,dz=p.z-g_player.body.p.z,d2=dx*dx+dz*dz;if(d2>maxD2)return 0;
  float distanceFade=1.0f-clampf((float)sqrt((double)d2)/(float)sqrt((double)maxD2),0,1);float lampStrength=strength*(.72f+.28f*distanceFade);
  lighting500_head_emit(p,scale,lampStrength);
  if(d2<190.0f*190.0f){float fade=1.0f-clampf((float)sqrt((double)d2)/205.0f,0,1);lighting500_pool_emit(p,5.5f*scale,lampStrength*(.55f+.45f*fade));}
  lighting500_wet_streak_emit(p,scale,lampStrength,d2);(*cap)--;return 1;
}

static int lighting500_near_registered(V3 p){for(int i=0;i<g_city360_light_count;i++){float dx=p.x-g_city360_light_pos[i].x,dz=p.z-g_city360_light_pos[i].z;if(dx*dx+dz*dz<3.1f*3.1f)return 1;}return 0;}
static void lighting500_registered_pass(int*cap,float maxD2,float splitD2,int farPass,float strength){
  for(int i=0;i<g_city360_light_count&&*cap>0;i++){V3 p=g_city360_light_pos[i];float dx=p.x-g_player.body.p.x,dz=p.z-g_player.body.p.z,d2=dx*dx+dz*dz;if(d2>maxD2)continue;if(!farPass&&d2>splitD2)continue;if(farPass&&d2<=splitD2)continue;if(lighting500_emit_site(p,g_city360_light_scale[i],maxD2,strength,cap))g_lighting500_registered++;}
}

static void lighting500_streamed_pass(int*cap,float maxD2,float splitD2,int farPass,float strength){
  stream08_resolve_asset_ids();int lamp=g_stream_lamp_asset08;if(lamp<0)return;int stride=g_perf80_quality==0?3:(g_perf80_quality==1?2:1),ordinal=0;
  for(int i=0;i<g_asset_instance_count08&&*cap>0;i++){AssetInstance08*x=&g_asset_instances08[i];if(x->asset!=lamp)continue;int take=(ordinal++%stride)==0;if(!take||lighting500_near_registered(x->p))continue;float dx=x->p.x-g_player.body.p.x,dz=x->p.z-g_player.body.p.z,d2=dx*dx+dz*dz;if(d2>maxD2)continue;if(!farPass&&d2>splitD2)continue;if(farPass&&d2<=splitD2)continue;if(lighting500_emit_site(x->p,x->scale,maxD2,strength,cap))g_lighting500_streamed++;}
}

static void lighting500_fallback(int*cap,float maxD2,float strength){
  for(int i=0;i<g_stream_road_count08&&*cap>0;i++){StreamRoad08*s=&g_stream_roads08[i];if(s->road<0||s->road>=g_world.roadCount)continue;RoadWay*r=&g_world.roads[s->road];if(r->width<5.5f)continue;V3 d=vsub(s->b,s->a),m=vmul(vadd(s->a,s->b),.5f);float len=vlen(d);if(len<8.0f)continue;u32 h=roads130_hash(s)^0x50011A7u;if((h%9u)!=2u)continue;V3 f=vmul(d,1.0f/maxf(1.0f,len)),rt=v3(f.z,0,-f.x);float side=(h&1u)?1.0f:-1.0f;V3 p=vadd(m,vmul(rt,side*(r->width*.5f+1.3f)));lighting500_emit_site(p,1.0f,maxD2,strength,cap);}
}

static void lighting500_draw(void){
  g_lighting500_lamps=g_lighting500_pools=g_lighting500_streamed=g_lighting500_registered=g_lighting500_wet_streaks=0;if(!g_renderer09_linear_output||!g_stream_ready08)return;
  float dark=renderer500_night_factor();if(g_weather>=2)dark=maxf(dark,.62f);else if(g_weather==1)dark=maxf(dark,.20f);g_lighting500_strength=renderer500_smooth01(dark);if(g_lighting500_strength<.025f)return;
  int cap=g_perf80_quality==0?18:(g_perf80_quality==1?34:58);float maxD=g_perf80_quality==0?235.0f:(g_perf80_quality==1?315.0f:410.0f),maxD2=maxD*maxD;float split=g_perf80_quality==0?105.0f:(g_perf80_quality==1?145.0f:185.0f),splitD2=split*split;
  renderer09_suspend_shader();glDisable(GL_TEXTURE_2D);glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE);glDepthMask(FALSE);glBegin(GL_QUADS);
  lighting500_registered_pass(&cap,maxD2,splitD2,0,g_lighting500_strength);lighting500_streamed_pass(&cap,maxD2,splitD2,0,g_lighting500_strength);lighting500_registered_pass(&cap,maxD2,splitD2,1,g_lighting500_strength);lighting500_streamed_pass(&cap,maxD2,splitD2,1,g_lighting500_strength);if(g_lighting500_registered==0&&g_lighting500_streamed==0)lighting500_fallback(&cap,maxD2,g_lighting500_strength);
  glEnd();glDepthMask(TRUE);glDisable(GL_BLEND);renderer09_resume_shader();if(g_lighting500_lamps)g_draw_calls++;
}

static void lighting500_overlay(void){if(!g_debug||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();char b[220];sprintf(b,"LIGHT 5.0 // poles %d | pools %d | wet %d | city %d | stream %d | %.2f",g_lighting500_lamps,g_lighting500_pools,g_lighting500_wet_streaks,g_lighting500_registered,g_lighting500_streamed,g_lighting500_strength);gl_text((float)g_w-462,127,b,.82f,.72f,.52f);settings_draw_2d_end();}
