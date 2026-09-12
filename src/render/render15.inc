// -----------------------------------------------------------------------------
// 0.15 suspension/drivetrain telemetry presentation.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy14();}
static void draw_roads(void){draw_roads_legacy14();}
static void draw_buildings(void){draw_buildings_legacy14();}
static void draw_hud(float fps){draw_hud_legacy14(fps);if(g_settings_menu_open)return;settings_draw_2d_begin();hud_panel(20,20,620,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 0.15.0 // WHEEL PHYSICS",.92f,.95f,1);if(g_debug){char b[240];hud_panel(835,(float)g_h-290,430,66);sprintf(b,"GEAR %d | RPM %.0f | ABS %.2f | TCS %.2f",g_gear15,g_engine_rpm15,g_abs15,g_tcs15);gl_text(850,(float)g_h-265,b,.70f,.92f,.72f);sprintf(b,"LOAD XFER %.2f | WHEEL LOAD %.0f/%.0f/%.0f/%.0f",g_load_transfer15,g_wheels15[0].normalLoad,g_wheels15[1].normalLoad,g_wheels15[2].normalLoad,g_wheels15[3].normalLoad);gl_text(850,(float)g_h-242,b,.66f,.82f,1);}settings_draw_2d_end();}
