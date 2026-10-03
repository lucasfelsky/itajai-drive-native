// Authored buildings own their footprint, independent of distance and streaming.
// 5.0.7: every entry was re-surveyed against the OpenStreetMap (ODbL) building
// polygon cached in itajai_osm_cache_v13.json. Center = oriented bounding box
// center of that polygon; yaw = heading of the street-facing facade (model local
// -Z); w/d = oriented extents (local X / local Z). `ref` records the OSM way and
// `conf` the survey confidence: 2 = polygon + entrance/square evidence,
// 1 = polygon only (facade side inferred from streets), 0 = no cadastral polygon.
// The function names keep the 506 prefix because twenty render/collision call
// sites and the validator use them as a stable API.
typedef struct {const char*name;double lat,lon;float w,d,yaw;const char*ref;int conf;} Landmark506;
static const Landmark506 g_landmarks506[]={
  {"Matriz",-26.9075840,-48.6618468,32.6f,60.9f,0.6790f,"osm:w857401220",2},
  {"Mercado Publico",-26.9060089,-48.6544588,21.3f,39.0f,-2.9845f,"osm:w298841666",1},
  {"Palacio Marcos Konder",-26.9071056,-48.6612274,21.0f,51.2f,0.4102f,"osm:w1097529372",2},
  {"Casa Konder",-26.9057948,-48.6553532,14.5f,15.7f,1.7733f,"osm:w1098072184",1},
  {"Casa Malburg",-26.9043835,-48.6561088,15.9f,16.9f,2.3010f,"osm:w1106146084",1},
  {"Imaculada Conceicao",-26.9050623,-48.6561982,14.2f,28.2f,-1.2425f,"osm:w681517239",2},
  {"Centreventos",-26.9096087,-48.6528916,94.9f,127.1f,1.7453f,"osm:w197483217",1},
  {"Pier Turistico",-26.904722,-48.654444,32,13,-.03f,"osm:w681517238 (pier)",0},
  {"Hotel Rota do Mar",-26.9058694,-48.6550575,70.8f,14.0f,0.2094f,"osm:w1106146082",1}
};
#define LANDMARK506_COUNT 9

