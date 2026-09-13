// -----------------------------------------------------------------------------
// Renderer 5.0.3 world pass: make the streamed/open-data city read unmistakably
// as Itajai before pushing renderer complexity further. Geometry is intentionally
// lightweight and additive so the existing 60 FPS target remains realistic.
// Public/open geographic anchors are used only as spatial/reference cues.
// -----------------------------------------------------------------------------
static int g_world503_landmarks=0,g_world503_props=0,g_world503_boats=0,g_world503_molhe_segments=0;

static void world503_cafe_set(V3 p,float yaw,u32 h){
  float r=.30f+((h>>2)&3u)*.035f,g=.23f+((h>>5)&3u)*.025f,b=.16f;
  draw_box(v3(p.x,p.y+.42f,p.z),1.15f,.10f,1.15f,yaw,r,g,b);
  draw_box(v3(p.x,p.y+.22f,p.z),.13f,.45f,.13f,yaw,.22f,.20f,.18f);
  city340_pyramid(v3(p.x,p.y+2.05f,p.z),2.45f,2.45f,.72f,yaw,(h&1u)?.13f:.82f,(h&1u)?.38f:.80f,(h&1u)?.27f:.66f);
  draw_box(v3(p.x,p.y+1.28f,p.z),.10f,1.55f,.10f,yaw,.30f,.29f,.27f);g_world503_props+=4;
}
static void world503_boat(V3 p,float yaw,float s,u32 h){
  float cr=(h&1u)?.90f:.16f,cg=(h&2u)?.22f:.50f,cb=(h&4u)?.18f:.72f;
  draw_box(v3(p.x,p.y+.34f*s,p.z),2.1f*s,.55f*s,6.4f*s,yaw,.88f,.88f,.84f);
  draw_box(city340_rot(v3(p.x,p.y+.72f*s,p.z),0,-.25f*s,yaw),1.55f*s,.72f*s,2.2f*s,yaw,cr,cg,cb);
  draw_box(city340_rot(v3(p.x,p.y+2.65f*s,p.z),0,.25f*s,yaw),.07f*s,4.0f*s,.07f*s,yaw,.44f,.44f,.42f);
  draw_box(city340_rot(v3(p.x,p.y+2.65f*s,p.z),.55f*s,.25f*s,yaw),1.15f*s,.05f*s,.05f*s,yaw,.70f,.70f,.66f);g_world503_boats++;
}
static void world503_market(void){
  // Mercado Publico: warm yellow frontage, white vertical rhythm, green awnings
  // and outdoor tables are the strongest street-level identifiers in photos.
  V3 p=city340_geo(-26.9063889,-48.6541667);if(!city340_near(p,900.0f))return;float y=terrain11_height(p.x,p.z),yaw=.04f;renderer09_set_material(0);
  V3 f=city340_rot(v3(p.x,y,p.z),0,-10.92f,yaw);
  draw_box(v3(f.x,y+4.25f,f.z),24.8f,8.35f,.30f,yaw,.74f,.58f,.20f);
  draw_box(v3(f.x,y+8.30f,f.z-.03f),25.4f,.50f,.36f,yaw,.90f,.89f,.82f);
  for(int k=-4;k<=4;k+=2){V3 q=city340_rot(v3(p.x,y,p.z),k*2.65f,-11.12f,yaw);draw_box(v3(q.x,y+4.20f,q.z),.42f,8.0f,.22f,yaw,.91f,.90f,.84f);}
  V3 gate=city340_rot(v3(p.x,y,p.z),0,-11.20f,yaw);draw_box(v3(gate.x,y+2.55f,gate.z),4.35f,5.0f,.24f,yaw,.12f,.12f,.10f);
  for(int k=-3;k<=3;k+=2){V3 a=city340_rot(v3(p.x,y,p.z),k*3.05f,-11.38f,yaw);draw_box(v3(a.x,y+2.65f,a.z),2.15f,.22f,1.20f,yaw,.10f,.34f,.20f);}
  renderer09_suspend_shader();identity150_text3d(city340_rot(v3(p.x,y+6.55f,p.z),-3.2f,-11.42f,yaw),"MERCADO PUBLICO",.97f,.94f,.84f);renderer09_resume_shader();
  city360_assets_init();for(int s=-1;s<=1;s+=2)city360_asset(g_city360_palm,city340_rot(v3(p.x,0,p.z),s*13.8f,-15.5f,yaw),yaw,.92f,.22f,.50f,.22f);
  for(int k=-2;k<=2;k++){V3 q=city340_rot(v3(p.x,y,p.z),k*4.3f,-16.8f,yaw);world503_cafe_set(q,yaw,(u32)(k+9)*741u);}g_world503_landmarks++;
}
static void world503_matriz(void){
  // Igreja Matriz / Praca Irineu Bornhausen: reinforce the civic square with a
  // fountain, gardens and the pale-grey/white architectural rhythm seen on site.
  V3 p=city340_geo(-26.90756,-48.66185);if(!city340_near(p,1000.0f))return;float y=terrain11_height(p.x,p.z),yaw=-.10f;renderer09_set_material(0);
  for(int s=-1;s<=1;s+=2){V3 q=city340_rot(v3(p.x,y,p.z),s*7.05f,-21.4f,yaw);draw_box(v3(q.x,y+10.8f,q.z),.48f,20.4f,.36f,yaw,.65f,.68f,.70f);}
  V3 plaza=city340_rot(v3(p.x,y,p.z),0,-42.0f,yaw);city340_octagon(v3(plaza.x,y+.02f,plaza.z),5.4f,.42f,.76f,.73f,.67f);city340_octagon(v3(plaza.x,y+.45f,plaza.z),4.55f,.12f,.32f,.62f,.70f);draw_box(v3(plaza.x,y+1.55f,plaza.z),.13f,2.25f,.13f,0,.80f,.88f,.92f);
  for(int s=-1;s<=1;s+=2){V3 q=city340_rot(v3(p.x,0,p.z),s*18.0f,-38.0f,yaw);city360_asset(g_city360_tree,q,0,.88f,.20f,.47f,.19f);V3 b=city340_rot(v3(p.x,0,p.z),s*11.0f,-33.0f,yaw);city360_asset(g_city360_bench,b,yaw,.92f,.35f,.27f,.20f);}g_world503_landmarks++;
}
static void world503_centreventos(void){
  // Centreventos on Av. Ministro Victor Konder: low wide mass, blue event facade,
  // glazed entrances and the busy forecourt that visually anchors the Beira-Rio.
  V3 p=city340_geo(-26.90959,-48.65287);if(!city340_near(p,1200.0f))return;float y=terrain11_height(p.x,p.z),yaw=-.03f;renderer09_set_material(0);
  V3 f=city340_rot(v3(p.x,y,p.z),0,-15.55f,yaw);draw_box(v3(f.x,y+6.5f,f.z),43.0f,7.0f,.36f,yaw,.08f,.25f,.39f);draw_box(city340_rot(v3(p.x,y+3.0f,p.z),0,-15.82f,yaw),31.0f,5.2f,.22f,yaw,.08f,.12f,.15f);
  draw_box(city340_rot(v3(p.x,y+6.7f,p.z),0,-18.1f,yaw),36.0f,.42f,4.5f,yaw,.86f,.87f,.85f);
  renderer09_suspend_shader();identity150_text3d(city340_rot(v3(p.x,y+8.25f,p.z),-9.0f,-15.86f,yaw),"CENTREVENTOS ITAJAI",.94f,.96f,.95f);renderer09_resume_shader();
  city360_assets_init();for(int k=-4;k<=4;k+=2){V3 q=city340_rot(v3(p.x,0,p.z),k*5.0f,-24.0f,yaw);city360_asset(g_city360_palm,q,yaw,.86f,.20f,.48f,.21f);}for(int k=-3;k<=3;k++){V3 q=city340_rot(v3(p.x,y,p.z),k*6.0f,-28.0f,yaw);draw_box(v3(q.x,y+3.8f,q.z),.10f,7.4f,.10f,0,.70f,.72f,.70f);g_world503_props++;}g_world503_landmarks++;
}
static void world503_marina(void){
  // Official tourism coordinate: 26 54 38 S / 48 39 13 W. Docks, masts and
  // varied small craft do more for the Itajai waterfront read than a larger hall.
  V3 p=city340_geo(-26.9105556,-48.6536111);if(!city340_near(p,1300.0f))return;float y=terrain11_height(p.x,p.z),yaw=.04f;renderer09_set_material(0);
  draw_box(city340_rot(v3(p.x,y+4.3f,p.z),0,-8.05f,yaw),27.0f,3.5f,.28f,yaw,.06f,.22f,.31f);renderer09_suspend_shader();identity150_text3d(city340_rot(v3(p.x,y+5.25f,p.z),-6.8f,-8.25f,yaw),"MARINA ITAJAI",.94f,.97f,.97f);renderer09_resume_shader();
  for(int d=-3;d<=3;d++){V3 dock=city340_rot(v3(p.x,y,p.z),d*9.0f,18.0f,yaw);draw_box(v3(dock.x,y+.20f,dock.z),1.25f,.20f,30.0f,yaw,.46f,.39f,.29f);for(int j=0;j<2;j++){V3 q=city340_rot(v3(p.x,y+.10f,p.z),d*9.0f+(j?2.5f:-2.5f),10.0f+j*12.0f,yaw);world503_boat(q,yaw+(j?3.14159f:0),.72f+(float)((d+4)&1)*.08f,(u32)(d+7)*313u+(u32)j);}}
  city360_assets_init();for(int k=-4;k<=4;k+=2)city360_asset(g_city360_lamp,city340_rot(v3(p.x,0,p.z),k*7.0f,-12.0f,yaw),yaw,.95f,.27f,.30f,.31f);g_world503_landmarks++;
}
static void world503_molhes(void){
  // Draw the south mole between an open-map entrance reference and a geotagged
  // south-light coordinate, instead of the old short generic slab.
  V3 a=city340_geo(-26.914663,-48.641616),b=city340_geo(-26.915029,-48.634071);V3 mid=vmul(vadd(a,b),.5f);if(!city340_near(mid,1900.0f)&&!city340_near(a,1300.0f))return;V3 d=vsub(b,a);float len=vlen(d);if(len<20.0f)return;V3 f=vmul(d,1.0f/len);float yaw=(float)atan2((double)f.x,(double)f.z);int segs=12;renderer09_set_material(0);city360_assets_init();
  for(int i=0;i<segs;i++){float t=((float)i+.5f)/(float)segs;V3 q=vadd(a,vmul(d,t));float yy=terrain11_height(q.x,q.z);draw_box(v3(q.x,yy+.18f,q.z),7.2f,.34f,len/(float)segs*.97f,yaw,.38f,.38f,.36f);if((i&1)==0)city360_asset(g_city360_lamp,v3(q.x,0,q.z),yaw,1.02f,.28f,.30f,.30f);g_world503_molhe_segments++;}
  float y=terrain11_height(b.x,b.z);city340_octagon(v3(b.x,y+.18f,b.z),2.55f,3.0f,.88f,.87f,.81f);city340_octagon(v3(b.x,y+3.18f,b.z),2.25f,3.0f,.72f,.12f,.10f);city340_octagon(v3(b.x,y+6.18f,b.z),2.0f,3.3f,.90f,.89f,.84f);city340_octagon(v3(b.x,y+9.48f,b.z),2.25f,1.25f,.72f,.12f,.10f);city340_pyramid(v3(b.x,y+10.73f,b.z),4.7f,4.7f,2.6f,yaw,.55f,.12f,.10f);if(atmos280_dark())draw_box(v3(b.x,y+13.7f,b.z),1.2f,1.2f,1.2f,0,1.0f,.48f,.24f);g_world503_landmarks++;
}
static void world503_beirario_life(void){
  // GTA/Forza-style life cue: sparse, repeatable micro-scenes instead of random
  // clutter. Existing pedestrians/traffic remain authoritative; this pass adds
  // cafes, planters and palms where the real Beira-Rio is socially active.
  if(!g_stream_ready08)return;city360_assets_init();int cap=12;
  for(int i=0;i<g_stream_road_count08&&cap>0;i++){StreamRoad08*s=&g_stream_roads08[i];if(s->road<0||s->road>=g_world.roadCount)continue;RoadWay*r=&g_world.roads[s->road];int beira=city330_contains_ci(r->name,"Victor Konder")||city330_contains_ci(r->name,"Beira-Rio")||city330_contains_ci(r->name,"Beira Rio");if(!beira)continue;V3 d=vsub(s->b,s->a),m=vmul(vadd(s->a,s->b),.5f);float len=vlen(d);if(len<8.0f||!city340_near(m,520.0f))continue;V3 f=vmul(d,1.0f/len),rt=v3(f.z,0,-f.x);float edge=r->width*.5f+6.0f,side=city360_water_side(s,edge);u32 h=roads130_hash(s)^0x503BEEFu;if((h%3u)==0u){V3 q=vadd(m,vmul(rt,side*edge));q.y=terrain11_height(q.x,q.z);world503_cafe_set(q,(float)atan2((double)f.x,(double)f.z),h);cap--;}else if((h%3u)==1u){V3 q=vadd(m,vmul(rt,side*(edge+1.2f)));city360_asset(g_city360_palm,q,0,.95f,.20f,.50f,.22f);cap--;}}
}
static void world503_atalaia(void){
  V3 p=city340_geo(-26.918402,-48.642266);if(!city340_near(p,1200.0f))return;float y=terrain11_height(p.x,p.z);city360_assets_init();renderer09_set_material(0);
  draw_box(v3(p.x,y+.08f,p.z),42.0f,.08f,7.0f,.05f,.62f,.58f,.46f);for(int i=-4;i<=4;i+=2)city360_asset(g_city360_palm,v3(p.x+i*7.5f,0,p.z+7.0f),0,.90f+(i&2)*.04f,.20f,.50f,.22f);draw_box(v3(p.x-13.0f,y+2.1f,p.z+10.0f),8.0f,4.2f,6.0f,.05f,.76f,.67f,.49f);draw_box(v3(p.x-13.0f,y+4.4f,p.z+10.0f),9.0f,.40f,7.0f,.05f,.35f,.20f,.12f);g_world503_landmarks++;
}
static void world503_draw(void){
  g_world503_landmarks=g_world503_props=g_world503_boats=g_world503_molhe_segments=0;
  world503_market();world503_matriz();world503_centreventos();world503_marina();world503_molhes();world503_beirario_life();world503_atalaia();renderer09_set_material(0);
}
static void world503_overlay(void){if(!g_debug||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();char b[220];sprintf(b,"WORLD 5.0.3 // landmarks %d | life props %d | boats %d | molhe segs %d",g_world503_landmarks,g_world503_props,g_world503_boats,g_world503_molhe_segments);gl_text(34,(float)g_h-369,b,.64f,.82f,.74f);settings_draw_2d_end();}
