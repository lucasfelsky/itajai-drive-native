// -----------------------------------------------------------------------------
// 3.7 Final City Polish
// A focused signature pass: historic Centro, football identity, skyline markers,
// landmark lighting and directional signs. Visual-only; no collision/physics edits.
// -----------------------------------------------------------------------------
static int g_city370_landmarks=0,g_city370_wayfinding=0,g_city370_lights=0;

static void city370_window(V3 p,float w,float h,float yaw,float lit){draw_box(p,w,h,.07f,yaw,lit?.88f:.18f,lit?.65f:.28f,lit?.30f:.34f);if(lit)g_city370_lights++;}
static void city370_palacio(void){
  // Palacio Marcos Konder / Museu Historico de Itajai.
  V3 p=city340_geo(-26.9072921,-48.6611456);if(!city340_near(p,1200.0f))return;float y=terrain11_height(p.x,p.z),yaw=-.03f;int dark=atmos280_dark();renderer09_set_material(3);
  const float br=.84f,bg=.63f,bb=.25f,tr=.94f,tg=.91f,tb=.80f;
  draw_box(v3(p.x,y+5.2f,p.z),25.0f,10.4f,13.0f,yaw,br,bg,bb);
  // Three characteristic tower masses.
  for(int s=-1;s<=1;s+=2){V3 t=city340_rot(v3(p.x,y,p.z),s*8.2f,-5.0f,yaw);draw_box(v3(t.x,y+7.2f,t.z),6.2f,14.4f,6.0f,yaw,br,bg,bb);draw_box(v3(t.x,y+14.7f,t.z),6.7f,.8f,6.5f,yaw,tr,tg,tb);city340_pyramid(v3(t.x,y+15.1f,t.z),6.5f,6.2f,4.2f,yaw,.40f,.27f,.15f);}
  V3 ct=city340_rot(v3(p.x,y,p.z),0,3.0f,yaw);draw_box(v3(ct.x,y+10.0f,ct.z),5.8f,10.0f,5.5f,yaw,br,bg,bb);draw_box(v3(ct.x,y+15.2f,ct.z),6.3f,.8f,6.0f,yaw,tr,tg,tb);city340_pyramid(v3(ct.x,y+15.6f,ct.z),6.0f,5.8f,3.8f,yaw,.40f,.27f,.15f);
  // White pilasters / cornices that define the yellow facade.
  for(int k=-5;k<=5;k+=2){V3 q=city340_rot(v3(p.x,y,p.z),k*2.1f,-6.62f,yaw);draw_box(v3(q.x,y+5.3f,q.z),.24f,9.9f,.12f,yaw,tr,tg,tb);}
  draw_box(city340_rot(v3(p.x,y+9.9f,p.z),0,-6.7f,yaw),23.2f,.42f,.14f,yaw,tr,tg,tb);
  draw_box(city340_rot(v3(p.x,y+1.0f,p.z),0,-6.75f,yaw),23.5f,.75f,.16f,yaw,.60f,.39f,.18f);
  // Tall arched-window rhythm approximated as paired vertical panes.
  for(int fl=0;fl<2;fl++)for(int k=-4;k<=4;k+=2){V3 q=city340_rot(v3(p.x,y,p.z),k*2.25f,-6.76f,yaw);city370_window(v3(q.x,y+3.2f+fl*3.45f,q.z),1.20f,2.05f,yaw,dark&&((k+fl)&2));}
  V3 door=city340_rot(v3(p.x,y,p.z),0,-6.82f,yaw);draw_box(v3(door.x,y+2.1f,door.z),3.2f,4.0f,.16f,yaw,.27f,.23f,.19f);
  renderer09_set_material(0);renderer09_suspend_shader();identity150_text3d(v3(door.x-4.7f,y+10.75f,door.z-.05f),"MUSEU HISTORICO",.98f,.94f,.82f);renderer09_resume_shader();
  if(dark){for(int s=-1;s<=1;s+=2){V3 l=city340_rot(v3(p.x,y,p.z),s*10.0f,-9.0f,yaw);draw_box(v3(l.x,y+3.0f,l.z),.16f,6.0f,.16f,0,.95f,.72f,.34f);draw_box(v3(l.x,y+6.0f,l.z),.55f,.35f,.55f,0,1.0f,.82f,.45f);g_city370_lights++;}}
  g_city370_landmarks++;
}

