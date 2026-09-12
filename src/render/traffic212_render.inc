// -----------------------------------------------------------------------------
// 2.12 traffic intent presentation: blinking amber turn/lane-change indicators.
// -----------------------------------------------------------------------------
static void traffic212_draw_indicators(void){
  if(!g_started||(((int)(g_time*3.2))&1))return;vehicle160_resolve();
  float maxD=minf(g_lod20_traffic_dist,115.0f);maxD*=maxD;
  renderer09_set_material(MAT09_GENERIC);
  for(int i=0;i<g_traffic_count;i++){
    Traffic*t=&g_traffic[i];int dir=(int)g_ai212_indicator[i];if(!t->active||!dir)continue;
    float dx=t->p.x-g_player.body.p.x,dz=t->p.z-g_player.body.p.z;if(dx*dx+dz*dz>maxD)continue;
    int model=(i*5+i/3)&7;const CarVisual160*v=&g_car_visual160[model];
    V3 f=v3((float)sin((double)t->yaw),0,(float)cos((double)t->yaw)),r=v3(f.z,0,-f.x);
    float side=(dir<0?-1.0f:1.0f)*v->w*.43f,front=v->l*.49f,rear=-v->l*.49f;
    V3 pf=vadd(t->p,vadd(vmul(f,front),vmul(r,side)));V3 pr=vadd(t->p,vadd(vmul(f,rear),vmul(r,side)));
    pf.y+=.05f;pr.y+=.05f;draw_box(pf,.14f,.13f,.07f,t->yaw,1.0f,.46f,.04f);draw_box(pr,.14f,.13f,.07f,t->yaw,1.0f,.40f,.03f);
  }
}
