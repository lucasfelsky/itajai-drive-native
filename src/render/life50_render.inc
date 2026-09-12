// -----------------------------------------------------------------------------
// 0.50 living-world rendering.
// -----------------------------------------------------------------------------
static void life50_draw_pedestrians(void){
  float maxD=(g_lod20_tier==0?150.0f:g_lod20_tier==1?220.0f:300.0f);maxD*=maxD;
  for(int i=0;i<LIFE50_PEDS;i++){Ped50*p=&g_peds50[i];if(!p->active)continue;float dx=p->p.x-g_player.body.p.x,dz=p->p.z-g_player.body.p.z;if(dx*dx+dz*dz>maxD)continue;V3 d=vsub(p->b,p->a);float yaw=(float)atan2((double)d.x,(double)d.z);float bob=(float)sin(g_time*7.0+p->phase)*.025f;float c=(i%5)/5.0f;draw_box(vadd(p->p,v3(0,.86f+bob,0)),.34f,1.12f,.28f,yaw,.20f+.45f*c,.28f+.25f*(1-c),.35f+.30f*c);draw_box(vadd(p->p,v3(0,1.55f+bob,0)),.31f,.31f,.31f,yaw,.70f,.54f,.42f);}
}
static void life50_draw_boats(void){
  float maxD=900.0f*900.0f;for(int i=0;i<LIFE50_BOATS;i++){Boat50*b=&g_boats50[i];if(!b->active)continue;float dx=b->p.x-g_player.body.p.x,dz=b->p.z-g_player.body.p.z;if(dx*dx+dz*dz>maxD)continue;V3 d=vsub(b->b,b->a);float yaw=(float)atan2((double)d.x,(double)d.z);float sz=5.5f+(i%3)*1.2f;draw_box(vadd(b->p,v3(0,.35f,0)),2.1f,.55f,sz,yaw,.82f,.82f,.78f);draw_box(vadd(b->p,v3(0,.85f,-.25f)),1.45f,.60f,sz*.42f,yaw,.22f,.35f,.42f);}
}
static void life50_draw_parked(void){
  traffic40_resolve_assets();float maxD=(g_lod20_tier==0?170.0f:g_lod20_tier==1?260.0f:360.0f);maxD*=maxD;static const float c[8][3]={{.68f,.70f,.72f},{.18f,.37f,.66f},{.66f,.13f,.12f},{.22f,.48f,.27f},{.78f,.77f,.72f},{.20f,.20f,.22f},{.58f,.42f,.20f},{.48f,.18f,.42f}};
  for(int i=0;i<LIFE50_PARKED;i++){Parked50*p=&g_parked50[i];float dx=p->p.x-g_player.body.p.x,dz=p->p.z-g_player.body.p.z;if(dx*dx+dz*dz>maxD)continue;int model=p->model&7,asset=g_traffic_car_assets40[model];if(asset<0)continue;glPushMatrix();glTranslatef(p->p.x,p->p.y-.55f,p->p.z);glRotatef(p->yaw*57.29578f,0,1,0);renderer09_set_material(0);asset08_draw_mesh(asset,c[model][0],c[model][1],c[model][2]);glPopMatrix();}
}
static void life50_draw(void){life50_draw_parked();life50_draw_pedestrians();life50_draw_boats();renderer09_set_material(0);}