static V3 landmark506_local(V3 p,int id){const Landmark506*l=&g_landmarks506[id];V3 c=project_geo(l->lat,l->lon);float cs=(float)cos(l->yaw),sn=(float)sin(l->yaw),x=p.x-c.x,z=p.z-c.z;return v3(x*cs-z*sn,0,x*sn+z*cs);}
static float landmark506_area(const V3*poly,int n){float area=0;for(int i=0;i<n;i++)area+=poly[i].x*poly[(i+1)%n].z-poly[(i+1)%n].x*poly[i].z;return absf(area)*.5f;}
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
  return landmark506_area(a,n);
}
// 5.0.7 ownership rule: a cadastral building is replaced only when the clipped
// intersection is both above the shared-edge tolerance AND a substantial share
// (30%) of the smaller of the two areas. A slightly misplaced boundary that cuts
// a few square metres out of a large legitimate neighbor no longer deletes it.
#define LANDMARK506_MIN_OVERLAP .25f
#define LANDMARK506_SHARE .30f
static int landmark506_claims(float overlap,float polyArea,int id){float lm=g_landmarks506[id].w*g_landmarks506[id].d,small=polyArea<lm?polyArea:lm;return overlap>LANDMARK506_MIN_OVERLAP&&overlap>=LANDMARK506_SHARE*small;}
// When several entries claim the same cadastral building (adjacent authored
// neighbors such as Casa Konder and Hotel Rota do Mar) the largest overlap wins.
static int landmark506_owner(const Building*b){
  int best=-1;float bestOverlap=0;
  V3 box[4]={v3(b->c.x-b->sx*.5f,0,b->c.z-b->sz*.5f),v3(b->c.x+b->sx*.5f,0,b->c.z-b->sz*.5f),v3(b->c.x+b->sx*.5f,0,b->c.z+b->sz*.5f),v3(b->c.x-b->sx*.5f,0,b->c.z+b->sz*.5f)};
  int id=g_building12_ready?building12_find_seed(b->seed):-1;const V3*poly=box;int n=4;float area=b->sx*b->sz;
  if(id>=0){int pn=g_building_poly_count12[id],s=g_building_poly_start12[id];if(pn>=3&&pn<=112&&s>=0&&s+pn<=g_building_vert_count12){poly=g_building_verts12+s;n=pn;area=landmark506_area(poly,n);}}
  for(int k=0;k<LANDMARK506_COUNT;k++){
    const Landmark506*l=&g_landmarks506[k];V3 p=project_geo(l->lat,l->lon);float extent=(l->w+l->d)*.5f;
    if(absf(b->c.x-p.x)>b->sx*.5f+extent||absf(b->c.z-p.z)>b->sz*.5f+extent)continue;
    float overlap=landmark506_overlap(poly,n,k);if(overlap>bestOverlap&&landmark506_claims(overlap,area,k)){bestOverlap=overlap;best=k;}
  }return best;
}
static int landmark506_replaces(const Building*b){return landmark506_owner(b)>=0;}
// Any positive box intersection keeps the exact neighbor geometry (LOD box
// suppression is conservative on purpose; it never deletes a building).
static int landmark506_box_overlap(const Building*b){
  V3 v[4]={v3(b->c.x-b->sx*.5f,0,b->c.z-b->sz*.5f),v3(b->c.x+b->sx*.5f,0,b->c.z-b->sz*.5f),v3(b->c.x+b->sx*.5f,0,b->c.z+b->sz*.5f),v3(b->c.x-b->sx*.5f,0,b->c.z+b->sz*.5f)};
  for(int k=0;k<LANDMARK506_COUNT;k++){const Landmark506*l=&g_landmarks506[k];V3 p=project_geo(l->lat,l->lon);float extent=(l->w+l->d)*.5f;if(absf(b->c.x-p.x)>b->sx*.5f+extent||absf(b->c.z-p.z)>b->sz*.5f+extent)continue;if(landmark506_overlap(v,4,k)>LANDMARK506_MIN_OVERLAP)return 1;}return 0;
}
// Collision components follow the occupied volumes of the authored models
// (same local frames as world507_landmarks.inc). Courtyards and forecourts stay open.
typedef struct{int landmark;float x,z,w,d;} Landmark506Solid;
static const Landmark506Solid g_landmark506_solids[]={
 // Matriz: nave + towers, transept, choir, chancel, apse.
 {0,0,-21.65f,23.4f,17.3f},{0,.6f,-6.15f,31.2f,13.7f},{0,.85f,10.05f,30.7f,18.7f},{0,0,21.95f,14,5.1f},{0,0,27.3f,12.8f,5.6f},
 // Mercado Publico: south pavilion, west/east/north wings around the open courtyard.
 {1,0,-12.8f,21.4f,13.4f},{1,-5.5f,5,5,22.2f},{1,8,5,5,22.2f},{1,1.25f,14.1f,18.5f,4},
 // Palacio Marcos Konder: main block, link, rear wing, annex.
 {2,-.95f,-13.65f,15.9f,18.3f},{2,1.85f,1,6.5f,11},{2,-2.7f,15.55f,12.8f,19.9f},{2,7.1f,13.45f,6.8f,12.3f},
 {3,0,0,14.5f,15.7f},{4,0,0,15.9f,16.9f},{5,0,0,14.2f,28.2f},
 // Centreventos: hall, two east blocks beside the service notch, north and south wings.
 {6,3.5f,-7.5f,75,99},{6,30.65f,52.75f,20.7f,21.5f},{6,-22.3f,52.75f,23.4f,21.5f},{6,44.2f,14.5f,6.4f,97},{6,-40.7f,15,13.4f,96},
 {7,0,0,31,12},
 // Hotel Rota do Mar: main block, west block set back from Casa Konder, east annex.
 {8,.8f,0,47.4f,14},{8,-29.15f,-.2f,12.5f,8.7f},{8,29.95f,-3.85f,10.9f,6.3f}
};
#define LANDMARK506_SOLIDS 25
static void landmark506_colliders(void){for(int i=0;i<LANDMARK506_SOLIDS;i++){const Landmark506Solid*s=&g_landmark506_solids[i];const Landmark506*l=&g_landmarks506[s->landmark];V3 p=project_geo(l->lat,l->lon);float cs=(float)cos(l->yaw),sn=(float)sin(l->yaw);foundation_add_aabb(p.x+s->x*cs+s->z*sn,p.z-s->x*sn+s->z*cs,(absf(cs)*s->w+absf(sn)*s->d)*.5f,(absf(sn)*s->w+absf(cs)*s->d)*.5f,COLLIDER_BUILDING,-100-i);}}
static int landmark506_contact(int source,V3 p,float radius,Contact*out,int collider){
  int i=-100-source;if(i<0||i>=LANDMARK506_SOLIDS)return -1;const Landmark506Solid*s=&g_landmark506_solids[i];const Landmark506*l=&g_landmarks506[s->landmark];V3 q=landmark506_local(p,s->landmark);q.x-=s->x;q.z-=s->z;
  StaticCollider box;box.c=v3(0,0,0);box.hx=s->w*.5f;box.hz=s->d*.5f;
  if(!circle_aabb_contact(q,radius,&box,out,collider))return 0;
  float cs=(float)cos(l->yaw),sn=(float)sin(l->yaw);V3 n=out->normal;out->normal=v3(n.x*cs+n.z*sn,0,-n.x*sn+n.z*cs);return 1;
}