static void city370_stadium(void){
  // Estadio Dr. Hercilio Luz / Gigantao das Avenidas.
  V3 p=city340_geo(-26.9083,-48.65991);if(!city340_near(p,1500.0f))return;float y=terrain11_height(p.x,p.z),yaw=.17f;int dark=atmos280_dark();renderer09_set_material(0);
  // Pitch.
  draw_box(v3(p.x,y+.055f,p.z),61.0f,.08f,92.0f,yaw,.15f,.43f,.12f);
  // White lines, center line and center mark.
  draw_box(v3(p.x,y+.105f,p.z),.13f,.025f,88.0f,yaw,.88f,.88f,.82f);
  for(int s=-1;s<=1;s+=2){V3 q=city340_rot(v3(p.x,y,p.z),s*29.0f,0,yaw);draw_box(v3(q.x,y+.105f,q.z),.13f,.025f,91.0f,yaw,.88f,.88f,.82f);}
  for(int s=-1;s<=1;s+=2){V3 q=city340_rot(v3(p.x,y,p.z),0,s*45.0f,yaw);draw_box(v3(q.x,y+.105f,q.z),59.0f,.025f,.13f,yaw,.88f,.88f,.82f);}
  // Red/blue stands, strongly characteristic of Marcilio Dias.
  for(int side=-1;side<=1;side+=2){V3 q=city340_rot(v3(p.x,y,p.z),side*35.0f,0,yaw);draw_box(v3(q.x,y+3.0f,q.z),7.0f,6.0f,89.0f,yaw,(side>0)?.08f:.64f,(side>0)?.25f:.08f,(side>0)?.55f:.10f);for(int k=-4;k<=4;k++){V3 a=city340_rot(v3(p.x,y,p.z),side*31.8f,k*9.0f,yaw);draw_box(v3(a.x,y+1.0f+(k&1)*.25f,a.z),1.0f,2.0f,7.0f,yaw,(k&1)?.64f:.08f,.08f,(k&1)?.10f:.55f);}}
  // End stands.
  for(int side=-1;side<=1;side+=2){V3 q=city340_rot(v3(p.x,y,p.z),0,side*49.0f,yaw);draw_box(v3(q.x,y+2.2f,q.z),65.0f,4.4f,6.0f,yaw,.08f,.25f,.55f);}
  // Four floodlights.
  for(int sx=-1;sx<=1;sx+=2)for(int sz=-1;sz<=1;sz+=2){V3 q=city340_rot(v3(p.x,y,p.z),sx*38.0f,sz*51.0f,yaw);draw_box(v3(q.x,y+10.0f,q.z),.32f,20.0f,.32f,0,.35f,.36f,.35f);draw_box(v3(q.x,y+20.0f,q.z),4.2f,.55f,.55f,yaw,dark?1.0f:.72f,dark?.91f:.72f,dark?.68f:.70f);if(dark)g_city370_lights++;}
  renderer09_suspend_shader();identity150_text3d(city340_rot(v3(p.x,y+6.2f,p.z),-31.7f,-17.0f,yaw),"MARCILIO DIAS",.98f,.91f,.75f);renderer09_resume_shader();g_city370_landmarks++;
}

