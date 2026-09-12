// -----------------------------------------------------------------------------
// 1.7 Graphics Uplift: inexpensive atmosphere, persistent wet-road response,
// local urban lighting and headlight beams.  All effects keep fixed-function
// compatibility and do not require an FBO/deferred renderer.
// -----------------------------------------------------------------------------
static int g_graphics170_lamps=0,g_graphics170_pools=0,g_graphics170_beams=0,g_graphics170_puddles=0;
static int g_graphics170_lamp_asset=-2;

static float graphics170_night(void){if(g_hour>=19||g_hour<5)return 1.0f;if(g_hour==18||g_hour==5)return .58f;return 0.0f;}
static void graphics170_resolve(void){if(g_graphics170_lamp_asset==-2)g_graphics170_lamp_asset=asset08_find("street_lamp");}

static void graphics170_sky_overlay(void){
  float night=graphics170_night(),dusk=(g_hour==17||g_hour==18||g_hour==5||g_hour==6)?.65f:0.0f;if(night<=0&&dusk<=0)return;
  renderer09_suspend_shader();glDisable(GL_DEPTH_TEST);glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);glMatrixMode(GL_PROJECTION);glPushMatrix();glLoadIdentity();glOrtho(0,g_w,g_h,0,-1,1);glMatrixMode(GL_MODELVIEW);glPushMatrix();glLoadIdentity();
  if(dusk>0){glBegin(GL_QUADS);glColor4f(1.0f,.35f,.10f,.12f*dusk);glVertex2f(0,g_h*.56f);glVertex2f(g_w,g_h*.56f);glColor4f(.35f,.18f,.30f,0);glVertex2f(g_w,g_h*.18f);glVertex2f(0,g_h*.18f);glEnd();}
  if(night>0){
    // Stable star field; tiny quads avoid relying on point-size support.
    glBegin(GL_QUADS);for(int i=0;i<72;i++){u32 h=renderer09_hash((u32)i,0x170u,0x51a7u);float x=12.0f+(h&2047)/2047.0f*(g_w-24.0f),y=8.0f+((h>>11)&1023)/1023.0f*(g_h*.48f);float s=1.0f+((h>>22)&3)*.42f,a=.16f+.34f*((h>>24)&7)/7.0f;glColor4f(.76f,.84f,1.0f,a*night);glVertex2f(x-s,y-s);glVertex2f(x+s,y-s);glVertex2f(x+s,y+s);glVertex2f(x-s,y+s);}glEnd();
    float mx=g_w*.79f,my=g_h*.16f,mr=17.0f;glColor4f(.86f,.90f,1.0f,.72f*night);glBegin(GL_QUADS);glVertex2f(mx-mr,my-mr);glVertex2f(mx+mr,my-mr);glVertex2f(mx+mr,my+mr);glVertex2f(mx-mr,my+mr);glEnd();glColor4f(.08f,.10f,.17f,.82f);glBegin(GL_QUADS);glVertex2f(mx-3,my-mr-2);glVertex2f(mx+mr+7,my-mr-2);glVertex2f(mx+mr+7,my+mr-2);glVertex2f(mx-3,my+mr-2);glEnd();
  }
  glPopMatrix();glMatrixMode(GL_PROJECTION);glPopMatrix();glMatrixMode(GL_MODELVIEW);glDisable(GL_BLEND);glEnable(GL_DEPTH_TEST);renderer09_resume_shader();
}

static void graphics170_pool(float x,float z,float radius,float r,float g,float b,float alpha){
  float y=terrain11_height(x,z)+.075f;const int n=14;glBegin(GL_TRIANGLES);for(int k=0;k<n;k++){float a=(float)k*6.2831853f/n,c=(float)(k+1)*6.2831853f/n;glColor4f(r,g,b,alpha);glVertex3f(x,y,z);glColor4f(r,g,b,0);glVertex3f(x+(float)cos((double)a)*radius,y,z+(float)sin((double)a)*radius);glVertex3f(x+(float)cos((double)c)*radius,y,z+(float)sin((double)c)*radius);}glEnd();g_graphics170_pools++;
}

static void graphics170_beam(V3 p,float yaw,float length,float alpha,float warm){
  V3 f=v3((float)sin((double)yaw),0,(float)cos((double)yaw)),rt=v3(f.z,0,-f.x);V3 a=vadd(p,vmul(f,1.25f)),b=vadd(p,vmul(f,length));float ya=roads130_height(a.x,a.z)+.055f,yb=roads130_height(b.x,b.z)+.055f;float nearW=.52f,farW=2.35f+length*.035f;V3 a0=vsub(a,vmul(rt,nearW)),a1=vadd(a,vmul(rt,nearW)),b0=vsub(b,vmul(rt,farW)),b1=vadd(b,vmul(rt,farW));glBegin(GL_QUADS);glColor4f(1.0f,warm?.78f:.88f,warm?.52f:.72f,alpha);glVertex3f(a0.x,ya,a0.z);glVertex3f(a1.x,ya,a1.z);glColor4f(.72f,.82f,1.0f,0);glVertex3f(b1.x,yb,b1.z);glVertex3f(b0.x,yb,b0.z);glEnd();g_graphics170_beams++;
}

