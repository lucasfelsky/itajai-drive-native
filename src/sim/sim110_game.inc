// -----------------------------------------------------------------------------
// 1.1 Driving Feel 2.0
// Adds axle slip angles, load transfer, progressive tyre saturation,
// drivetrain personality (FWD/RWD/AWD), engine braking and camera FOV state
// on top of the stable 1.0 chassis. 2.2 softens the rear-lock handbrake response.
// -----------------------------------------------------------------------------
enum { DRIVE110_FWD=0, DRIVE110_RWD=1, DRIVE110_AWD=2 };
typedef struct {
  int drive;
  float frontWeight,cgHeight,cornerFront,cornerRear,engineBrake,throttleSaturation,yawDamping;
} DriveTune110;
static const DriveTune110 g_drive_tune110[4]={
  {DRIVE110_FWD,.62f,.48f,76000.0f,69000.0f,.72f,.20f,1.85f},
  {DRIVE110_FWD,.59f,.50f,80000.0f,73500.0f,.68f,.18f,1.72f},
  {DRIVE110_RWD,.53f,.47f,83500.0f,79000.0f,.82f,.30f,1.42f},
  {DRIVE110_AWD,.56f,.57f,81500.0f,79000.0f,.78f,.13f,1.68f}
};
static float g_ax110=0,g_ay110=0,g_front_load110=0,g_rear_load110=0;
static float g_front_slip110=0,g_rear_slip110=0,g_understeer110=0,g_oversteer110=0;
static float g_fov110=60.0f,g_fov_target110=60.0f,g_engine_brake110=0;
static float g_prev_long110=0,g_prev_lat110=0;

static const char*sim110_drive_name(int d){return d==DRIVE110_RWD?"RWD":d==DRIVE110_AWD?"AWD":"FWD";}
static float sim110_force_clamp(float f,float limit){if(f>limit)return limit;if(f<-limit)return-limit;return f;}

