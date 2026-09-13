// -----------------------------------------------------------------------------
// Foundation 0.5: spatial grid, collision world, physics body and surfaces
// 4.0 capacity pass: sized for full-municipality buildings and road streaming.
// -----------------------------------------------------------------------------
#define GRID_BUCKETS 131072
#define MAX_GRID_CELLS 131072
#define MAX_GRID_COLLIDER_LINKS 700000
#define MAX_GRID_ROAD_LINKS 760000
#define MAX_STATIC_COLLIDERS (MAX_BUILDINGS+MAX_SIGNALS+64)
#define FOUNDATION_QUERY_MAX 512
#define GRID_CELL_SIZE 40.0f

enum { COLLIDER_BUILDING=1, COLLIDER_SIGNAL=2 };
enum { SURFACE_ASPHALT=0, SURFACE_CURB=1, SURFACE_GRASS=2 };

typedef struct { V3 c; float hx,hz; int type,source; } StaticCollider;
typedef struct { int x,z,colliderHead,roadHead,nextHash; } GridCell;
typedef struct { int item,next; } GridLink;
typedef struct {
  V3 p,vel;
  float yaw,angularVel;
  float mass,radius,restitution,grip;
  int surface;
} PhysicsBody;

typedef struct { V3 normal; float penetration; int collider; } Contact;

static StaticCollider g_static_colliders[MAX_STATIC_COLLIDERS];
static int g_static_collider_count=0;
static GridCell g_grid_cells[MAX_GRID_CELLS];
static int g_grid_cell_count=0;
static int g_grid_buckets[GRID_BUCKETS];
static GridLink g_grid_collider_links[MAX_GRID_COLLIDER_LINKS];
static int g_grid_collider_link_count=0;
static GridLink g_grid_road_links[MAX_GRID_ROAD_LINKS];
static int g_grid_road_link_count=0;
static u32 g_collider_seen[MAX_STATIC_COLLIDERS];
static u32 g_segment_seen[MAX_SEGMENTS];
static u32 g_foundation_query_stamp=1;
static int g_foundation_queries=0;
static int g_foundation_candidates=0;
static int g_foundation_contacts=0;
static int g_foundation_dynamic_contacts=0;
static int g_foundation_last_candidates=0;
static float g_foundation_last_road_distance=9999.0f;

