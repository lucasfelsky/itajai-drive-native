// -----------------------------------------------------------------------------
// Renderer 5.0 HDR sky accents. The historical sky remains the compatibility
// backdrop; this layer adds energy above 1.0 only while rendering into RGBA16F,
// giving the final bloom/tonemap pass real sun, moon and lightning highlights.
// -----------------------------------------------------------------------------
static void sky500_quad(float x,float y,float r,float cr,float cg,float cb,float a){glColor4f(cr,cg,cb,a);glVertex2f(x-r,y-r);glVertex2f(x+r,y-r);glVertex2f(x+r,y+r);glVertex2f(x-r,y+r);}
static void renderer500_draw_sky_extras(void){
  if(!g_renderer09_linear_output)return;renderer09_suspend_shader();glDisable(GL_DEPTH_TEST);glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE);glMatrixMode(GL_PROJECTION);glPushMatrix();glLoadIdentity();glOrtho(0,g_w,g_h,0,-1,1);glMatrixMode(GL_MODELVIEW);glPushMatrix();glLoadIdentity();
  int day=g_hour>=6&&g_hour<=18;float cloud=clampf(g_cloud60,0,1);if(day){float t=((float)g_hour-6.0f)/12.0f,sx=80.0f+t*(g_w-160.0f),sy=180.0f-(float)sin((double)(t*3.14159265f))*110.0f;float clear=clampf(1.0f-cloud*.72f,0.20f,1.0f);glBegin(GL_QUADS);sky500_quad(sx,sy,105.0f,1.0f,.44f,.10f,.025f*clear);sky500_quad(sx,sy,52.0f,1.0f,.62f,.18f,.070f*clear);sky500_quad(sx,sy,20.0f,1.0f,.82f,.42f,.22f*clear);sky500_quad(sx,sy,7.0f,1.0f,1.0f,.88f,.78f*clear);glEnd();}
  else {float visibility=clampf(1.0f-cloud*.82f,0.08f,1.0f);glPointSize(1.4f);glBegin(GL_POINTS);for(int i=0;i<44;i++){u32 h=renderer09_hash((u32)i,0x5005u,0x51a7u);float x=((h&65535u)/65535.0f)*g_w,y=18.0f+(((h>>16)&65535u)/65535.0f)*(g_h*.58f);float a=.10f+.18f*((h>>8)&7u)/7.0f;glColor4f(.72f,.82f,1.0f,a*visibility);glVertex2f(x,y);}glEnd();float nt=g_hour<6?((float)g_hour+6.0f)/12.0f:((float)g_hour-18.0f)/12.0f;float mx=g_w*(.78f-.48f*nt),my=g_h*(.20f-.05f*(float)sin(nt*3.14159265f));glBegin(GL_QUADS);sky500_quad(mx,my,48.0f,.34f,.50f,.78f,.035f*visibility);sky500_quad(mx,my,24.0f,.58f,.72f,1.0f,.085f*visibility);sky500_quad(mx,my,9.0f,.82f,.90f,1.0f,.48f*visibility);glEnd();}
  if(g_lightning60>.01f){glBegin(GL_QUADS);glColor4f(.72f,.82f,1.0f,.12f*g_lightning60);glVertex2f(0,0);glVertex2f((float)g_w,0);glVertex2f((float)g_w,(float)g_h);glVertex2f(0,(float)g_h);glEnd();}
  glPopMatrix();glMatrixMode(GL_PROJECTION);glPopMatrix();glMatrixMode(GL_MODELVIEW);glDisable(GL_BLEND);glEnable(GL_DEPTH_TEST);renderer09_resume_shader();
}
