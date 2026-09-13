// -----------------------------------------------------------------------------
// Renderer 5.0 debug navigation.
// TAB keeps the historical show/hide behavior. SHIFT+TAB cycles pages without
// toggling the overlay off, so telemetry can stay readable at any resolution.
// -----------------------------------------------------------------------------
static int g_window_active500=1;
static int g_debug500_page=0;
static int g_debug500_shift_tab_prev=0;
static void game_update(float dt){
  int debug_before=g_debug;
  game_update_legacy_events500(dt);
  int shift_tab=key_down(VK_TAB)&&key_down(0x10);
  if(shift_tab&&!g_debug500_shift_tab_prev){
    // The historical input layer already sees TAB. Undo only that toggle when
    // SHIFT is held, then advance the Renderer 5.0 page instead.
    if(g_debug!=debug_before)g_debug=debug_before;
    if(!g_debug)g_debug=1;
    g_debug500_page=(g_debug500_page+1)%3;
  }
  g_debug500_shift_tab_prev=shift_tab;
}
