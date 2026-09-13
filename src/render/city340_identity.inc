// -----------------------------------------------------------------------------
// 3.4 City Identity: recognizable Itajai landmark silhouettes and close-range
// facade detail. Coordinates use public/open geographic references and the
// engine's existing project_geo() transform.
// -----------------------------------------------------------------------------
#ifndef GL_TRIANGLES
#define GL_TRIANGLES 0x0004
#endif
static int g_city340_landmarks_drawn=0,g_city340_facades_drawn=0;
static int city340_near(V3 p,float r){float dx=p.x-g_player.body.p.x,dz=p.z-g_player.body.p.z;return dx*dx+dz*dz<r*r;}
static V3 city340_geo(double lat,double lon){return project_geo(lat,lon);}
static V3 city340_rot(V3 c,float x,float z,float yaw){float cs=(float)cos((double)yaw),sn=(float)sin((double)yaw);return v3(c.x+x*cs+z*sn,c.y,c.z-x*sn+z*cs);}

static void city340_pyramid(V3 c,float w,float d,float h,float yaw,float r,float g,float b){
  V3 p0=city340_rot(c,-w*.5f,-d*.5f,yaw),p1=city340_rot(c,w*.5f,-d*.5f,yaw),p2=city340_rot(c,w*.5f,d*.5f,yaw),p3=city340_rot(c,-w*.5f,d*.5f,yaw),tip=v3(c.x,c.y+h,c.z);glColor3f(r,g,b);glBegin(GL_TRIANGLES);
  glVertex3f(p0.x,c.y,p0.z);glVertex3f(p1.x,c.y,p1.z);glVertex3f(tip.x,tip.y,tip.z);
  glVertex3f(p1.x,c.y,p1.z);glVertex3f(p2.x,c.y,p2.z);glVertex3f(tip.x,tip.y,tip.z);
  glVertex3f(p2.x,c.y,p2.z);glVertex3f(p3.x,c.y,p3.z);glVertex3f(tip.x,tip.y,tip.z);
  glVertex3f(p3.x,c.y,p3.z);glVertex3f(p0.x,c.y,p0.z);glVertex3f(tip.x,tip.y,tip.z);glEnd();g_draw_calls++;
}
static void city340_octagon(V3 c,float rad,float h,float r,float g,float b){
  glColor3f(r,g,b);glBegin(GL_QUADS);for(int i=0;i<8;i++){float a=(float)(i*0.78539816339),q=(float)((i+1)*0.78539816339);float x0=c.x+(float)cos(a)*rad,z0=c.z+(float)sin(a)*rad,x1=c.x+(float)cos(q)*rad,z1=c.z+(float)sin(q)*rad;glVertex3f(x0,c.y,z0);glVertex3f(x1,c.y,z1);glVertex3f(x1,c.y+h,z1);glVertex3f(x0,c.y+h,z0);}glEnd();g_draw_calls++;
}
static void city340_cross(V3 p,float s){draw_box(v3(p.x,p.y+s*.58f,p.z),s*.15f,s*1.15f,s*.12f,0,.88f,.82f,.56f);draw_box(v3(p.x,p.y+s*.76f,p.z),s*.68f,s*.15f,s*.12f,0,.88f,.82f,.56f);}

static void city340_church(void){
  // Igreja Matriz do Santissimo Sacramento, Praca Irineu Bornhausen.
  V3 p=city340_geo(-26.9074694,-48.6619194);if(!city340_near(p,1200))return;float y=terrain11_height(p.x,p.z),yaw=-.10f;
  renderer09_set_material(3);
  draw_box(v3(p.x,y+7.2f,p.z),19.0f,14.4f,34.0f,yaw,.90f,.90f,.87f);
  draw_box(city340_rot(v3(p.x,y+8.0f,p.z),0,-17.0f,yaw),18.0f,16.0f,4.6f,yaw,.92f,.92f,.89f);
  for(int s=-1;s<=1;s+=2){V3 t=city340_rot(v3(p.x,y,p.z),s*7.0f,-18.8f,yaw);draw_box(v3(t.x,y+12.0f,t.z),5.0f,24.0f,5.0f,yaw,.91f,.91f,.88f);city340_pyramid(v3(t.x,y+24.0f,t.z),5.8f,5.8f,9.5f,yaw,.55f,.48f,.38f);city340_cross(v3(t.x,y+33.5f,t.z),1.8f);}
  V3 mid=city340_rot(v3(p.x,y,p.z),0,-19.7f,yaw);city340_pyramid(v3(mid.x,y+16.0f,mid.z),10.0f,2.8f,8.4f,yaw,.88f,.88f,.85f);
  draw_box(city340_rot(v3(p.x,y+8.0f,p.z),0,-20.15f,yaw),4.8f,6.8f,.30f,yaw,.19f,.24f,.28f);
  draw_box(city340_rot(v3(p.x,y+14.0f,p.z),0,-20.17f,yaw),3.0f,3.0f,.32f,yaw,.42f,.30f,.20f);
  renderer09_set_material(0);g_city340_landmarks_drawn++;
}

