// -----------------------------------------------------------------------------
// 5.0 simcade handling finish.
// Adds car-specific forward grip/weather response and high-speed yaw stability
// after the established physics stack. Reverse remains untouched (3.0.1 contract).
// -----------------------------------------------------------------------------
static const float g_grip500[4]={1.12f,1.05f,1.08f,.94f};
static const float g_yaw500[4]={1.02f,1.00f,.98f,.88f};
static int g_handling500_tcs=0;
static void handling500_post(float dt){if(dt<=0||!g_started||g_settings_menu_open||g_map380_open||g_garage215_open||g_photo70)return;float v=g_player.speed;if(v<=1.1f)return;V3 f=v3((float)sin((double)g_player.body.yaw),0,(float)cos((double)g_player.body.yaw)),r=v3(f.z,0,-f.x);float longitudinal=vdot(g_player.body.vel,f),lateral=vdot(g_player.body.vel,r);if(longitudinal<=1.0f)return;int car=g_player.car&3;float wet=clampf(g_wetness60,0,1),surface=g_player.body.surface==SURFACE_GRASS?.58f:g_player.body.surface==SURFACE_CURB?.82f:1.0f;float grip=g_grip500[car]*(1.0f-wet*.24f)*surface;int hand=key_down(VK_SPACE);float latRate=(hand?.42f:1.45f)*grip*(1.0f+clampf(longitudinal/32.0f,0,1)*.34f);lateral*=maxf(0.0f,1.0f-dt*latRate);float maxYaw=(.98f-clampf(longitudinal/46.0f,0,1)*.38f)*g_yaw500[car];if(hand)maxYaw*=1.48f;g_player.body.angularVel=clampf(g_player.body.angularVel,-maxYaw,maxYaw);if(wet>.45f&&absf(g_player.slip)>.35f&&!hand&&longitudinal>14.0f){longitudinal*=1.0f-dt*.18f;g_handling500_tcs++;}g_player.body.vel=vadd(vmul(f,longitudinal),vmul(r,lateral));g_player.speed=longitudinal;g_player.lateralSpeed=lateral;g_player.slip=clampf((float)atan2((double)lateral,(double)maxf(1.0f,longitudinal)),-1,1);}
static void game_update(float dt){game_update_legacy500(dt);handling500_post(dt);}
