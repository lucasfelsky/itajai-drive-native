// -----------------------------------------------------------------------------
// 2.4 Vehicle Dynamics Core.
// A fixed 120 Hz tyre-force layer consolidates the later handling corrections
// without moving the world/camera/input loop off the proven legacy chain yet.
// ----------------------------------------------------------------------------
typedef struct {
  float tyreMu,frontBias,cgHeight,cornerFront,cornerRear,driveFront;
} DynamicsTune24;
static const DynamicsTune24 g_dyn24_tune[4]={
  {1.00f,.62f,.48f,72000.0f,65500.0f,1.00f}, // Uno FWD
  {1.03f,.59f,.50f,76000.0f,70000.0f,1.00f}, // Gol FWD
  {1.06f,.58f,.49f,79000.0f,73500.0f,1.00f}, // HB20 FWD
  {1.00f,.56f,.57f,80000.0f,77500.0f,.52f}   // Renegade AWD-ish
};
static double g_dyn24_accum=0.0;
static int g_dyn24_ticks=0;
static float g_dyn24_front_slip=0,g_dyn24_rear_slip=0,g_dyn24_front_force=0,g_dyn24_rear_force=0;
static float g_dyn24_traction_use=0,g_dyn24_brake_use=0,g_dyn24_load_front=0,g_dyn24_load_rear=0;
static float g_dyn24_prev_long=0;

static float dyn24_sat_force(float slip,float corner,float limit){
  float raw=-slip*corner;if(limit<1.0f)return 0;float x=raw/limit;
  // Smooth saturation, close to linear near zero and bounded at the friction circle.
  return limit*(x/(1.0f+absf(x)));
}

static void dyn24_tick(float h){
  if(!g_started||g_settings_menu_open||g_editor19||g_photo70)return;
  const VehicleProfile*vp=&g_vehicle_profiles[g_player.car];const DynamicsTune24*t=&g_dyn24_tune[g_player.car];
  V3 f=v3((float)sin((double)g_player.body.yaw),0,(float)cos((double)g_player.body.yaw)),r=v3(f.z,0,-f.x);
  float longV=vdot(g_player.body.vel,f),latV=vdot(g_player.body.vel,r),speed=absf(longV);
  float throttle=key_down('W')?1.0f:0.0f,brake=key_down('S')?1.0f:0.0f,hand=key_down(VK_SPACE)?1.0f:0.0f;
  const DriveTune110*old=&g_drive_tune110[g_player.car];

  float ax=(longV-g_dyn24_prev_long)/maxf(h,.0005f);g_dyn24_prev_long=longV;
  float total=vp->mass*9.81f;float transfer=vp->mass*ax*t->cgHeight/maxf(1.7f,vp->wheelbase);
  float front=clampf(total*t->frontBias-transfer,total*.24f,total*.76f),rear=total-front;g_dyn24_load_front=front;g_dyn24_load_rear=rear;
  float lf=vp->wheelbase*(1.0f-t->frontBias),lr=vp->wheelbase*t->frontBias;
  float steer=g_player.steer*vp->maxSteer,den=maxf(2.3f,speed);
  float aF=(float)atan2((double)(latV+g_player.body.angularVel*lf),(double)den)-steer;
  float aR=(float)atan2((double)(latV-g_player.body.angularVel*lr),(double)den);
  g_dyn24_front_slip=lerpf(g_dyn24_front_slip,aF,clampf(h*18.0f,0,1));g_dyn24_rear_slip=lerpf(g_dyn24_rear_slip,aR,clampf(h*18.0f,0,1));

  float surf=sim_surface_grip(g_player.body.surface)*vp->baseGrip*t->tyreMu;
  float muF=surf,muR=surf;if(hand>0)muR*=.62f;
  float drivenFront=t->driveFront,drivenRear=1.0f-drivenFront;
  muF*=1.0f-throttle*.16f*drivenFront;muR*=1.0f-throttle*.18f*drivenRear;
  float limF=maxf(400.0f,front*muF),limR=maxf(400.0f,rear*muR);
  float fyF=dyn24_sat_force(aF,t->cornerFront,limF),fyR=dyn24_sat_force(aR,t->cornerRear,limR);
  g_dyn24_front_force=fyF;g_dyn24_rear_force=fyR;

  float latAcc=(fyF+fyR)/maxf(650.0f,vp->mass);float iz=maxf(520.0f,vp->mass*(vp->wheelbase*vp->wheelbase+vp->track*vp->track)/12.0f);
  float yawAcc=(fyF*lf-fyR*lr)/iz;
  // This replaces part of the historical correction rather than stacking another full model.
  latV+=latAcc*h*.34f;g_player.body.angularVel+=yawAcc*h*.28f;
  g_player.body.angularVel*=clampf(1.0f-h*(.46f+speed*.010f),.90f,1.0f);

  // Longitudinal acceleration budget: clamp sudden frame-dependent impulses to tyre capacity.
  float drivenLoad=front*drivenFront+rear*drivenRear;if(drivenLoad<total*.25f)drivenLoad=total*.25f;
  float tractionAcc=(drivenLoad*surf)/maxf(650.0f,vp->mass);float requested=(longV-g_dyn24_prev_long)/maxf(h,.0005f);(void)requested;
  g_dyn24_traction_use=clampf((throttle*vp->engineAccel)/maxf(.25f,tractionAcc),0,1.5f);
  g_dyn24_brake_use=clampf((brake*vp->brakeAccel)/maxf(.25f,total*surf/maxf(650.0f,vp->mass)),0,1.5f);
  if(g_dyn24_traction_use>1.0f&&speed>1.0f){float cut=clampf((g_dyn24_traction_use-1.0f)*.28f,0,.18f);longV=sim_move_toward(longV,0,cut*h*vp->engineAccel);g_tcs15=maxf(g_tcs15,cut);}
  if(brake>0&&g_dyn24_brake_use>1.0f&&speed>3.0f)g_abs15=maxf(g_abs15,clampf((g_dyn24_brake_use-1.0f)*.22f,0,.28f));

  // Wheel telemetry now comes from the same axle/slip state used by the force layer.
  for(int i=0;i<4;i++){
    WheelState15*w=&g_wheels15[i];w->normalLoad=(i<2?front:rear)*.5f;w->slipLat=i<2?aF:aR;
    float driveShare=i<2?drivenFront*.5f:drivenRear*.5f;w->slipLong=clampf(throttle*driveShare*.30f-brake*.16f-(hand&&i>=2?.38f:0),-1,1);
  }
  f=v3((float)sin((double)g_player.body.yaw),0,(float)cos((double)g_player.body.yaw));r=v3(f.z,0,-f.x);
  g_player.body.vel=vadd(vmul(f,longV),vmul(r,latV));g_player.speed=longV;g_player.lateralSpeed=latV;
  g_player.slip=speed>.6f?clampf((float)atan2((double)latV,(double)maxf(1.0f,speed)),-1,1):0;
  (void)old;
}

static void game_update(float dt){
  game_update_legacy230(dt);if(dt<=0)return;
  const double fixed=1.0/120.0;g_dyn24_accum+=dt;if(g_dyn24_accum>.12)g_dyn24_accum=.12;g_dyn24_ticks=0;
  while(g_dyn24_accum>=fixed&&g_dyn24_ticks<16){dyn24_tick((float)fixed);g_dyn24_accum-=fixed;g_dyn24_ticks++;}
}