static void city340_market(void){
  // Mercado Publico / Mercado Velho, Rua Felix Busso Asseburg.
  V3 p=city340_geo(-26.9063889,-48.6541667);if(!city340_near(p,1000))return;float y=terrain11_height(p.x,p.z),yaw=.04f;
  renderer09_set_material(3);float cr=.74f,cg=.61f,cb=.43f;
  draw_box(city340_rot(v3(p.x,y+3.4f,p.z),0,-7.0f,yaw),25.0f,6.8f,7.0f,yaw,cr,cg,cb);
  draw_box(city340_rot(v3(p.x,y+3.4f,p.z),0,7.0f,yaw),25.0f,6.8f,7.0f,yaw,cr,cg,cb);
  draw_box(city340_rot(v3(p.x,y+3.4f,p.z),-9.0f,0,yaw),7.0f,6.8f,14.0f,yaw,cr,cg,cb);
  draw_box(city340_rot(v3(p.x,y+3.4f,p.z),9.0f,0,yaw),7.0f,6.8f,14.0f,yaw,cr,cg,cb);
  draw_box(city340_rot(v3(p.x,y+4.2f,p.z),0,-10.65f,yaw),7.0f,2.2f,.35f,yaw,.25f,.18f,.12f);
  for(int i=-3;i<=3;i++)draw_box(city340_rot(v3(p.x,y+2.0f,p.z),i*3.0f,-10.75f,yaw),1.6f,2.8f,.18f,yaw,.18f,.24f,.22f);
  renderer09_set_material(0);g_city340_landmarks_drawn++;
}

static void city340_centreventos(void){
  V3 p=city340_geo(-26.90959,-48.65287);if(!city340_near(p,1400))return;float y=terrain11_height(p.x,p.z),yaw=-.03f;
  renderer09_set_material(3);draw_box(v3(p.x,y+5.1f,p.z),48.0f,10.2f,30.0f,yaw,.58f,.62f,.64f);
  for(int i=-2;i<=2;i++){float z=i*5.5f;float hh=1.6f-(float)(i*i)*.13f;draw_box(city340_rot(v3(p.x,y+10.6f,p.z),0,z,yaw),45.0f,hh,5.8f,yaw,.72f,.75f,.75f);}
  V3 f=city340_rot(v3(p.x,y+4.1f,p.z),0,-15.2f,yaw);draw_box(f,34.0f,6.4f,.35f,yaw,.12f,.25f,.31f);
  renderer09_set_material(0);g_city340_landmarks_drawn++;
}

static void city340_univali(void){
  V3 p=city340_geo(-26.91620,-48.66405);if(!city340_near(p,1400))return;float y=terrain11_height(p.x,p.z),yaw=.12f;
  renderer09_set_material(3);draw_box(v3(p.x,y+7.0f,p.z),34.0f,14.0f,12.0f,yaw,.75f,.76f,.73f);
  draw_box(city340_rot(v3(p.x,y+4.2f,p.z),20.0f,3.5f,yaw),22.0f,8.4f,8.0f,yaw,.64f,.68f,.70f);
  draw_box(city340_rot(v3(p.x,y+4.2f,p.z),-20.0f,4.0f,yaw),19.0f,8.4f,9.0f,yaw,.68f,.70f,.69f);
  V3 s=city340_rot(v3(p.x,y+3.2f,p.z),0,-9.0f,yaw);draw_box(s,14.0f,2.2f,.28f,yaw,.07f,.23f,.42f);
  renderer09_set_material(0);renderer09_suspend_shader();identity150_text3d(v3(s.x-5.3f,y+3.35f,s.z-.18f),"UNIVALI",.96f,.96f,.96f);renderer09_resume_shader();g_city340_landmarks_drawn++;
}

static void city340_molhes(void){
  V3 p=city340_geo(-26.914663,-48.641616);if(!city340_near(p,1600))return;float y=terrain11_height(p.x,p.z);
  renderer09_set_material(0);draw_box(v3(p.x,y+.20f,p.z),8.0f,.38f,92.0f,.06f,.40f,.41f,.40f);
  V3 l=city340_rot(v3(p.x,y,p.z),0,-35.0f,.06f);city340_octagon(v3(l.x,y+.15f,l.z),2.2f,13.0f,.88f,.86f,.79f);city340_octagon(v3(l.x,y+13.15f,l.z),2.7f,1.8f,.72f,.18f,.14f);city340_pyramid(v3(l.x,y+14.95f,l.z),4.5f,4.5f,3.4f,0,.72f,.18f,.14f);g_city340_landmarks_drawn++;
}

