// -----------------------------------------------------------------------------
// 2.60 Traffic 3.0 presentation: utility hazards and compact debug telemetry.
// -----------------------------------------------------------------------------
static void traffic260_draw_hazards(void){
  if(!g_started||(((int)(g_time*2.6f))&1))return;vehicle160_resolve();float maxD=minf(g_lod20_traffic_dist,105.0f);maxD*=maxD;renderer09_set_material(MAT09_GENERIC);
  for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active||!g_traffic260_hazard[i])continue;float dx=t->p.x-g_player.body.p.x,dz=t->p.z-g_player.body.p.z;if(dx*dx+dz*dz>maxD)continue;int model=(i*5+i/3)&7;const CarVisual160*v=&g_car_visual160[model];V3 f=v3((float)sin((double)t->yaw),0,(float)cos((double)t->yaw)),r=v3(f.z,0,-f.x);for(int s=-1;s<=1;s+=2){float side=s*v->w*.43f;V3 pf=vadd(t->p,vadd(vmul(f,v->l*.49f),vmul(r,side))),pr=vadd(t->p,vadd(vmul(f,-v->l*.49f),vmul(r,side)));pf.y+=.06f;pr.y+=.06f;draw_box(pf,.14f,.12f,.07f,t->yaw,1.0f,.44f,.03f);draw_box(pr,.14f,.12f,.07f,t->yaw,1.0f,.38f,.03f);}}
}
static void traffic260_draw_debug(void){if(!g_debug||g_photo70_hide_hud)return;settings_draw_2d_begin();char b[190];sprintf(b,"TRAFFIC 3.0 // press %.2f  fila %d  media %.1f km/h",g_traffic260_pressure,g_traffic260_queue,g_traffic260_avg_speed*3.6f);gl_text(35,975,b,.70f,.84f,.95f);settings_draw_2d_end();}
