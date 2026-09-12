// -----------------------------------------------------------------------------
// 1.9 Vehicle Presentation.
// Procedural glTF bodies use recognizable real-world Brazilian proportions;
// badges/logos are deliberately omitted. 1.9 assigns dedicated paint, glass,
// rubber and chrome materials while retaining the 1.8 cockpit/interior pass.
// -----------------------------------------------------------------------------
#define CAR160_COUNT 8
#define CAR160_HATCH 0
#define CAR160_BOX_HATCH 1
#define CAR160_PICKUP 2
#define CAR160_SEDAN 3
#define CAR160_SUV 4

typedef struct {
  const char*asset; const char*label;
  float w,l,h,wheelbase,track,wheelR,cabinZ,cabinL;
  int style;
} CarVisual160;

static const CarVisual160 g_car_visual160[CAR160_COUNT]={
  {"car_fiat_uno_way_2014",   "FIAT UNO WAY 2014",      1.64f,3.81f,1.55f,2.38f,1.42f,.30f,-.06f,2.46f,CAR160_BOX_HATCH},
  {"car_vw_gol_g6_2016",     "VW GOL G6",              1.66f,3.90f,1.46f,2.47f,1.44f,.30f,-.12f,2.52f,CAR160_HATCH},
  {"car_hyundai_hb20_2024",  "HYUNDAI HB20",           1.72f,4.02f,1.47f,2.53f,1.47f,.31f,-.08f,2.56f,CAR160_HATCH},
  {"car_fiat_strada_2024",   "FIAT STRADA",            1.73f,4.48f,1.61f,2.74f,1.48f,.32f, .67f,1.63f,CAR160_PICKUP},
  {"car_toyota_corolla_2020","TOYOTA COROLLA",         1.78f,4.63f,1.45f,2.70f,1.53f,.32f,-.11f,2.55f,CAR160_SEDAN},
  {"car_jeep_renegade_2021", "JEEP RENEGADE",           1.81f,4.23f,1.67f,2.57f,1.55f,.34f,-.17f,2.70f,CAR160_SUV},
  {"car_chevrolet_onix_2024", "CHEVROLET ONIX",         1.73f,4.16f,1.48f,2.55f,1.48f,.31f,-.10f,2.63f,CAR160_HATCH},
  {"car_chevrolet_celta_2015","CHEVROLET CELTA",       1.63f,3.79f,1.43f,2.44f,1.42f,.29f,-.14f,2.42f,CAR160_HATCH}
};

static int g_car_asset160[CAR160_COUNT]={-2,-2,-2,-2,-2,-2,-2,-2};
static const int g_player_model160[4]={0,1,2,5};
static int g_vehicle180_cockpit_drawn=0;

static void vehicle160_resolve(void){for(int i=0;i<CAR160_COUNT;i++)if(g_car_asset160[i]==-2)g_car_asset160[i]=asset08_find(g_car_visual160[i].asset);}
static const char*vehicle160_player_name(void){int m=g_player_model160[g_player.car&3];return g_car_visual160[m].label;}

static void vehicle160_local_box_rot(float x,float y,float z,float sx,float sy,float sz,float rx,float ry,float rz,float r,float g,float b){
  glPushMatrix();glTranslatef(x,y,z);if(ry!=0)glRotatef(ry,0,1,0);if(rx!=0)glRotatef(rx,1,0,0);if(rz!=0)glRotatef(rz,0,0,1);glScalef(sx,sy,sz);glColor3f(r,g,b);draw_unit_box();glPopMatrix();
}

