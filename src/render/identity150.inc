// -----------------------------------------------------------------------------
// 1.5 Itajai Identity Pass
// Procedural Brazilian/coastal identity layered on top of OSM building classes.
// All generation is deterministic from OSM/building/road seeds.
// -----------------------------------------------------------------------------
__declspec(dllimport) void APIENTRY glRasterPos3f(float,float,float);

static int g_identity_signs150=0,g_identity_balconies150=0,g_identity_walls150=0;
static int g_identity_service150=0,g_identity_port150=0,g_identity_landmarks150=0;
static const char*g_shop_names150[]={"MERCADO","FARMACIA","PADARIA","AUTO PECAS","CAFE","RESTAURANTE","MATERIAIS","LOJA","CONVENIENCIA","ESTACIONAMENTO"};
#define ID150_SHOPS ((int)(sizeof(g_shop_names150)/sizeof(g_shop_names150[0])))

typedef struct {const char*name;V3 p;float yaw;} IdentityLandmark150;
static const IdentityLandmark150 g_identity_landmarks150[]={
  {"CENTRO DE ITAJAI",{-330,0,45},.10f},
  {"BEIRA-RIO",{-75,0,1045},-.05f},
  {"PORTO DE ITAJAI",{-1450,0,-1290},.20f},
  {"MOLHES DA BARRA",{1280,0,-845},-.12f},
  {"ATALAIA",{2440,0,380},.12f},
  {"CABECUDAS",{2635,0,2050},-.08f},
  {"PRAIA BRAVA",{2240,0,5300},.08f}
};
#define ID150_LANDMARKS ((int)(sizeof(g_identity_landmarks150)/sizeof(g_identity_landmarks150[0])))

static void identity150_text3d(V3 p,const char*s,float r,float g,float b){if(!g_font_base||!s||!s[0])return;glColor3f(r,g,b);glRasterPos3f(p.x,p.y,p.z);glListBase(g_font_base-32);glCallLists((int)strlen(s),GL_UNSIGNED_BYTE,s);}
static int identity150_near(V3 p,float range){float dx=p.x-g_player.body.p.x,dz=p.z-g_player.body.p.z;return dx*dx+dz*dz<range*range;}
static float identity150_base(const Building*b){return terrain11_height(b->c.x,b->c.z);}

static void identity150_commercial(Building*b){
  if(!identity150_near(b->c,210.0f))return;u32 h=b->seed;float base=identity150_base(b);float front=b->c.z+b->sz*.5f+.13f;float boardW=clampf(b->sx*.72f,2.4f,10.0f);float cr=.12f+.28f*((h>>3)&3)/3.0f,cg=.22f+.32f*((h>>7)&3)/3.0f,cb=.30f+.35f*((h>>11)&3)/3.0f;
  draw_box(v3(b->c.x,base+2.55f,front),boardW,.70f,.16f,0,cr,cg,cb);g_identity_signs150++;
  // Awning gives low-rise Brazilian storefronts a stronger street edge.
  if((h&3u)!=0){float aw=.55f+.15f*((h>>14)&3);draw_box(v3(b->c.x,base+2.05f,front+.35f),boardW*.92f,.12f,aw,0,.68f,.18f+.20f*((h>>17)&1),.14f);}
  // Occasional small parking forecourt beside commercial footprints.
  if((h%11u)==0u&&b->sx<28.0f){float px=b->c.x+b->sx*.5f+5.2f,pz=b->c.z;draw_box(v3(px,terrain11_height(px,pz)+.035f,pz),8.6f,.07f,minf(12.0f,b->sz),0,.19f,.20f,.21f);for(int k=-2;k<=2;k++)draw_box(v3(px+k*1.55f,terrain11_height(px+k*1.55f,pz)+.08f,pz),.045f,.025f,minf(10.0f,b->sz*.80f),0,.82f,.82f,.76f);g_identity_service150++;}
}

