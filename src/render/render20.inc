// -----------------------------------------------------------------------------
// 0.20 adaptive LOD/performance presentation.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy19();}
static void draw_roads(void){draw_roads_legacy19();}
static void draw_buildings(void){draw_buildings_legacy19();}
static void draw_hud(float fps){lod20_sample_fps(fps);draw_hud_legacy19(fps);if(g_settings_menu_open)return;settings_draw_2d_begin();hud_panel(20,20,760,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 0.20.0 // ADAPTIVE LOD",.92f,.95f,1);if(g_debug){char b[220];hud_panel(20,58,360,54);sprintf(b,"LOD %d  mesh lists %d  props %.0fm  traffic %.0fm",g_lod20_tier,g_lod20_mesh_lists_built,g_lod20_prop_dist,g_lod20_traffic_dist);gl_text(34,82,b,.70f,.88f,.78f);sprintf(b,"smoothed FPS %.1f",g_lod20_fps);gl_text(34,102,b,.62f,.76f,.90f);}settings_draw_2d_end();}