static void vehicle160_cylinder_x(float radius,float width,float r,float g,float b){
  const int n=12;glColor3f(r,g,b);glBegin(GL_QUADS);for(int k=0;k<n;k++){float a=(float)k*6.2831853f/n,beta=(float)(k+1)*6.2831853f/n;float ya=(float)cos((double)a)*radius,za=(float)sin((double)a)*radius,yb=(float)cos((double)beta)*radius,zb=(float)sin((double)beta)*radius;glNormal3f(0,ya/radius,za/radius);glVertex3f(-width*.5f,ya,za);glVertex3f(width*.5f,ya,za);glVertex3f(width*.5f,yb,zb);glVertex3f(-width*.5f,yb,zb);}glEnd();
  glBegin(GL_TRIANGLES);for(int side=-1;side<=1;side+=2)for(int k=0;k<n;k++){float a=(float)k*6.2831853f/n,beta=(float)(k+1)*6.2831853f/n;float x=side*width*.5f;glNormal3f((float)side,0,0);glVertex3f(x,0,0);if(side>0){glVertex3f(x,(float)cos((double)a)*radius,(float)sin((double)a)*radius);glVertex3f(x,(float)cos((double)beta)*radius,(float)sin((double)beta)*radius);}else{glVertex3f(x,(float)cos((double)beta)*radius,(float)sin((double)beta)*radius);glVertex3f(x,(float)cos((double)a)*radius,(float)sin((double)a)*radius);}}glEnd();
}

static void vehicle160_wheel(float x,float y,float z,float radius,float width,float steerDeg,float spinDeg){
  glPushMatrix();glTranslatef(x,y,z);if(steerDeg!=0)glRotatef(steerDeg,0,1,0);glRotatef(spinDeg,1,0,0);renderer09_set_material(MAT09_RUBBER);vehicle160_cylinder_x(radius,width,.035f,.035f,.042f);renderer09_set_material(MAT09_CHROME);vehicle160_cylinder_x(radius*.57f,width*1.035f,.42f,.43f,.45f);vehicle160_cylinder_x(radius*.20f,width*1.05f,.10f,.11f,.12f);renderer09_set_material(MAT09_GENERIC);glPopMatrix();
}

static void vehicle180_plate(float y,float z,float front){
  renderer09_set_material(MAT09_GENERIC);vehicle160_local_box_rot(0,y,z,.52f,.13f,.025f,0,0,0,.86f,.88f,.87f);vehicle160_local_box_rot(0,y+.055f,z+(front?.016f:-.016f),.50f,.020f,.030f,0,0,0,.12f,.24f,.42f);
}

static void vehicle160_trim(int model,int braking,int headlights){
  const CarVisual160*v=&g_car_visual160[model];float W=v->w,L=v->l,H=v->h,cz=v->cabinZ,cl=v->cabinL;
  float gy=.98f+(H-1.43f)*.27f,gh=clampf((H-.83f)*.55f,.30f,.48f),sideZ=cz-.03f;
  renderer09_set_material(MAT09_GLASS);
  vehicle160_local_box_rot(-W*.486f,gy,sideZ,.035f,gh,cl*.66f,0,0,0,.055f,.095f,.125f);vehicle160_local_box_rot(W*.486f,gy,sideZ,.035f,gh,cl*.66f,0,0,0,.055f,.095f,.125f);
  float rake=v->style==CAR160_BOX_HATCH||v->style==CAR160_SUV?8.0f:18.0f;vehicle160_local_box_rot(0,gy,cz+cl*.48f,W*.72f,gh*.92f,.045f,-rake,0,0,.07f,.11f,.14f);
  if(v->style!=CAR160_PICKUP)vehicle160_local_box_rot(0,gy,cz-cl*.48f,W*.70f,gh*.90f,.045f,rake*.70f,0,0,.065f,.10f,.13f);else vehicle160_local_box_rot(0,1.08f,cz-cl*.47f,W*.66f,.42f,.045f,2,0,0,.06f,.10f,.13f);
  renderer09_set_material(MAT09_RUBBER);
  vehicle160_local_box_rot(0,.47f,L*.496f,W*.84f,.16f,.075f,0,0,0,.07f,.075f,.08f);vehicle160_local_box_rot(0,.47f,-L*.496f,W*.86f,.16f,.075f,0,0,0,.07f,.075f,.08f);vehicle160_local_box_rot(0,.43f,L*.501f,W*.39f,.20f,.040f,0,0,0,.035f,.04f,.045f);
  renderer09_set_material(MAT09_GLASS);
  float lx=W*.31f,fy=.66f+(H-1.45f)*.08f;for(int s=-1;s<=1;s+=2){vehicle160_local_box_rot(s*lx,fy,L*.505f,W*.20f,.16f,.035f,0,0,0,headlights?1.0f:.72f,headlights?.94f:.72f,headlights?.72f:.68f);vehicle160_local_box_rot(s*lx,.66f,-L*.505f,W*.18f,.17f,.035f,0,0,0,braking?1.0f:.55f,.035f,.025f);}
  // Side mirrors and door handles improve the silhouette substantially at close range.
  renderer09_set_material(MAT09_RUBBER);for(int s=-1;s<=1;s+=2){vehicle160_local_box_rot(s*W*.54f,gy-.02f,cz+cl*.29f,.16f,.12f,.26f,0,0,0,.055f,.065f,.07f);renderer09_set_material(MAT09_GLASS);vehicle160_local_box_rot(s*W*.545f,gy-.01f,cz+cl*.30f,.012f,.080f,.18f,0,0,0,.11f,.17f,.21f);renderer09_set_material(MAT09_CHROME);vehicle160_local_box_rot(s*W*.505f,.88f,cz-.06f,.025f,.035f,.22f,0,0,0,.25f,.26f,.27f);renderer09_set_material(MAT09_RUBBER);}
  vehicle180_plate(.49f,L*.512f,1);vehicle180_plate(.52f,-L*.512f,0);
  if(v->style==CAR160_SUV){renderer09_set_material(MAT09_CHROME);for(int k=-3;k<=3;k++)vehicle160_local_box_rot(k*.105f,.62f,L*.508f,.045f,.24f,.028f,0,0,0,.20f,.21f,.21f);}if(v->style==CAR160_BOX_HATCH){renderer09_set_material(MAT09_RUBBER);vehicle160_local_box_rot(0,.38f,0,W*.98f,.12f,L*.73f,0,0,0,.09f,.09f,.085f);}if(v->style==CAR160_SEDAN){renderer09_set_material(MAT09_CHROME);vehicle160_local_box_rot(0,.57f,L*.508f,W*.52f,.075f,.030f,0,0,0,.42f,.43f,.44f);}if(v->style==CAR160_PICKUP){renderer09_set_material(MAT09_RUBBER);vehicle160_local_box_rot(0,.73f,-L*.30f,W*.78f,.055f,L*.31f,0,0,0,.095f,.10f,.10f);}renderer09_set_material(MAT09_GENERIC);
}

