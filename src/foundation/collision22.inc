// -----------------------------------------------------------------------------
// 2.2 QA Fix Pass: exact building-footprint narrow phase.
// The existing AABBs remain the cheap spatial-grid broad phase; buildings that
// have OSM polygons use circle-vs-polygon contact so empty AABB corners no longer
// behave like invisible walls. Buildings without a sidecar polygon keep the old
// AABB fallback.
// -----------------------------------------------------------------------------
static int collision22_point_in_poly(int source,V3 p){
  if(source<0||source>=g_world.buildingCount)return 0;int n=(int)g_building_poly_count12[source];if(n<3)return 0;int start=g_building_poly_start12[source],inside=0;
  for(int i=0,j=n-1;i<n;j=i++){
    V3 a=g_building_verts12[start+i],b=g_building_verts12[start+j];
    int cross=((a.z>p.z)!=(b.z>p.z));
    if(cross){float den=b.z-a.z;if(absf(den)<.000001f)den=den<0?-.000001f:.000001f;float x=a.x+(p.z-a.z)*(b.x-a.x)/den;if(p.x<x)inside=!inside;}
  }
  return inside;
}

// Returns -1 when this building has no exact polygon, 0 for no contact, 1 hit.
static int collision22_building_contact(int source,V3 p,float radius,Contact*out,int colliderId){
  if(source<=-100)return landmark506_contact(source,p,radius,out,colliderId);
  if(source<0||source>=g_world.buildingCount)return -1;int n=(int)g_building_poly_count12[source];if(n<3)return -1;int start=g_building_poly_start12[source];
  float best=1e30f;V3 nearest=p;int bestEdge=0;
  for(int i=0;i<n;i++){
    V3 a=g_building_verts12[start+i],b=g_building_verts12[start+((i+1)%n)];V3 ab=vsub(b,a),ap=vsub(p,a);ab.y=0;ap.y=0;float den=vdot(ab,ab);float t=den>.000001f?clampf(vdot(ap,ab)/den,0,1):0;V3 q=vadd(a,vmul(ab,t));float dx=p.x-q.x,dz=p.z-q.z,ds=dx*dx+dz*dz;if(ds<best){best=ds;nearest=q;bestEdge=i;}
  }
  int inside=collision22_point_in_poly(source,p);float dist=(float)sqrt((double)maxf(best,0));if(!inside&&dist>=radius)return 0;
  V3 normal=v3(0,0,0);float pen=0;
  if(dist>.0001f){
    if(inside){normal=v3((nearest.x-p.x)/dist,0,(nearest.z-p.z)/dist);pen=radius+dist;}
    else{normal=v3((p.x-nearest.x)/dist,0,(p.z-nearest.z)/dist);pen=radius-dist;}
  }else{
    V3 a=g_building_verts12[start+bestEdge],b=g_building_verts12[start+((bestEdge+1)%n)],e=vsub(b,a);float el=maxf(.0001f,(float)sqrt((double)(e.x*e.x+e.z*e.z)));V3 n0=v3(-e.z/el,0,e.x/el);V3 test=vadd(p,vmul(n0,.08f));if(collision22_point_in_poly(source,test))n0=vmul(n0,-1);normal=n0;pen=radius+.002f;
  }
  out->normal=normal;out->penetration=maxf(.001f,pen);out->collider=colliderId;return 1;
}

static int physics_resolve_static22(PhysicsBody*b,float*longSpeed){
  static int ids[FOUNDATION_QUERY_MAX];int total=0;for(int pass=0;pass<3;pass++){
    int n=foundation_query_colliders(b->p,b->radius+1.0f,ids,FOUNDATION_QUERY_MAX),hitThis=0;
    for(int i=0;i<n;i++){
      StaticCollider*c=&g_static_colliders[ids[i]];Contact ct;int hit=-1;
      if(c->type==COLLIDER_BUILDING&&(g_building12_ready||c->source<=-100))hit=collision22_building_contact(c->source,b->p,b->radius,&ct,ids[i]);
      if(hit<0)hit=circle_aabb_contact(b->p,b->radius,c,&ct,ids[i]);
      if(!hit)continue;
      b->p=vadd(b->p,vmul(ct.normal,ct.penetration+.002f));float vn=vdot(b->vel,ct.normal);if(vn<0)b->vel=vsub(b->vel,vmul(ct.normal,(1.0f+b->restitution)*vn));
      if(longSpeed){V3 f=v3((float)sin(b->yaw),0,(float)cos(b->yaw));float head=absf(vdot(f,ct.normal));float keep=clampf(.72f-head*.62f,.08f,.72f);*longSpeed*=keep;if(absf(*longSpeed)<.25f)*longSpeed=0;}
      hitThis++;total++;
    }
    if(!hitThis)break;
  }
  g_foundation_contacts+=total;return total;
}
