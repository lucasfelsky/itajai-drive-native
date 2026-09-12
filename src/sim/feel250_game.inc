// -----------------------------------------------------------------------------
// 2.50 Game Feel & Polish.
// Lightweight persistent skid traces and wet/loose-surface spray telemetry.
// -----------------------------------------------------------------------------
#define FEEL250_MARKS 192
typedef struct { V3 p; float yaw,life,width; } SkidMark250;
static SkidMark250 g_skid250[FEEL250_MARKS];static int g_skid250_head=0,g_skid250_live=0;
static float g_skid250_timer=0,g_spray250=0,g_impact250=0;static int g_feel250_prev_contacts=0;
static void feel250_add_mark(V3 p,float yaw,float width){SkidMark250*m=&g_skid250[g_skid250_head++%FEEL250_MARKS];m->p=p;m->p.y=terrain11_height(p.x,p.z)+.045f;m->yaw=yaw;m->life=13.0f;m->width=width;}
static void feel250_update(float dt){
  int live=0;for(int i=0;i<FEEL250_MARKS;i++){if(g_skid250[i].life>0){g_skid250[i].life-=dt;if(g_skid250[i].life>0)live++;}}g_skid250_live=live;
  float speed=absf(g_player.speed),slip=absf(g_player.slip);int hand=key_down(VK_SPACE);g_skid250_timer-=dt;
  if(speed>5.0f&&(slip>.11f||hand)&&g_skid250_timer<=0){const VehicleProfile*vp=&g_vehicle_profiles[g_player.car];V3 f=v3((float)sin((double)g_player.body.yaw),0,(float)cos((double)g_player.body.yaw)),r=v3(f.z,0,-f.x);float rear=-vp->wheelbase*.48f,side=vp->track*.47f;feel250_add_mark(vadd(g_player.body.p,vadd(vmul(f,rear),vmul(r,side))),g_player.body.yaw,.12f);feel250_add_mark(vadd(g_player.body.p,vadd(vmul(f,rear),vmul(r,-side))),g_player.body.yaw,.12f);g_skid250_timer=.075f;}
  float wet=(g_weather>=2||g_wetness60>.18f)?1.0f:0.0f,loose=g_player.body.surface==SURFACE_GRASS?.85f:g_player.body.surface==SURFACE_CURB?.25f:0.0f;g_spray250=lerpf(g_spray250,clampf(speed/34.0f,0,1)*maxf(wet*.65f,loose),clampf(dt*7.0f,0,1));
  int contacts=g_last_static_contacts+g_last_dynamic_contacts;if(contacts>g_feel250_prev_contacts)g_impact250=1.0f;else g_impact250=maxf(0,g_impact250-dt*2.8f);g_feel250_prev_contacts=contacts;
}
static void game_update(float dt){game_update_legacy215(dt);if(!g_started||g_settings_menu_open||g_editor19||g_photo70||g_garage215_open)return;feel250_update(dt);}
