// -----------------------------------------------------------------------------
// 0.16 glTF vehicle bodies + animated procedural wheels.
// -----------------------------------------------------------------------------
static int g_car_asset16[4]={-2,-2,-2,-2};
static const char*g_car_names16[4]={"car_compact_hatch","car_compact_sedan","car_sport_hatch","car_compact_suv"};
static void vehicle16_resolve(void){for(int i=0;i<4;i++)if(g_car_asset16[i]==-2)g_car_asset16[i]=asset08_find(g_car_names16[i]);}
static void draw_player_vehicle(void){
  vehicle16_resolve();const VehicleProfile*vp=&g_vehicle_profiles[g_player.car];float cc[4][3]={{.72f,.72f,.68f},{.18f,.50f,.82f},{.75f,.13f,.12f},{.23f,.60f,.34f}};float*c=cc[g_player.car];glPushMatrix();glTranslatef(g_player.body.p.x,g_player.body.p.y-.67f,g_player.body.p.z);glRotatef(g_player.body.yaw*57.29578f,0,1,0);glRotatef(g_player.pitch*57.29578f,1,0,0);glRotatef(g_player.roll*57.29578f,0,0,1);renderer09_set_material(0);if(g_car_asset16[g_player.car]>=0)asset08_draw_mesh(g_car_asset16[g_player.car],c[0],c[1],c[2]);else{draw_local_box(0,.80f,0,g_player.car==3?2.02f:1.82f,.60f,g_player.car==1?4.30f:4.0f,c[0],c[1],c[2]);draw_local_box(0,1.25f,-.1f,1.48f,.62f,1.9f,.10f,.15f,.20f);}float hx=vp->track*.5f,hz=vp->wheelbase*.5f;for(int i=0;i<4;i++){int front=i<2,rightSide=(i&1);float x=rightSide?hx:-hx,z=front?hz:-hz;float comp=g_wheels15[i].compression;glPushMatrix();glTranslatef(x,.25f-(comp-.5f)*.12f,z);if(front)glRotatef(g_player.steer*vp->maxSteer*57.29578f,0,1,0);glRotatef((float)g_time*absf(g_player.speed)*42.0f*(rightSide?1:-1),1,0,0);glScalef(.22f,.36f,.38f);glColor3f(.035f,.035f,.04f);draw_unit_box();glPopMatrix();}glPopMatrix();renderer09_set_material(0);
}
