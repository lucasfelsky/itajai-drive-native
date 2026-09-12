// -----------------------------------------------------------------------------
// 0.30 curated quality district: Beira-Rio / Fazenda waterfront strip.
// Adds a dense hand-authored-looking layer on top of OSM geometry.
// -----------------------------------------------------------------------------
static V3 g_district30_center={-99.0f,0.0f,1002.0f};
static int g_district30_drawn_props=0;
static int district30_near(float r){float dx=g_player.body.p.x-g_district30_center.x,dz=g_player.body.p.z-g_district30_center.z;return dx*dx+dz*dz<r*r;}
static void district30_surface(void){
  if(!district30_near(1050.0f))return;renderer09_set_material(0);
  // Waterfront promenade and two small paved lookout plazas.
  glColor3f(.34f,.35f,.34f);glBegin(GL_QUADS);
  glVertex3f(-390,.055f,930);glVertex3f(250,.055f,930);glVertex3f(250,.055f,952);glVertex3f(-390,.055f,952);
  glVertex3f(-250,.056f,900);glVertex3f(-150,.056f,900);glVertex3f(-150,.056f,972);glVertex3f(-250,.056f,972);
  glVertex3f(35,.056f,908);glVertex3f(145,.056f,908);glVertex3f(145,.056f,978);glVertex3f(35,.056f,978);glEnd();g_draw_calls++;
  // Bike/pedestrian accents.
  glLineWidth(2.0f);glColor3f(.72f,.20f,.16f);glBegin(GL_LINES);for(int x=-370;x<235;x+=18){glVertex3f((float)x,.063f,941);glVertex3f((float)x+9,.063f,941);}glEnd();g_draw_calls++;
}
static void district30_prop_asset(const char*name,V3 p,float yaw,float scale,float r,float g,float b){int id=asset08_find(name);if(id>=0){asset08_draw_instance(id,p,yaw,scale,r,g,b);g_district30_drawn_props++;}}
static void district30_props(void){
  g_district30_drawn_props=0;if(!district30_near(1050.0f))return;
  // Lamps on both sides of the promenade.
  for(int i=0;i<24;i++){float x=-360.0f+i*25.0f;district30_prop_asset("street_lamp",v3(x,.02f,936),0,1.0f,.28f,.30f,.32f);if((i&1)==0)district30_prop_asset("urban_bench",v3(x+8,.02f,947),1.5708f,.95f,.34f,.27f,.20f);}
  // Coastal vegetation grouped rather than perfectly periodic.
  for(int i=0;i<15;i++){float x=-345.0f+i*41.0f;float z=958.0f+((i*13)%17);district30_prop_asset((i%3)?"coastal_palm":"urban_tree",v3(x,.02f,z),(i&1)?.22f:-.18f,.90f+(i%4)*.08f,.22f,.52f,.20f);}
  // Waterfront guard rail segments.
  for(int i=0;i<31;i++){float x=-382.0f+i*20.0f;district30_prop_asset("road_guardrail",v3(x,.02f,926),1.5708f,1.05f,.42f,.44f,.46f);}
  // Bus shelters + directional signs give the strip identifiable anchors.
  district30_prop_asset("bus_shelter",v3(-215,.02f,988),1.5708f,1.05f,.28f,.35f,.38f);
  district30_prop_asset("bus_shelter",v3(82,.02f,987),1.5708f,1.05f,.28f,.35f,.38f);
  district30_prop_asset("road_sign",v3(-300,.02f,965),0,1.15f,.18f,.36f,.70f);
  district30_prop_asset("road_sign",v3(180,.02f,965),0,1.15f,.18f,.36f,.70f);
  // Small civic sculpture / viewpoint marker built from primitives.
  draw_box(v3(-198,1.2f,934),2.2f,2.4f,.35f,0,.72f,.72f,.68f);
  draw_box(v3(92,1.0f,934),1.8f,2.0f,.35f,0,.72f,.72f,.68f);
}
