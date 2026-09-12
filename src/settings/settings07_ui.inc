// -----------------------------------------------------------------------------
// 0.7.1 settings UI overlay. Keyboard-first and intentionally engine-native.
// -----------------------------------------------------------------------------
static void settings_draw_2d_begin(void){glMatrixMode(GL_PROJECTION);glPushMatrix();glLoadIdentity();glOrtho(0,g_w,g_h,0,-1,1);glMatrixMode(GL_MODELVIEW);glPushMatrix();glLoadIdentity();glDisable(GL_DEPTH_TEST);glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);}
static void settings_draw_2d_end(void){glDisable(GL_BLEND);glEnable(GL_DEPTH_TEST);glPopMatrix();glMatrixMode(GL_PROJECTION);glPopMatrix();glMatrixMode(GL_MODELVIEW);}
static void settings_draw_menu(void){
  if(!g_settings_menu_open)return;settings_draw_2d_begin();
  glColor4f(0,0,0,.66f);glBegin(GL_QUADS);glVertex2f(0,0);glVertex2f((float)g_w,0);glVertex2f((float)g_w,(float)g_h);glVertex2f(0,(float)g_h);glEnd();
  float w=520,h=330,x=(g_w-w)*.5f,y=(g_h-h)*.5f;hud_panel(x,y,w,h);gl_text(x+30,y+42,"CONFIGURACOES",1,1,1);gl_text(x+30,y+66,"CAMERA / CONTROLES",.58f,.78f,1);
  char buf[180];for(int i=0;i<5;i++){float yy=y+112+i*42;float rr=i==g_settings_selected?1.0f:.74f,gg=i==g_settings_selected?.88f:.78f,bb=i==g_settings_selected?.38f:.84f;const char*prefix=i==g_settings_selected?"> ":"  ";if(i==0)sprintf(buf,"%sSensibilidade da camera     %d%%",prefix,settings_sensitivity_percent());else if(i==1)sprintf(buf,"%sInverter eixo Y             %s",prefix,g_camera_invert_y?"SIM":"NAO");else if(i==2)sprintf(buf,"%sRestaurar padrao",prefix);else if(i==3)sprintf(buf,"%sVoltar ao jogo",prefix);else sprintf(buf,"%sSair do jogo",prefix);gl_text(x+42,yy,buf,rr,gg,bb);}
  gl_text(x+30,y+h-30,"W/S ou setas: navegar   A/D ou setas: ajustar   ENTER: selecionar",.58f,.64f,.72f);settings_draw_2d_end();
}
static void draw_hud(float fps){
  draw_hud_legacy07(fps);
  settings_draw_2d_begin();hud_panel(20,20,500,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 0.7.1",.92f,.95f,1);settings_draw_2d_end();
  settings_draw_menu();
}
