// -----------------------------------------------------------------------------
// 2.11 driving camera behavior.
// Third-person chase cameras have a canonical rear position, yield to mouse-look,
// then return after mouse idle. Reverse automatically swings the camera around so
// the driver can see the direction of travel; after stopping it stays there for
// roughly five seconds, or returns immediately once forward travel resumes.
// -----------------------------------------------------------------------------
static float g_camera_reverse_hold211=0.0f;
static int g_camera_reverse_view211=0;
static int g_camera_auto_state211=0; // 0 rear chase, 1 reverse-facing

static void camera211_update(float dt){
  if(dt<=0||!g_started||g_settings_menu_open||g_editor19||g_photo70)return;
  g_camera_mouse_idle211=minf(999.0f,g_camera_mouse_idle211+dt);

  // Cockpit and overhead/free presentation remain fully manual.
  if(g_player.camera!=0&&g_player.camera!=1){g_camera_reverse_hold211=0;g_camera_reverse_view211=0;g_camera_auto_state211=0;return;}

  int w=key_down('W'),s=key_down('S');
  float v=g_player.speed,av=absf(v);
  int reversing=(v<-.35f)||(s&&!w&&v<.15f);
  int forwarding=(v>.35f)||(w&&!s);
  int stopped=av<.24f&&!w&&!s;

  if(reversing){
    g_camera_reverse_view211=1;
    g_camera_reverse_hold211=5.0f;
  }else if(forwarding){
    g_camera_reverse_view211=0;
    g_camera_reverse_hold211=0.0f;
  }else if(stopped&&g_camera_reverse_view211){
    g_camera_reverse_hold211=maxf(0.0f,g_camera_reverse_hold211-dt);
    if(g_camera_reverse_hold211<=0.0f)g_camera_reverse_view211=0;
  }

  // A short grace period keeps auto-follow from fighting active mouse-look.
  // Once the player stops moving the mouse, return to the canonical state.
  float idleNeeded=.90f;
  if(g_camera_mouse_idle211<idleNeeded)return;
  g_camera_mouse_override211=0;

  float wantedYaw=g_camera_reverse_view211?3.14159265f:0.0f;
  g_camera_auto_state211=g_camera_reverse_view211?1:0;
  g_camera_yaw_target=sim07_wrap_angle(wantedYaw);
  g_camera_pitch_target=0.0f;
}

static void game_update(float dt){
  game_update_legacy250(dt);
  camera211_update(dt);
}
