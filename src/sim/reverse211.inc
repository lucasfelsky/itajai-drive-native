// -----------------------------------------------------------------------------
// 2.11 intuitive reverse steering.
// Keyboard left/right is treated as desired vehicle direction rather than raw
// steering-wheel angle while reversing. This keeps A meaning "reverse left" and
// D meaning "reverse right" from the driver's/game-camera perspective.
// -----------------------------------------------------------------------------
static int g_reverse_steer211=0;

static float reverse211_steer_input(float raw){
  int forward=key_down('W'),reverse=key_down('S');
  float speed=g_player.speed;
  if(speed<-.18f||(reverse&&!forward&&speed<.30f))g_reverse_steer211=1;
  else if(speed>.18f||forward||(!reverse&&absf(speed)<.08f))g_reverse_steer211=0;
  return g_reverse_steer211?-raw:raw;
}