static void graphics170_road_light_pass(void){
  float wet=clampf(g_wetness60,0,1),night=graphics170_night();if(wet<=.02f&&night<=.02f)return;graphics170_resolve();g_graphics170_lamps=0;g_graphics170_pools=0;g_graphics170_beams=0;g_graphics170_puddles=0;
  renderer09_suspend_shader();glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  if(wet>.02f&&g_stream_ready08){
    glBegin(GL_QUADS);for(int i=0;i<g_stream_road_count08;i++){StreamRoad08*s=&g_stream_roads08[i];if(s->road<0||s->road>=g_world.roadCount)continue;V3 m=vmul(vadd(s->a,s->b),.5f);float dx=m.x-g_player.body.p.x,dz=m.z-g_player.body.p.z;if(dx*dx+dz*dz>210*210)continue;V3 d=vsub(s->b,s->a);float len=vlen(d);if(len<6)continue;V3 rt=v3(d.z/len,0,-d.x/len);u32 h=roads130_hash(s);float off=(((h>>4)&255)/255.0f-.5f)*minf(2.4f,g_world.roads[s->road].width*.26f),hw=.16f+.34f*((h>>12)&255)/255.0f;V3 a0=vadd(s->a,vmul(rt,off-hw)),a1=vadd(s->a,vmul(rt,off+hw)),b0=vadd(s->b,vmul(rt,off-hw)),b1=vadd(s->b,vmul(rt,off+hw));float ya=roads130_height(s->a.x,s->a.z)+.041f,yb=roads130_height(s->b.x,s->b.z)+.041f;float al=(.022f+.050f*night)*wet;glColor4f(.42f+.30f*night,.55f+.18f*night,.68f+.14f*night,al);glVertex3f(a0.x,ya,a0.z);glVertex3f(a1.x,ya,a1.z);glVertex3f(b1.x,yb,b1.z);glVertex3f(b0.x,yb,b0.z);
      if((h%9u)==2u&&len>15){float t=.25f+((h>>20)&255)/255.0f*.50f;V3 q=vadd(s->a,vmul(d,t));float py=roads130_height(q.x,q.z)+.046f,pw=.50f+((h>>25)&7)*.10f,pl=1.1f+((h>>28)&3)*.32f;V3 q0=vadd(vadd(q,vmul(rt,-pw)),vmul(vnorm(d),-pl)),q1=vadd(vadd(q,vmul(rt,pw)),vmul(vnorm(d),-pl)),q2=vadd(vadd(q,vmul(rt,pw)),vmul(vnorm(d),pl)),q3=vadd(vadd(q,vmul(rt,-pw)),vmul(vnorm(d),pl));glColor4f(.18f,.25f,.31f,.10f*wet);glVertex3f(q0.x,py,q0.z);glVertex3f(q1.x,py,q1.z);glVertex3f(q2.x,py,q2.z);glVertex3f(q3.x,py,q3.z);g_graphics170_puddles++;}}
    glEnd();
  }
  if(night>.02f){
    for(int i=0;i<g_asset_instance_count08;i++){AssetInstance08*x=&g_asset_instances08[i];if(x->asset!=g_graphics170_lamp_asset)continue;float dx=x->p.x-g_player.body.p.x,dz=x->p.z-g_player.body.p.z;if(dx*dx+dz*dz>180*180)continue;graphics170_pool(x->p.x,x->p.z,5.8f*x->scale,1.0f,.66f,.28f,.095f*night*(.75f+.35f*wet));g_graphics170_lamps++;}
    if(g_stream_ready08)for(int i=0;i<g_stream_building_count08;i++){Building*b=&g_stream_buildings08[i].b;if(urban10_building_class(b)!=BLD10_COMMERCIAL||(b->seed&3u)!=1u)continue;float dx=b->c.x-g_player.body.p.x,dz=b->c.z-g_player.body.p.z;if(dx*dx+dz*dz>145*145)continue;graphics170_pool(b->c.x,b->c.z+b->sz*.5f+1.8f,3.6f,1.0f,.48f,.18f,.055f*night);}
    graphics170_beam(g_player.body.p,g_player.body.yaw,27.0f,.105f*night,1);for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active)continue;float dx=t->p.x-g_player.body.p.x,dz=t->p.z-g_player.body.p.z;if(dx*dx+dz*dz>85*85)continue;graphics170_beam(t->p,t->yaw,14.0f,.035f*night,1);if(g_traffic_brake_amount[i]>.12f){V3 f=v3((float)sin((double)t->yaw),0,(float)cos((double)t->yaw));V3 rear=vsub(t->p,vmul(f,2.0f));graphics170_pool(rear.x,rear.z,1.5f,1.0f,.03f,.02f,.055f*night);}}
  }
  glDisable(GL_BLEND);renderer09_resume_shader();g_draw_calls++;
}

static void graphics170_vegetation_volume(void){
  static int tree=-2,palm=-2;if(tree==-2){tree=asset08_find("urban_tree");palm=asset08_find("coastal_palm");}if(tree<0&&palm<0)return;renderer09_set_material(0);
  for(int i=0;i<g_asset_instance_count08;i++){AssetInstance08*x=&g_asset_instances08[i];if(x->asset!=tree&&x->asset!=palm)continue;float dx=x->p.x-g_player.body.p.x,dz=x->p.z-g_player.body.p.z;if(dx*dx+dz*dz>95*95)continue;u32 h=x->seed;float base=terrain11_height(x->p.x,x->p.z),isPalm=x->asset==palm?1.0f:0.0f;float y=base+(isPalm?5.1f:3.25f)*x->scale;float spread=(isPalm?1.8f:1.25f)*x->scale;for(int k=0;k<3;k++){float a=(h>>(k*5))&31,ang=a/31.0f*6.2831853f;float ox=(float)cos((double)ang)*spread*.45f,oz=(float)sin((double)ang)*spread*.45f;draw_box(v3(x->p.x+ox,y+(k-1)*.18f,x->p.z+oz),spread*(isPalm?1.25f:1.0f),.38f+spread*.28f,spread*(isPalm?.48f:.92f),ang,isPalm?.18f:.16f,isPalm?.42f:.38f,isPalm?.16f:.14f);}}
}
