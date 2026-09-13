// -----------------------------------------------------------------------------
// 0.9 world render overrides using the programmable compatibility renderer.
// -----------------------------------------------------------------------------
static void setup_camera(void){
  float aspect=(float)g_w/maxf(1,(float)g_h);glMatrixMode(GL_PROJECTION);glLoadIdentity();double near=.15,top=near*.57735,right=top*aspect;glFrustum(-right,right,-top,top,near,2200);glMatrixMode(GL_MODELVIEW);glLoadIdentity();V3 eye,target;sim07_camera_pose(g_player.camera,g_player.body.p,g_player.body.yaw,&eye,&target);g_renderer_camera_eye09=eye;V3 fw=vnorm(vsub(target,eye)),up=v3(0,1,0),side=vnorm(vcross(fw,up)),camUp=vcross(side,fw);float m[16]={side.x,camUp.x,-fw.x,0,side.y,camUp.y,-fw.y,0,side.z,camUp.z,-fw.z,0,-vdot(side,eye),-vdot(camUp,eye),vdot(fw,eye),1};glMultMatrixf(m);renderer09_begin_world(side,camUp,fw);
}
static void draw_ground(void){
  renderer09_set_material(1);float span=1800.0f,x=g_player.body.p.x,z=g_player.body.p.z;glColor3f(.78f,.88f,.76f);glNormal3f(0,1,0);glBegin(GL_QUADS);glTexCoord2f((x-span)*.045f,(z-span)*.045f);glVertex3f(x-span,-.055f,z-span);glTexCoord2f((x+span)*.045f,(z-span)*.045f);glVertex3f(x+span,-.055f,z-span);glTexCoord2f((x+span)*.045f,(z+span)*.045f);glVertex3f(x+span,-.055f,z+span);glTexCoord2f((x-span)*.045f,(z+span)*.045f);glVertex3f(x-span,-.055f,z+span);glEnd();renderer09_set_material(0);g_draw_calls++;
}
static void draw_roads(void){
  if(!g_stream_ready08||g_stream_road_count08<=0){renderer09_set_material(2);draw_roads_legacy07();renderer09_set_material(0);return;}renderer09_set_material(2);glBegin(GL_QUADS);glNormal3f(0,1,0);
  for(int i=0;i<g_stream_road_count08;i++){StreamRoad08*s=&g_stream_roads08[i];if(s->road<0||s->road>=g_world.roadCount)continue;V3 mid=vmul(vadd(s->a,s->b),.5f);float dx=mid.x-g_player.body.p.x,dz=mid.z-g_player.body.p.z;if(dx*dx+dz*dz>700*700)continue;V3 d=vsub(s->b,s->a);float l=vlen(d);if(l<.1f)continue;V3 r=v3(d.z/l,0,-d.x/l);float hw=g_world.roads[s->road].width*.5f;V3 a1=vadd(s->a,vmul(r,hw)),a2=vsub(s->a,vmul(r,hw)),b1=vadd(s->b,vmul(r,hw)),b2=vsub(s->b,vmul(r,hw));glColor3f(.78f,.80f,.82f);glTexCoord2f(0,0);glVertex3f(a1.x,.01f,a1.z);glTexCoord2f(1,0);glVertex3f(a2.x,.01f,a2.z);glTexCoord2f(1,l*.12f);glVertex3f(b2.x,.01f,b2.z);glTexCoord2f(0,l*.12f);glVertex3f(b1.x,.01f,b1.z);}glEnd();g_draw_calls++;renderer09_set_material(0);draw_lane_markings08();
}
static void draw_buildings(void){
  if(!g_stream_ready08||g_stream_building_count08<=0){renderer09_set_material(3);draw_buildings_legacy07();renderer09_set_material(0);stream08_draw_assets();return;}renderer09_set_material(3);
  for(int i=0;i<g_stream_building_count08;i++){Building*b=&g_stream_buildings08[i].b;if(landmark506_replaces(b))continue;float dx=b->c.x-g_player.body.p.x,dz=b->c.z-g_player.body.p.z,ds=dx*dx+dz*dz;if(ds>520*520)continue;float h=(float)((b->seed>>8)&255)/255.0f;float r=.72f+h*.20f,g=.72f+h*.16f,bl=.70f+h*.14f;if(b->h>30){r=.78f;g=.80f;bl=.84f;}draw_box(b->c,b->sx,b->h,b->sz,0,r,g,bl);}renderer09_set_material(0);stream08_draw_assets();
}
static void draw_hud(float fps){
  renderer09_end_world();draw_hud_legacy08(fps);if(g_settings_menu_open)return;settings_draw_2d_begin();hud_panel(20,20,500,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 0.9.0",.92f,.95f,1);if(g_debug){char b[220];hud_panel(460,(float)g_h-92,360,64);sprintf(b,"RENDERER: %s",g_renderer09_shader?"GLSL 1.20 PROGRAMAVEL":"FIXED-FUNCTION FALLBACK");gl_text(476,(float)g_h-65,b,g_renderer09_shader?.54f:1.0f,g_renderer09_shader?1.0f:.70f,.62f);sprintf(b,"materiais: grass/asphalt/building | chuva %d",g_weather);gl_text(476,(float)g_h-42,b,.68f,.76f,.84f);}settings_draw_2d_end();
}