static void city340_bico(void){
  V3 p=city340_geo(-26.920731,-48.640541);if(!city340_near(p,1200))return;float y=terrain11_height(p.x,p.z);renderer09_set_material(0);
  draw_box(v3(p.x,y+3.2f,p.z),7.8f,6.4f,6.0f,.18f,.36f,.34f,.31f);
  draw_box(v3(p.x+2.8f,y+6.4f,p.z-.6f),4.5f,5.4f,3.8f,-.28f,.39f,.37f,.33f);
  draw_box(v3(p.x+5.0f,y+8.3f,p.z-1.8f),5.8f,2.2f,3.0f,-.42f,.42f,.39f,.34f);
  draw_box(v3(p.x+7.3f,y+8.7f,p.z-2.4f),3.3f,1.3f,2.0f,-.56f,.43f,.40f,.35f);g_city340_landmarks_drawn++;
}

static void city340_port(void){
  V3 p=project_geo(-26.8997,-48.6574);if(!city340_near(p,1700))return;float y=terrain11_height(p.x,p.z);renderer09_set_material(0);
  for(int i=0;i<4;i++){V3 q=v3(p.x+i*34.0f,y,p.z-i*5.0f);draw_box(v3(q.x,y+16.0f,q.z),1.8f,32.0f,1.8f,0,.67f,.69f,.65f);draw_box(v3(q.x+7.5f,y+30.0f,q.z),17.0f,1.5f,1.5f,0,.76f,.77f,.71f);draw_box(v3(q.x+15.0f,y+25.0f,q.z),1.1f,10.0f,1.1f,0,.68f,.70f,.66f);}
  for(int i=0;i<10;i++){float x=p.x-25.0f+(i%5)*12.0f,z=p.z+18.0f+(i/5)*4.8f;draw_box(v3(x,y+1.4f,z),10.5f,2.8f,4.0f,0,(i%3==0)?.55f:.16f,(i%3==1)?.34f:.22f,(i%3==2)?.46f:.18f);}
  g_city340_landmarks_drawn++;
}

static void city340_facades(void){
  g_city340_facades_drawn=0;if(!g_stream_ready08)return;renderer09_set_material(0);
  for(int i=0;i<g_stream_building_count08;i++){Building*b=&g_stream_buildings08[i].b;if(!city340_near(b->c,150.0f)||b->h<5.5f)continue;V3 core=city340_geo(-26.90675,-48.65845),delta=vsub(b->c,core);if(delta.x*delta.x+delta.z*delta.z<760*760)continue;int id=building12_find_seed(b->seed);if(id<0)continue;int n=g_building_poly_count12[id],s=g_building_poly_start12[id];if(n<3||s<0||s+n>g_building_vert_count12)continue;int floors=(int)(b->h/3.15f);if(floors<1)floors=1;if(floors>8)floors=8;float base=terrain11_height(b->c.x,b->c.z);for(int e=0;e<n;e+=maxf(1,n/6)){V3 a=g_building_verts12[s+e],c=g_building_verts12[s+(e+1)%n],d=vsub(c,a);float len=vlen(d);if(len<5.0f)continue;V3 m=vmul(vadd(a,c),.5f);float yaw=(float)atan2((double)-d.z,(double)d.x);int bays=(int)(len/3.6f);if(bays<1)bays=1;if(bays>7)bays=7;V3 f=vmul(d,1.0f/len);for(int fl=0;fl<floors;fl++){float yy=base+1.7f+fl*3.15f;if(yy>base+b->h-.7f)break;for(int k=0;k<bays;k++){float along=((k+.5f)/bays-.5f)*len*.82f;V3 w=vadd(m,vmul(f,along));float lit=((b->seed+(u32)(e*17+fl*31+k*13))&7u)==0u?1.0f:.0f;draw_box(v3(w.x,yy,w.z),minf(2.0f,len/bays*.52f),1.20f,.08f,yaw,lit?.78f:.17f,lit?.70f:.24f,lit?.49f:.29f);g_city340_facades_drawn++;}}}}
}

static void city340_draw(void){
  g_city340_landmarks_drawn=0;city340_church();city340_market();city340_centreventos();city340_univali();city340_molhes();city340_bico();city340_port();city340_facades();renderer09_set_material(0);
}
static void city340_draw_overlay(void){
  if(!g_debug||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();char b[180];hud_panel(20,(float)g_h-166,610,28);sprintf(b,"CITY 3.4 // landmarks %d | facade details %d | MATRIZ + MERCADO + CENTREVENTOS + UNIVALI + MOLHES",g_city340_landmarks_drawn,g_city340_facades_drawn);gl_text(34,(float)g_h-145,b,.72f,.86f,.72f);settings_draw_2d_end();
}
