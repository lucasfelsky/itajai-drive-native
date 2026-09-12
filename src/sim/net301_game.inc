// 3.1 Multiplayer gameplay tail.
// Networking runs after the complete validated local simulation for this frame.
static void game_update(float dt){
  game_update_legacy260(dt);
  net301_update(dt);
}
