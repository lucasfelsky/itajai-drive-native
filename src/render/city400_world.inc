// -----------------------------------------------------------------------------
// 4.0 Whole-city identity / world completion pass.
// Extends the curated urban language beyond Centro/Fazenda using the complete
// municipal cadastral/road world. Major real-world anchors remain lightweight
// native geometry so streaming and the no-external-engine architecture stay intact.
// -----------------------------------------------------------------------------
static int g_city400_outer_props=0,g_city400_outer_walks=0,g_city400_landmarks=0,g_city400_district_labels=0;

typedef struct {const char*name;double lat,lon;} City400District;
static const City400District g_city400_districts[]={
  {"SAO JOAO",-26.90072,-48.67731},
  {"SAO VICENTE",-26.90776,-48.67914},
  {"CIDADE NOVA",-26.92110,-48.69405},
  {"CORDEIROS",-26.88489,-48.69623},
  {"SALSEIROS",-26.86508,-48.72837},
  {"SANTA REGINA",-26.87810,-48.74889}
};
#define CITY400_DISTRICT_COUNT ((int)(sizeof(g_city400_districts)/sizeof(g_city400_districts[0])))

static int city400_outer_zone(V3 p){float d=0;urban10_sector_at(p,&d);if(d<1450.0f)return 0;if(p.x<-8500.0f)return 3;if(p.x<-3600.0f)return 2;return 1;}
static void city400_outer_roads(void){
  if(!g_stream_ready08)return;city360_assets_init();renderer09_set_material(0);
  for(int i=0;i<g_stream_road_count08;i++){StreamRoad08*s=&g_stream_roads08[i];if(s->road<0||s->road>=g_world.roadCount)continue;RoadWay*r=&g_world.roads[s->road];V3 d=vsub(s->b,s->a),m=vmul(vadd(s->a,s->b),.5f);float len=vlen(d);if(len<5||!city340_near(m,620.0f))continue;int zone=city400_outer_zone(m);if(!zone)continue;V3 f=vmul(d,1.0f/len),rt=v3(f.z,0,-f.x);float yaw=(float)atan2((double)f.x,(double)f.z),edge=r->width*.5f+1.8f;u32 h=roads130_hash(s)^0x40040u;
    int highway=str_eq(r->type,"motorway")||str_eq(r->type,"trunk");int arterial=highway||str_eq(r->type,"primary")||str_eq(r->type,"secondary")||str_eq(r->type,"tertiary");
    if(zone==1){
      // Sao Vicente/Cidade Nova transition: dense residential/commercial streets.
      if(!highway){city360_strip(s,edge,1.85f,.073f,.45f,.44f,.41f);city360_strip(s,-edge,1.85f,.073f,.45f,.44f,.41f);g_city400_outer_walks+=2;}
      if((h%7u)==1u){city360_asset(g_city360_lamp,vadd(m,vmul(rt,edge+1.0f)),yaw,.96f,.28f,.30f,.31f);g_city400_outer_props++;}
      if((h%10u)==2u){city360_asset(g_city360_tree,vadd(m,vmul(rt,-edge-1.7f)),yaw,.85f,.20f,.48f,.20f);g_city400_outer_props++;}
      if(arterial&&(h%31u)==6u){city360_asset(g_city360_bus,vadd(m,vmul(rt,edge+2.6f)),yaw,.95f,.28f,.35f,.38f);g_city400_outer_props++;}
    }else if(zone==2){
      // Cordeiros/Salseiros logistics belt: wider margins, warehouses and guardrail language.
      if(arterial&&(h%5u)==0u){city360_asset(g_city360_guard,vadd(m,vmul(rt,edge+.8f)),yaw,1.0f,.43f,.45f,.45f);g_city400_outer_props++;}
      if((h%13u)==3u){city360_asset(g_city360_lamp,vadd(m,vmul(rt,edge+2.2f)),yaw,1.05f,.28f,.30f,.30f);g_city400_outer_props++;}
      if(!highway&&(h%17u)==5u){city360_asset(g_city360_tree,vadd(m,vmul(rt,-edge-2.8f)),yaw,.84f,.19f,.45f,.18f);g_city400_outer_props++;}
      if(arterial&&(h%29u)==11u){city360_asset(g_city360_container,vadd(m,vmul(rt,-edge-8.0f)),yaw,.94f,.30f,.25f,.19f);g_city400_outer_props++;}
    }else{
      // Inland/rural Itajai: road hierarchy remains real, but scenery opens up.
      if(highway&&(h%4u)==0u){city360_asset(g_city360_guard,vadd(m,vmul(rt,edge+.7f)),yaw,1.05f,.45f,.46f,.44f);g_city400_outer_props++;}
      if((h%6u)==1u){float side=(h&1u)?1.0f:-1.0f;city360_asset(g_city360_tree,vadd(m,vmul(rt,side*(edge+4.5f+(h%4u)))),yaw,.92f+(h%3u)*.07f,.18f,.45f,.17f);g_city400_outer_props++;}
      if(arterial&&(h%19u)==4u){city360_asset(g_city360_lamp,vadd(m,vmul(rt,edge+2.1f)),yaw,.98f,.27f,.29f,.29f);g_city400_outer_props++;}
    }
  }
}

