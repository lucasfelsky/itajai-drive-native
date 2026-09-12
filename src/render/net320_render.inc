// -----------------------------------------------------------------------------
// 3.2 Internet Multiplayer HUD.
// Shows relay state, shareable room code and RTT without replacing the existing HUD.
// -----------------------------------------------------------------------------
static void draw_hud(float fps){
  draw_hud_legacy320(fps);
  if(!g_started||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;
  if(!g_net320_active&&!g_net320_relay_valid&&g_net320_state!=NET320_STATE_ERROR)return;
  settings_draw_2d_begin();
  float w=382.0f,h=g_net320_room[0]?118.0f:92.0f,x=(float)g_w-w-20.0f,y=58.0f;hud_panel(x,y,w,h);
  char line[180];sprintf(line,"INTERNET // %s",net320_state_name());gl_text(x+16,y+25,line,g_net320_state==NET320_STATE_CONNECTED?.45f:1.0f,g_net320_state==NET320_STATE_CONNECTED?1.0f:.80f,.72f);
  if(g_net320_room[0]){sprintf(line,"CODIGO DA SALA: %s",g_net320_room);gl_text(x+16,y+50,line,1,.94f,.44f);}
  if(g_net320_state==NET320_STATE_ERROR){sprintf(line,"%s",net320_error_name());gl_text(x+16,y+(g_net320_room[0]?75:50),line,1,.42f,.34f);}
  else{if(g_net320_ping_ms>=0)sprintf(line,"Relay %s:%d   Ping %.0f ms",g_net320_relay_host,g_net320_relay_port,g_net320_ping_ms);else sprintf(line,"Relay %s:%d",g_net320_relay_host,g_net320_relay_port);gl_text(x+16,y+(g_net320_room[0]?75:50),line,.65f,.76f,.86f);}
  gl_text(x+16,y+h-15,"F9 criar sala   F11 desconectar",.55f,.62f,.70f);
  settings_draw_2d_end();
}
