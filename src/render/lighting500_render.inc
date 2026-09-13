// -----------------------------------------------------------------------------
// Renderer 5.0 urban lighting. HDR bulbs and ground pools are driven by the
// actual streamed street-lamp instances so visible poles and light sources stay
// perfectly aligned. A compact road fallback keeps lighting alive if the asset
// pack is unavailable. Everything is emitted in one additive quad batch.
// -----------------------------------------------------------------------------
static int g_lighting500_lamps=0,g_lighting500_pools=0,g_lighting500_streamed=0;

static void lighting500_head_emit(V3 p,float scale){
  float y=terrain11_height(p.x,p.z)+3.52f*scale;
  V3 c=v3(p.x,y,p.z),to=vsub(g_renderer_camera_eye09,c);to.y=0;
  float tl=vlen(to);V3 side=tl>.01f?v3(to.z/tl,0,-to.x/tl):v3(1,0,0);
  for(int layer=0;layer<3;layer++){
    float r=layer==0?.12f:layer==1?.34f:.78f;
    float a=layer==0?.90f:layer==1?.31f:.072f;
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

static int lighting500_emit_site(V3 p,float scale,float maxD2,int*cap){
  if(*cap<=0)return 0;float dx=p.x-g_player.body.p.x,dz=p.z-g_player.body.p.z,d2=dx*dx+dz*dz;if(d2>maxD2)return 0;
  lighting500_head_emit(p,scale);
  if(d2<190.0f*190.0f){float fade=1.0f-clampf((float)sqrt((double)d2)/205.0f,0,1);lighting500_pool_emit(p,5.5f*scale,.55f+.45f*fade);}
  (*cap)--;return 1;
}

static void lighting500_streamed_pass(int*cap,float maxD2,float splitD2,int farPass){
  stream08_resolve_asset_ids();int lamp=g_stream_lamp_asset08;if(lamp<0)return;
  int stride=g_perf80_quality==0?3:(g_perf80_quality==1?2:1);
  for(int i=0;i<g_asset_instance_count08&&*cap>0;i+=stride){AssetInstance08*x=&g_asset_instances08[i];if(x->asset!=lamp)continue;float dx=x->p.x-g_player.body.p.x,dz=x->p.z-g_player.body.p.z,d2=dx*dx+dz*dz;if(d2>maxD2)continue;if(!farPass&&d2>splitD2)continue;if(farPass&&d2<=splitD2)continue;if(lighting500_emit_site(x->p,x->scale,maxD2,cap))g_lighting500_streamed++;}
}

static void lighting500_fallback(int*cap,float maxD2){
  for(int i=0;i<g_stream_road_count08&&*cap>0;i++){StreamRoad08*s=&g_stream_roads08[i];if(s->road<0||s->road>=g_world.roadCount)continue;RoadWay*r=&g_world.roads[s->road];if(r->width<5.5f)continue;V3 d=vsub(s->b,s->a),m=vmul(vadd(s->a,s->b),.5f);float len=vlen(d);if(len<8.0f)continue;u32 h=roads130_hash(s)^0x50011A7u;if((h%9u)!=2u)continue;V3 f=vmul(d,1.0f/maxf(1.0f,len)),rt=v3(f.z,0,-f.x);float side=(h&1u)?1.0f:-1.0f;V3 p=vadd(m,vmul(rt,side*(r->width*.5f+1.3f)));lighting500_emit_site(p,1.0f,maxD2,cap);}
}

static void lighting500_draw(void){
  g_lighting500_lamps=g_lighting500_pools=g_lighting500_streamed=0;if(!g_renderer09_linear_output||!g_stream_ready08)return;
  float h=(float)g_hour+g_clock_accum60/60.0f;float dark=0;if(h>=19.0f||h<5.0f)dark=1.0f;else if(h>=17.4f&&h<19.0f)dark=(h-17.4f)/1.6f;else if(h>=5.0f&&h<6.4f)dark=(6.4f-h)/1.4f;if(g_weather>=2)dark=maxf(dark,.62f);if(dark<.05f)return;
  int cap=g_perf80_quality==0?18:(g_perf80_quality==1?34:58);float maxD=g_perf80_quality==0?235.0f:(g_perf80_quality==1?315.0f:410.0f),maxD2=maxD*maxD;float split=g_perf80_quality==0?105.0f:(g_perf80_quality==1?145.0f:185.0f),splitD2=split*split;
  renderer09_suspend_shader();glDisable(GL_TEXTURE_2D);glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE);glDepthMask(FALSE);glBegin(GL_QUADS);
  lighting500_streamed_pass(&cap,maxD2,splitD2,0);lighting500_streamed_pass(&cap,maxD2,splitD2,1);if(g_lighting500_streamed==0)lighting500_fallback(&cap,maxD2);
  glEnd();glDepthMask(TRUE);glDisable(GL_BLEND);renderer09_resume_shader();if(g_lighting500_lamps)g_draw_calls++;
}

static void lighting500_overlay(void){if(!g_debug||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();char b[170];sprintf(b,"LIGHT 5.0 // poles %d | pools %d | streamed %d",g_lighting500_lamps,g_lighting500_pools,g_lighting500_streamed);gl_text((float)g_w-462,108,b,.82f,.72f,.52f);settings_draw_2d_end();}
