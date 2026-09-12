// -----------------------------------------------------------------------------
// 0.90 final curated content pass across the vertical-slice districts.
// Uses existing authored glTF props + native primitives so the world has
// recognizable Porto, Molhes, Atalaia/Cabecudas and Praia Brava signatures.
// -----------------------------------------------------------------------------
static int g_content90_props=0;
static int content90_near(V3 c,float r){float dx=g_player.body.p.x-c.x,dz=g_player.body.p.z-c.z;return dx*dx+dz*dz<r*r;}
static void content90_asset(const char*name,V3 p,float yaw,float scale,float r,float g,float b){int id=asset08_find(name);if(id>=0){asset08_draw_instance(id,p,yaw,scale,r,g,b);g_content90_props++;}}
static void content90_porto(void){V3 c=v3(-1489,0,-1336);if(!content90_near(c,1150))return;
  for(int row=0;row<4;row++)for(int i=0;i<9;i++){float x=-1740+i*20.0f,z=-1500+row*17.0f;content90_asset("port_container",v3(x,.02f,z),(row&1)?1.5708f:0,1.0f,.72f-(i%3)*.10f,.18f+(row%3)*.12f,.12f+(i%4)*.12f);}
  for(int i=0;i<5;i++){float x=-1690+i*92.0f;draw_box(v3(x,10,-1550),1.6f,20,1.6f,0,.84f,.64f,.18f);draw_box(v3(x+13,19,-1550),27,.8f,.8f,0,.84f,.64f,.18f);content90_asset("street_lamp",v3(x+30,.02f,-1458),0,1.05f,.28f,.30f,.32f);}
  for(int i=0;i<8;i++)content90_asset("road_sign",v3(-1710+i*48.0f,.02f,-1398),0,1.0f,.18f,.35f,.66f);
}
static void content90_molhes(void){V3 c=v3(1290,0,-891);if(!content90_near(c,1000))return;
  for(int i=0;i<28;i++){float x=1040+i*18.0f;content90_asset("road_guardrail",v3(x,.02f,-905),1.5708f,1.0f,.42f,.44f,.46f);if((i%4)==0)content90_asset("street_lamp",v3(x,.02f,-884),0,1.0f,.28f,.30f,.32f);}
  for(int i=0;i<18;i++){float x=1035+i*29.0f;float z=-930-((i*7)%18);draw_box(v3(x,.22f,z),2.8f,.55f,2.1f,(i%5)*.21f,.32f,.33f,.31f);}
  draw_box(v3(1390,4.2f,-895),1.2f,8.4f,1.2f,0,.84f,.84f,.76f);draw_box(v3(1390,8.8f,-895),2.0f,1.0f,2.0f,0,.86f,.18f,.12f);
}
static void content90_atalaia(void){V3 c=v3(2482,0,334);if(!content90_near(c,1050))return;
  for(int i=0;i<16;i++){float x=2280+i*28.0f,z=300+((i*11)%24);content90_asset((i%3)?"coastal_palm":"urban_tree",v3(x,.02f,z),(i&1)?.18f:-.13f,.9f+(i%3)*.1f,.22f,.54f,.20f);if((i%4)==0)content90_asset("urban_bench",v3(x+8,.02f,z+9),1.5708f,.9f,.34f,.27f,.20f);}
  draw_box(v3(2490,.35f,385),28,.7f,9,0,.48f,.47f,.43f);content90_asset("road_sign",v3(2465,.02f,370),0,1.2f,.16f,.36f,.70f);
}
static void content90_cabecudas(void){V3 c=v3(2680,0,2004);if(!content90_near(c,1050))return;
  for(int i=0;i<18;i++){float x=2480+i*27.0f,z=1960+((i*9)%36);content90_asset("coastal_palm",v3(x,.02f,z),0,.86f+(i%4)*.08f,.24f,.56f,.22f);if((i%5)==0)content90_asset("urban_bench",v3(x+6,.02f,z+8),1.5708f,.9f,.34f,.27f,.20f);}
  for(int i=0;i<8;i++)content90_asset("street_lamp",v3(2520+i*54.0f,.02f,2020),0,1.0f,.28f,.30f,.32f);
}
static void content90_brava(void){V3 c=v3(2283,0,5343);if(!content90_near(c,1200))return;
  for(int i=0;i<22;i++){float x=2010+i*27.0f,z=5290+((i*17)%42);content90_asset((i&1)?"coastal_palm":"urban_tree",v3(x,.02f,z),0,.9f+(i%3)*.08f,.22f,.55f,.21f);}
  for(int i=0;i<12;i++)content90_asset("street_lamp",v3(2050+i*50.0f,.02f,5375),0,1.0f,.28f,.30f,.32f);
  draw_box(v3(2260,4.0f,5305),1.0f,8.0f,1.0f,0,.72f,.72f,.68f);draw_box(v3(2300,4.0f,5305),1.0f,8.0f,1.0f,0,.72f,.72f,.68f);draw_box(v3(2280,7.5f,5305),41,.8f,1.0f,0,.72f,.72f,.68f);
}
static void content90_draw(void){g_content90_props=0;content90_porto();content90_molhes();content90_atalaia();content90_cabecudas();content90_brava();renderer09_set_material(0);}
