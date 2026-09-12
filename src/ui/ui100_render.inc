// -----------------------------------------------------------------------------
// 1.0 final help/system overlay.
// -----------------------------------------------------------------------------
static void ui100_status_line(float x,float y,const char*name,int ok){char b[96];sprintf(b,"%-18s %s",name,ok?"OK":"...");gl_text(x,y,b,ok?.64f:.92f,ok?.92f:.72f,ok?.70f:.38f);}
static void ui100_draw_help(void){
  if(!g_help100||g_photo70_hide_hud)return;float w=610,h=350,x=28,y=(float)g_h-h-28;if(y<70)y=70;hud_panel(x,y,w,h);
  gl_text(x+20,y+28,"ITAJAI DRIVE 1.0 // CONTROLES",1,.94f,.76f);
  gl_text(x+20,y+55,"W/S          acelerar / frear / re",.86f,.90f,.94f);gl_text(x+20,y+77,"A/D          direcao",.86f,.90f,.94f);gl_text(x+20,y+99,"SPACE        freio de mao",.86f,.90f,.94f);gl_text(x+20,y+121,"C / E        camera / trocar carro",.86f,.90f,.94f);gl_text(x+20,y+143,"R / M        reset / GPS aleatorio",.86f,.90f,.94f);gl_text(x+20,y+165,"P / H        Photo Mode / esconder HUD",.86f,.90f,.94f);gl_text(x+20,y+187,"F2           proximo ponto do tour",.86f,.90f,.94f);gl_text(x+20,y+209,"F10          editor de mundo",.86f,.90f,.94f);gl_text(x+20,y+231,"TAB T Y      debug / horario / clima",.86f,.90f,.94f);gl_text(x+20,y+253,"F1 / ESC     ajuda / sair",.86f,.90f,.94f);
  gl_text(x+330,y+55,"SISTEMAS",.70f,.86f,1.0f);ui100_status_line(x+330,y+80,"OSM / estradas",g_world.roadCount>0);ui100_status_line(x+330,y+101,"grafo / A*",g_world.edgeCount>0);ui100_status_line(x+330,y+122,"colisoes",g_static_collider_count>0);ui100_status_line(x+330,y+143,"asset PAK",g_asset_pack_ok08);ui100_status_line(x+330,y+164,"renderer",g_glrc!=NULL);ui100_status_line(x+330,y+185,"audio",g_audio_started18);ui100_status_line(x+330,y+206,"editor",g_landmark_count19>0);ui100_status_line(x+330,y+227,"mundo vivo",g_life50_init);ui100_status_line(x+330,y+248,"streaming",g_stream_ready08);ui100_status_line(x+330,y+269,"footprints",g_building_polys12>0);
  char b[180];sprintf(b,"SELF-CHECK %d/10  |  TOUR: %s",g_selfcheck100,g_tour100_name);gl_text(x+20,y+305,b,.72f,.94f,.76f);gl_text(x+20,y+329,"F1 fecha esta tela. Dirija sem objetivo, ou use F2 para o tour.",.62f,.70f,.78f);
}