static void city370_morro(void){
  // Morro da Cruz: preserve physics terrain, add only a recognizable lookout/cross skyline marker.
  V3 p=city340_geo(-26.91875,-48.66350);if(!city340_near(p,1800.0f))return;float y=terrain11_height(p.x,p.z);renderer09_set_material(0);
  draw_box(v3(p.x,y+2.7f,p.z),12.0f,5.4f,8.0f,.08f,.32f,.31f,.28f);draw_box(v3(p.x,y+5.7f,p.z),14.0f,.55f,9.4f,.08f,.48f,.42f,.34f);
  for(int s=-1;s<=1;s+=2){draw_box(v3(p.x+s*5.2f,y+7.2f,p.z),.38f,3.2f,.38f,0,.40f,.39f,.36f);}
  city340_cross(v3(p.x,y+6.0f,p.z),4.5f);
  renderer09_suspend_shader();identity150_text3d(v3(p.x-5.4f,y+3.2f,p.z-4.15f),"MORRO DA CRUZ",.92f,.93f,.87f);renderer09_resume_shader();g_city370_landmarks++;
}

static void city370_molhes_light(void){
  if(!atmos280_dark())return;V3 p=city340_geo(-26.914663,-48.641616);if(!city340_near(p,1800.0f))return;float y=terrain11_height(p.x,p.z);V3 l=city340_rot(v3(p.x,y,p.z),0,-35.0f,.06f);glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);draw_box(v3(l.x,y+18.8f,l.z),1.5f,1.5f,1.5f,0,1.0f,.35f,.20f);draw_box(v3(l.x,y+18.8f,l.z),3.8f,.20f,3.8f,0,1.0f,.58f,.30f);glDisable(GL_BLEND);g_city370_lights++;}

static void city370_wayfinding(void){
  g_city370_wayfinding=0;if(!g_stream_ready08)return;renderer09_set_material(0);
  for(int i=0;i<g_stream_road_count08&&g_city370_wayfinding<5;i++){StreamRoad08*s=&g_stream_roads08[i];if(s->road<0||s->road>=g_world.roadCount)continue;RoadWay*r=&g_world.roads[s->road];if(r->width<9.0f)continue;u32 h=roads130_hash(s);if((h%47u)!=9u)continue;V3 d=vsub(s->b,s->a),m=vmul(vadd(s->a,s->b),.5f);float len=vlen(d);if(len<18.0f||!city340_near(m,440.0f))continue;V3 f=vmul(d,1.0f/len),rt=v3(f.z,0,-f.x);float yaw=(float)atan2((double)f.x,(double)f.z),side=(h&1u)?1.0f:-1.0f;V3 q=vadd(m,vmul(rt,side*(r->width*.5f+3.0f)));float y=terrain11_height(q.x,q.z);int sec=urban10_sector_at(m,NULL);const char*a=sec==2?"CENTRO  |  PORTO":sec>=3?"CENTRO  |  PRAIAS":sec==1?"CENTRO  |  BEIRA-RIO":"PORTO  |  UNIVALI";
    draw_box(v3(q.x,y+2.3f,q.z),.22f,4.6f,.22f,yaw,.42f,.44f,.43f);V3 panel=vadd(q,vmul(f,2.9f));draw_box(v3(panel.x,y+4.45f,panel.z),6.4f,1.25f,.16f,yaw,.05f,.26f,.47f);renderer09_suspend_shader();identity150_text3d(v3(panel.x-2.65f,y+4.55f,panel.z-.12f),a,.94f,.97f,.94f);renderer09_resume_shader();g_city370_wayfinding++;}
}

static void city370_draw(void){g_city370_landmarks=0;g_city370_lights=0;city370_palacio();city370_stadium();city370_morro();city370_molhes_light();city370_wayfinding();renderer09_set_material(0);}
static void city370_overlay(void){if(!g_debug||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();char b[220];hud_panel(20,(float)g_h-262,610,28);sprintf(b,"CITY 3.7 // signature landmarks %d | wayfinding %d | night accents %d",g_city370_landmarks,g_city370_wayfinding,g_city370_lights);gl_text(34,(float)g_h-241,b,.72f,.84f,.72f);settings_draw_2d_end();}
