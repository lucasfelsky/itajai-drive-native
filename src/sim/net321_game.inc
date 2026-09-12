// 3.2.1 Hamachi / virtual-LAN menu gameplay tail.
static void game_update(float dt){
  game_update_legacy320(dt);
  net321_update(dt);
}
