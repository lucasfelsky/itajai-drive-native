// -----------------------------------------------------------------------------
// 1.2 Renderer 2.0 presentation layer.
// Adds a fallback-visible headlight pool on top of the shader headlight and
// exposes renderer/weather state in debug telemetry. Renderer 5.0 owns its HDR
// vehicle-light pass, so this compatibility pool is skipped only on that path.
// -----------------------------------------------------------------------------
static void render120_headlight_pool(void){
  if(g_renderer09_linear_output)return;float night=(g_hour>=6&&g_hour<=18)?0.0f:1.0f;float strength=clampf(night+g_wetness60*.35f,0,1);if(strength<.05f)return;
  V3 f=v3((float)sin((double)g_player.body.yaw),0,(float)cos((double)g_player.body.yaw)),r=v3(f.z,0,-f.x);V3 p=vadd(g_player.body.p,vmul(f,1.65f));float nearW=1.20f,farW=5.8f,len=28.0f;V3 a=vadd(p,vmul(r,-nearW)),b=vadd(p,vmul(r,nearW)),q=vadd(p,vmul(f,len)),c=vadd(q,vmul(r,farW)),d=vadd(q,vmul(r,-farW));a.y=b.y=.045f;c.y=d.y=.045f;
  renderer09_suspend_shader();glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);glDisable(GL_TEXTURE_2D);glBegin(GL_QUADS);glColor4f(1.0f,.88f,.68f,.11f*strength);glVertex3f(a.x,a.y,a.z);glVertex3f(b.x,b.y,b.z);glColor4f(1.0f,.78f,.48f,.008f);glVertex3f(c.x,c.y,c.z);glVertex3f(d.x,d.y,d.z);glEnd();glDisable(GL_BLEND);renderer09_resume_shader();g_draw_calls++;
}
static void draw_ground(void){draw_ground_legacy110();}
static void draw_roads(void){draw_roads_legacy110();render120_headlight_pool();}
static void draw_buildings(void){draw_buildings_legacy110();}
static void draw_hud(float fps){
  draw_hud_legacy110(fps);if(g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();hud_panel(20,20,1080,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 1.2.0 // RENDERER 2.0",1.0f,.94f,.76f);
  if(g_debug){char b[220];hud_panel(560,60,500,96);sprintf(b,"RENDER2: %s | wet %.2f | cloud %.2f",g_renderer09_shader?"GLSL PBR-COMPAT":"FIXED FALLBACK",g_wetness60,g_cloud60);gl_text(575,84,b,g_renderer09_shader?.62f:1.0f,g_renderer09_shader?.94f:.70f,.72f);sprintf(b,"headlights %s | night windows %s | water material 4",(g_hour<6||g_hour>18||g_wetness60>.2f)?"ON":"DIM",g_renderer09_shader?"ON":"fallback");gl_text(575,106,b,.72f,.82f,.94f);sprintf(b,"lightning %.2f | wind %.2f | fog weather %d",g_lightning60,g_wind60,g_weather);gl_text(575,128,b,.72f,.78f,.86f);}
  settings_draw_2d_end();
}
