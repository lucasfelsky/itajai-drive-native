// 0.9 lit/texturable box primitives. The 0.6 versions remain available as
// *_legacy06 after macro-renaming in native.c.
static void draw_unit_box(void){glBegin(GL_QUADS);
  glNormal3f(0,0,1); glTexCoord2f(0,0);glVertex3f(-.5f,-.5f,.5f);glTexCoord2f(1,0);glVertex3f(.5f,-.5f,.5f);glTexCoord2f(1,1);glVertex3f(.5f,.5f,.5f);glTexCoord2f(0,1);glVertex3f(-.5f,.5f,.5f);
  glNormal3f(0,0,-1);glTexCoord2f(0,0);glVertex3f(.5f,-.5f,-.5f);glTexCoord2f(1,0);glVertex3f(-.5f,-.5f,-.5f);glTexCoord2f(1,1);glVertex3f(-.5f,.5f,-.5f);glTexCoord2f(0,1);glVertex3f(.5f,.5f,-.5f);
  glNormal3f(-1,0,0);glTexCoord2f(0,0);glVertex3f(-.5f,-.5f,-.5f);glTexCoord2f(1,0);glVertex3f(-.5f,-.5f,.5f);glTexCoord2f(1,1);glVertex3f(-.5f,.5f,.5f);glTexCoord2f(0,1);glVertex3f(-.5f,.5f,-.5f);
  glNormal3f(1,0,0); glTexCoord2f(0,0);glVertex3f(.5f,-.5f,.5f);glTexCoord2f(1,0);glVertex3f(.5f,-.5f,-.5f);glTexCoord2f(1,1);glVertex3f(.5f,.5f,-.5f);glTexCoord2f(0,1);glVertex3f(.5f,.5f,.5f);
  glNormal3f(0,1,0); glTexCoord2f(0,0);glVertex3f(-.5f,.5f,.5f);glTexCoord2f(1,0);glVertex3f(.5f,.5f,.5f);glTexCoord2f(1,1);glVertex3f(.5f,.5f,-.5f);glTexCoord2f(0,1);glVertex3f(-.5f,.5f,-.5f);
  glNormal3f(0,-1,0);glTexCoord2f(0,0);glVertex3f(-.5f,-.5f,-.5f);glTexCoord2f(1,0);glVertex3f(.5f,-.5f,-.5f);glTexCoord2f(1,1);glVertex3f(.5f,-.5f,.5f);glTexCoord2f(0,1);glVertex3f(-.5f,-.5f,.5f);
  glEnd();g_draw_calls++;}
static void draw_box(V3 p,float sx,float sy,float sz,float yaw,float r,float g,float b){glPushMatrix();glTranslatef(p.x,p.y,p.z);glRotatef(yaw*57.29578f,0,1,0);glScalef(sx,sy,sz);glColor3f(r,g,b);draw_unit_box();glPopMatrix();}
