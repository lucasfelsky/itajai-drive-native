// -----------------------------------------------------------------------------
// 3.0.1 intuitive reverse steering hotfix.
// Keyboard left/right is treated as desired vehicle direction while reversing.
// The signed-speed bicycle model already reverses yaw naturally, so this wrapper
// only flips the requested steering direction once and reduces reverse authority
// slightly to keep parking/reversing predictable instead of twitchy.
// -----------------------------------------------------------------------------
static int g_reverse_steer211=0;
static float g_reverse_steer_scale301=1.0f;

static float reverse211_steer_input(float raw){
  int forward=key_down('W'),reverse=key_down('S');
  float speed=g_player.speed;
  // Hysteresis prevents the sign from flapping around zero while changing gear.
  if(speed<-.22f||(reverse&&!forward&&speed<.18f))g_reverse_steer211=1;
  else if(speed>.28f||forward||(!reverse&&absf(speed)<.06f))g_reverse_steer211=0;
  g_reverse_steer_scale301=g_reverse_steer211?.72f:1.0f;
  return g_reverse_steer211?(-raw*g_reverse_steer_scale301):raw;
}
