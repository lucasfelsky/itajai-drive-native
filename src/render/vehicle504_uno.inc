// Renderer 5.0.4 hero treatment for the Novo Uno Way.
static int g_vehicle504_uno_parts=0;
static void vehicle504_uno_bar(float x,float y,float z,float sx,float sy,float sz,float rx,float ry,float rz,float r,float g,float b){vehicle160_local_box_rot(x,y,z,sx,sy,sz,rx,ry,rz,r,g,b);g_vehicle504_uno_parts++;}
static void vehicle504_uno_disc(float x,float y,float z,float rad,float r,float g,float b){glPushMatrix();glTranslatef(x,y,z);glRotatef(90,0,1,0);vehicle160_cylinder_x(rad,.035f,r,g,b);glPopMatrix();g_vehicle504_uno_parts++;}
static void vehicle504_uno_local(void){const CarVisual160*v=&g_car_visual160[0];float W=v->w,L=v->l,H=v->h;renderer09_set_material(MAT09_RUBBER);
  // Way crossover mask, side cladding and silver lower protector.
  vehicle504_uno_bar(0,.55f,L*.501f,W*.88f,.39f,.055f,0,0,0,.075f,.080f,.082f);vehicle504_uno_bar(0,.37f,L*.516f,W*.60f,.12f,.030f,0,0,0,.035f,.040f,.042f);vehicle504_uno_bar(0,.315f,L*.520f,W*.47f,.055f,.035f,0,0,0,.48f,.49f,.48f);
  vehicle504_uno_bar(-W*.475f,.48f,0,.085f,.22f,L*.60f,0,0,0,.065f,.070f,.072f);vehicle504_uno_bar(W*.475f,.48f,0,.085f,.22f,L*.60f,0,0,0,.065f,.070f,.072f);
  // Four vertical front slots are one of the most readable Novo Uno cues.
  renderer09_set_material(MAT09_CHROME);for(int k=0;k<4;k++)vehicle504_uno_bar(W*(.035f+k*.075f),.67f,L*.532f,.038f,.20f,.025f,0,0,0,.30f,.31f,.31f);
  // Large swept headlamp housings, fog lamps and black lower intake.
  renderer09_set_material(MAT09_GLASS);for(int s=-1;s<=1;s+=2){vehicle504_uno_bar(s*W*.30f,.73f,L*.526f,W*.25f,.20f,.030f,-3,0,s*5,.78f,.80f,.74f);vehicle504_uno_disc(s*W*.31f,.42f,L*.535f,.085f,.74f,.78f,.72f);}renderer09_set_material(MAT09_RUBBER);vehicle504_uno_bar(0,.43f,L*.534f,W*.43f,.14f,.028f,0,0,0,.025f,.030f,.032f);
  // Upright rear hatch, large rear glass and tall pillar-mounted lamps.
  renderer09_set_material(MAT09_GLASS);vehicle504_uno_bar(0,1.13f,-L*.515f,W*.69f,.56f,.030f,1,0,0,.055f,.095f,.125f);for(int s=-1;s<=1;s+=2){vehicle504_uno_bar(s*W*.405f,1.08f,-L*.526f,W*.105f,.59f,.035f,0,0,0,.60f,.045f,.030f);vehicle504_uno_bar(s*W*.405f,1.22f,-L*.531f,W*.075f,.13f,.040f,0,0,0,.88f,.22f,.08f);}renderer09_set_material(MAT09_RUBBER);vehicle504_uno_bar(0,.49f,-L*.527f,W*.84f,.21f,.040f,0,0,0,.07f,.075f,.078f);vehicle504_uno_bar(0,.35f,-L*.535f,W*.45f,.055f,.035f,0,0,0,.46f,.47f,.46f);
  // Roof rails, B pillars, window belt and visible door shut lines.
  vehicle504_uno_bar(-W*.34f,H*.91f,-.05f,.055f,.065f,L*.58f,0,0,0,.045f,.050f,.052f);vehicle504_uno_bar(W*.34f,H*.91f,-.05f,.055f,.065f,L*.58f,0,0,0,.045f,.050f,.052f);for(int s=-1;s<=1;s+=2){vehicle504_uno_bar(s*W*.493f,1.12f,-.05f,.030f,.53f,.065f,0,0,0,.035f,.040f,.043f);vehicle504_uno_bar(s*W*.500f,.79f,-.08f,.026f,.050f,L*.54f,0,0,0,.12f,.13f,.13f);vehicle504_uno_bar(s*W*.501f,.84f,.38f,.030f,.62f,.040f,0,0,0,.16f,.17f,.17f);vehicle504_uno_bar(s*W*.501f,.84f,-.37f,.030f,.62f,.040f,0,0,0,.16f,.17f,.17f);}
  // Rear wiper and short roof antenna.
  vehicle504_uno_bar(.05f,1.15f,-L*.537f,.035f,.035f,.35f,0,0,-13,.08f,.085f,.09f);vehicle504_uno_bar(0,H*.965f,-.27f,.025f,.16f,.025f,-25,0,0,.05f,.055f,.058f);renderer09_set_material(MAT09_GENERIC);
}
static void vehicle504_uno_player(void){if(!g_started||g_player.camera==2||g_player_model160[g_player.car&3]!=0)return;glPushMatrix();glTranslatef(g_player.body.p.x,g_player.body.p.y-.66f,g_player.body.p.z);glRotatef(g_player.body.yaw*57.29578f,0,1,0);glRotatef(g_player.pitch*57.29578f,1,0,0);glRotatef(g_player.roll*57.29578f,0,0,1);vehicle504_uno_local();glPopMatrix();}
static void vehicle504_uno_traffic(void){float md=minf(g_lod20_traffic_dist,72.0f);md*=md;for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active||((i*5+i/3)&7)!=0)continue;float dx=t->p.x-g_player.body.p.x,dz=t->p.z-g_player.body.p.z;if(dx*dx+dz*dz>md)continue;glPushMatrix();glTranslatef(t->p.x,t->p.y-.55f,t->p.z);glRotatef(t->yaw*57.29578f,0,1,0);vehicle504_uno_local();glPopMatrix();}}
