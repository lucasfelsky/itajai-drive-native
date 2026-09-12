// -----------------------------------------------------------------------------
// 1.6 traffic presentation using the eight recognizable Brazilian car models.
// Simulation stays intentionally decoupled from visual choice.
// -----------------------------------------------------------------------------
static void draw_traffic(void){
  vehicle160_resolve();float maxD=g_lod20_traffic_dist*g_lod20_traffic_dist;
  static const float palette[14][3]={{.72f,.72f,.68f},{.16f,.42f,.78f},{.74f,.12f,.10f},{.20f,.56f,.30f},{.82f,.82f,.84f},{.18f,.18f,.20f},{.70f,.50f,.22f},{.54f,.18f,.48f},{.78f,.72f,.58f},{.26f,.48f,.58f},{.56f,.60f,.64f},{.12f,.30f,.55f},{.88f,.88f,.86f},{.38f,.18f,.10f}};
  int lights=(g_hour<6||g_hour>=18||g_weather>=2);
  for(int i=0;i<g_traffic_count;i++){
    Traffic*t=&g_traffic[i];if(!t->active)continue;float dx=t->p.x-g_player.body.p.x,dz=t->p.z-g_player.body.p.z;if(dx*dx+dz*dz>maxD)continue;
    int car=(i*5+i/3)&7;const float*c=palette[(i*7+i/2)%14];float spin=(float)g_time*absf(t->speed)*50.0f;
    glPushMatrix();glTranslatef(t->p.x,t->p.y-.55f,t->p.z);glRotatef(t->yaw*57.29578f,0,1,0);vehicle160_draw_model_local(car,c[0],c[1],c[2],g_traffic_brake_amount[i]>.12f,lights,spin,0,1);glPopMatrix();
  }
}
