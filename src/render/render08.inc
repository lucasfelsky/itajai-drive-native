// -----------------------------------------------------------------------------
// 0.8 render overrides: consume disk-streamed regional geometry and draw assets
// from the compiled glTF/GLB runtime pack.
// -----------------------------------------------------------------------------
static void draw_lane_markings08(void){
  if(!g_stream_ready08||g_stream_road_count08<=0){draw_lane_markings_legacy07();return;}glLineWidth(1.25f);glBegin(GL_LINES);glNormal3f(0,1,0);
  for(int i=0;i<g_stream_road_count08;i++){StreamRoad08*s=&g_stream_roads08[i];V3 mid=vmul(vadd(s->a,s->b),.5f);float px=mid.x-g_player.body.p.x,pz=mid.z-g_player.body.p.z;if(px*px+pz*pz>430*430)continue;V3 d=vsub(s->b,s->a);float len=vlen(d);if(len<1||s->road<0||s->road>=g_world.roadCount)continue;V3 right=v3(d.z/len,0,-d.x/len);RoadWay*r=&g_world.roads[s->road];float half=r->width*.5f;
    if(r->width>=8){glColor3f(.78f,.80f,.79f);float eo=half-.16f;V3 la=vadd(s->a,vmul(right,eo)),lb=vadd(s->b,vmul(right,eo)),ra=vsub(s->a,vmul(right,eo)),rb=vsub(s->b,vmul(right,eo));glVertex3f(la.x,.032f,la.z);glVertex3f(lb.x,.032f,lb.z);glVertex3f(ra.x,.032f,ra.z);glVertex3f(rb.x,.032f,rb.z);}
    int total=road_total_lanes(s->road);if(!r->oneway){glColor3f(.86f,.72f,.16f);glVertex3f(s->a.x,.036f,s->a.z);glVertex3f(s->b.x,.036f,s->b.z);int per=(int)maxf(1,(float)(total/2));float lw=half/per;for(int side=-1;side<=1;side+=2)for(int k=1;k<per;k++){float off=side*(half-k*lw);glColor3f(.88f,.90f,.88f);for(float q=0;q<len;q+=8){float q1=minf(q+4,len),t0=q/len,t1=q1/len;V3 a=vadd(vadd(s->a,vmul(d,t0)),vmul(right,off)),b=vadd(vadd(s->a,vmul(d,t1)),vmul(right,off));glVertex3f(a.x,.038f,a.z);glVertex3f(b.x,.038f,b.z);}}}else if(total>1){float lw=r->width/total;for(int k=1;k<total;k++){float off=half-k*lw;glColor3f(.88f,.90f,.88f);for(float q=0;q<len;q+=8){float q1=minf(q+4,len),t0=q/len,t1=q1/len;V3 a=vadd(vadd(s->a,vmul(d,t0)),vmul(right,off)),b=vadd(vadd(s->a,vmul(d,t1)),vmul(right,off));glVertex3f(a.x,.038f,a.z);glVertex3f(b.x,.038f,b.z);}}}
  }glEnd();g_draw_calls++;
}

static void draw_roads(void){
  if(!g_stream_ready08||g_stream_road_count08<=0){draw_roads_legacy07();return;}glBegin(GL_QUADS);glNormal3f(0,1,0);
  for(int i=0;i<g_stream_road_count08;i++){StreamRoad08*s=&g_stream_roads08[i];if(s->road<0||s->road>=g_world.roadCount)continue;V3 mid=vmul(vadd(s->a,s->b),.5f);float dx=mid.x-g_player.body.p.x,dz=mid.z-g_player.body.p.z;if(dx*dx+dz*dz>700*700)continue;V3 d=vsub(s->b,s->a);float l=vlen(d);if(l<.1f)continue;V3 r=v3(d.z/l,0,-d.x/l);float hw=g_world.roads[s->road].width*.5f;V3 a1=vadd(s->a,vmul(r,hw)),a2=vsub(s->a,vmul(r,hw)),b1=vadd(s->b,vmul(r,hw)),b2=vsub(s->b,vmul(r,hw));glColor3f(.11f,.12f,.13f);glTexCoord2f(0,0);glVertex3f(a1.x,.01f,a1.z);glTexCoord2f(1,0);glVertex3f(a2.x,.01f,a2.z);glTexCoord2f(1,l*.12f);glVertex3f(b2.x,.01f,b2.z);glTexCoord2f(0,l*.12f);glVertex3f(b1.x,.01f,b1.z);}glEnd();g_draw_calls++;draw_lane_markings08();
}

static void draw_buildings(void){
  if(!g_stream_ready08||g_stream_building_count08<=0){draw_buildings_legacy07();stream08_draw_assets();return;}
  for(int i=0;i<g_stream_building_count08;i++){Building*b=&g_stream_buildings08[i].b;float dx=b->c.x-g_player.body.p.x,dz=b->c.z-g_player.body.p.z,ds=dx*dx+dz*dz;if(ds>520*520)continue;float h=(float)((b->seed>>8)&255)/255.0f;float r=.38f+h*.25f,g=.38f+h*.20f,bl=.36f+h*.18f;if(b->h>30){r=.48f;g=.50f;bl=.52f;}draw_box(b->c,b->sx,b->h,b->sz,0,r,g,bl);}stream08_draw_assets();
}

static void draw_hud(float fps){
  draw_hud_legacy071(fps);if(g_settings_menu_open)return;settings_draw_2d_begin();hud_panel(20,20,500,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 0.8.0",.92f,.95f,1);if(g_debug){char b[220];hud_panel(20,(float)g_h-120,420,92);sprintf(b,"REGIONAL STREAM %s | regioes %d",g_stream_ready08?"ATIVO":"FALLBACK",g_stream_loaded_regions08);gl_text(34,(float)g_h-91,b,.58f,.88f,1);sprintf(b,"roads %d | buildings %d | assets %d",g_stream_road_count08,g_stream_building_count08,g_asset_instance_count08);gl_text(34,(float)g_h-68,b,.72f,.86f,.92f);sprintf(b,"asset pack %s (%d meshes) | contacts %d",g_asset_pack_ok08?"OK":"AUSENTE",g_asset_count08,g_stream_asset_contacts08);gl_text(34,(float)g_h-45,b,.72f,.86f,.92f);}settings_draw_2d_end();
}
