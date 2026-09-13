// -----------------------------------------------------------------------------
// 0.9 atmosphere, rain and inexpensive projected shadows.
// -----------------------------------------------------------------------------
static void renderer09_draw_sky(void){
  glDisable(GL_DEPTH_TEST);glDisable(GL_TEXTURE_2D);if(g_renderer09_shader)p09UseProgram(0);glMatrixMode(GL_PROJECTION);glPushMatrix();glLoadIdentity();glOrtho(0,g_w,g_h,0,-1,1);glMatrixMode(GL_MODELVIEW);glPushMatrix();glLoadIdentity();
  float day=(g_hour>=6&&g_hour<=18)?1.0f:.12f,storm=g_weather>=2?.68f:(g_weather==1?.85f:1.0f);float tr=.12f*day*storm,tg=.30f*day*storm,tb=.52f*day*storm,br=.38f*day*storm,bg=.56f*day*storm,bb=.68f*day*storm;if(g_hour<6||g_hour>18){tr=.015f;tg=.025f;tb=.065f;br=.035f;bg=.055f;bb=.11f;}
  glBegin(GL_QUADS);glColor3f(tr,tg,tb);glVertex2f(0,0);glVertex2f((float)g_w,0);glColor3f(br,bg,bb);glVertex2f((float)g_w,(float)g_h);glVertex2f(0,(float)g_h);glEnd();
  if(g_weather<2&&g_hour>=6&&g_hour<=18){float t=((float)g_hour-6)/12.0f;float sx=80+t*(g_w-160),sy=180-(float)sin((double)(t*3.14159265f))*110;glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);glColor4f(1.0f,.82f,.42f,.82f);glBegin(GL_QUADS);glVertex2f(sx-14,sy-14);glVertex2f(sx+14,sy-14);glVertex2f(sx+14,sy+14);glVertex2f(sx-14,sy+14);glEnd();glDisable(GL_BLEND);}
  glPopMatrix();glMatrixMode(GL_PROJECTION);glPopMatrix();glMatrixMode(GL_MODELVIEW);glEnable(GL_DEPTH_TEST);
}
static void renderer09_shadow_quad(float x,float z,float sx,float sz,float ox,float oz,float a){glColor4f(0,0,0,a);glVertex3f(x-sx*.5f+ox,.045f,z-sz*.5f+oz);glVertex3f(x+sx*.5f+ox,.045f,z-sz*.5f+oz);glVertex3f(x+sx*.5f+ox,.045f,z+sz*.5f+oz);glVertex3f(x-sx*.5f+ox,.045f,z+sz*.5f+oz);}
static void renderer09_draw_shadows(void){
  renderer09_suspend_shader();glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);glBegin(GL_QUADS);float ox=(g_hour<12?1.8f:-1.8f),oz=1.25f;renderer09_shadow_quad(g_player.body.p.x,g_player.body.p.z,2.1f,4.4f,ox*.35f,oz*.35f,.28f);
  for(int i=0;i<g_traffic_count;i++){Traffic*t=&g_traffic[i];if(!t->active)continue;float dx=t->p.x-g_player.body.p.x,dz=t->p.z-g_player.body.p.z;if(dx*dx+dz*dz<180*180)renderer09_shadow_quad(t->p.x,t->p.z,1.9f,4.1f,ox*.3f,oz*.3f,.20f);}
  if(g_stream_ready08)for(int i=0;i<g_stream_building_count08;i++){Building*b=&g_stream_buildings08[i].b;if(landmark506_replaces(b))continue;float dx=b->c.x-g_player.body.p.x,dz=b->c.z-g_player.body.p.z;if(dx*dx+dz*dz>210*210)continue;float reach=clampf(b->h*.10f,1.2f,8.0f);renderer09_shadow_quad(b->c.x,b->c.z,b->sx,b->sz,ox*reach*.18f,oz*reach*.18f,.11f);}
  for(int i=0;i<g_asset_instance_count08;i++){AssetInstance08*x=&g_asset_instances08[i];float dx=x->p.x-g_player.body.p.x,dz=x->p.z-g_player.body.p.z;if(dx*dx+dz*dz<140*140){float rr=(x->asset>=0&&x->asset<g_asset_count08)?g_assets08[x->asset].colliderRadius*x->scale:.5f;renderer09_shadow_quad(x->p.x,x->p.z,maxf(.5f,rr*1.4f),maxf(.5f,rr*1.4f),ox*.25f,oz*.25f,.16f);}}
  glEnd();glDisable(GL_BLEND);renderer09_resume_shader();g_draw_calls++;
}
static void renderer09_draw_rain(void){
  if(g_weather<2)return;renderer09_set_material(0);glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);glColor4f(.62f,.78f,1.0f,g_weather==2?.42f:.60f);glLineWidth(g_weather==2?1.0f:1.35f);glBegin(GL_LINES);int drops=g_weather==2?110:190;
  for(int i=0;i<drops;i++){u32 h=renderer09_hash((u32)i,0x55aau,0x91e1u);float fx=((h&1023)/1023.0f-.5f)*150.0f,fz=(((h>>10)&1023)/1023.0f-.5f)*150.0f;float cyc=(float)g_time*(g_weather==2?1.55f:2.1f)+((h>>20)&1023)/1023.0f;int whole=(int)cyc;float frac=cyc-whole;float y=1.0f+(1.0f-frac)*24.0f;float x=g_player.body.p.x+fx,z=g_player.body.p.z+fz;glVertex3f(x,y,z);glVertex3f(x-.16f,y-.95f,z+.10f);}glEnd();glDisable(GL_BLEND);g_draw_calls++;
}
