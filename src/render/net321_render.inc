// -----------------------------------------------------------------------------
// 3.2.1 Hamachi / virtual-LAN multiplayer UI.
// F12 opens the keyboard-first panel. The compact badge stays unobtrusive while
// driving and reports whether a Hamachi-style adapter was detected.
// -----------------------------------------------------------------------------
static void net321_draw_badge(void){
  if(!g_started||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud)||g_net321_menu_open)return;
  settings_draw_2d_begin();
  float w=330.0f,h=48.0f,x=(float)g_w-w-20.0f,y=(float)g_h-h-20.0f;hud_panel(x,y,w,h);
  char line[180];
  if(g_net301_mode!=NET301_MODE_OFF){sprintf(line,"MULTIPLAYER // %s",net321_direct_state());gl_text(x+14,y+20,line,.62f,1.0f,.72f);}
  else if(g_net321_hamachi_found){sprintf(line,"HAMACHI %s // F12 MULTIPLAYER",g_net321_hamachi_ip);gl_text(x+14,y+20,line,.66f,.88f,1.0f);}
  else gl_text(x+14,y+20,"MULTIPLAYER // F12",.66f,.74f,.84f);
  settings_draw_2d_end();
}

static void net321_draw_menu(void){
  if(!g_net321_menu_open)return;
  settings_draw_2d_begin();
  glColor4f(0,0,0,.70f);glBegin(GL_QUADS);glVertex2f(0,0);glVertex2f((float)g_w,0);glVertex2f((float)g_w,(float)g_h);glVertex2f(0,(float)g_h);glEnd();
  float w=620.0f,h=410.0f,x=((float)g_w-w)*.5f,y=((float)g_h-h)*.5f;hud_panel(x,y,w,h);
  gl_text(x+30,y+42,"MULTIPLAYER",1,1,1);
  gl_text(x+30,y+68,"LAN / HAMACHI",.56f,.82f,1.0f);
  char line[220];
  if(g_net321_hamachi_found){
    sprintf(line,"Hamachi detectado: %s%s",g_net321_hamachi_ip,g_net321_hamachi_probable?" (provavel)":"");
    gl_text(x+30,y+103,line,.58f,1.0f,.68f);
    if(g_net321_hamachi_adapter[0]){sprintf(line,"Adaptador: %.70s",g_net321_hamachi_adapter);gl_text(x+30,y+126,line,.58f,.66f,.76f);}
  }else{
    gl_text(x+30,y+103,"Hamachi nao detectado. Abra/conecte o Hamachi e aguarde alguns segundos.",1.0f,.58f,.42f);
    gl_text(x+30,y+126,"Direct IP continua disponivel mesmo sem Hamachi.",.62f,.68f,.76f);
  }

  const char*labels[3]={"CRIAR JOGO","ENTRAR EM JOGO","DESCONECTAR"};
  for(int i=0;i<3;i++){
    float yy=y+182.0f+i*62.0f;int sel=i==g_net321_selected;
    if(sel){glColor4f(.18f,.30f,.42f,.78f);glBegin(GL_QUADS);glVertex2f(x+24,yy-28);glVertex2f(x+w-24,yy-28);glVertex2f(x+w-24,yy+19);glVertex2f(x+24,yy+19);glEnd();}
    sprintf(line,"%s %s",sel?">":" ",labels[i]);gl_text(x+38,yy,line,sel?1.0f:.76f,sel?.92f:.80f,sel?.54f:.86f);
    if(i==0){sprintf(line,"Host UDP 27015  |  seu IP: %s",g_net321_hamachi_found?g_net321_hamachi_ip:"Hamachi nao detectado");gl_text(x+240,yy,line,.56f,.68f,.78f);}
    else if(i==1){sprintf(line,"IP do host: %s%s",g_net321_join_ip[0]?g_net321_join_ip:"digite o IPv4 Hamachi",sel?"_":"");gl_text(x+240,yy,line,.72f,.84f,1.0f);}
    else {sprintf(line,"Estado atual: %s",net321_direct_state());gl_text(x+240,yy,line,.56f,.68f,.78f);}
  }
  if(g_net321_status[0]){sprintf(line,"STATUS: %s",g_net321_status);gl_text(x+30,y+h-62,line,.72f,.92f,.72f);}
  gl_text(x+30,y+h-32,"SETAS/W/S navegar   digite o IP na opcao ENTRAR   ENTER confirmar   F12 fechar",.56f,.64f,.72f);
  settings_draw_2d_end();
}

static void net321_draw_overlay(void){
  net321_draw_badge();
  net321_draw_menu();
}
