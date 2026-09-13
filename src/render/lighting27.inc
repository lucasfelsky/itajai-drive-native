// -----------------------------------------------------------------------------
// 2.7 Lighting & Weather 2.0 - compatibility-safe world-space effects.
// Adds traffic headlight projection, wet-road light streaks and tyre spray while
// preserving the existing GLSL/fixed-function fallback. Renderer 5.0 also uses
// additive HDR energy for traffic headlight pools, while the legacy direct path
// retains its original alpha blend. Every temporary fixed-function pass restores
// the world shader before returning.
// -----------------------------------------------------------------------------
static int g_light27_headlights=0,g_light27_spray=0,g_light27_wet_strips=0;
static float lighting27_night(void){float h=(float)g_hour+g_clock_accum60/60.0f;if(h>=19.0f||h<5.0f)return 1.0f;if(h>=18.0f&&h<19.0f)return h-18.0f;if(h>=5.0f&&h<6.0f)return 6.0f-h;return 0.0f;}
static void lighting27_quad(V3 c,float yaw,float halfW,float front,float back,float r,float g,float b,float a){V3 f=v3((float)sin((double)yaw),0,(float)cos((double)yaw)),rr=v3(f.z,0,-f.x);V3 p0=vadd(vadd(c,vmul(f,front)),vmul(rr,-halfW)),p1=vadd(vadd(c,vmul(f,front)),vmul(rr,halfW)),p2=vadd(vadd(c,vmul(f,back)),vmul(rr,halfW)),p3=vadd(vadd(c,vmul(f,back)),vmul(rr,-halfW));glColor4f(r,g,b,a);glVertex3f(p0.x,p0.y,p0.z);glVertex3f(p1.x,p1.y,p1.z);glVertex3f(p2.x,p2.y,p2.z);glVertex3f(p3.x,p3.y,p3.z);}
static void lighting27_draw_traffic_headlights(void){
  float night=lighting27_night();g_light27_headlights=0;if(night<.03f)return;int hdr=g_renderer09_linear_output;renderer09_suspend_shader();glDisable(GL_TEXTURE_2D);glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,hdr?GL_ONE:GL_ONE_MINUS_SRC_ALPHA);glDepthMask(FALSE);glBegin(GL_QUADS);
  for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active)continue;float dx=t->p.x-g_player.body.p.x,dz=t->p.z-g_player.body.p.z;if(dx*dx+dz*dz>145.0f*145.0f)continue;V3 f=v3((float)sin((double)t->yaw),0,(float)cos((double)t->yaw));V3 c=vadd(t->p,vmul(f,4.4f));c.y=terrain11_height(c.x,c.z)+.072f;float boost=1.0f+g_wetness60*.78f;if(hdr)lighting27_quad(c,t->yaw,1.68f,6.2f,-3.1f,1.34f,1.03f,.68f,.047f*night*boost);else lighting27_quad(c,t->yaw,1.65f,5.8f,-3.4f,1.0f,.86f,.64f,.032f*night*(1.0f+g_wetness60*.70f));g_light27_headlights++;}
  glEnd();glDepthMask(TRUE);glDisable(GL_BLEND);renderer09_resume_shader();if(g_light27_headlights)g_draw_calls++;
}
static void lighting27_draw_wet_strips(void){
  g_light27_wet_strips=0;if(g_wetness60<.18f||!g_stream_ready08)return;float night=lighting27_night();renderer09_suspend_shader();glDisable(GL_TEXTURE_2D);glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);glDepthMask(FALSE);glBegin(GL_QUADS);
  int stride=g_lod20_tier>=2?3:2;for(int i=0;i<g_stream_road_count08;i+=stride){StreamRoad08*s=&g_stream_roads08[i];V3 d=vsub(s->b,s->a);float len=vlen(d);if(len<3.0f)continue;float dx=(s->a.x+s->b.x)*.5f-g_player.body.p.x,dz=(s->a.z+s->b.z)*.5f-g_player.body.p.z;if(dx*dx+dz*dz>260.0f*260.0f)continue;V3 rr=v3(d.z/len,0,-d.x/len);float rw=(s->road>=0&&s->road<g_world.roadCount)?g_world.roads[s->road].width:7.0f;float w=minf(1.25f,rw*.10f),a=.014f*g_wetness60*(.75f+night*.65f);V3 p0=vadd(s->a,vmul(rr,-w)),p1=vadd(s->a,vmul(rr,w)),p2=vadd(s->b,vmul(rr,w)),p3=vadd(s->b,vmul(rr,-w));float y0=roads130_height(s->a.x,s->a.z),y1=roads130_height(s->b.x,s->b.z);glColor4f(.42f,.58f,.72f,a);glVertex3f(p0.x,y0+.018f,p0.z);glVertex3f(p1.x,y0+.018f,p1.z);glVertex3f(p2.x,y1+.018f,p2.z);glVertex3f(p3.x,y1+.018f,p3.z);g_light27_wet_strips++;}
  glEnd();glDepthMask(TRUE);glDisable(GL_BLEND);renderer09_resume_shader();if(g_light27_wet_strips)g_draw_calls++;
}
static void lighting27_spray_for(V3 p,float yaw,float speed,float scale){
  if(g_wetness60<.25f||speed<4.0f)return;V3 f=v3((float)sin((double)yaw),0,(float)cos((double)yaw)),rr=v3(f.z,0,-f.x);float amt=clampf((speed-4.0f)/18.0f,0,1)*g_wetness60;for(int side=-1;side<=1;side+=2){V3 o=vadd(vadd(p,vmul(f,-1.45f*scale)),vmul(rr,side*.57f*scale));glColor4f(.72f,.80f,.84f,.10f*amt);glVertex3f(o.x,o.y-.55f,o.z);V3 q=vadd(vadd(o,vmul(f,-(1.3f+speed*.055f))),v3(0,.34f*amt,0));glVertex3f(q.x,q.y-.55f,q.z);g_light27_spray++;}}
static void lighting27_draw_spray(void){
  g_light27_spray=0;if(g_wetness60<.25f)return;renderer09_suspend_shader();glDisable(GL_TEXTURE_2D);glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);glLineWidth(2.0f);glBegin(GL_LINES);lighting27_spray_for(g_player.body.p,g_player.body.yaw,absf(g_player.speed),1.0f);for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active)continue;float dx=t->p.x-g_player.body.p.x,dz=t->p.z-g_player.body.p.z;if(dx*dx+dz*dz>95.0f*95.0f)continue;lighting27_spray_for(t->p,t->yaw,t->speed,.92f);}glEnd();glDisable(GL_BLEND);renderer09_resume_shader();if(g_light27_spray)g_draw_calls++;
}
