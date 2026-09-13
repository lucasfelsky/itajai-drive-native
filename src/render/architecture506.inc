// Original mesh details for historic buildings. Local front faces negative Z.
// Real semicircular profiles replace rectangular painted window approximations.
static void architecture506_arch(V3 p,float w,float h,float yaw,float r,float g,float b){
  float rad=w*.5f,spring=h-rad,trim=.17f;
  renderer09_set_material(0);glPushMatrix();glTranslatef(p.x,p.y,p.z);glRotatef(yaw*57.29578f,0,1,0);
  draw_box(v3(0,spring*.5f,0),w,spring,.10f,0,r,g,b);
  glNormal3f(0,0,-1);glColor3f(r,g,b);glBegin(GL_TRIANGLES);
  for(int i=0;i<16;i++){float a=i*3.14159265f/16,c=(i+1)*3.14159265f/16;glVertex3f(0,spring,-.06f);glVertex3f(rad*(float)cos(c),spring+rad*(float)sin(c),-.06f);glVertex3f(rad*(float)cos(a),spring+rad*(float)sin(a),-.06f);}glEnd();
  glColor3f(.91f,.88f,.77f);glBegin(GL_QUADS);
  for(int i=0;i<16;i++){float a=i*3.14159265f/16,c=(i+1)*3.14159265f/16;glVertex3f(rad*(float)cos(a),spring+rad*(float)sin(a),-.10f);glVertex3f(rad*(float)cos(c),spring+rad*(float)sin(c),-.10f);glVertex3f((rad+trim)*(float)cos(c),spring+(rad+trim)*(float)sin(c),-.10f);glVertex3f((rad+trim)*(float)cos(a),spring+(rad+trim)*(float)sin(a),-.10f);}glEnd();g_draw_calls+=2;
  for(int s=-1;s<=1;s+=2)draw_box(v3(s*(rad+trim*.5f),spring*.5f,-.10f),trim,spring,.16f,0,.91f,.88f,.77f);
  draw_box(v3(0,h*.43f,-.12f),.075f,h*.86f,.08f,0,.59f,.55f,.43f);
  draw_box(v3(0,spring*.63f,-.12f),w,.075f,.08f,0,.59f,.55f,.43f);
  draw_box(v3(0,-.09f,-.10f),w+.48f,.18f,.30f,0,.91f,.88f,.77f);glPopMatrix();
}
static void architecture506_rose(V3 p,float rad,float yaw){
  glPushMatrix();glTranslatef(p.x,p.y,p.z);glRotatef(yaw*57.29578f,0,1,0);glNormal3f(0,0,-1);
  glBegin(GL_TRIANGLES);for(int i=0;i<24;i++){float a=i*.2617994f,b=(i+1)*.2617994f;glColor3f(i%3==0?.38f:.12f,i%3==1?.39f:.24f,i%3==2?.51f:.25f);glVertex3f(0,0,-.08f);glVertex3f(rad*(float)cos(b),rad*(float)sin(b),-.08f);glVertex3f(rad*(float)cos(a),rad*(float)sin(a),-.08f);}glEnd();
  glColor3f(.86f,.83f,.73f);glBegin(GL_QUADS);for(int i=0;i<32;i++){float a=i*.1963495f,b=(i+1)*.1963495f;glVertex3f(rad*(float)cos(a),rad*(float)sin(a),-.12f);glVertex3f(rad*(float)cos(b),rad*(float)sin(b),-.12f);glVertex3f((rad+.22f)*(float)cos(b),(rad+.22f)*(float)sin(b),-.12f);glVertex3f((rad+.22f)*(float)cos(a),(rad+.22f)*(float)sin(a),-.12f);}glEnd();g_draw_calls+=2;
  for(int i=0;i<8;i++){glPushMatrix();glRotatef(i*45.0f,0,0,1);draw_box(v3(0,rad*.5f,-.14f),.07f,rad,.07f,0,.84f,.80f,.69f);glPopMatrix();}glPopMatrix();
}
static void architecture506_clock(V3 p,float rad,float yaw){
  glPushMatrix();glTranslatef(p.x,p.y,p.z);glRotatef(yaw*57.29578f,0,1,0);glNormal3f(0,0,-1);glColor3f(.83f,.81f,.72f);glBegin(GL_TRIANGLES);
  for(int i=0;i<32;i++){float a=i*.1963495f,b=(i+1)*.1963495f;glVertex3f(0,0,-.10f);glVertex3f(rad*(float)cos(b),rad*(float)sin(b),-.10f);glVertex3f(rad*(float)cos(a),rad*(float)sin(a),-.10f);}glEnd();g_draw_calls++;
  for(int i=0;i<12;i++){glPushMatrix();glRotatef(i*30.0f,0,0,1);draw_box(v3(0,rad*.81f,-.15f),.065f,rad*.17f,.05f,0,.17f,.19f,.18f);glPopMatrix();}
  draw_box(v3(0,rad*.28f,-.17f),.07f,rad*.56f,.06f,0,.12f,.14f,.13f);draw_box(v3(rad*.23f,0,-.17f),rad*.46f,.07f,.06f,0,.12f,.14f,.13f);glPopMatrix();
}
static void architecture506_roof(V3 p,float w,float d,float rise,float yaw){
  glPushMatrix();glTranslatef(p.x,p.y,p.z);glRotatef(yaw*57.29578f,0,1,0);glColor3f(.46f,.27f,.17f);glBegin(GL_QUADS);
  for(int s=-1;s<=1;s+=2){V3 n=vnorm(v3(s*rise,w*.5f,0));glNormal3f(n.x,n.y,n.z);glVertex3f(s*w*.5f,0,-d*.5f);glVertex3f(s*w*.5f,0,d*.5f);glVertex3f(0,rise,d*.5f);glVertex3f(0,rise,-d*.5f);}glEnd();g_draw_calls++;
  draw_box(v3(0,rise,0),.24f,.20f,d+.2f,0,.57f,.35f,.22f);glPopMatrix();
}
