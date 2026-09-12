// -----------------------------------------------------------------------------
// 1.9 Production Presentation: reflective road studs and localized vehicle
// reflections. These are intentionally lightweight and keep the GL 1.x fallback.
// -----------------------------------------------------------------------------
static int g_graphics190_studs=0,g_graphics190_car_reflections=0;

static void graphics190_reflection_quad(V3 p,float yaw,float w,float l,float r,float g,float b,float a){
  V3 f=v3((float)sin((double)yaw),0,(float)cos((double)yaw)),rt=v3(f.z,0,-f.x);V3 a0=vadd(vadd(p,vmul(rt,-w)),vmul(f,-l)),a1=vadd(vadd(p,vmul(rt,w)),vmul(f,-l)),b0=vadd(vadd(p,vmul(rt,-w*.62f)),vmul(f,l)),b1=vadd(vadd(p,vmul(rt,w*.62f)),vmul(f,l));float y=roads130_height(p.x,p.z)+.048f;glColor4f(r,g,b,a);glVertex3f(a0.x,y,a0.z);glVertex3f(a1.x,y,a1.z);glColor4f(r*.58f,g*.58f,b*.58f,0);glVertex3f(b1.x,y,b1.z);glVertex3f(b0.x,y,b0.z);
}

static void graphics190_vehicle_reflections(void){
  float wet=clampf(g_wetness60,0,1);if(wet<.08f)return;renderer09_suspend_shader();glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);glBegin(GL_QUADS);g_graphics190_car_reflections=0;
  static const float pc[4][3]={{.72f,.69f,.58f},{.18f,.48f,.78f},{.72f,.12f,.10f},{.24f,.51f,.28f}};float*pcv=pc[g_player.car&3];graphics190_reflection_quad(g_player.body.p,g_player.body.yaw,.72f,2.45f,pcv[0],pcv[1],pcv[2],.055f*wet);g_graphics190_car_reflections++;
  static const float tc[8][3]={{.72f,.72f,.68f},{.16f,.42f,.78f},{.74f,.12f,.10f},{.20f,.56f,.30f},{.82f,.82f,.84f},{.18f,.18f,.20f},{.70f,.50f,.22f},{.54f,.18f,.48f}};
  for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active)continue;float dx=t->p.x-g_player.body.p.x,dz=t->p.z-g_player.body.p.z;if(dx*dx+dz*dz>110.0f*110.0f)continue;float*c=tc[i&7];graphics190_reflection_quad(t->p,t->yaw,.68f,2.15f,c[0],c[1],c[2],.030f*wet);g_graphics190_car_reflections++;}
  glEnd();glDisable(GL_BLEND);renderer09_resume_shader();g_draw_calls++;
}

static void graphics190_road_studs(void){
  float night=graphics170_night();if(night<.15f||!g_stream_ready08)return;renderer09_suspend_shader();glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE);g_graphics190_studs=0;
  for(int i=0;i<g_stream_road_count08;i++){StreamRoad08*s=&g_stream_roads08[i];if(s->road<0||s->road>=g_world.roadCount)continue;Road*r=&g_world.roads[s->road];if(r->width<6.5f)continue;V3 d=vsub(s->b,s->a);float len=vlen(d);if(len<8)continue;V3 mid=vmul(vadd(s->a,s->b),.5f);float dx=mid.x-g_player.body.p.x,dz=mid.z-g_player.body.p.z;if(dx*dx+dz*dz>145.0f*145.0f)continue;V3 dir=vmul(d,1.0f/len),rt=v3(dir.z,0,-dir.x);int n=(int)(len/9.0f);if(n>12)n=12;for(int k=1;k<n;k++){float t=(float)k/(float)n;V3 p=vadd(s->a,vmul(d,t));float y=roads130_height(p.x,p.z)+.058f,off=(r->oneway?0.0f:r->width*.02f);p=vadd(p,vmul(rt,off));float q=.055f;glColor4f(1.0f,.93f,.67f,.30f*night);glBegin(GL_QUADS);glVertex3f(p.x-q,y,p.z-q);glVertex3f(p.x+q,y,p.z-q);glVertex3f(p.x+q,y,p.z+q);glVertex3f(p.x-q,y,p.z+q);glEnd();g_graphics190_studs++;}}
  glDisable(GL_BLEND);renderer09_resume_shader();g_draw_calls++;
}