static void identity150_house(Building*b){
  if(!identity150_near(b->c,150.0f)||(b->seed&3u)==0u)return;float base=identity150_base(b),z=b->c.z+b->sz*.5f+1.15f,w=maxf(3.4f,b->sx*.82f),gate=minf(3.4f,w*.36f),side=maxf(.35f,(w-gate)*.5f);float x0=b->c.x-w*.5f;draw_box(v3(x0+side*.5f,base+.62f,z),side,1.15f,.18f,0,.63f,.60f,.54f);draw_box(v3(b->c.x+gate*.5f+side*.5f,base+.62f,z),side,1.15f,.18f,0,.63f,.60f,.54f);draw_box(v3(b->c.x,base+.74f,z+.015f),gate,1.42f,.12f,0,.28f,.30f,.31f);g_identity_walls150++;
}

static void identity150_balconies(Building*b,int sector){
  if((sector!=4&&sector!=5&&sector!=6)||b->h<24.0f||!identity150_near(b->c,185.0f)||(b->seed&1u))return;float base=identity150_base(b),front=b->c.z+b->sz*.5f+.48f;int floors=(int)(b->h/4.2f);if(floors>7)floors=7;float w=clampf(b->sx*.68f,3.5f,11.0f);for(int k=1;k<=floors;k++){float y=base+3.3f+k*4.15f;if(y>base+b->h-.8f)break;draw_box(v3(b->c.x,y,front),w,.14f,.92f,0,.72f,.74f,.75f);draw_box(v3(b->c.x,y+.52f,front+.38f),w*.96f,.72f,.055f,0,.42f,.54f,.60f);g_identity_balconies150++;}}

static void identity150_port(Building*b){
  if(urban10_sector_at(b->c,NULL)!=2||urban10_building_class(b)!=BLD10_INDUSTRIAL||!identity150_near(b->c,260.0f))return;float base=identity150_base(b),front=b->c.z+b->sz*.5f+.10f;int doors=(int)(b->sx/6.0f);if(doors<1)doors=1;if(doors>5)doors=5;for(int k=0;k<doors;k++){float x=b->c.x+(k-(doors-1)*.5f)*minf(6.0f,b->sx/maxf(1,(float)doors));draw_box(v3(x,base+2.0f,front),3.4f,3.8f,.14f,0,.20f,.23f,.25f);for(int s=0;s<3;s++)draw_box(v3(x,base+.85f+s*.88f,front+.08f),3.0f,.055f,.05f,0,.58f,.61f,.62f);}if((b->seed%4u)==0u)draw_box(v3(b->c.x,base+b->h+.35f,b->c.z),minf(12.0f,b->sx*.65f),.55f,1.2f,0,.18f,.30f,.42f);g_identity_port150++;}

static void identity150_building_pass(void){
  g_identity_signs150=0;g_identity_balconies150=0;g_identity_walls150=0;g_identity_port150=0;if(!g_stream_ready08)return;
  for(int i=0;i<g_stream_building_count08;i++){Building*b=&g_stream_buildings08[i].b;if(!identity150_near(b->c,280.0f))continue;int cls=urban10_building_class(b),sector=urban10_sector_at(b->c,NULL);if(cls==BLD10_COMMERCIAL)identity150_commercial(b);else if(cls==BLD10_HOUSE)identity150_house(b);else if(cls==BLD10_INDUSTRIAL)identity150_port(b);if(cls==BLD10_APARTMENT)identity150_balconies(b,sector);}
}

