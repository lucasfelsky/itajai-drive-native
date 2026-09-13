// -----------------------------------------------------------------------------
// 4.0 Renderer polish: road readability details that complement the existing
// shader/headlight/wet-window stack instead of replacing it.
// 5.0 appends a junction merge pass after this layer.
// -----------------------------------------------------------------------------
static void roads500_draw_junctions(void);
static int g_render400_reflectors=0,g_render400_wet_markers=0;
static void render400_road_details(void){
  g_render400_reflectors=g_render400_wet_markers=0;if(!g_stream_ready08)return;int dark=atmos280_dark();float wet=clampf(g_wetness60,0,1);renderer09_set_material(MAT09_GENERIC);
  int cap=g_perf80_quality==0?26:g_perf80_quality==1?48:74;
  for(int i=0;i<g_stream_road_count08&&g_render400_reflectors<cap;i++){StreamRoad08*s=&g_stream_roads08[i];if(s->road<0||s->road>=g_world.roadCount)continue;RoadWay*r=&g_world.roads[s->road];if(r->width<7.0f)continue;V3 d=vsub(s->b,s->a),m=vmul(vadd(s->a,s->b),.5f);float len=vlen(d);if(len<9.0f||!city340_near(m,210.0f))continue;u32 h=roads130_hash(s)^0x4a0d91u;if((h&1u)&&r->width<10.0f)continue;V3 f=vmul(d,1.0f/len),rt=v3(f.z,0,-f.x);float edge=maxf(2.2f,r->width*.46f),y=terrain11_road_grade(m.x,m.z)+.075f;
    float br=dark?1.0f:.62f,bg=dark?.84f:.62f,bb=dark?.43f:.58f;for(int side=-1;side<=1;side+=2){V3 q=vadd(m,vmul(rt,side*edge));draw_box(v3(q.x,y,q.z),.12f,.035f,.20f,(float)atan2((double)f.x,(double)f.z),br,bg,bb);g_render400_reflectors++;}
    if(wet>.25f&&g_render400_wet_markers<28&&(h%4u)==0u){V3 q=vadd(m,vmul(rt,((h>>3)&1u)?.7f:-.7f));draw_box(v3(q.x,y-.018f,q.z),.10f,.018f,minf(2.8f,len*.18f),(float)atan2((double)f.x,(double)f.z),.44f+.22f*wet,.55f+.20f*wet,.62f+.18f*wet);g_render400_wet_markers++;}
  }
  renderer09_set_material(MAT09_GENERIC);roads500_draw_junctions();
}
static void render400_overlay(void){if(!g_debug||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();char b[180];hud_panel(20,(float)g_h-326,520,28);sprintf(b,"RENDER 4.0 // reflectors %d | wet road accents %d",g_render400_reflectors,g_render400_wet_markers);gl_text(34,(float)g_h-305,b,.68f,.80f,.90f);settings_draw_2d_end();}
