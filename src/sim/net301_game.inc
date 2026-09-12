// 3.1 Multiplayer gameplay tail.
// Networking runs after the complete validated local simulation for this frame.
// 3.2 declares the same internal-linkage flag later in the translation unit;
// this tentative declaration lets Internet relay mode suspend direct hotkeys/socket IO.
static int g_net320_active;
static void game_update(float dt){
  game_update_legacy260(dt);
  if(!g_net320_active)net301_update(dt);
}