static void identity150_service_pass(void){
  g_identity_service150=0;if(!g_stream_ready08)return;for(int i=0;i<g_stream_road_count08;i++){StreamRoad08*s=&g_stream_roads08[i];if(s->road<0||s->road>=g_world.roadCount)continue;RoadWay*r=&g_world.roads[s->road];if(r->width<10.0f||(roads130_hash(s)%79u)!=3u)continue;V3 m=vmul(vadd(s->a,s->b),.5f);if(!identity150_near(m,260.0f))continue;int sector=urban10_sector_at(m,NULL);if(sector==4||sector==5||sector==6)continue;V3 d=vsub(s->b,s->a);float len=vlen(d);if(len<18)return;V3 f=vmul(d,1.0f/len),rt=v3(f.z,0,-f.x);float side=(roads130_hash(s)&1u)?1.0f:-1.0f;V3 p=vadd(m,vmul(rt,side*(r->width*.5f+7.8f)));float base=terrain11_height(p.x,p.z),yaw=(float)atan2((double)f.x,(double)f.z);draw_box(v3(p.x,base+3.4f,p.z),8.2f,.45f,6.4f,yaw,.88f,.87f,.80f);for(int q=-1;q<=1;q+=2){V3 pole=vadd(p,vmul(f,q*2.8f));draw_box(v3(pole.x,base+1.65f,pole.z),.20f,3.25f,.20f,yaw,.70f,.71f,.68f);}V3 sign=vadd(p,vmul(rt,side*4.5f));draw_box(v3(sign.x,base+2.4f,sign.z),1.3f,4.8f,.45f,yaw,.16f,.38f,.62f);g_identity_service150++;}
}

static void identity150_landmarks_geometry(void){
  g_identity_landmarks150=0;for(int i=0;i<ID150_LANDMARKS;i++){V3 p=g_identity_landmarks150[i].p;if(!identity150_near(p,650.0f))continue;float base=terrain11_height(p.x,p.z);float w=i==2?15.0f:i==6?12.0f:10.0f;draw_box(v3(p.x,base+2.15f,p.z),w,2.0f,.42f,g_identity_landmarks150[i].yaw,.11f,.24f,.34f);draw_box(v3(p.x-w*.38f,base+1.0f,p.z),.28f,2.0f,.28f,g_identity_landmarks150[i].yaw,.55f,.56f,.55f);draw_box(v3(p.x+w*.38f,base+1.0f,p.z),.28f,2.0f,.28f,g_identity_landmarks150[i].yaw,.55f,.56f,.55f);g_identity_landmarks150++;}
}

static void identity150_text_pass(void){
  renderer09_suspend_shader();
  if(g_stream_ready08){for(int i=0;i<g_stream_building_count08;i++){Building*b=&g_stream_buildings08[i].b;if(urban10_building_class(b)!=BLD10_COMMERCIAL||!identity150_near(b->c,170.0f))continue;const char*name=g_shop_names150[(b->seed>>9)%ID150_SHOPS];float base=identity150_base(b);identity150_text3d(v3(b->c.x-b->sx*.30f,base+2.60f,b->c.z+b->sz*.5f+.24f),name,.98f,.91f,.70f);}}
  for(int i=0;i<ID150_LANDMARKS;i++){V3 p=g_identity_landmarks150[i].p;if(!identity150_near(p,520.0f))continue;float base=terrain11_height(p.x,p.z);float w=i==2?15.0f:i==6?12.0f:10.0f;identity150_text3d(v3(p.x-w*.39f,base+2.35f,p.z+.24f),g_identity_landmarks150[i].name,.95f,.95f,.88f);}
  // Recompute service stations for text, keeping geometry/text deterministic.
  if(g_stream_ready08){for(int i=0;i<g_stream_road_count08;i++){StreamRoad08*s=&g_stream_roads08[i];if(s->road<0||s->road>=g_world.roadCount||g_world.roads[s->road].width<10.0f||(roads130_hash(s)%79u)!=3u)continue;V3 m=vmul(vadd(s->a,s->b),.5f);if(!identity150_near(m,220.0f))continue;int sector=urban10_sector_at(m,NULL);if(sector==4||sector==5||sector==6)continue;V3 d=vsub(s->b,s->a);float len=vlen(d);if(len<18)continue;V3 f=vmul(d,1.0f/len),rt=v3(f.z,0,-f.x);float side=(roads130_hash(s)&1u)?1.0f:-1.0f;V3 p=vadd(m,vmul(rt,side*(g_world.roads[s->road].width*.5f+12.3f)));float base=terrain11_height(p.x,p.z);identity150_text3d(v3(p.x-.55f,base+4.3f,p.z),"POSTO",1.0f,.92f,.72f);}}
  renderer09_resume_shader();
}

static void identity150_draw(void){identity150_building_pass();identity150_service_pass();identity150_landmarks_geometry();identity150_text_pass();renderer09_set_material(0);}
