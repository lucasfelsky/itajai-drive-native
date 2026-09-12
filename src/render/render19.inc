// -----------------------------------------------------------------------------
// 0.19 landmark/editor presentation.
// -----------------------------------------------------------------------------
static void draw_ground(void){draw_ground_legacy18();}
static void draw_roads(void){draw_roads_legacy18();}
static void draw_buildings(void){draw_buildings_legacy18();editor19_draw();}
static void draw_hud(float fps){draw_hud_legacy18(fps);if(g_settings_menu_open)return;settings_draw_2d_begin();hud_panel(20,20,700,31);gl_text(34,43,"ITAJAI DRIVE // NATIVE ENGINE 0.19.0 // WORLD EDITOR",.92f,.95f,1);if(g_editor19){char b[220];hud_panel(20,(float)g_h-155,610,125);gl_text(38,(float)g_h-128,"WORLD EDITOR // F10 fechar",1,.82f,.28f);sprintf(b,"Tipo %d: %s | escala %.1f | yaw %.0f",g_editor_type19+1,g_landmark_asset_names19[g_editor_type19],g_editor_scale19,g_editor_yaw19*57.29578f);gl_text(38,(float)g_h-102,b,.9f,.92f,1);gl_text(38,(float)g_h-78,"1-8 tipo | ENTER colocar | DEL remover | [ ] girar | +/- escala",.74f,.82f,.9f);sprintf(b,"landmarks: %d (autosave itajai_landmarks_v19.bin)",g_landmark_count19);gl_text(38,(float)g_h-54,b,.62f,.78f,.65f);}settings_draw_2d_end();}
