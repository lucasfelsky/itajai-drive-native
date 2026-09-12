// -----------------------------------------------------------------------------
// 2.50 Game Feel & Polish renderer.
// Persistent tyre traces plus cheap wet/loose spray and a restrained impact edge
// flash. Everything is additive on top of the 2.40 world and can be culled cheaply.
// -----------------------------------------------------------------------------
static void feel250_draw_skids(void){
  renderer09_suspend_shader();glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);glDisable(GL_CULL_FACE);
  for(int i=0;i<FEEL250_MARKS;i++){SkidMark250*m=&g_skid250[i];if(m->life<=0)continue;float dx=m->p.x-g_player.body.p.x,dz=m->p.z-g_player.body.p.z;if(dx*dx+dz*dz>210.0f*210.0f)continue;float a=clampf(m->life/2.0f,0,1)*.48f;V3 f=v3((float)sin((double)m->yaw),0,(float)cos((double)m->yaw)),r=v3(f.z,0,-f.x);float hl=.34f,hw=m->width*.5f;V3 p0=vadd(m->p,vadd(vmul(f,-hl),vmul(r,-hw))),p1=vadd(m->p,vadd(vmul(f,hl),vmul(r,-hw))),p2=vadd(m->p,vadd(vmul(f,hl),vmul(r,hw))),p3=vadd(m->p,vadd(vmul(f,-hl),vmul(r,hw)));glColor4f(.045f,.045f,.043f,a);glBegin(GL_QUADS);glVertex3f(p0.x,p0.y,p0.z);glVertex3f(p1.x,p1.y,p1.z);glVertex3f(p2.x,p2.y,p2.z);glVertex3f(p3.x,p3.y,p3.z);glEnd();g_draw_calls++;}
  glDisable(GL_BLEND);renderer09_resume_shader();
}
static void feel250_draw_spray(void){
  if(g_spray250<.035f)return;renderer09_suspend_shader();glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);V3 f=v3((float)sin((double)g_player.body.yaw),0,(float)cos((double)g_player.body.yaw)),r=v3(f.z,0,-f.x),base=vadd(g_player.body.p,vmul(f,-1.45f));float wet=(g_weather>=2||g_weather60_wetness>.18f)?1.0f:0.0f;glLineWidth(2.0f);glBegin(GL_LINES);for(int i=0;i<12;i++){float phase=g_time*(2.3f+.11f*i)+i*1.37f,t=(float)(i+1)/12.0f,side=((i&1)?1.0f:-1.0f)*(.26f+.58f*t);V3 a=vadd(base,vmul(r,side)),b=vadd(a,vadd(vmul(f,-(.55f+1.45f*t)),v3((float)sin(phase)*.12f,.12f+.45f*t,0)));float alpha=g_spray250*(.34f-.16f*t);if(wet>0)glColor4f(.72f,.78f,.80f,alpha);else glColor4f(.62f,.54f,.38f,alpha);glVertex3f(a.x,a.y+.18f,a.z);glVertex3f(b.x,b.y,b.z);}glEnd();g_draw_calls++;glLineWidth(1.0f);glDisable(GL_BLEND);renderer09_resume_shader();
}
static void feel250_draw_impact(void){
  if(g_impact250<.02f||g_photo70_hide_hud)return;settings_draw_2d_begin();glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);float a=.20f*g_impact250,t=8.0f;glColor4f(1.0f,.62f,.38f,a);glBegin(GL_QUADS);glVertex2f(0,0);glVertex2f((float)g_w,0);glVertex2f((float)g_w,t);glVertex2f(0,t);glVertex2f(0,(float)g_h-t);glVertex2f((float)g_w,(float)g_h-t);glVertex2f((float)g_w,(float)g_h);glVertex2f(0,(float)g_h);glVertex2f(0,0);glVertex2f(t,0);glVertex2f(t,(float)g_h);glVertex2f(0,(float)g_h);glVertex2f((float)g_w-t,0);glVertex2f((float)g_w,0);glVertex2f((float)g_w,(float)g_h);glVertex2f((float)g_w-t,(float)g_h);glEnd();glDisable(GL_BLEND);settings_draw_2d_end();
}
static void feel250_draw(void){feel250_draw_skids();feel250_draw_spray();}
