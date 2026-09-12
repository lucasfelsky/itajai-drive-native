// -----------------------------------------------------------------------------
// 3.1 Multiplayer remote-player presentation.
// Renders a single interpolated remote car using the existing vehicle visual
// pipeline. This deliberately has no collision/physics participation yet.
// -----------------------------------------------------------------------------
static void net301_draw_remote_player(void){
  if(!g_net301_remote.active||!g_started)return;
  int model=g_player_model160[g_net301_remote.car&3];
  float cc[4][3]={{.92f,.42f,.18f},{.18f,.72f,.42f},{.84f,.84f,.20f},{.62f,.28f,.82f}};
  float*c=cc[g_net301_remote.car&3];
  int braking=(g_net301_remote.flags&NET301_FLAG_BRAKE)!=0;
  int lights=(g_net301_remote.flags&NET301_FLAG_HEADLIGHTS)!=0;
  float spin=(float)g_time*absf(g_net301_remote.speed)*52.0f;
  float steer=g_net301_remote.steer*g_vehicle_profiles[g_net301_remote.car&3].maxSteer*57.29578f;
  glPushMatrix();
  glTranslatef(g_net301_remote.renderP.x,g_net301_remote.renderP.y-.66f,g_net301_remote.renderP.z);
  glRotatef(g_net301_remote.renderYaw*57.29578f,0,1,0);
  glRotatef(g_net301_remote.renderPitch*57.29578f,1,0,0);
  glRotatef(g_net301_remote.renderRoll*57.29578f,0,0,1);
  vehicle160_draw_model_local(model,c[0],c[1],c[2],braking,lights,spin,steer,1);
  vehicle211_parts_local(model);
  glPopMatrix();
}
