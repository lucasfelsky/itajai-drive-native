// -----------------------------------------------------------------------------
// 0.70 minimap and photo-mode overlay.
// 3.9.1 switches the minimap to heading-up navigation: the player stays pointed
// toward the top of the display while roads, traffic and GPS rotate around it.
// 3.9.2 fixes the lateral basis to match the engine camera handedness, removing
// the horizontal mirror while preserving heading-up behavior and true north.
// -----------------------------------------------------------------------------
static void ui70_map_point(V3 p,float cx,float cy,float scale,float*outX,float*outY){
  float dx=p.x-g_player.body.p.x,dz=p.z-g_player.body.p.z,a=g_player.body.yaw;
  float s=(float)sin(a),c=(float)cos(a);
  // Camera screen-right is cross(forward, up): (-cos(yaw), 0, sin(yaw)).
  // Using the opposite vector mirrors left/right when the minimap is heading-up.
  float localRight=-dx*c+dz*s;
  float localForward=dx*s+dz*c;
  *outX=cx+localRight*scale;
  *outY=cy-localForward*scale;
}
static int ui70_in_range(V3 p,float r){float dx=p.x-g_player.body.p.x,dz=p.z-g_player.body.p.z;return dx*dx+dz*dz<r*r;}
static void ui70_draw_minimap(void){
  float size=224.0f,x0=(float)g_w-size-22.0f,y0=22.0f,cx=x0+size*.5f,cy=y0+size*.5f,range=g_minimap_range70,scale=(size*.46f)/range;
  hud_panel(x0,y0,size,size);glColor4f(.10f,.13f,.15f,.88f);glBegin(GL_QUADS);glVertex2f(x0+5,y0+5);glVertex2f(x0+size-5,y0+5);glVertex2f(x0+size-5,y0+size-5);glVertex2f(x0+5,y0+size-5);glEnd();
  glLineWidth(1.0f);glColor3f(.46f,.50f,.53f);glBegin(GL_LINES);for(int i=0;i<g_stream_road_count08;i++){StreamRoad08*s=&g_stream_roads08[i];V3 m=vmul(vadd(s->a,s->b),.5f);if(!ui70_in_range(m,range*1.15f))continue;float ax,ay,bx,by;ui70_map_point(s->a,cx,cy,scale,&ax,&ay);ui70_map_point(s->b,cx,cy,scale,&bx,&by);glVertex2f(ax,ay);glVertex2f(bx,by);}glEnd();
  if(g_gps_n>0){glLineWidth(2.5f);glColor3f(.10f,.62f,1.0f);glBegin(GL_LINES);for(int i=0;i<g_gps_n;i++){int eid=g_gps_route[i];if(eid<0||eid>=g_world.edgeCount)continue;Edge*e=&g_world.edges[eid];V3 a=g_world.nodes[e->from].p,b=g_world.nodes[e->to].p,m=vmul(vadd(a,b),.5f);if(!ui70_in_range(m,range*1.3f))continue;float ax,ay,bx,by;ui70_map_point(a,cx,cy,scale,&ax,&ay);ui70_map_point(b,cx,cy,scale,&bx,&by);glVertex2f(ax,ay);glVertex2f(bx,by);}glEnd();}
  glColor3f(.92f,.72f,.18f);glBegin(GL_QUADS);for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active||!ui70_in_range(t->p,range))continue;float px,py;ui70_map_point(t->p,cx,cy,scale,&px,&py);glVertex2f(px-2,py-2);glVertex2f(px+2,py-2);glVertex2f(px+2,py+2);glVertex2f(px-2,py+2);}glEnd();
  // Heading-up: player icon is fixed pointing toward the top of the minimap.
  glColor3f(1,.22f,.16f);glBegin(GL_TRIANGLES);glVertex2f(cx,cy-10);glVertex2f(cx+6,cy+6);glVertex2f(cx-6,cy+6);glEnd();
  // True north uses the same corrected lateral basis as map geometry.
  float a=g_player.body.yaw,sn=(float)sin(a),cs=(float)cos(a),nr=size*.39f;
  float nx=cx-sn*nr,ny=cy+cs*nr;glColor3f(.96f,.82f,.30f);glBegin(GL_LINES);glVertex2f(nx,ny+5);glVertex2f(nx,ny-5);glEnd();gl_text(nx-4,ny-8,"N",.96f,.84f,.34f);
  gl_text(x0+10,y0+18,"GPS / ITAJAI // HEADING UP",.82f,.88f,.94f);
}
static void ui70_draw_photo_overlay(void){if(!g_photo70)return;hud_panel(20,(float)g_h-74,450,54);gl_text(34,(float)g_h-50,"PHOTO MODE // mouse: orbita  P: sair  H: HUD",1,.92f,.72f);gl_text(34,(float)g_h-30,"simulacao congelada",.70f,.78f,.84f);}
