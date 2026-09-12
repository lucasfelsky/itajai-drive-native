// -----------------------------------------------------------------------------
// 2.13 Living City 2.0 lightweight cyclist/tug rendering.
// -----------------------------------------------------------------------------
static void life213_draw_cyclists(void){
  float maxD=(g_perf80_quality==0?170.0f:260.0f);maxD*=maxD;renderer09_set_material(MAT09_GENERIC);
  for(int i=0;i<LIFE213_CYCLISTS;i++){Cyclist213*c=&g_cyclists213[i];if(!c->active)continue;float dx=c->p.x-g_player.body.p.x,dz=c->p.z-g_player.body.p.z;if(dx*dx+dz*dz>maxD)continue;V3 d=vsub(c->b,c->a);float yaw=(float)atan2((double)d.x,(double)d.z);float pedal=(float)sin(g_time*7.5+c->phase);
    glPushMatrix();glTranslatef(c->p.x,c->p.y,c->p.z);glRotatef(yaw*57.29578f,0,1,0);
    // wheels/frame
    draw_local_box(-.02f,.38f,-.62f,.08f,.68f,.08f,.055f,.06f,.065f);draw_local_box(-.02f,.38f,.62f,.08f,.68f,.08f,.055f,.06f,.065f);
    draw_local_box(0,.48f,0,.06f,.06f,1.08f,.12f,.14f,.16f);draw_local_box(0,.62f,-.18f,.06f,.48f,.06f,.12f,.14f,.16f);
    // rider
    float shirt=.22f+(i%5)*.11f;draw_local_box(0,.96f,-.06f,.34f,.72f,.26f,shirt,.34f,.62f-shirt*.3f);draw_local_box(0,1.43f,-.08f,.29f,.29f,.29f,.70f,.54f,.42f);
    draw_local_box(-.12f,.55f,.08f,.08f,.55f,.08f,.12f,.13f,.15f);draw_local_box(.12f,.55f,-.08f,.08f,.55f,.08f,.12f,.13f,.15f+pedal*.01f);
    glPopMatrix();
  }
}
static void life213_draw_tugs(void){
  float maxD=1000.0f*1000.0f;renderer09_set_material(MAT09_GENERIC);
  for(int i=0;i<LIFE213_TUGS;i++){Tug213*t=&g_tugs213[i];if(!t->active)continue;float dx=t->p.x-g_player.body.p.x,dz=t->p.z-g_player.body.p.z;if(dx*dx+dz*dz>maxD)continue;V3 d=vsub(t->b,t->a);float yaw=(float)atan2((double)d.x,(double)d.z);float s=1.0f+i*.10f;
    draw_box(vadd(t->p,v3(0,.42f,0)),3.0f*s,.72f,7.2f*s,yaw,.18f,.25f,.29f);draw_box(vadd(t->p,v3(0,1.02f,-.45f)),2.1f*s,.82f,3.0f*s,yaw,.78f,.72f,.56f);draw_box(vadd(t->p,v3(0,1.73f,-.30f)),1.45f*s,.58f,1.8f*s,yaw,.25f,.43f,.52f);
  }
}
static void life213_draw(void){life213_draw_cyclists();life213_draw_tugs();renderer09_set_material(MAT09_GENERIC);}
