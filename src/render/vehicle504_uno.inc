// Renderer 5.0.4 hero treatment for the Novo Uno Way.
static int g_vehicle504_uno_parts=0;
static void vehicle504_uno_bar(float x,float y,float z,float sx,float sy,float sz,float rx,float ry,float rz,float r,float g,float b){vehicle160_local_box_rot(x,y,z,sx,sy,sz,rx,ry,rz,r,g,b);g_vehicle504_uno_parts++;}
static void vehicle504_uno_disc(float x,float y,float z,float rad,float r,float g,float b){glPushMatrix();glTranslatef(x,y,z);glRotatef(90,0,1,0);vehicle160_cylinder_x(rad,.035f,r,g,b);glPopMatrix();g_vehicle504_uno_parts++;}
static void vehicle505_uno_lamp(float x,float y,float z,float w,float h,float r,float g,float b){
  // Rounded rectangular lens; one perimeter strip and a curved face fan.
  glColor3f(r,g,b);glNormal3f(0,0,1);glBegin(GL_TRIANGLE_FAN);glVertex3f(x,y,z+.022f);
  for(int k=0;k<=32;k++){float a=k*6.2831853f/32,c=(float)cos(a),sn=(float)sin(a);float xx=(c<0?-1.0f:1.0f)*(float)sqrt(absf(c))*w*.5f,yy=(sn<0?-1.0f:1.0f)*(float)sqrt(absf(sn))*h*.5f;glVertex3f(x+xx,y+yy,z);}
  glEnd();g_draw_calls++;g_vehicle504_uno_parts++;
}
static void vehicle504_uno_local(void){const CarVisual160*v=&g_car_visual160[0];float W=v->w,L=v->l,H=v->h;renderer09_set_material(MAT09_RUBBER);
  // Way crossover mask, side cladding and silver lower protector.
  vehicle504_uno_bar(0,.49f,L*.501f,W*.88f,.26f,.055f,0,0,0,.075f,.080f,.082f);vehicle504_uno_bar(0,.37f,L*.516f,W*.60f,.12f,.030f,0,0,0,.035f,.040f,.042f);vehicle504_uno_bar(0,.315f,L*.520f,W*.47f,.055f,.035f,0,0,0,.48f,.49f,.48f);
  vehicle504_uno_bar(-W*.475f,.48f,0,.085f,.22f,L*.60f,0,0,0,.065f,.070f,.072f);vehicle504_uno_bar(W*.475f,.48f,0,.085f,.22f,L*.60f,0,0,0,.065f,.070f,.072f);
  // Three asymmetric rounded intake apertures identify the original Novo Uno.
  renderer09_set_material(MAT09_RUBBER);for(int k=0;k<3;k++)vehicle505_uno_lamp(.12f+k*.115f,.80f,1.91f,.073f,.085f,.045f,.052f,.052f);
  for(int side=-1;side<=1;side+=2){
    renderer09_set_material(MAT09_RUBBER);vehicle505_uno_lamp(side*.535f,.805f,1.91f,.365f,.255f,.10f,.115f,.12f);
    renderer09_set_material(MAT09_GLASS);vehicle505_uno_lamp(side*.535f,.805f,1.939f,.316f,.21f,.65f,.71f,.72f);
    vehicle505_uno_lamp(side*.535f,.805f,1.968f,.18f,.13f,.80f,.83f,.80f);
    renderer09_set_material(MAT09_RUBBER);vehicle505_uno_lamp(side*.515f,.425f,1.935f,.17f,.155f,.025f,.031f,.033f);
    renderer09_set_material(MAT09_GLASS);vehicle504_uno_disc(side*.515f,.425f,1.96f,.059f,.69f,.73f,.71f);
  }
  renderer09_set_material(MAT09_RUBBER);vehicle505_uno_lamp(0,.435f,1.973f,.66f,.14f,.025f,.03f,.032f);
  // Tall lamps follow the real hatch edge; glazing comes from the mesh pipeline.
  renderer09_set_material(MAT09_GLASS);for(int side=-1;side<=1;side+=2){
    vehicle504_uno_bar(side*.728f,1.17f,-1.765f,.112f,.53f,.035f,-10,0,0,.66f,.035f,.025f);
    vehicle504_uno_bar(side*.728f,1.23f,-1.794f,.083f,.11f,.038f,-10,0,0,.83f,.48f,.24f);
    renderer09_set_material(MAT09_RUBBER);vehicle504_uno_bar(side*.888f,1.035f,.59f,.17f,.115f,.215f,0,0,0,.055f,.062f,.063f);
    renderer09_set_material(MAT09_GLASS);vehicle504_uno_bar(side*.91f,1.04f,.575f,.012f,.077f,.15f,0,0,0,.20f,.29f,.33f);
    renderer09_set_material(MAT09_RUBBER);for(int door=0;door<2;door++)vehicle504_uno_bar(side*.817f,.918f,-.39f+door*.83f,.028f,.035f,.16f,0,0,0,.10f,.11f,.11f);
  }
  renderer09_set_material(MAT09_RUBBER);
  vehicle504_uno_bar(.07f,1.064f,-1.805f,.38f,.018f,.022f,0,0,-9,.08f,.085f,.09f);
  vehicle504_uno_bar(0,1.63f,-1.24f,.015f,.21f,.015f,-25,0,0,.055f,.06f,.06f);
  vehicle180_plate(.52f,1.98f,1);vehicle180_plate(.67f,-1.924f,0);
  renderer09_set_material(MAT09_GENERIC);
}
static void vehicle504_uno_player(void){if(!g_started||g_player.camera==2||g_player_model160[g_player.car&3]!=0)return;glPushMatrix();glTranslatef(g_player.body.p.x,g_player.body.p.y-.66f,g_player.body.p.z);glRotatef(g_player.body.yaw*57.29578f,0,1,0);glRotatef(g_player.pitch*57.29578f,1,0,0);glRotatef(g_player.roll*57.29578f,0,0,1);vehicle504_uno_local();glPopMatrix();}
static void vehicle504_uno_traffic(void){float md=minf(g_lod20_traffic_dist,72.0f);md*=md;for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active||((i*5+i/3)&7)!=0)continue;float dx=t->p.x-g_player.body.p.x,dz=t->p.z-g_player.body.p.z;if(dx*dx+dz*dz>md)continue;glPushMatrix();glTranslatef(t->p.x,t->p.y-.55f,t->p.z);glRotatef(t->yaw*57.29578f,0,1,0);vehicle504_uno_local();glPopMatrix();}}
