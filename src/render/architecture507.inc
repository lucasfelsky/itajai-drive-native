// 5.0.7 original architectural primitives shared by the authored Centro buildings.
// All profiles are generated; nothing here is traced from a photograph.
// Hip roof: four sloped faces meeting a ridge along local Z (ridge length d-2*w/2*k).
static void architecture507_hip(V3 c,float w,float d,float h,float yaw,float r,float g,float b){
  float ridge=d-w;if(ridge<0)ridge=0;float hx=w*.5f,hz=d*.5f,rz=ridge*.5f;
  glPushMatrix();glTranslatef(c.x,c.y,c.z);glRotatef(yaw*57.29578f,0,1,0);glColor3f(r,g,b);
  glBegin(GL_QUADS);
  {V3 n=vnorm(v3(h,hx,0));glNormal3f(-n.x,n.y,0);glVertex3f(-hx,0,-hz);glVertex3f(0,h,-rz);glVertex3f(0,h,rz);glVertex3f(-hx,0,hz);
   glNormal3f(n.x,n.y,0);glVertex3f(hx,0,hz);glVertex3f(0,h,rz);glVertex3f(0,h,-rz);glVertex3f(hx,0,-hz);}
  glEnd();
  glBegin(GL_TRIANGLES);
  {V3 n=vnorm(v3(0,hx,h));glNormal3f(0,n.y,-n.z);glVertex3f(hx,0,-hz);glVertex3f(0,h,-rz);glVertex3f(-hx,0,-hz);
   glNormal3f(0,n.y,n.z);glVertex3f(-hx,0,hz);glVertex3f(0,h,rz);glVertex3f(hx,0,hz);}
  glEnd();glPopMatrix();g_draw_calls+=2;
}
// Vertical prism (round tower approximation) with optional closed top.
static void architecture507_drum(V3 base,float rad,float h,int sides,float r,float g,float b){
  glColor3f(r,g,b);glBegin(GL_QUADS);
  for(int i=0;i<sides;i++){float a=i*6.2831853f/sides,q=(i+1)*6.2831853f/sides,x0=(float)cos(a),z0=(float)sin(a),x1=(float)cos(q),z1=(float)sin(q);glNormal3f((x0+x1)*.5f,0,(z0+z1)*.5f);
    glVertex3f(base.x+x0*rad,base.y,base.z+z0*rad);glVertex3f(base.x+x1*rad,base.y,base.z+z1*rad);glVertex3f(base.x+x1*rad,base.y+h,base.z+z1*rad);glVertex3f(base.x+x0*rad,base.y+h,base.z+z0*rad);}
  glEnd();glNormal3f(0,1,0);glBegin(GL_TRIANGLES);
  for(int i=0;i<sides;i++){float a=i*6.2831853f/sides,q=(i+1)*6.2831853f/sides;glVertex3f(base.x,base.y+h,base.z);glVertex3f(base.x+(float)cos(q)*rad,base.y+h,base.z+(float)sin(q)*rad);glVertex3f(base.x+(float)cos(a)*rad,base.y+h,base.z+(float)sin(a)*rad);}
  glEnd();g_draw_calls+=2;
}
static void architecture507_cone(V3 base,float rad,float h,int sides,float r,float g,float b){
  glColor3f(r,g,b);glBegin(GL_TRIANGLES);
  for(int i=0;i<sides;i++){float a=i*6.2831853f/sides,q=(i+1)*6.2831853f/sides;V3 n=vnorm(v3((float)cos((a+q)*.5f)*h,rad,(float)sin((a+q)*.5f)*h));glNormal3f(n.x,n.y,n.z);
    glVertex3f(base.x+(float)cos(a)*rad,base.y,base.z+(float)sin(a)*rad);glVertex3f(base.x+(float)cos(q)*rad,base.y,base.z+(float)sin(q)*rad);glVertex3f(base.x,base.y+h,base.z);}
  glEnd();g_draw_calls++;
}
// Half drum + half cone: church apse closing a wall that faces local +Z.
static void architecture507_apse(V3 c,float rad,float h,float roof,float yaw,float r,float g,float b,float rr,float rg,float rb){
  glPushMatrix();glTranslatef(c.x,c.y,c.z);glRotatef(yaw*57.29578f,0,1,0);int sides=10;glColor3f(r,g,b);glBegin(GL_QUADS);
  for(int i=0;i<sides;i++){float a=i*3.14159265f/sides,q=(i+1)*3.14159265f/sides,x0=(float)cos(a),z0=(float)sin(a),x1=(float)cos(q),z1=(float)sin(q);glNormal3f((x0+x1)*.5f,0,(z0+z1)*.5f);
    glVertex3f(x0*rad,0,z0*rad);glVertex3f(x1*rad,0,z1*rad);glVertex3f(x1*rad,h,z1*rad);glVertex3f(x0*rad,h,z0*rad);}
  glEnd();glColor3f(rr,rg,rb);glBegin(GL_TRIANGLES);
  for(int i=0;i<sides;i++){float a=i*3.14159265f/sides,q=(i+1)*3.14159265f/sides;V3 n=vnorm(v3((float)cos((a+q)*.5f)*roof,rad,(float)sin((a+q)*.5f)*roof));glNormal3f(n.x,n.y,n.z);
    glVertex3f((float)cos(a)*rad,h,(float)sin(a)*rad);glVertex3f((float)cos(q)*rad,h,(float)sin(q)*rad);glVertex3f(0,h+roof,0);}
  glEnd();glPopMatrix();g_draw_calls+=2;
}
// Segmental barrel vault spanning local X, running along local Z (event hall roof).
static void architecture507_vault(V3 c,float w,float d,float rise,float yaw,float r,float g,float b){
  int segs=10;glPushMatrix();glTranslatef(c.x,c.y,c.z);glRotatef(yaw*57.29578f,0,1,0);glBegin(GL_QUADS);
  for(int i=0;i<segs;i++){float t0=(float)i/segs,t1=(float)(i+1)/segs,x0=(t0-.5f)*w,x1=(t1-.5f)*w,y0=rise*(float)sin(t0*3.14159265f),y1=rise*(float)sin(t1*3.14159265f);
    V3 n=vnorm(v3(-(y1-y0),x1-x0,0));float shade=.86f+.10f*n.y;glColor3f(r*shade,g*shade,b*shade);glNormal3f(n.x,n.y,0);
    glVertex3f(x0,y0,-d*.5f);glVertex3f(x1,y1,-d*.5f);glVertex3f(x1,y1,d*.5f);glVertex3f(x0,y0,d*.5f);}
  glEnd();
  // End caps.
  glColor3f(r*.78f,g*.78f,b*.78f);glBegin(GL_TRIANGLES);
  for(int s=-1;s<=1;s+=2){glNormal3f(0,0,(float)s);for(int i=0;i<segs;i++){float t0=(float)i/segs,t1=(float)(i+1)/segs,x0=(t0-.5f)*w,x1=(t1-.5f)*w,y0=rise*(float)sin(t0*3.14159265f),y1=rise*(float)sin(t1*3.14159265f);
    if(s>0){glVertex3f(0,0,d*.5f);glVertex3f(x0,y0,d*.5f);glVertex3f(x1,y1,d*.5f);}else{glVertex3f(0,0,-d*.5f);glVertex3f(x1,y1,-d*.5f);glVertex3f(x0,y0,-d*.5f);}}}
  glEnd();glPopMatrix();g_draw_calls+=2;
}
// Curved wall following a local polyline (x,z pairs), extruded from y0 to y1.
static void architecture507_wall(V3 c,const float*pts,int n,float y0,float y1,float yaw,float r,float g,float b){
  glPushMatrix();glTranslatef(c.x,c.y,c.z);glRotatef(yaw*57.29578f,0,1,0);glColor3f(r,g,b);glBegin(GL_QUADS);
  for(int i=0;i+1<n;i++){float x0=pts[i*2],z0=pts[i*2+1],x1=pts[i*2+2],z1=pts[i*2+3];V3 nn=vnorm(v3(-(z1-z0),0,x1-x0));glNormal3f(nn.x,0,nn.z);glVertex3f(x0,y0,z0);glVertex3f(x1,y0,z1);glVertex3f(x1,y1,z1);glVertex3f(x0,y1,z0);}
  glEnd();glPopMatrix();g_draw_calls++;
}
