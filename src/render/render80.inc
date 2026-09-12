// -----------------------------------------------------------------------------
// 0.80 polish + performance presentation.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy70();}
static void draw_roads(void){draw_roads_legacy70();}
static void draw_buildings(void){draw_buildings_legacy70();}
static void draw_hud(float fps){draw_hud_legacy70(fps);if(g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();hud_panel(20,20,930,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 0.80.0 // PERFORMANCE POLISH",.92f,.95f,1);if(g_debug){char b[240];hud_panel(20,214,470,72);sprintf(b,"frame budget %.0f%%  quality %d/2",g_perf80_budget*100.0f,g_perf80_quality);gl_text(34,238,b,.72f,.90f,.76f);sprintf(b,"buildings exact %d  LOD %d  exact range %.0fm",g_perf80_exact_buildings,g_perf80_lod_buildings,g_perf80_exact_building_range);gl_text(34,258,b,.72f,.86f,.92f);sprintf(b,"decor stride pedestrians %d  parked %d",g_perf80_ped_stride,g_perf80_parked_stride);gl_text(34,278,b,.70f,.82f,.88f);}settings_draw_2d_end();}
