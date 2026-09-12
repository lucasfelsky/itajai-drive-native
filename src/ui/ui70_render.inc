// -----------------------------------------------------------------------------
// 0.70 north-up minimap and photo-mode overlay.
// -----------------------------------------------------------------------------
static void ui70_map_point(V3 p,float cx,float cy,float scale,float*outX,float*outY){*outX=cx+(p.x-g_player.body.p.x)*scale;*outY=cy-(p.z-g_player.body.p.z)*scale;}
static int ui70_in_range(V3 p,float r){float dx=p.x-g_player.body.p.x,dz=p.z-g_player.body.p.z;return dx*dx+dz*dz<r*r;}
static void ui70_draw_minimap(void){
  float size=224.0f,x0=(float)g_w-size-22.0f,y0=22.0f,cx=x0+size*.5f,cy=y0+size*.5f,range=g_minimap_range70,scale=(size*.46f)/range;
  hud_panel(x0,y0,size,size);glColor4f(.10f,.13f,.15f,.88f);glBegin(GL_QUADS);glVertex2f(x0+5,y0+5);glVertex2f(x0+size-5,y0+5);glVertex2f(x0+size-5,y0+size-5);glVertex2f(x0+5,y0+size-5);glEnd();
  glLineWidth(1.0f);glColor3f(.46f,.50f,.53f);glBegin(GL_LINES);for(int i=0;i<g_stream_road_count08;i++){StreamRoad08*s=&g_stream_roads08[i];V3 m=vmul(vadd(s->a,s->b),.5f);if(!ui70_in_range(m,range*1.15f))continue;float ax,ay,bx,by;ui70_map_point(s->a,cx,cy,scale,&ax,&ay);ui70_map_point(s->b,cx,cy,scale,&bx,&by);glVertex2f(ax,ay);glVertex2f(bx,by);}glEnd();
  if(g_gps_n>0){glLineWidth(2.5f);glColor3f(.10f,.62f,1.0f);glBegin(GL_LINES);for(int i=0;i<g_gps_n;i++){int eid=g_gps_route[i];if(eid<0||eid>=g_world.edgeCount)continue;Edge*e=&g_world.edges[eid];V3 a=g_world.nodes[e->from].p,b=g_world.nodes[e->to].p,m=vmul(vadd(a,b),.5f);if(!ui70_in_range(m,range*1.3f))continue;float ax,ay,bx,by;ui70_map_point(a,cx,cy,scale,&ax,&ay);ui70_map_point(b,cx,cy,scale,&bx,&by);glVertex2f(ax,ay);glVertex2f(bx,by);}glEnd();}
  glColor3f(.92f,.72f,.18f);glBegin(GL_QUADS);for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active||!ui70_in_range(t->p,range))continue;float px,py;ui70_map_point(t->p,cx,cy,scale,&px,&py);glVertex2f(px-2,py-2);glVertex2f(px+2,py-2);glVertex2f(px+2,py+2);glVertex2f(px-2,py+2);}glEnd();
  float a=g_player.body.yaw,s=(float)sin(a),c=(float)cos(a);glColor3f(1,.22f,.16f);glBegin(GL_TRIANGLES);glVertex2f(cx+s*9,cy-c*9);glVertex2f(cx+c*5-s*4,cy+s*5+c*4);glVertex2f(cx-c*5-s*4,cy-s*5+c*4);glEnd();gl_text(x0+10,y0+18,"GPS / ITAJAI",.82f,.88f,.94f);
}
static void ui70_draw_photo_overlay(void){if(!g_photo70)return;hud_panel(20,(float)g_h-74,450,54);gl_text(34,(float)g_h-50,"PHOTO MODE // mouse: orbita  P: sair  H: HUD",1,.92f,.72f);gl_text(34,(float)g_h-30,"simulacao congelada",.70f,.78f,.84f);}
