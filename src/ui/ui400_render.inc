// -----------------------------------------------------------------------------
// 4.0 production driving UX.
// Compact route card + optional F2 Drive Hub. Debug telemetry remains separate.
// -----------------------------------------------------------------------------
static void ui400_route_card(void){
  if(!g_nav400_active||g_gps_n<=0)return;float w=500.0f,h=82.0f,x=((float)g_w-w)*.5f,y=18.0f;hud_panel(x,y,w,h);char a[180],b[180];float nd=g_nav400_next_distance;const char*unit=nd>=1000.0f?"km":"m";float value=nd>=1000.0f?nd/1000.0f:nd;if(nd>=1000)sprintf(a,"%s em %.1f %s",g_nav400_turn,value,unit);else sprintf(a,"%s em %.0f %s",g_nav400_turn,value,unit);sprintf(b,"%s   |   %.1f km restantes   |   ETA %.0f min",g_nav400_next_road,g_nav400_remaining/1000.0f,maxf(1.0f,g_nav400_eta/60.0f));gl_text(x+18,y+29,a,1.0f,.88f,.38f);gl_text(x+18,y+56,b,.72f,.84f,.92f);
}
static void ui400_drive_hub(void){
  if(!g_drivehub400_open)return;float w=450.0f,h=310.0f,x=24.0f,y=((float)g_h-h)*.5f;hud_panel(x,y,w,h);char b[220];gl_text(x+22,y+34,"DRIVE HUB",1.0f,.92f,.66f);gl_text(x+22,y+58,"F2 fechar",.56f,.66f,.76f);
  int road=-1;float center=0;foundation_nearest_road(g_player.body.p,&road,&center);const char*rn=(road>=0&&road<g_world.roadCount&&g_world.roads[road].name[0])?g_world.roads[road].name:"VIA SEM NOME";float limit=(road>=0&&road<g_world.roadCount)?g_world.roads[road].speed*3.6f:0;
  sprintf(b,"CARRO   %s",vehicle160_player_name());gl_text(x+22,y+94,b,.84f,.88f,.91f);sprintf(b,"LOCAL   %s",urban10_sector_name());gl_text(x+22,y+119,b,.76f,.84f,.88f);sprintf(b,"VIA     %s",rn);gl_text(x+22,y+144,b,.76f,.84f,.88f);if(limit>1){sprintf(b,"LIMITE  %.0f km/h",limit);gl_text(x+22,y+169,b,.76f,.84f,.88f);}
  const char*weather=g_weather==0?"LIMPO":g_weather==1?"NUBLADO":g_weather==2?"CHUVA":"TEMPESTADE";sprintf(b,"AMBIENTE   %02d:00  |  %s",g_hour,weather);gl_text(x+22,y+199,b,.70f,.79f,.86f);
  if(g_nav400_active){sprintf(b,"GPS   %.1f km  |  ETA %.0f min",g_nav400_remaining/1000.0f,maxf(1.0f,g_nav400_eta/60.0f));gl_text(x+22,y+229,b,.52f,.86f,1.0f);sprintf(b,"%s -> %s",g_nav400_turn,g_nav400_next_road);gl_text(x+22,y+253,b,.92f,.86f,.48f);}else gl_text(x+22,y+229,"GPS   sem destino // M abre o mapa",.58f,.72f,.82f);
  gl_text(x+22,y+h-22,"M mapa   F3 garagem   C camera   T horario   Y clima",.55f,.63f,.71f);
}
static void ui400_draw(void){
  if(!g_started||g_settings_menu_open||g_map380_open||g_garage215_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();ui400_route_card();ui400_drive_hub();if(!g_drivehub400_open)gl_text((float)g_w-92.0f,(float)g_h-68.0f,"F2 HUB",.46f,.56f,.66f);settings_draw_2d_end();
}
