// -----------------------------------------------------------------------------
// 4.0 Vehicle Presentation pass.
// Adds model-specific silhouette cues on top of the generated glTF body/glass/
// trim contract. No trademark badges are needed: proportions + signature parts
// make the eight Brazilian-market cars readable at gameplay distance.
// -----------------------------------------------------------------------------
static int g_vehicle400_detail_draws=0;
static void vehicle400_bar(float x,float y,float z,float sx,float sy,float sz,float r,float g,float b){vehicle160_local_box_rot(x,y,z,sx,sy,sz,0,0,0,r,g,b);g_vehicle400_detail_draws++;}
static void vehicle400_detail_local(int model){
  if(model<0||model>=CAR160_COUNT)return;const CarVisual160*v=&g_car_visual160[model];float W=v->w,L=v->l,H=v->h;
  renderer09_set_material(MAT09_RUBBER);
  switch(model){
    case 0: // Uno Way: roof rails + pronounced black cladding / upright tail.
      vehicle400_bar(-W*.34f,H*.87f,-.05f,.055f,.06f,L*.56f,.07f,.075f,.075f);vehicle400_bar(W*.34f,H*.87f,-.05f,.055f,.06f,L*.56f,.07f,.075f,.075f);
      vehicle400_bar(0,.50f,-L*.46f,W*.83f,.16f,.07f,.08f,.085f,.085f);break;
    case 1: // Gol G6: compact roof spoiler + broad lower grille.
      vehicle400_bar(0,H*.79f,-L*.47f,W*.63f,.08f,.22f,.07f,.075f,.08f);vehicle400_bar(0,.54f,L*.502f,W*.58f,.14f,.04f,.045f,.048f,.05f);break;
    case 2: // HB20: split/narrow lighting signature and small roof antenna.
      renderer09_set_material(MAT09_GLASS);vehicle400_bar(0,.70f,L*.513f,W*.58f,.035f,.035f,.78f,.82f,.72f);renderer09_set_material(MAT09_RUBBER);vehicle400_bar(0,H*.91f,-.22f,.035f,.16f,.28f,.05f,.055f,.06f);break;
    case 3: // Strada: exposed bed rails + cab/bed protection hoop.
      renderer09_set_material(MAT09_CHROME);vehicle400_bar(-W*.39f,.88f,-L*.29f,.045f,.055f,L*.30f,.30f,.31f,.31f);vehicle400_bar(W*.39f,.88f,-L*.29f,.045f,.055f,L*.30f,.30f,.31f,.31f);vehicle400_bar(0,1.40f,.18f,W*.74f,.055f,.06f,.28f,.29f,.29f);vehicle400_bar(-W*.36f,1.10f,.18f,.055f,.62f,.06f,.28f,.29f,.29f);vehicle400_bar(W*.36f,1.10f,.18f,.055f,.62f,.06f,.28f,.29f,.29f);break;
    case 4: // Corolla: chrome belt line + subtle trunk lip.
      renderer09_set_material(MAT09_CHROME);vehicle400_bar(-W*.49f,.88f,-.05f,.025f,.025f,L*.54f,.48f,.49f,.50f);vehicle400_bar(W*.49f,.88f,-.05f,.025f,.025f,L*.54f,.48f,.49f,.50f);vehicle400_bar(0,.93f,-L*.49f,W*.58f,.045f,.18f,.40f,.41f,.42f);break;
    case 5: // Renegade: roof rails + seven-slot-inspired front rhythm / square cladding.
      renderer09_set_material(MAT09_RUBBER);vehicle400_bar(-W*.32f,H*.91f,-.05f,.065f,.07f,L*.57f,.06f,.065f,.07f);vehicle400_bar(W*.32f,H*.91f,-.05f,.065f,.07f,L*.57f,.06f,.065f,.07f);
      renderer09_set_material(MAT09_CHROME);for(int k=-3;k<=3;k++)vehicle400_bar(k*W*.075f,.68f,L*.514f,.028f,.24f,.025f,.34f,.35f,.35f);break;
    case 6: // Onix: slim front/rear light bars + shark-fin antenna.
      renderer09_set_material(MAT09_GLASS);vehicle400_bar(0,.71f,L*.512f,W*.62f,.030f,.030f,.84f,.85f,.72f);vehicle400_bar(0,.75f,-L*.512f,W*.58f,.030f,.030f,.68f,.05f,.035f);renderer09_set_material(MAT09_RUBBER);vehicle400_bar(0,H*.92f,-.27f,.05f,.14f,.18f,.05f,.055f,.06f);break;
    case 7: // Celta: simple chunky bumpers + tiny roof antenna.
      renderer09_set_material(MAT09_RUBBER);vehicle400_bar(0,.46f,L*.505f,W*.78f,.18f,.055f,.08f,.085f,.085f);vehicle400_bar(0,.48f,-L*.505f,W*.80f,.18f,.055f,.08f,.085f,.085f);vehicle400_bar(0,H*.90f,-.18f,.025f,.20f,.025f,.05f,.055f,.06f);break;
  }
  renderer09_set_material(MAT09_GENERIC);
}
static void vehicle400_draw_player(void){
  if(!g_started||g_player.camera==2)return;int model=g_player_model160[g_player.car&3];glPushMatrix();glTranslatef(g_player.body.p.x,g_player.body.p.y-.66f,g_player.body.p.z);glRotatef(g_player.body.yaw*57.29578f,0,1,0);glRotatef(g_player.pitch*57.29578f,1,0,0);glRotatef(g_player.roll*57.29578f,0,0,1);vehicle400_detail_local(model);glPopMatrix();
}
static void vehicle400_draw_traffic(void){
  float maxD=minf(g_lod20_traffic_dist,82.0f);maxD*=maxD;for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active)continue;float dx=t->p.x-g_player.body.p.x,dz=t->p.z-g_player.body.p.z;if(dx*dx+dz*dz>maxD)continue;int model=(i*5+i/3)&7;glPushMatrix();glTranslatef(t->p.x,t->p.y-.55f,t->p.z);glRotatef(t->yaw*57.29578f,0,1,0);vehicle400_detail_local(model);glPopMatrix();}
}