static void vehicle180_steering_ring(float cx,float cy,float cz,float radius,float steerDeg){
  renderer09_set_material(MAT09_RUBBER);glPushMatrix();glTranslatef(cx,cy,cz);glRotatef(-12.0f,1,0,0);glRotatef(steerDeg*1.55f,0,0,1);for(int k=0;k<12;k++){float a=(float)k*6.2831853f/12.0f,x=(float)cos((double)a)*radius,y=(float)sin((double)a)*radius;vehicle160_local_box_rot(x,y,0,.085f,.030f,.030f,0,0,a*57.29578f+90.0f,.045f,.047f,.050f);}vehicle160_local_box_rot(0,0,.015f,.11f,.11f,.045f,0,0,0,.11f,.12f,.12f);vehicle160_local_box_rot(0,0,.010f,radius*.86f,.025f,.025f,0,0,0,.055f,.057f,.060f);glPopMatrix();renderer09_set_material(MAT09_GENERIC);
}

static void vehicle180_cockpit_local(int model,float steerDeg){
  const CarVisual160*v=&g_car_visual160[model];float W=v->w,L=v->l;g_vehicle180_cockpit_drawn=1;
  renderer09_set_material(MAT09_CAR_PAINT);vehicle160_local_box_rot(0,.69f,L*.31f,W*.93f,.20f,L*.34f,-2,0,0,.24f,.25f,.26f);
  renderer09_set_material(MAT09_RUBBER);vehicle160_local_box_rot(0,.80f,.58f,W*.91f,.23f,.48f,-7,0,0,.075f,.080f,.085f);vehicle160_local_box_rot(0,.89f,.43f,W*.62f,.11f,.10f,-12,0,0,.025f,.030f,.035f);
  renderer09_set_material(MAT09_GLASS);vehicle160_local_box_rot(-W*.22f,.94f,.47f,.31f,.15f,.035f,-12,0,0,.06f,.09f,.12f);
  vehicle180_steering_ring(-W*.22f,.91f,.38f,.18f,steerDeg);
  renderer09_set_material(MAT09_RUBBER);vehicle160_local_box_rot(.03f,.75f,.43f,.25f,.30f,.08f,-9,0,0,.035f,.038f,.042f);vehicle160_local_box_rot(.03f,.83f,.38f,.16f,.05f,.025f,-9,0,0,.12f,.16f,.18f);renderer09_set_material(MAT09_CHROME);vehicle160_local_box_rot(W*.25f,.82f,.42f,.29f,.035f,.035f,-9,0,0,.16f,.17f,.17f);
  renderer09_set_material(MAT09_RUBBER);vehicle160_local_box_rot(-W*.38f,1.17f,.82f,.055f,.66f,.055f,-18,0,-5,.08f,.085f,.09f);vehicle160_local_box_rot(W*.38f,1.17f,.82f,.055f,.66f,.055f,-18,0,5,.08f,.085f,.09f);renderer09_set_material(MAT09_GLASS);vehicle160_local_box_rot(0,1.28f,.65f,.31f,.10f,.055f,0,0,0,.055f,.065f,.07f);
  renderer09_set_material(MAT09_RUBBER);vehicle160_local_box_rot(-W*.23f,.82f,-.38f,.43f,.63f,.36f,-5,0,0,.11f,.115f,.12f);vehicle160_local_box_rot(W*.23f,.82f,-.38f,.43f,.63f,.36f,-5,0,0,.11f,.115f,.12f);renderer09_set_material(MAT09_GENERIC);
}

