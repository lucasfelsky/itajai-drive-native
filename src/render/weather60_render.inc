// -----------------------------------------------------------------------------
// 0.60 climate rendering helpers: wet road memory and lightning exposure flash.
// -----------------------------------------------------------------------------
static void weather60_draw_road_sheen(void){
  if(g_wetness60<.04f||!g_stream_ready08)return;renderer09_suspend_shader();glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);glBegin(GL_QUADS);
  for(int i=0;i<g_stream_road_count08;i++){StreamRoad08*s=&g_stream_roads08[i];if(s->road<0||s->road>=g_world.roadCount)continue;V3 m=vmul(vadd(s->a,s->b),.5f);float dx=m.x-g_player.body.p.x,dz=m.z-g_player.body.p.z;if(dx*dx+dz*dz>230.0f*230.0f)continue;V3 d=vsub(s->b,s->a);float l=vlen(d);if(l<1.5f)continue;V3 r=v3(d.z/l,0,-d.x/l);float hw=minf(g_world.roads[s->road].width*.36f,3.2f);V3 a=vadd(s->a,vmul(r,hw)),b=vsub(s->a,vmul(r,hw)),c=vsub(s->b,vmul(r,hw)),e=vadd(s->b,vmul(r,hw));float flick=.55f+.45f*(float)sin(g_time*.35+i*.17);glColor4f(.38f,.52f,.64f,(.012f+.015f*flick)*g_wetness60);glVertex3f(a.x,.038f,a.z);glVertex3f(b.x,.038f,b.z);glVertex3f(c.x,.038f,c.z);glVertex3f(e.x,.038f,e.z);}glEnd();glDisable(GL_BLEND);renderer09_resume_shader();g_draw_calls++;
}
static void weather60_draw_flash(void){
  if(g_lightning60<=.005f)return;renderer09_suspend_shader();glDisable(GL_DEPTH_TEST);glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);glMatrixMode(GL_PROJECTION);glPushMatrix();glLoadIdentity();glOrtho(0,g_w,g_h,0,-1,1);glMatrixMode(GL_MODELVIEW);glPushMatrix();glLoadIdentity();glColor4f(.82f,.90f,1.0f,.45f*g_lightning60);glBegin(GL_QUADS);glVertex2f(0,0);glVertex2f((float)g_w,0);glVertex2f((float)g_w,(float)g_h);glVertex2f(0,(float)g_h);glEnd();glPopMatrix();glMatrixMode(GL_PROJECTION);glPopMatrix();glMatrixMode(GL_MODELVIEW);glDisable(GL_BLEND);glEnable(GL_DEPTH_TEST);renderer09_resume_shader();
}