static void sim110_update_driving(float dt){
  if(dt<=0||dt>.12f)return;
  const VehicleProfile*vp=&g_vehicle_profiles[g_player.car];const DriveTune110*t=&g_drive_tune110[g_player.car];
  V3 f=v3((float)sin((double)g_player.body.yaw),0,(float)cos((double)g_player.body.yaw));V3 r=v3(f.z,0,-f.x);
  float longV=vdot(g_player.body.vel,f),latV=vdot(g_player.body.vel,r),speed=absf(longV);
  float throttle=key_down('W')?1.0f:0.0f,brake=key_down('S')?1.0f:0.0f,hand=key_down(VK_SPACE)?1.0f:0.0f;
  float invdt=1.0f/maxf(dt,.001f);float rawAx=(longV-g_prev_long110)*invdt;float rawAy=(latV-g_prev_lat110)*invdt+longV*g_player.body.angularVel;
  g_ax110=lerpf(g_ax110,clampf(rawAx,-16.0f,13.0f),clampf(dt*5.5f,0,1));g_ay110=lerpf(g_ay110,clampf(rawAy,-16.0f,16.0f),clampf(dt*5.5f,0,1));g_prev_long110=longV;g_prev_lat110=latV;

  float total=vp->mass*9.81f;float transfer=vp->mass*g_ax110*t->cgHeight/maxf(1.8f,vp->wheelbase);
  float front=total*t->frontWeight-transfer,rear=total-front;front=clampf(front,total*.22f,total*.78f);rear=total-front;g_front_load110=front;g_rear_load110=rear;
  float lf=vp->wheelbase*(1.0f-t->frontWeight),lr=vp->wheelbase*t->frontWeight;
  float steer=g_player.steer*vp->maxSteer;float denom=maxf(2.0f,speed);
  float alphaF=(float)atan2((double)(latV+g_player.body.angularVel*lf),(double)denom)-steer;
  float alphaR=(float)atan2((double)(latV-g_player.body.angularVel*lr),(double)denom);
  g_front_slip110=lerpf(g_front_slip110,alphaF,clampf(dt*10.0f,0,1));g_rear_slip110=lerpf(g_rear_slip110,alphaR,clampf(dt*10.0f,0,1));

  float surfaceGrip=sim_surface_grip(g_player.body.surface)*vp->baseGrip;float frontMu=surfaceGrip,rearMu=surfaceGrip;
  // Combined-slip personality: driven tyres give up some lateral grip under power.
  if(t->drive==DRIVE110_FWD)frontMu*=1.0f-throttle*t->throttleSaturation;
  else if(t->drive==DRIVE110_RWD)rearMu*=1.0f-throttle*t->throttleSaturation;
  else {frontMu*=1.0f-throttle*t->throttleSaturation*.45f;rearMu*=1.0f-throttle*t->throttleSaturation*.55f;}
  // 2.2: rear wheels still lock enough to rotate the car, but retain substantially
  // more lateral authority than the old .34 multiplier (QA reported soap-like slides).
  if(hand>0)rearMu*=.58f;
  frontMu=clampf(frontMu,.16f,1.35f);rearMu=clampf(rearMu,.12f,1.35f);

  float fyF=sim110_force_clamp(-alphaF*t->cornerFront,frontMu*front);
  float fyR=sim110_force_clamp(-alphaR*t->cornerRear,rearMu*rear);
  float latAcc=(fyF+fyR)/maxf(600.0f,vp->mass);float iz=maxf(500.0f,vp->mass*(vp->wheelbase*vp->wheelbase+vp->track*vp->track)/12.0f);
  float yawAcc=(fyF*lf-fyR*lr)/iz;
  // Blend rather than replace the proven 0.7 bicycle model.
  latV+=latAcc*dt*.70f;g_player.body.angularVel+=yawAcc*dt*.58f;
  g_player.body.angularVel*=clampf(1.0f-t->yawDamping*dt*(speed<2.0f?1.8f:1.0f),0,1);
  if(hand>0&&speed>6.0f)g_player.body.angularVel+=g_player.steer*sim_sign(longV)*dt*(.18f+speed*.004f);

  // Engine braking and rolling drivetrain drag are deliberately mild.
  g_engine_brake110=0;if(throttle<.01f&&brake<.01f&&speed>.4f){g_engine_brake110=t->engineBrake*(.75f+.08f*g_gear15);longV=sim_move_toward(longV,0,g_engine_brake110*dt);}

  // Approximate longitudinal traction budget of the driven axle(s).
  float drivenLoad=t->drive==DRIVE110_FWD?front:t->drive==DRIVE110_RWD?rear:total*.90f;
  float demand=throttle*vp->engineAccel*vp->mass,traction=maxf(500.0f,drivenLoad*surfaceGrip*.94f);
  if(demand>traction&&speed>1.0f){float excess=clampf((demand-traction)/maxf(1.0f,demand),0,.55f);g_tcs15=maxf(g_tcs15,excess);longV=sim_move_toward(longV,0,excess*.65f*dt);}
  if(brake>0&&surfaceGrip<.80f&&speed>5.0f)g_abs15=maxf(g_abs15,clampf((.80f-surfaceGrip)*.65f,0,.38f));

  // Report handling balance from axle saturation. Positive values are normalized HUD telemetry.
  float satF=absf(fyF)/maxf(1.0f,frontMu*front),satR=absf(fyR)/maxf(1.0f,rearMu*rear);
  g_understeer110=lerpf(g_understeer110,clampf(satF-satR,0,1),clampf(dt*6.0f,0,1));
  g_oversteer110=lerpf(g_oversteer110,clampf(satR-satF,0,1),clampf(dt*6.0f,0,1));

  // Suspension asymmetry feeds a small extra body attitude response.
  float left=(g_wheels15[0].compression+g_wheels15[2].compression)*.5f,rightC=(g_wheels15[1].compression+g_wheels15[3].compression)*.5f;
  float frontC=(g_wheels15[0].compression+g_wheels15[1].compression)*.5f,rearC=(g_wheels15[2].compression+g_wheels15[3].compression)*.5f;
  g_player.roll=lerpf(g_player.roll,clampf(g_player.roll+(rightC-left)*.055f,-.16f,.16f),clampf(dt*5.0f,0,1));
  g_player.pitch=lerpf(g_player.pitch,clampf(g_player.pitch+(frontC-rearC)*.040f,-.12f,.12f),clampf(dt*4.5f,0,1));

  // Recompose velocity using the post-tyre state.
  f=v3((float)sin((double)g_player.body.yaw),0,(float)cos((double)g_player.body.yaw));r=v3(f.z,0,-f.x);
  g_player.body.vel=vadd(vmul(f,longV),vmul(r,latV));g_player.speed=longV;g_player.lateralSpeed=latV;
  g_player.slip=speed>.6f?clampf((float)atan2((double)latV,(double)maxf(1.0f,speed)),-1,1):0;

  float speedNorm=clampf(speed/maxf(20.0f,vp->maxSpeed),0,1);float base=g_player.camera==2?64.0f:g_player.camera==3?57.0f:60.0f;
  g_fov_target110=base+(g_player.camera==3?2.5f:8.0f)*speedNorm;g_fov110=lerpf(g_fov110,g_fov_target110,clampf(dt*3.6f,0,1));
}

static void sim110_update_engine(float dt){
  const DriveTune110*t=&g_drive_tune110[g_player.car];float throttle=key_down('W')?1.0f:0.0f;float speed=absf(g_player.speed),wheelRpm=speed/(6.2831853f*.31f)*60.0f;
  float finalDrive=t->drive==DRIVE110_AWD?4.10f:t->drive==DRIVE110_RWD?3.72f:3.92f;float target=wheelRpm*g_gear_ratios15[g_gear15]*finalDrive;
  if(speed<.45f)target=820.0f+throttle*1850.0f;target=clampf(target,780.0f,6900.0f);g_engine_rpm15=lerpf(g_engine_rpm15,target,clampf(dt*(throttle?9.0f:5.0f),0,1));
  if(g_engine_rpm15>6250&&g_gear15<5)g_gear15++;else if(g_engine_rpm15<1450&&g_gear15>1)g_gear15--;
}

static void game_update(float dt){
  game_update_legacy100(dt);if(!g_started)return;if(g_settings_menu_open||g_editor19||g_photo70)return;sim110_update_driving(dt);sim110_update_engine(dt);
}
