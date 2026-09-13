// -----------------------------------------------------------------------------
// 2.80 Night & Wet City 3.0.
// Cheap deterministic facade lights and wet-road glints. This pass intentionally
// reuses streamed geometry so atmosphere follows the currently loaded city rather
// than adding another world database.
// -----------------------------------------------------------------------------
static int g_atmos280_windows=0,g_atmos280_glints=0;
static int atmos280_dark(void){return g_hour>=18||g_hour<6||g_weather>=2;}
static float atmos280_hash01(u32 h){h^=h>>16;h*=0x7feb352du;h^=h>>15;h*=0x846ca68bu;h^=h>>16;return (h&65535)/65535.0f;}

static void atmos280_draw_windows(void){
  g_atmos280_windows=0;if(!atmos280_dark())return;renderer09_set_material(MAT09_GENERIC);int cap=g_perf80_quality==0?34:g_perf80_quality==1?58:86;
  for(int i=0;i<g_stream_building_count08&&g_atmos280_windows<cap;i+=(g_perf80_quality==0?3:2)){
    Building*b=&g_stream_buildings08[i].b;if(landmark506_replaces(b))continue;float dx=b->c.x-g_player.body.p.x,dz=b->c.z-g_player.body.p.z;if(dx*dx+dz*dz>235.0f*235.0f||b->h<4.5f)continue;u32 seed=b->seed^(u32)(i*2654435761u);float q=atmos280_hash01(seed);if(q<.38f)continue;
    int rows=(int)(b->h/4.0f);if(rows<1)rows=1;if(rows>4)rows=4;int cols=(int)(maxf(b->sx,b->sz)/8.0f);if(cols<1)cols=1;if(cols>4)cols=4;float base=terrain11_height(b->c.x,b->c.z);int front=((seed>>5)&1u)!=0;
    for(int r=0;r<rows&&g_atmos280_windows<cap;r++)for(int c=0;c<cols&&g_atmos280_windows<cap;c++){
      u32 hs=seed^(u32)(r*131+c*977);if(atmos280_hash01(hs)<.42f)continue;float warm=.70f+.25f*atmos280_hash01(hs^0x51u),yy=base+1.65f+r*3.45f;if(yy>base+b->h-.75f)continue;
      if(front){float span=b->sx*.72f,x=b->c.x-span*.5f+(c+.5f)*span/cols,z=b->c.z+b->sz*.5f+.035f;draw_box(v3(x,yy,z),minf(1.15f,span/cols*.46f),.78f,.055f,0,1.0f,.70f+.17f*warm,.34f+.34f*warm);}else{float span=b->sz*.72f,z=b->c.z-span*.5f+(c+.5f)*span/cols,x=b->c.x+b->sx*.5f+.035f;draw_box(v3(x,yy,z),.055f,.78f,minf(1.15f,span/cols*.46f),0,1.0f,.70f+.17f*warm,.34f+.34f*warm);}g_atmos280_windows++;
    }
  }
}

static void atmos280_draw_wet_roads(void){
  g_atmos280_glints=0;if(g_wetness60<.16f&&g_weather<2)return;renderer09_suspend_shader();glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);int cap=g_perf80_quality==0?12:g_perf80_quality==1?22:34;
  for(int i=0;i<g_stream_road_count08&&g_atmos280_glints<cap;i+=7){StreamRoad08*s=&g_stream_roads08[i];if(s->road<0||s->road>=g_world.roadCount)continue;V3 d=vsub(s->b,s->a);float len=vlen(d);if(len<7.0f)continue;V3 mid=vmul(vadd(s->a,s->b),.5f);float dx=mid.x-g_player.body.p.x,dz=mid.z-g_player.body.p.z;if(dx*dx+dz*dz>185.0f*185.0f)continue;V3 f=vmul(d,1.0f/len),r=v3(f.z,0,-f.x);float width=minf(g_world.roads[s->road].width*.28f,2.4f),half=minf(len*.17f,5.8f);u32 hs=stream08_hash((i32)(mid.x*2),(i32)(mid.z*2),(u32)i);float off=(atmos280_hash01(hs)-.5f)*maxf(.6f,g_world.roads[s->road].width*.28f);mid=vadd(mid,vmul(r,off));mid.y=terrain11_road_grade(mid.x,mid.z)+.055f;float a=(.08f+.15f*g_wetness60)*(atmos280_dark()?1.25f:1.0f);glColor4f(.72f,.82f,.89f,a);V3 p0=vadd(mid,vadd(vmul(f,-half),vmul(r,-width))),p1=vadd(mid,vadd(vmul(f,half),vmul(r,-width))),p2=vadd(mid,vadd(vmul(f,half),vmul(r,width))),p3=vadd(mid,vadd(vmul(f,-half),vmul(r,width)));glBegin(GL_QUADS);glVertex3f(p0.x,p0.y,p0.z);glVertex3f(p1.x,p1.y,p1.z);glVertex3f(p2.x,p2.y,p2.z);glVertex3f(p3.x,p3.y,p3.z);glEnd();g_atmos280_glints++;g_draw_calls++;}
  glDisable(GL_BLEND);renderer09_resume_shader();
}
