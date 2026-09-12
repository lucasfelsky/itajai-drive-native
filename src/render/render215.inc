// -----------------------------------------------------------------------------
// 2.15 Production UI: native garage/vehicle selector and cleaner product overlay.
// -----------------------------------------------------------------------------
static void ui215_draw_garage(void){
  if(!g_garage215_open)return;settings_draw_2d_begin();float w=690,h=330,x=(g_w-w)*.5f,y=(g_h-h)*.5f;hud_panel(x,y,w,h);
  gl_text(x+24,y+32,"GARAGEM // SELECAO DE VEICULO",1.0f,.91f,.70f);gl_text(x+24,y+56,"Setas/A-D escolhem  |  ENTER confirma  |  F3 fecha",.66f,.73f,.80f);
  for(int i=0;i<4;i++){float yy=y+92+i*45.0f;char b[260];const VehicleProfile*p=&g_vehicle_profiles[i];sprintf(b,"%c %-22s  %s  %4.0f kg  %3.0f km/h",i==g_garage215_selected?'>':' ',g_garage215_names[i],g_garage215_drive[i],p->mass,p->maxSpeed*3.6f);float hi=i==g_garage215_selected?1.0f:.72f;gl_text(x+30,yy,b,hi,i==g_garage215_selected?.88f:.76f,i==g_garage215_selected?.48f:.80f);}
  const VehicleProfile*p=&g_vehicle_profiles[g_garage215_selected&3];char d[300];sprintf(d,"Wheelbase %.2f m  |  track %.2f m  |  steer %.1f deg  |  grip %.2f",p->wheelbase,p->track,p->maxSteer*57.29578f,p->baseGrip);gl_text(x+24,y+h-52,d,.65f,.80f,.92f);gl_text(x+24,y+h-27,"A selecao troca tambem fisica, audio, cockpit e casco de colisao.",.58f,.66f,.72f);settings_draw_2d_end();
}
static void draw_ground(void){draw_ground_legacy214();}
static void draw_roads(void){draw_roads_legacy214();}
static void draw_buildings(void){draw_buildings_legacy214();}
static void draw_hud(float fps){draw_hud_legacy214(fps);if(!g_photo70_hide_hud){settings_draw_2d_begin();gl_text(g_w-220,28,"F3 GARAGEM",.62f,.72f,.82f);settings_draw_2d_end();}ui215_draw_garage();}
