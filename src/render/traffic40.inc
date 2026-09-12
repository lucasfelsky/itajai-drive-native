// -----------------------------------------------------------------------------
// 0.40 expanded Brazil-inspired traffic fleet: eight fictional silhouettes.
// Player physics/profiles remain the proven four-car set; ambient traffic gets
// much more visual variety without changing route simulation.
// -----------------------------------------------------------------------------
static int g_traffic_car_assets40[8]={-2,-2,-2,-2,-2,-2,-2,-2};
static const char*g_traffic_car_names40[8]={
  "car_compact_hatch","car_compact_sedan","car_sport_hatch","car_compact_suv",
  "car_city_hatch","car_small_pickup","car_wagon","car_midsize_sedan"
};
static void traffic40_resolve_assets(void){for(int i=0;i<8;i++)if(g_traffic_car_assets40[i]==-2)g_traffic_car_assets40[i]=asset08_find(g_traffic_car_names40[i]);}
static void draw_traffic(void){
  traffic40_resolve_assets();float maxD=g_lod20_traffic_dist*g_lod20_traffic_dist;
  static const float palette[12][3]={{.72f,.72f,.68f},{.16f,.42f,.78f},{.74f,.12f,.10f},{.20f,.56f,.30f},{.82f,.82f,.84f},{.18f,.18f,.20f},{.70f,.50f,.22f},{.54f,.18f,.48f},{.78f,.72f,.58f},{.26f,.48f,.58f},{.56f,.60f,.64f},{.12f,.30f,.55f}};
  for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active)continue;float dx=t->p.x-g_player.body.p.x,dz=t->p.z-g_player.body.p.z;if(dx*dx+dz*dz>maxD)continue;int car=(i*5+i/3)&7;const float*c=palette[(i*7+i/2)%12];
    glPushMatrix();glTranslatef(t->p.x,t->p.y-.55f,t->p.z);glRotatef(t->yaw*57.29578f,0,1,0);renderer09_set_material(0);int asset=g_traffic_car_assets40[car];if(asset>=0)asset08_draw_mesh(asset,c[0],c[1],c[2]);else{float w=car==5?1.82f:1.76f,l=car==5?4.45f:(car==7?4.6f:4.0f);draw_local_box(0,.75f,0,w,.58f,l,c[0],c[1],c[2]);draw_local_box(0,1.17f,-.1f,w*.82f,.55f,l*.48f,.10f,.15f,.20f);}glPopMatrix();
    if(g_traffic_brake_amount[i]>.12f){V3 f=v3((float)sin(t->yaw),0,(float)cos(t->yaw)),rt=v3(f.z,0,-f.x),rear=vadd(t->p,vmul(f,-2.02f));draw_box(vadd(vadd(rear,vmul(rt,.58f)),v3(0,.22f,0)),.20f,.16f,.12f,t->yaw,1,.04f,.03f);draw_box(vadd(vsub(rear,vmul(rt,.58f)),v3(0,.22f,0)),.20f,.16f,.12f,t->yaw,1,.04f,.03f);}
  }
}
