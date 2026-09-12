// -----------------------------------------------------------------------------
// 0.11 terrain/coast renderer.
// -----------------------------------------------------------------------------
#define TERRAIN11_N 31
#define TERRAIN11_STEP 42.0f
static float g_terrain_grid11[TERRAIN11_N][TERRAIN11_N];

static void terrain11_draw_ocean(void){
  float pz=g_player.body.p.z;int strips=38;float z0=pz-1900.0f,step=100.0f;
  renderer09_set_material(0);glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);glNormal3f(0,1,0);glBegin(GL_QUADS);
  for(int i=0;i<strips;i++){float a=z0+i*step,b=a+step,sa=terrain11_shore_x(a)+6.0f,sb=terrain11_shore_x(b)+6.0f;float wa=-.18f+(float)sin((double)(g_time*.65+a*.018f))*.045f,wb=-.18f+(float)sin((double)(g_time*.65+b*.018f))*.045f;glColor4f(.08f,.33f,.48f,.78f);glVertex3f(sa,wa,a);glVertex3f(sa+2200,wa,a);glVertex3f(sb+2200,wb,b);glVertex3f(sb,wb,b);}glEnd();glDisable(GL_BLEND);renderer09_set_material(0);g_draw_calls++;
}

static void draw_ground(void){
  float ox=(float)((int)(g_player.body.p.x/TERRAIN11_STEP))*TERRAIN11_STEP-(TERRAIN11_N/2)*TERRAIN11_STEP;
  float oz=(float)((int)(g_player.body.p.z/TERRAIN11_STEP))*TERRAIN11_STEP-(TERRAIN11_N/2)*TERRAIN11_STEP;
  for(int z=0;z<TERRAIN11_N;z++)for(int x=0;x<TERRAIN11_N;x++)g_terrain_grid11[z][x]=terrain11_height(ox+x*TERRAIN11_STEP,oz+z*TERRAIN11_STEP);
  renderer09_set_material(1);glNormal3f(0,1,0);glBegin(GL_QUADS);
  for(int z=0;z<TERRAIN11_N-1;z++)for(int x=0;x<TERRAIN11_N-1;x++){float x0=ox+x*TERRAIN11_STEP,x1=x0+TERRAIN11_STEP,z0=oz+z*TERRAIN11_STEP,z1=z0+TERRAIN11_STEP;float shore=terrain11_shore_x((z0+z1)*.5f),mid=(x0+x1)*.5f;float beach=clampf((mid-(shore-115.0f))/120.0f,0,1);if(mid>shore+10.0f)continue;if(beach>.05f)glColor3f(.76f,.69f,.50f);else glColor3f(.55f,.72f,.48f);glTexCoord2f(x0*.035f,z0*.035f);glVertex3f(x0,g_terrain_grid11[z][x],z0);glTexCoord2f(x1*.035f,z0*.035f);glVertex3f(x1,g_terrain_grid11[z][x+1],z0);glTexCoord2f(x1*.035f,z1*.035f);glVertex3f(x1,g_terrain_grid11[z+1][x+1],z1);glTexCoord2f(x0*.035f,z1*.035f);glVertex3f(x0,g_terrain_grid11[z+1][x],z1);}glEnd();renderer09_set_material(0);g_draw_calls++;terrain11_draw_ocean();
}

static void draw_roads(void){draw_roads_legacy10();}
static void draw_buildings(void){draw_buildings_legacy10();}
static void draw_hud(float fps){draw_hud_legacy10(fps);if(g_settings_menu_open)return;settings_draw_2d_begin();hud_panel(20,20,520,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 0.11.0",.92f,.95f,1);if(g_debug){char b[220];hud_panel(835,(float)g_h-150,420,31);sprintf(b,"TERRAIN %.1fm | WATER %.1fm | %s",g_terrain_height11,g_water_depth11,g_player_in_water11?"OCEANO":"TERRA");gl_text(850,(float)g_h-127,b,.62f,.84f,1);}settings_draw_2d_end();}
