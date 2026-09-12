// -----------------------------------------------------------------------------
// 0.50 living world simulation: lightweight pedestrians, boats and parked-car
// presence. These agents are visual ambience, intentionally cheaper than road AI.
// -----------------------------------------------------------------------------
#define LIFE50_PEDS 36
#define LIFE50_BOATS 7
#define LIFE50_PARKED 26
typedef struct { V3 a,b,p; float t,speed,phase; int active; } Ped50;
typedef struct { V3 a,b,p; float t,speed,phase; int active; } Boat50;
typedef struct { V3 p; float yaw; int model; } Parked50;
static Ped50 g_peds50[LIFE50_PEDS];static Boat50 g_boats50[LIFE50_BOATS];static Parked50 g_parked50[LIFE50_PARKED];
static int g_life50_init=0,g_life50_active_peds=0,g_life50_active_boats=0;

static void life50_init(void){if(g_life50_init)return;g_life50_init=1;
  for(int i=0;i<LIFE50_PEDS;i++){Ped50*p=&g_peds50[i];int waterfront=i<24;float baseX=waterfront?-360.0f+(i%12)*52.0f:-570.0f+(i%6)*65.0f;float baseZ=waterfront?944.0f+(i/12)*13.0f:80.0f+(i/6)*45.0f;p->a=v3(baseX,.08f,baseZ);p->b=v3(baseX+(waterfront?34.0f:24.0f),.08f,baseZ+(waterfront?0.0f:26.0f));p->t=(i%7)/7.0f;p->speed=.055f+(i%5)*.009f;p->phase=(float)(i*17);p->active=1;}
  for(int i=0;i<LIFE50_BOATS;i++){Boat50*b=&g_boats50[i];b->a=v3(-1760.0f+i*78.0f,.12f,-1190.0f+i*31.0f);b->b=v3(780.0f-i*55.0f,.12f,-920.0f+i*42.0f);b->t=(i+1)/(float)(LIFE50_BOATS+1);b->speed=.010f+(i%3)*.003f;b->phase=(float)(i*31);b->active=1;}
  for(int i=0;i<LIFE50_PARKED;i++){Parked50*p=&g_parked50[i];if(i<14){p->p=v3(-510.0f+i*38.0f,.58f,1005.0f+((i&1)?18.0f:0));p->yaw=1.5708f;}else{int j=i-14;p->p=v3(-680.0f+j*44.0f,.58f,155.0f+((j%3)*18.0f));p->yaw=(j&1)?0:3.14159f;}p->model=(i*3+i/4)&7;}
}
static float life50_day_activity(void){if(g_hour>=7&&g_hour<10)return .82f;if(g_hour>=10&&g_hour<18)return 1.0f;if(g_hour>=18&&g_hour<22)return .72f;return .24f;}
static void life50_update(float dt){life50_init();float activity=life50_day_activity();g_life50_active_peds=0;g_life50_active_boats=0;
  for(int i=0;i<LIFE50_PEDS;i++){Ped50*p=&g_peds50[i];p->active=((i%100)/100.0f)<activity;if(!p->active)continue;p->t+=p->speed*dt;if(p->t>1.0f){p->t-=1.0f;V3 q=p->a;p->a=p->b;p->b=q;}p->p=vadd(p->a,vmul(vsub(p->b,p->a),p->t));p->p.y=.08f;g_life50_active_peds++;}
  for(int i=0;i<LIFE50_BOATS;i++){Boat50*b=&g_boats50[i];b->active=g_hour>=5&&g_hour<23;if(!b->active)continue;b->t+=b->speed*dt;if(b->t>1.0f){b->t-=1.0f;V3 q=b->a;b->a=b->b;b->b=q;}b->p=vadd(b->a,vmul(vsub(b->b,b->a),b->t));b->p.y=.12f+(float)sin(g_time*.8+b->phase)*.03f;g_life50_active_boats++;}
}
