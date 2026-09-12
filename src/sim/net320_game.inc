// 3.2 Internet Multiplayer gameplay tail.
// Relay work runs after the complete 3.1 direct/LAN networking wrapper.
static void game_update(float dt){
  game_update_legacy301(dt);
  net320_update(dt);
}
