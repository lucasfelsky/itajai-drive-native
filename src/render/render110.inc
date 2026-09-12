// -----------------------------------------------------------------------------
// 1.1 presentation: speed-sensitive FOV and handling telemetry.
// Projection is updated at the start of world drawing, after the legacy camera
// has already established its model-view matrix.
// -----------------------------------------------------------------------------
static void sim110_apply_projection(void){
  float aspect=(float)g_w/maxf(1,(float)g_h);double near=.15;double half=(double)g_fov110*.5*3.141592653589793/180.0;double cs=cos(half),sn=sin(half);if(cs<.05)cs=.05;double top=near*(sn/cs),right=top*aspect;
  glMatrixMode(GL_PROJECTION);glLoadIdentity();glFrustum(-right,right,-top,top,near,2200);glMatrixMode(GL_MODELVIEW);
}
static void draw_ground(void){sim110_apply_projection();draw_ground_legacy100();}
static void draw_roads(void){draw_roads_legacy100();}
static void draw_buildings(void){draw_buildings_legacy100();}
static void draw_hud(float fps){
  draw_hud_legacy100(fps);if(g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();
  hud_panel(20,20,1040,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 1.1.0 // DRIVING FEEL 2.0",1.0f,.94f,.76f);
  if(g_debug){char b[220];hud_panel(20,60,520,96);const DriveTune110*t=&g_drive_tune110[g_player.car];sprintf(b,"DRIVE %s | FOV %.1f | ENGINE BRAKE %.2f",sim110_drive_name(t->drive),g_fov110,g_engine_brake110);gl_text(34,84,b,.70f,.92f,.76f);sprintf(b,"AX %.2f AY %.2f | LOAD F/R %.0f / %.0f N",g_ax110,g_ay110,g_front_load110,g_rear_load110);gl_text(34,106,b,.68f,.80f,.92f);sprintf(b,"SLIP F/R %.3f / %.3f | UNDER %.2f OVER %.2f",g_front_slip110,g_rear_slip110,g_understeer110,g_oversteer110);gl_text(34,128,b,.82f,.74f,.92f);}
  settings_draw_2d_end();
}
