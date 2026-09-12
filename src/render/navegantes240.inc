// -----------------------------------------------------------------------------
// 2.40 Navegantes Gateway.
// The 0.10 OSM bbox already covers the lower Navegantes bank/corridor. This pass
// gives that existing map data its own art direction: logistics yards, river edge
// and an airport-approach light rhythm. It is intentionally a gateway, not a claim
// of full-city photogrammetric coverage.
// -----------------------------------------------------------------------------
static int g_navegantes240_active=0,g_navegantes240_draws=0;
static int navegantes240_near(float x,float z,float d){float dx=x-g_player.body.p.x,dz=z-g_player.body.p.z;return dx*dx+dz*dz<d*d;}
static void navegantes240_box(float x,float z,float sx,float sy,float sz,float yaw,float r,float g,float b){float y=terrain11_height(x,z)+sy*.5f;draw_box(v3(x,y,z),sx,sy,sz,yaw,r,g,b);g_navegantes240_draws++;}
static void navegantes240_draw(void){
  g_navegantes240_active=(g_player.body.p.x>180&&g_player.body.p.x<1750&&g_player.body.p.z>720&&g_player.body.p.z<3000);g_navegantes240_draws=0;
  // Logistics sheds/yards on the north-bank approach.
  for(int i=0;i<10;i++){float x=430.0f+(i%5)*48.0f,z=1280.0f+(i/5)*58.0f;if(!navegantes240_near(x,z,650))continue;navegantes240_box(x,z,34.0f,6.2f,22.0f,.03f*(i&1),.42f,.44f,.43f);navegantes240_box(x,z-12.5f,18.0f,.22f,7.0f,0,.74f,.66f,.24f);}
  // Airport/corridor approach-light cadence farther north.
  for(int i=0;i<20;i++){float x=875.0f+(i&1?7.5f:-7.5f),z=1710.0f+i*33.0f;if(!navegantes240_near(x,z,760))continue;navegantes240_box(x,z,.28f,.75f,.28f,0,.72f,.72f,.68f);if(g_hour>=18||g_hour<6||g_weather>=2)navegantes240_box(x,z,.44f,.10f,.44f,0,.96f,.84f,.54f);}
  // River-edge barriers and service bollards distinguish the opposite bank.
  for(int i=0;i<18;i++){float x=220.0f+i*30.0f,z=885.0f+(float)sin(i*.31f)*7.0f;if(!navegantes240_near(x,z,520))continue;navegantes240_box(x,z,2.1f,.72f,.16f,0,.23f,.26f,.27f);}
  renderer09_set_material(MAT09_GENERIC);
}
