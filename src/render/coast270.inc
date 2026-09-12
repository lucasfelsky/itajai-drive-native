// -----------------------------------------------------------------------------
// 2.70 Coast & River 3.0.
// Art-directed Itajai-Acu/channel and Beira-Rio water surfaces that complement the
// existing east-coast ocean. Coordinates are intentionally conservative and tied
// to the same waterfront/boat zones already used by the living-world pass.
// -----------------------------------------------------------------------------
static int g_coast270_draws=0;
static int coast270_near(float x,float z,float d){float dx=x-g_player.body.p.x,dz=z-g_player.body.p.z;return dx*dx+dz*dz<d*d;}
static void coast270_water_quad(float x0,float z0,float x1,float z1,float alpha){float wave=(float)sin(g_time*.62+(x0+z0)*.011f)*.035f;glColor4f(.07f,.30f,.43f,alpha);glTexCoord2f(x0*.014f,z0*.014f);glVertex3f(x0,.03f+wave,z0);glTexCoord2f(x1*.014f,z0*.014f);glVertex3f(x1,.03f+wave,z0);glTexCoord2f(x1*.014f,z1*.014f);glVertex3f(x1,.03f-wave*.4f,z1);glTexCoord2f(x0*.014f,z1*.014f);glVertex3f(x0,.03f-wave*.4f,z1);}
static void coast270_draw_channel(void){
  if(!coast270_near(-320,-1040,2200))return;renderer09_set_material(MAT09_WATER);renderer09_suspend_shader();glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);glBegin(GL_QUADS);
  // Port/channel: segmented so the surface subtly bends toward the Molhes mouth.
  const float xs[7]={-1950,-1450,-900,-350,250,780,1320};const float zs[7]={-1190,-1165,-1135,-1105,-1060,-1015,-970};
  for(int i=0;i<6;i++){float half0=105.0f+i*3.0f,half1=108.0f+i*3.0f;coast270_water_quad(xs[i],zs[i]-half0,xs[i+1],zs[i+1]+half1,.82f);}glEnd();g_coast270_draws++;
  // Beira-Rio waterfront sheet beyond the pedestrian promenade.
  if(coast270_near(-110,1120,900)){glBegin(GL_QUADS);coast270_water_quad(-880,1070,620,1265,.78f);glEnd();g_coast270_draws++;}
  glDisable(GL_BLEND);renderer09_resume_shader();renderer09_set_material(MAT09_GENERIC);
}
static void coast270_draw_foam(void){
  if(g_player.body.p.x<900)return;renderer09_suspend_shader();glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);glLineWidth(2.0f);glBegin(GL_LINES);for(int i=-14;i<=14;i++){float z=g_player.body.p.z+i*55.0f,x=terrain11_shore_x(z)+10.0f;float a=.34f+.16f*(float)sin(g_time*1.15+i*.7f);glColor4f(.82f,.90f,.91f,a);glVertex3f(x,.08f,z-18);glVertex3f(x+4.5f,.08f,z+18);}glEnd();glLineWidth(1.0f);glDisable(GL_BLEND);renderer09_resume_shader();g_coast270_draws++;}
static void coast270_draw(void){g_coast270_draws=0;coast270_draw_channel();coast270_draw_foam();}