static int grid_coord(float v){int q=(int)(v/GRID_CELL_SIZE);if(v<0.0f&&(float)q*GRID_CELL_SIZE!=v)q--;return q;}
static u32 grid_hash(int x,int z){return ((u32)(x*73856093)^(u32)(z*19349663))&(GRID_BUCKETS-1);}
static void foundation_clear(void){
  g_static_collider_count=0;g_grid_cell_count=0;g_grid_collider_link_count=0;g_grid_road_link_count=0;
  g_foundation_queries=0;g_foundation_candidates=0;g_foundation_contacts=0;g_foundation_dynamic_contacts=0;g_foundation_last_candidates=0;
  for(int i=0;i<GRID_BUCKETS;i++)g_grid_buckets[i]=-1;
  memset(g_collider_seen,0,sizeof(g_collider_seen));memset(g_segment_seen,0,sizeof(g_segment_seen));g_foundation_query_stamp=1;
}
static int grid_find_cell(int x,int z,int create){u32 h=grid_hash(x,z);for(int i=g_grid_buckets[h];i>=0;i=g_grid_cells[i].nextHash)if(g_grid_cells[i].x==x&&g_grid_cells[i].z==z)return i;if(!create||g_grid_cell_count>=MAX_GRID_CELLS)return -1;int id=g_grid_cell_count++;GridCell*c=&g_grid_cells[id];c->x=x;c->z=z;c->colliderHead=-1;c->roadHead=-1;c->nextHash=g_grid_buckets[h];g_grid_buckets[h]=id;return id;}
static void grid_insert_collider_cell(int cell,int collider){if(cell<0||g_grid_collider_link_count>=MAX_GRID_COLLIDER_LINKS)return;int id=g_grid_collider_link_count++;g_grid_collider_links[id].item=collider;g_grid_collider_links[id].next=g_grid_cells[cell].colliderHead;g_grid_cells[cell].colliderHead=id;}
static void grid_insert_road_cell(int cell,int segment){if(cell<0||g_grid_road_link_count>=MAX_GRID_ROAD_LINKS)return;int id=g_grid_road_link_count++;g_grid_road_links[id].item=segment;g_grid_road_links[id].next=g_grid_cells[cell].roadHead;g_grid_cells[cell].roadHead=id;}
static int foundation_add_aabb(float cx,float cz,float hx,float hz,int type,int source){if(g_static_collider_count>=MAX_STATIC_COLLIDERS)return -1;int id=g_static_collider_count++;StaticCollider*c=&g_static_colliders[id];c->c=v3(cx,0,cz);c->hx=maxf(.08f,hx);c->hz=maxf(.08f,hz);c->type=type;c->source=source;int x0=grid_coord(cx-c->hx),x1=grid_coord(cx+c->hx),z0=grid_coord(cz-c->hz),z1=grid_coord(cz+c->hz);for(int z=z0;z<=z1;z++)for(int x=x0;x<=x1;x++)grid_insert_collider_cell(grid_find_cell(x,z,1),id);return id;}
static void foundation_index_road(int si){RoadSegment*s=&g_world.segs[si];float hw=g_world.roads[s->road].width*.5f+2.0f;float minx=minf(s->a.x,s->b.x)-hw,maxx=maxf(s->a.x,s->b.x)+hw,minz=minf(s->a.z,s->b.z)-hw,maxz=maxf(s->a.z,s->b.z)+hw;int x0=grid_coord(minx),x1=grid_coord(maxx),z0=grid_coord(minz),z1=grid_coord(maxz);for(int z=z0;z<=z1;z++)for(int x=x0;x<=x1;x++)grid_insert_road_cell(grid_find_cell(x,z,1),si);}
static void foundation_build_static(void){foundation_clear();for(int i=0;i<g_world.buildingCount;i++){Building*b=&g_world.buildings[i];if(landmark506_replaces(b))continue;foundation_add_aabb(b->c.x,b->c.z,b->sx*.5f+.10f,b->sz*.5f+.10f,COLLIDER_BUILDING,i);}landmark506_colliders();for(int i=0;i<g_world.signalCount;i++){Signal*s=&g_world.signals[i];foundation_add_aabb(s->p.x,s->p.z,.24f,.24f,COLLIDER_SIGNAL,i);}for(int i=0;i<g_world.segCount;i++)foundation_index_road(i);}
static u32 foundation_next_stamp(void){g_foundation_query_stamp++;if(g_foundation_query_stamp==0){memset(g_collider_seen,0,sizeof(g_collider_seen));memset(g_segment_seen,0,sizeof(g_segment_seen));g_foundation_query_stamp=1;}return g_foundation_query_stamp;}
static int foundation_query_colliders(V3 p,float radius,int*out,int cap){u32 stamp=foundation_next_stamp();int n=0,candidates=0;int x0=grid_coord(p.x-radius),x1=grid_coord(p.x+radius),z0=grid_coord(p.z-radius),z1=grid_coord(p.z+radius);for(int z=z0;z<=z1;z++)for(int x=x0;x<=x1;x++){int ci=grid_find_cell(x,z,0);if(ci<0)continue;for(int li=g_grid_cells[ci].colliderHead;li>=0;li=g_grid_collider_links[li].next){int item=g_grid_collider_links[li].item;if(item<0||item>=g_static_collider_count||g_collider_seen[item]==stamp)continue;g_collider_seen[item]=stamp;candidates++;if(n<cap)out[n++]=item;}}g_foundation_queries++;g_foundation_candidates+=candidates;g_foundation_last_candidates=candidates;return n;}
static float point_segment_distance_xz(V3 p,V3 a,V3 b){V3 ab=vsub(b,a),ap=vsub(p,a);ab.y=0;ap.y=0;float den=vdot(ab,ab);float t=den>.0001f?clampf(vdot(ap,ab)/den,0,1):0;V3 q=vadd(a,vmul(ab,t)),d=vsub(p,q);d.y=0;return vlen(d);}
static int foundation_nearest_road(V3 p,int*roadOut,float*centerDistanceOut){u32 stamp=foundation_next_stamp();float best=1e30f;int bestRoad=-1;int cx=grid_coord(p.x),cz=grid_coord(p.z);for(int ring=0;ring<=2&&best>GRID_CELL_SIZE*ring;ring++){for(int z=cz-ring;z<=cz+ring;z++)for(int x=cx-ring;x<=cx+ring;x++){if(ring>0&&x>cx-ring&&x<cx+ring&&z>cz-ring&&z<cz+ring)continue;int ci=grid_find_cell(x,z,0);if(ci<0)continue;for(int li=g_grid_cells[ci].roadHead;li>=0;li=g_grid_road_links[li].next){int si=g_grid_road_links[li].item;if(si<0||si>=g_world.segCount||g_segment_seen[si]==stamp)continue;g_segment_seen[si]=stamp;RoadSegment*s=&g_world.segs[si];float d=point_segment_distance_xz(p,s->a,s->b);if(d<best){best=d;bestRoad=s->road;}}}}if(roadOut)*roadOut=bestRoad;if(centerDistanceOut)*centerDistanceOut=best;g_foundation_last_road_distance=best;return bestRoad>=0;}
static int foundation_surface_at(V3 p,int*roadOut,float*edgeDistanceOut){float center=9999;int road=-1;foundation_nearest_road(p,&road,&center);float edge=9999;if(road>=0){edge=center-g_world.roads[road].width*.5f;if(edge<=.12f){if(edgeDistanceOut)*edgeDistanceOut=edge;if(roadOut)*roadOut=road;return SURFACE_ASPHALT;}if(edge<=1.05f){if(edgeDistanceOut)*edgeDistanceOut=edge;if(roadOut)*roadOut=road;return SURFACE_CURB;}}if(edgeDistanceOut)*edgeDistanceOut=edge;if(roadOut)*roadOut=road;return SURFACE_GRASS;}
static int circle_aabb_contact(V3 p,float radius,const StaticCollider*c,Contact*out,int colliderId){float minx=c->c.x-c->hx,maxx=c->c.x+c->hx,minz=c->c.z-c->hz,maxz=c->c.z+c->hz;float qx=clampf(p.x,minx,maxx),qz=clampf(p.z,minz,maxz);float dx=p.x-qx,dz=p.z-qz,ds=dx*dx+dz*dz;if(ds>=radius*radius)return 0;V3 n=v3(0,0,0);float pen=0;if(ds>.000001f){float d=(float)sqrt((double)ds);n=v3(dx/d,0,dz/d);pen=radius-d;}else{float left=p.x-minx,right=maxx-p.x,top=p.z-minz,bottom=maxz-p.z;float m=left;n=v3(-1,0,0);pen=radius+left;if(right<m){m=right;n=v3(1,0,0);pen=radius+right;}if(top<m){m=top;n=v3(0,0,-1);pen=radius+top;}if(bottom<m){n=v3(0,0,1);pen=radius+bottom;}}out->normal=n;out->penetration=pen;out->collider=colliderId;return 1;}
static void physics_body_init(PhysicsBody*b,V3 p,float radius,float mass){memset(b,0,sizeof(*b));b->p=p;b->radius=radius;b->mass=maxf(1,mass);b->restitution=.08f;b->grip=1.0f;b->surface=SURFACE_ASPHALT;}
static int physics_resolve_static(PhysicsBody*b,float*longSpeed){static int ids[FOUNDATION_QUERY_MAX];int total=0;for(int pass=0;pass<3;pass++){int n=foundation_query_colliders(b->p,b->radius+1.0f,ids,FOUNDATION_QUERY_MAX),hitThis=0;for(int i=0;i<n;i++){StaticCollider*c=&g_static_colliders[ids[i]];Contact ct;if(!circle_aabb_contact(b->p,b->radius,c,&ct,ids[i]))continue;b->p=vadd(b->p,vmul(ct.normal,ct.penetration+.002f));float vn=vdot(b->vel,ct.normal);if(vn<0)b->vel=vsub(b->vel,vmul(ct.normal,(1.0f+b->restitution)*vn));if(longSpeed){V3 f=v3((float)sin(b->yaw),0,(float)cos(b->yaw));float head=absf(vdot(f,ct.normal));float keep=clampf(.72f-head*.62f,.08f,.72f);*longSpeed*=keep;if(absf(*longSpeed)<.25f)*longSpeed=0;}hitThis++;total++;}if(!hitThis)break;}g_foundation_contacts+=total;return total;}
static const char* foundation_surface_name(int s){return s==SURFACE_ASPHALT?"ASFALTO":s==SURFACE_CURB?"MEIO-FIO / CALCADA":"GRAMA / FORA DA VIA";}
static void foundation_draw_debug(V3 player){int cx=grid_coord(player.x),cz=grid_coord(player.z);glLineWidth(1.0f);glColor4f(.12f,.82f,1.0f,.55f);glBegin(GL_LINES);for(int i=-3;i<=4;i++){float x=(cx+i)*GRID_CELL_SIZE;glVertex3f(x,.08f,(cz-3)*GRID_CELL_SIZE);glVertex3f(x,.08f,(cz+4)*GRID_CELL_SIZE);}for(int i=-3;i<=4;i++){float z=(cz+i)*GRID_CELL_SIZE;glVertex3f((cx-3)*GRID_CELL_SIZE,.08f,z);glVertex3f((cx+4)*GRID_CELL_SIZE,.08f,z);}glEnd();g_draw_calls++;
  static int ids[FOUNDATION_QUERY_MAX];int n=foundation_query_colliders(player,110.0f,ids,FOUNDATION_QUERY_MAX);glColor4f(1.0f,.28f,.12f,.85f);glBegin(GL_LINES);for(int i=0;i<n;i++){StaticCollider*c=&g_static_colliders[ids[i]];float x0=c->c.x-c->hx,x1=c->c.x+c->hx,z0=c->c.z-c->hz,z1=c->c.z+c->hz,y=.12f;glVertex3f(x0,y,z0);glVertex3f(x1,y,z0);glVertex3f(x1,y,z0);glVertex3f(x1,y,z1);glVertex3f(x1,y,z1);glVertex3f(x0,y,z1);glVertex3f(x0,y,z1);glVertex3f(x0,y,z0);}glEnd();g_draw_calls++;}