static void city400_marieta(void){
  V3 p=city340_geo(-26.910123,-48.659069);if(!city340_near(p,1300.0f))return;float y=terrain11_height(p.x,p.z),yaw=.03f;renderer09_set_material(0);
  draw_box(v3(p.x,y+13.5f,p.z),34.0f,27.0f,18.0f,yaw,.78f,.80f,.79f);draw_box(v3(p.x+11.0f,y+20.0f,p.z+3.0f),13.0f,40.0f,13.0f,yaw,.86f,.87f,.85f);
  for(int fl=0;fl<8;fl++)for(int k=-3;k<=3;k++){float wx=p.x+k*3.7f,wz=p.z-9.08f;city370_window(v3(wx,y+3.0f+fl*3.0f,wz),1.7f,1.05f,yaw,atmos280_dark()&&((fl+k)&1));}
  draw_box(v3(p.x-9.0f,y+2.1f,p.z-9.3f),7.0f,3.4f,.25f,yaw,.72f,.08f,.08f);renderer09_suspend_shader();identity150_text3d(v3(p.x-14.0f,y+5.0f,p.z-9.45f),"HOSPITAL MARIETA",.96f,.96f,.94f);renderer09_resume_shader();g_city400_landmarks++;
}
static void city400_teatro(void){
  V3 p=city340_geo(-26.92051,-48.65538);if(!city340_near(p,1200.0f))return;float y=terrain11_height(p.x,p.z),yaw=-.08f;renderer09_set_material(0);draw_box(v3(p.x,y+5.1f,p.z),31.0f,10.2f,22.0f,yaw,.37f,.32f,.27f);draw_box(v3(p.x,y+4.3f,p.z-11.2f),25.0f,7.0f,.4f,yaw,.16f,.20f,.21f);for(int k=-4;k<=4;k++)draw_box(v3(p.x+k*2.6f,y+4.1f,p.z-11.45f),1.25f,4.1f,.18f,yaw,.08f,.13f,.15f);renderer09_suspend_shader();identity150_text3d(v3(p.x-8.8f,y+8.0f,p.z-11.6f),"TEATRO MUNICIPAL",.96f,.91f,.78f);renderer09_resume_shader();g_city400_landmarks++;
}
static void city400_marina(void){
  V3 p=city340_geo(-26.9105556,-48.6536111);if(!city340_near(p,1500.0f))return;float y=terrain11_height(p.x,p.z);renderer09_set_material(0);draw_box(v3(p.x,y+3.0f,p.z),32.0f,6.0f,15.0f,.04f,.68f,.70f,.69f);draw_box(v3(p.x,y+4.1f,p.z-7.7f),26.0f,2.1f,.22f,.04f,.08f,.27f,.38f);
  for(int i=-4;i<=4;i++){V3 q=v3(p.x+i*8.0f,y+.14f,p.z+16.0f);draw_box(q,1.2f,.18f,18.0f,.04f,.48f,.42f,.31f);draw_box(v3(q.x,y+1.0f,q.z+5.0f),3.8f,.75f,8.0f,.04f,.88f,.88f,.84f);}
  renderer09_suspend_shader();identity150_text3d(v3(p.x-7.0f,y+5.0f,p.z-8.0f),"MARINA ITAJAI",.94f,.97f,.97f);renderer09_resume_shader();g_city400_landmarks++;
}
static void city400_atalaia(void){
  V3 p=city340_geo(-26.9244,-48.6416);if(!city340_near(p,1500.0f))return;float y=terrain11_height(p.x,p.z);city360_assets_init();for(int i=0;i<13;i++){float a=i*.71f;city360_asset(g_city360_tree,v3(p.x+(float)cos(a)*20.0f,0,p.z+(float)sin(a)*16.0f),a,.95f+(i%3)*.08f,.18f,.46f,.18f);}draw_box(v3(p.x,y+2.2f,p.z),10.0f,4.4f,6.0f,.1f,.35f,.32f,.26f);renderer09_suspend_shader();identity150_text3d(v3(p.x-5.5f,y+4.8f,p.z-3.2f),"PARQUE DA ATALAIA",.90f,.96f,.86f);renderer09_resume_shader();g_city400_landmarks++;
}
static void city400_draw(void){g_city400_outer_props=g_city400_outer_walks=g_city400_landmarks=0;city400_outer_roads();city400_marieta();city400_teatro();city400_marina();city400_atalaia();renderer09_set_material(0);}

static void city400_map_extras(void){
  if(!g_map380_open)return;settings_draw_2d_begin();g_city400_district_labels=0;
  if(g_map380_range<6500.0f){for(int i=0;i<CITY400_DISTRICT_COUNT;i++){V3 p=city340_geo(g_city400_districts[i].lat,g_city400_districts[i].lon);if(!map380_visible(p,70))continue;float x,y;map380_world_to_screen(p,&x,&y);gl_text(x+5,y-5,g_city400_districts[i].name,.46f,.58f,.64f);g_city400_district_labels++;}}
  settings_draw_2d_end();
}
static void city400_overlay(void){if(!g_debug||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();char b[220];hud_panel(20,(float)g_h-294,660,28);sprintf(b,"WORLD 4.0 // outer props %d | outer walks %d | landmarks %d | district labels %d",g_city400_outer_props,g_city400_outer_walks,g_city400_landmarks,g_city400_district_labels);gl_text(34,(float)g_h-273,b,.70f,.84f,.72f);settings_draw_2d_end();}
