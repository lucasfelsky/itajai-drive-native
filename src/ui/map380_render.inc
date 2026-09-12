// -----------------------------------------------------------------------------
// 3.8 full-screen interactive map renderer.
// Uses the complete resident road graph, so destinations can be chosen outside
// the currently streamed 3D region. North is up.
// -----------------------------------------------------------------------------
static float g_map380_vx=0,g_map380_vy=0,g_map380_vw=0,g_map380_vh=0,g_map380_scale=1;
static int map380_major_road(const RoadWay*r){return str_eq(r->type,"motorway")||str_eq(r->type,"trunk")||str_eq(r->type,"primary")||str_eq(r->type,"secondary")||str_eq(r->type,"tertiary");}
static void map380_world_to_screen(V3 p,float*outX,float*outY){float cx=g_map380_vx+g_map380_vw*.5f,cy=g_map380_vy+g_map380_vh*.5f;*outX=cx+(p.x-g_map380_center.x)*g_map380_scale;*outY=cy-(p.z-g_map380_center.z)*g_map380_scale;}
static int map380_visible(V3 p,float pad){float x,y;map380_world_to_screen(p,&x,&y);return x>=g_map380_vx-pad&&x<=g_map380_vx+g_map380_vw+pad&&y>=g_map380_vy-pad&&y<=g_map380_vy+g_map380_vh+pad;}
static void map380_line_world(V3 a,V3 b){float ax,ay,bx,by;map380_world_to_screen(a,&ax,&ay);map380_world_to_screen(b,&bx,&by);glVertex2f(ax,ay);glVertex2f(bx,by);}
static void map380_marker(float x,float y,float r,float cr,float cg,float cb){glColor3f(cr,cg,cb);glBegin(GL_LINE_LOOP);for(int i=0;i<18;i++){float a=(float)i*6.2831853f/18.0f;glVertex2f(x+(float)cos(a)*r,y+(float)sin(a)*r);}glEnd();glBegin(GL_LINES);glVertex2f(x-r-4,y);glVertex2f(x+r+4,y);glVertex2f(x,y-r-4);glVertex2f(x,y+r+4);glEnd();}
static void map380_landmark(V3 p,const char*name,float cr,float cg,float cb){if(!map380_visible(p,22))return;float x,y;map380_world_to_screen(p,&x,&y);glColor3f(cr,cg,cb);glBegin(GL_QUADS);glVertex2f(x-3,y-3);glVertex2f(x+3,y-3);glVertex2f(x+3,y+3);glVertex2f(x-3,y+3);glEnd();if(g_map380_range<2700.0f)gl_text(x+7,y-4,name,.76f,.80f,.82f);}
static void map380_draw_roads(void){
  glLineWidth(1.0f);glColor3f(.30f,.34f,.37f);glBegin(GL_LINES);
  for(int i=0;i<g_world.segCount;i++){RoadSegment*s=&g_world.segs[i];if(s->road<0||s->road>=g_world.roadCount)continue;RoadWay*r=&g_world.roads[s->road];if(map380_major_road(r))continue;if(g_map380_range>3000.0f&&(i&1))continue;V3 m=vmul(vadd(s->a,s->b),.5f);if(!map380_visible(m,80))continue;map380_line_world(s->a,s->b);}glEnd();
  glLineWidth(2.0f);glColor3f(.57f,.61f,.63f);glBegin(GL_LINES);
  for(int i=0;i<g_world.segCount;i++){RoadSegment*s=&g_world.segs[i];if(s->road<0||s->road>=g_world.roadCount)continue;RoadWay*r=&g_world.roads[s->road];if(!map380_major_road(r))continue;V3 m=vmul(vadd(s->a,s->b),.5f);if(!map380_visible(m,100))continue;map380_line_world(s->a,s->b);}glEnd();
}
static void map380_draw_route(void){if(g_gps_n<=0)return;glLineWidth(4.0f);glColor3f(.08f,.63f,1.0f);glBegin(GL_LINES);for(int i=0;i<g_gps_n;i++){int eid=g_gps_route[i];if(eid<0||eid>=g_world.edgeCount)continue;Edge*e=&g_world.edges[eid];V3 a=g_world.nodes[e->from].p,b=g_world.nodes[e->to].p,m=vmul(vadd(a,b),.5f);if(!map380_visible(m,120))continue;map380_line_world(a,b);}glEnd();}
static void map380_draw_landmarks(void){
  map380_landmark(city340_geo(-26.9074694,-48.6619194),"IGREJA MATRIZ",.96f,.78f,.28f);
  map380_landmark(city340_geo(-26.9063889,-48.6541667),"MERCADO PUBLICO",.96f,.58f,.20f);
  map380_landmark(city340_geo(-26.90959,-48.65287),"CENTREVENTOS",.32f,.78f,.91f);
  map380_landmark(city340_geo(-26.9187,-48.6615),"UNIVALI",.35f,.70f,.95f);
  map380_landmark(city340_geo(-26.8997,-48.6574),"PORTO DE ITAJAI",.32f,.67f,.86f);
  map380_landmark(city340_geo(-26.9105,-48.6416),"MOLHES DA BARRA",.90f,.88f,.66f);
  map380_landmark(city340_geo(-26.9088,-48.6590),"MUSEU HISTORICO",.95f,.76f,.26f);
}
static void map380_draw_player(void){float x,y;map380_world_to_screen(g_player.body.p,&x,&y);float a=g_player.body.yaw,s=(float)sin(a),c=(float)cos(a);glColor3f(1,.22f,.14f);glBegin(GL_TRIANGLES);glVertex2f(x+s*11,y-c*11);glVertex2f(x+c*6-s*5,y+s*6+c*5);glVertex2f(x-c*6-s*5,y-s*6+c*5);glEnd();}
static void map380_draw_destination(void){if(g_gps_goal07<0||g_gps_goal07>=g_world.nodeCount)return;float x,y;map380_world_to_screen(g_world.nodes[g_gps_goal07].p,&x,&y);map380_marker(x,y,8.0f,.08f,.72f,1.0f);}
static void map380_draw_cursor(void){float x,y;map380_world_to_screen(g_map380_cursor,&x,&y);map380_marker(x,y,11.0f,1.0f,.87f,.18f);}
static void map380_draw(void){
  if(!g_map380_open)return;settings_draw_2d_begin();
  float margin=28.0f,top=62.0f,bottom=58.0f;g_map380_vx=margin;g_map380_vy=top;g_map380_vw=maxf(320.0f,(float)g_w-margin*2);g_map380_vh=maxf(240.0f,(float)g_h-top-bottom);g_map380_scale=g_map380_vh/(2.0f*g_map380_range);
  glColor4f(.018f,.026f,.032f,.98f);glBegin(GL_QUADS);glVertex2f(0,0);glVertex2f((float)g_w,0);glVertex2f((float)g_w,(float)g_h);glVertex2f(0,(float)g_h);glEnd();
  hud_panel(g_map380_vx-8,g_map380_vy-8,g_map380_vw+16,g_map380_vh+16);glColor3f(.065f,.083f,.094f);glBegin(GL_QUADS);glVertex2f(g_map380_vx,g_map380_vy);glVertex2f(g_map380_vx+g_map380_vw,g_map380_vy);glVertex2f(g_map380_vx+g_map380_vw,g_map380_vy+g_map380_vh);glVertex2f(g_map380_vx,g_map380_vy+g_map380_vh);glEnd();
  map380_draw_roads();map380_draw_route();map380_draw_landmarks();map380_draw_destination();map380_draw_player();map380_draw_cursor();
  gl_text(30,31,"MAPA // ITAJAI DRIVE",1,.94f,.78f);gl_text(245,31,"M: fechar   clique/ENTER: destino   setas: cursor   +/-: zoom   HOME: jogador   DEL: cancelar",.73f,.80f,.85f);
  char b[240];sprintf(b,"%s   |   zoom %.1f km   |   rotas definidas %d",g_map380_status,(g_map380_range*2.0f)/1000.0f,g_map380_routes_set);gl_text(34,(float)g_h-27,b,.82f,.88f,.92f);
  settings_draw_2d_end();
}
static int map380_screen_to_world(int sx,int sy,V3*out){if(!g_map380_open||g_map380_scale<=0||sx<g_map380_vx||sx>g_map380_vx+g_map380_vw||sy<g_map380_vy||sy>g_map380_vy+g_map380_vh)return 0;float cx=g_map380_vx+g_map380_vw*.5f,cy=g_map380_vy+g_map380_vh*.5f;*out=v3(g_map380_center.x+(sx-cx)/g_map380_scale,0,g_map380_center.z-(sy-cy)/g_map380_scale);return 1;}
static void map380_click_screen(int sx,int sy){V3 p;if(!map380_screen_to_world(sx,sy,&p))return;g_map380_cursor=p;map380_set_destination(p);map380_keep_cursor_visible();}
