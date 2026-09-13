// -----------------------------------------------------------------------------
// Renderer 5.0 continuous world lighting.
// The compatibility renderer historically flips daylight at integer hours. HDR
// presentation instead reapplies the same material uniforms after camera setup
// using a smooth dawn/dusk factor, preserving the old path as an exact fallback.
// -----------------------------------------------------------------------------
static float g_worldlight500_day=1.0f,g_worldlight500_sun=1.0f;
static void worldlight500_apply(void){
  if(!g_renderer09_linear_output||!g_renderer09_shader)return;
  float h=(float)g_hour+g_clock_accum60/60.0f,night=renderer500_night_factor(),day=1.0f-night;g_worldlight500_day=day;
  V3 eye,target;sim07_camera_pose(g_player.camera,g_player.body.p,g_player.body.yaw,&eye,&target);V3 fw=vnorm(vsub(target,eye)),up0=v3(0,1,0),side=vnorm(vcross(fw,up0)),up=vcross(side,fw);g_renderer_camera_eye09=eye;
  float phase=(h-6.0f)/12.0f*3.14159265f,elev=(float)sin((double)phase);if(elev<.035f)elev=.035f;V3 sunW=vnorm(v3((float)cos((double)phase)*.55f,elev,-.62f)),sunE=v3(vdot(side,sunW),vdot(up,sunW),-vdot(fw,sunW));
  float cloud=clampf(1.0f-g_cloud60*.52f,.42f,1.0f),ambient=.105f+.285f*day*cloud+.050f*night,sunStrength=1.58f*day*cloud+g_lightning60*2.25f;g_worldlight500_sun=sunStrength;
  float fogD=.00110f+g_cloud60*.00110f+(g_weather>=2?.00115f:0);V3 dayFog=v3(.115f,.175f,.245f),nightFog=v3(.014f,.024f,.043f);float fr=lerpf(nightFog.x,dayFog.x,day),fg=lerpf(nightFog.y,dayFog.y,day),fb=lerpf(nightFog.z,dayFog.z,day);if(g_weather>=2){fr*=.73f;fg*=.80f;fb*=.87f;}
  V3 carF=v3((float)sin((double)g_player.body.yaw),0,(float)cos((double)g_player.body.yaw)),hp=vadd(g_player.body.p,vadd(vmul(carF,1.78f),v3(0,.38f,0))),rel=vsub(hp,eye);V3 headE=v3(vdot(side,rel),vdot(up,rel),-vdot(fw,rel)),dirE=v3(vdot(side,carF),vdot(up,carF),-vdot(fw,carF));float headOn=clampf(night+g_wetness60*.45f+(g_weather>=2?.18f:0),0,1);
  if(g_u_sun09>=0)p09Uniform3f(g_u_sun09,sunE.x,sunE.y,sunE.z);if(g_u_ambient09>=0)p09Uniform1f(g_u_ambient09,ambient);if(g_u_sun_strength09>=0)p09Uniform1f(g_u_sun_strength09,sunStrength);if(g_u_fog_color09>=0)p09Uniform3f(g_u_fog_color09,fr,fg,fb);if(g_u_fog_density09>=0)p09Uniform1f(g_u_fog_density09,fogD);if(g_u_night12>=0)p09Uniform1f(g_u_night12,night);if(g_u_head_on12>=0)p09Uniform1f(g_u_head_on12,headOn);if(g_u_head_pos12>=0)p09Uniform3f(g_u_head_pos12,headE.x,headE.y,headE.z);if(g_u_head_dir12>=0)p09Uniform3f(g_u_head_dir12,dirE.x,dirE.y,dirE.z);renderer09_set_material(MAT09_GENERIC);
}
