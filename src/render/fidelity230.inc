// -----------------------------------------------------------------------------
// 2.30 Itajai Fidelity Pass. Curated high-density accents for the existing OSM
// city: waterfront, Centro/Beira-Rio and port approach. Art-directed, not surveyed.
// -----------------------------------------------------------------------------
static int g_fidelity230_draws=0;
static int fidelity230_near(float x,float z,float d){float dx=x-g_player.body.p.x,dz=z-g_player.body.p.z;return dx*dx+dz*dz<d*d;}
static void fidelity230_box(float x,float z,float sx,float sy,float sz,float r,float g,float b){float y=terrain11_height(x,z)+sy*.5f;draw_box(v3(x,y,z),sx,sy,sz,0,r,g,b);g_fidelity230_draws++;}
static void fidelity230_waterfront(void){
  // Beira-Rio promenade: low railing rhythm, benches/planters and warm bollards.
  for(int i=0;i<26;i++){float x=-560.0f+i*25.0f,z=1038.0f+(float)sin(i*.38f)*8.0f;if(!fidelity230_near(x,z,520))continue;fidelity230_box(x,z,1.5f,.82f,.12f,.22f,.25f,.27f);if((i&2)==0){fidelity230_box(x+4,z-5,2.2f,.48f,.70f,.31f,.28f,.20f);fidelity230_box(x-4,z-3,1.2f,.65f,1.2f,.18f,.32f,.16f);}}
  if(fidelity230_near(-110,1000,620)){float y=terrain11_height(-110,1000)+.04f;renderer09_set_material(MAT09_GENERIC);gl_text_world(v3(-110,y+3.2f,1000),0,"BEIRA-RIO // ITAJAI",.032f,.92f,.86f,.68f);}
}
static void fidelity230_port(void){
  // Port service edge: security blocks, striped gates and denser container walls.
  for(int row=0;row<3;row++)for(int i=0;i<12;i++){float x=-1770.0f+i*18.0f,z=-1450.0f+row*16.0f;if(!fidelity230_near(x,z,620))continue;float c=(i+row)&2?1.0f:.35f;fidelity230_box(x,z,12.0f,2.55f,2.45f,.18f+.42f*c,.18f,.14f+.28f*(1-c));if((i%4)==0)fidelity230_box(x,z,11.7f,2.45f,2.2f,.63f,.20f,.09f);}
  for(int i=0;i<9;i++){float x=-1645.0f+i*16.0f,z=-1320.0f;if(!fidelity230_near(x,z,520))continue;fidelity230_box(x,z,1.2f,2.0f,.35f,(i&1)?.90f:.16f,(i&1)?.72f:.18f,(i&1)?.12f:.20f);}
}
static void fidelity230_centro(void){
  // Street-level visual noise: awnings, planters and parking separators.
  for(int i=0;i<18;i++){float x=-620.0f+i*38.0f,z=-90.0f+(i%3)*34.0f;if(!fidelity230_near(x,z,440))continue;fidelity230_box(x,z,5.2f,.18f,1.5f,.62f,.22f+.08f*(i%3),.16f);fidelity230_box(x+4.2f,z+2.0f,.8f,.64f,.8f,.17f,.35f,.18f);}
}
static void fidelity230_draw(void){g_fidelity230_draws=0;fidelity230_waterfront();fidelity230_port();fidelity230_centro();renderer09_set_material(MAT09_GENERIC);}