static void vehicle160_draw_model_local(int model,float r,float g,float b,int braking,int headlights,float spinDeg,float steerDeg,int drawWheels){
  vehicle160_resolve();const CarVisual160*v=&g_car_visual160[model];int asset=g_car_asset160[model];renderer09_set_material(MAT09_CAR_PAINT);if(asset>=0)asset08_draw_mesh(asset,r,g,b);else{draw_local_box(0,.70f,0,v->w,.62f,v->l,r,g,b);draw_local_box(0,1.10f,v->cabinZ,v->w*.78f,.62f,v->cabinL,r*.82f,g*.82f,b*.82f);}vehicle160_trim(model,braking,headlights);
  if(drawWheels){float hx=v->track*.5f,hz=v->wheelbase*.5f;for(int k=0;k<4;k++){int front=k<2,right=(k&1);float comp=.50f;if(model==g_player_model160[g_player.car&3])comp=g_wheels15[k].compression;float x=right?hx:-hx,z=front?hz:-hz,y=.34f-(comp-.50f)*.10f;vehicle160_wheel(x,y,z,v->wheelR,.20f,front?steerDeg:0,spinDeg*(right?1.0f:-1.0f));}}renderer09_set_material(MAT09_GENERIC);
}

static void draw_player_vehicle(void){
  int model=g_player_model160[g_player.car&3];float cc[4][3]={{.72f,.69f,.58f},{.18f,.48f,.78f},{.72f,.12f,.10f},{.24f,.51f,.28f}};float*c=cc[g_player.car&3];int lights=(g_hour<6||g_hour>=18||g_weather>=2);float spin=(float)g_time*absf(g_player.speed)*52.0f,steer=g_player.steer*g_vehicle_profiles[g_player.car].maxSteer*57.29578f;g_vehicle180_cockpit_drawn=0;
  glPushMatrix();glTranslatef(g_player.body.p.x,g_player.body.p.y-.66f,g_player.body.p.z);glRotatef(g_player.body.yaw*57.29578f,0,1,0);glRotatef(g_player.pitch*57.29578f,1,0,0);glRotatef(g_player.roll*57.29578f,0,0,1);if(g_player.camera==2)vehicle180_cockpit_local(model,steer);else vehicle160_draw_model_local(model,c[0],c[1],c[2],0,lights,spin,steer,1);glPopMatrix();renderer09_set_material(MAT09_GENERIC);
}

// Legacy internal names are kept because older presentation layers are still
// compiled into the single translation unit even though 1.9 supersedes them.
static void vehicle16_resolve(void){vehicle160_resolve();}
#define g_car_asset16 g_car_asset160
static void traffic40_resolve_assets(void){vehicle160_resolve();}
#define g_traffic_car_assets40 g_car_asset160
