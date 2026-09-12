// -----------------------------------------------------------------------------
// 2.13 Living City 2.0 state.
// Extends the cheap ambience layer with cyclists, working-channel tugs and
// pedestrian crossing pauses without promoting them to full road-physics agents.
// -----------------------------------------------------------------------------
#define LIFE213_CYCLISTS 14
#define LIFE213_TUGS 3
typedef struct { V3 a,b,p; float t,speed,phase; int active; } Cyclist213;
typedef struct { V3 a,b,p; float t,speed,phase; int active; } Tug213;
static Cyclist213 g_cyclists213[LIFE213_CYCLISTS];
static Tug213 g_tugs213[LIFE213_TUGS];
static float g_ped213_pause[LIFE50_PEDS],g_ped213_last_t[LIFE50_PEDS];
static int g_life213_init=0,g_life213_active_cyclists=0,g_life213_active_tugs=0,g_life213_waiting_peds=0;

static void life213_init(void){if(g_life213_init)return;g_life213_init=1;
  for(int i=0;i<LIFE213_CYCLISTS;i++){Cyclist213*c=&g_cyclists213[i];int coast=i>=8;if(!coast){c->a=v3(-500.0f+(i%4)*82.0f,.10f,930.0f+(i/4)*12.0f);c->b=v3(170.0f-(i%3)*45.0f,.10f,955.0f+(i/4)*15.0f);}else{int j=i-8;c->a=v3(1970.0f+j*42.0f,.10f,5200.0f+j*18.0f);c->b=v3(2480.0f-j*28.0f,.10f,5390.0f-j*9.0f);}float len=maxf(20.0f,vlen(vsub(c->b,c->a)));c->t=(i+1)/(float)(LIFE213_CYCLISTS+1);c->speed=(3.2f+(i%4)*.45f)/len;c->phase=(float)(i*23);c->active=1;}
  for(int i=0;i<LIFE213_TUGS;i++){Tug213*t=&g_tugs213[i];t->a=v3(-1700.0f+i*120.0f,.14f,-1220.0f+i*35.0f);t->b=v3(540.0f-i*85.0f,.14f,-940.0f+i*55.0f);float len=maxf(100.0f,vlen(vsub(t->b,t->a)));t->t=.18f+i*.27f;t->speed=(4.0f+i*.55f)/len;t->phase=(float)(i*41);t->active=1;}
  for(int i=0;i<LIFE50_PEDS;i++)g_ped213_last_t[i]=g_peds50[i].t;
}

static void life213_update(float dt){life213_init();g_life213_active_cyclists=0;g_life213_active_tugs=0;g_life213_waiting_peds=0;
  // Some walkers pause near the middle of their path as if waiting at a crossing.
  // The pause is deterministic and short so it adds rhythm without navmesh cost.
  for(int i=0;i<LIFE50_PEDS;i++){Ped50*p=&g_peds50[i];if(!p->active){g_ped213_last_t[i]=p->t;continue;}float last=g_ped213_last_t[i];if(g_ped213_pause[i]<=0&&i%4==0&&last<.46f&&p->t>=.46f)g_ped213_pause[i]=1.4f+(i%3)*.65f;if(g_ped213_pause[i]>0){g_ped213_pause[i]-=dt;p->t-=p->speed*dt;if(p->t<0)p->t=0;p->p=vadd(p->a,vmul(vsub(p->b,p->a),p->t));p->p.y=.08f;g_life213_waiting_peds++;}g_ped213_last_t[i]=p->t;}
  float activity=life50_day_activity();
  for(int i=0;i<LIFE213_CYCLISTS;i++){Cyclist213*c=&g_cyclists213[i];c->active=activity>.35f&&((i%10)/10.0f)<minf(1.0f,activity+.18f);if(!c->active)continue;c->t+=c->speed*dt;if(c->t>1.0f){c->t-=1.0f;V3 q=c->a;c->a=c->b;c->b=q;}c->p=vadd(c->a,vmul(vsub(c->b,c->a),c->t));c->p.y=.10f;g_life213_active_cyclists++;}
  for(int i=0;i<LIFE213_TUGS;i++){Tug213*t=&g_tugs213[i];t->active=g_hour>=4&&g_hour<24;if(!t->active)continue;t->t+=t->speed*dt;if(t->t>1.0f){t->t-=1.0f;V3 q=t->a;t->a=t->b;t->b=q;}t->p=vadd(t->a,vmul(vsub(t->b,t->a),t->t));t->p.y=.14f+(float)sin(g_time*.7+t->phase)*.025f;g_life213_active_tugs++;}
}
