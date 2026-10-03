// Authored buildings own their footprint, independent of distance and streaming.
// Dimensions describe the original game models, not a cadastral survey.
typedef struct {const char*name;double lat,lon;float w,d,yaw;} Landmark506;
static const Landmark506 g_landmarks506[]={
  {"Matriz",-26.9074694,-48.6619194,20,44,-.10f},
  {"Mercado Publico",-26.9063889,-48.6541667,26,23,.04f},
  {"Palacio Marcos Konder",-26.9072921,-48.6611456,26,17,-.03f},
  {"Casa Konder",-26.905817,-48.655353,16,10,.08f},
  {"Casa Malburg",-26.90439,-48.65611,18,11,.04f},
  {"Imaculada Conceicao",-26.905031,-48.656181,14,24,.03f},
  {"Centreventos",-26.90959,-48.65287,49,31,-.03f},
  {"Terminal",-26.904722,-48.654444,32,13,-.03f}
};
#define LANDMARK506_COUNT 8

static V3 landmark506_local(V3 p,int id){const Landmark506*l=&g_landmarks506[id];V3 c=project_geo(l->lat,l->lon);float cs=(float)cos(l->yaw),sn=(float)sin(l->yaw),x=p.x-c.x,z=p.z-c.z;return v3(x*cs-z*sn,0,x*sn+z*cs);}
// Clip the exact municipal polygon, not its bounding box. Area tolerance keeps
// shared boundaries and near-touching neighboring buildings intact.
static float landmark506_overlap(const V3*poly,int n,int id){
  V3 a[128],b[128];if(n<3||n>112)return 0;
  for(int i=0;i<n;i++)a[i]=landmark506_local(poly[i],id);
  for(int edge=0;edge<4;edge++){
    float lim=(edge<2?g_landmarks506[id].w:g_landmarks506[id].d)*.5f,sign=(edge&1)?-1.0f:1.0f;int m=0;
    for(int i=0;i<n;i++){V3 p=a[i],q=a[(i+1)%n];float dp=sign*(edge<2?p.x:p.z)-lim,dq=sign*(edge<2?q.x:q.z)-lim;
      if(dp<=0&&m<127)b[m++]=p;
      if((dp<0&&dq>0)||(dp>0&&dq<0)){float t=dp/(dp-dq);if(m<127)b[m++]=vadd(p,vmul(vsub(q,p),t));}
    }n=m;if(n<3)return 0;for(int i=0;i<n;i++)a[i]=b[i];
  }
  float area=0;for(int i=0;i<n;i++)area+=a[i].x*a[(i+1)%n].z-a[(i+1)%n].x*a[i].z;return absf(area)*.5f;
}
static int landmark506_owner(const Building*b){
  for(int k=0;k<LANDMARK506_COUNT;k++){
    const Landmark506*l=&g_landmarks506[k];V3 p=project_geo(l->lat,l->lon);float extent=(l->w+l->d)*.5f;
    if(absf(b->c.x-p.x)>b->sx*.5f+extent||absf(b->c.z-p.z)>b->sz*.5f+extent)continue;
    int id=g_building12_ready?building12_find_seed(b->seed):-1;
    if(id>=0){int n=g_building_poly_count12[id],s=g_building_poly_start12[id];if(n>=3&&n<=112&&s>=0&&s+n<=g_building_vert_count12){if(landmark506_overlap(g_building_verts12+s,n,k)>.25f)return k;continue;}}
    V3 v[4]={v3(b->c.x-b->sx*.5f,0,b->c.z-b->sz*.5f),v3(b->c.x+b->sx*.5f,0,b->c.z-b->sz*.5f),v3(b->c.x+b->sx*.5f,0,b->c.z+b->sz*.5f),v3(b->c.x-b->sx*.5f,0,b->c.z+b->sz*.5f)};
    if(landmark506_overlap(v,4,k)>.25f)return k;
  }return -1;
}
static int landmark506_replaces(const Building*b){return landmark506_owner(b)>=0;}
static int landmark506_box_overlap(const Building*b){
  V3 v[4]={v3(b->c.x-b->sx*.5f,0,b->c.z-b->sz*.5f),v3(b->c.x+b->sx*.5f,0,b->c.z-b->sz*.5f),v3(b->c.x+b->sx*.5f,0,b->c.z+b->sz*.5f),v3(b->c.x-b->sx*.5f,0,b->c.z+b->sz*.5f)};
  for(int k=0;k<LANDMARK506_COUNT;k++){const Landmark506*l=&g_landmarks506[k];V3 p=project_geo(l->lat,l->lon);float extent=(l->w+l->d)*.5f;if(absf(b->c.x-p.x)>b->sx*.5f+extent||absf(b->c.z-p.z)>b->sz*.5f+extent)continue;if(landmark506_overlap(v,4,k)>.25f)return 1;}return 0;
}
// Collision components follow occupied volumes; the market courtyard stays open.
typedef struct{int landmark;float x,z,w,d;} Landmark506Solid;
static const Landmark506Solid g_landmark506_solids[]={
 {0,0,0,19,34},{0,0,-17,18,4.6f},{0,-7,-18.8f,5,5},{0,7,-18.8f,5,5},
 {1,0,-7,25,7},{1,0,7,25,7},{1,-9,0,7,14},{1,9,0,7,14},
 {2,0,0,25,13},{2,-8.2f,-5,6.2f,6},{2,8.2f,-5,6.2f,6},
 {3,0,0,15.5f,8.8f},{4,0,0,17,10},{5,0,0,13,22},{6,0,0,48,30},{7,0,0,31,12}
};
#define LANDMARK506_SOLIDS 16
static void landmark506_colliders(void){for(int i=0;i<LANDMARK506_SOLIDS;i++){const Landmark506Solid*s=&g_landmark506_solids[i];const Landmark506*l=&g_landmarks506[s->landmark];V3 p=project_geo(l->lat,l->lon);float cs=(float)cos(l->yaw),sn=(float)sin(l->yaw);foundation_add_aabb(p.x+s->x*cs+s->z*sn,p.z-s->x*sn+s->z*cs,(absf(cs)*s->w+absf(sn)*s->d)*.5f,(absf(sn)*s->w+absf(cs)*s->d)*.5f,COLLIDER_BUILDING,-100-i);}}
static int landmark506_contact(int source,V3 p,float radius,Contact*out,int collider){
  int i=-100-source;if(i<0||i>=LANDMARK506_SOLIDS)return -1;const Landmark506Solid*s=&g_landmark506_solids[i];const Landmark506*l=&g_landmarks506[s->landmark];V3 q=landmark506_local(p,s->landmark);q.x-=s->x;q.z-=s->z;
  StaticCollider box;box.c=v3(0,0,0);box.hx=s->w*.5f;box.hz=s->d*.5f;
  if(!circle_aabb_contact(q,radius,&box,out,collider))return 0;
  float cs=(float)cos(l->yaw),sn=(float)sin(l->yaw);V3 n=out->normal;out->normal=v3(n.x*cs+n.z*sn,0,-n.x*sn+n.z*cs);return 1;
}

