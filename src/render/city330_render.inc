// 3.3 City Rebuild diagnostics. Kept out of the normal HUD; TAB/debug only.
static void city330_draw_overlay(void){
  if(!g_started||!g_debug||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;
  settings_draw_2d_begin();
  float x=20.0f,y=(float)g_h-132.0f,w=610.0f,h=72.0f;hud_panel(x,y,w,h);char a[220],b[220];
  if(g_city330_municipal_buildings>0)sprintf(a,"CITY 3.3 // GEOITAJAI EDIFICACOES %d // %s",g_city330_municipal_buildings,g_city330_municipal_live?"LIVE + CACHE":(g_city330_municipal_cached?"CACHE":"ATIVO"));
  else sprintf(a,"CITY 3.3 // EDIFICACOES OSM FALLBACK");
  sprintf(b,"paginas municipais %d | footprints exatos %d | vertices %d | OSM vias %d",g_city330_municipal_pages,g_building_polys12,g_building_vert_count12,g_world.roadCount);
  gl_text(x+15,y+27,a,g_city330_municipal_buildings>0?.52f:1.0f,g_city330_municipal_buildings>0?1.0f:.72f,.72f);
  gl_text(x+15,y+51,b,.62f,.72f,.82f);settings_draw_2d_end();
}
