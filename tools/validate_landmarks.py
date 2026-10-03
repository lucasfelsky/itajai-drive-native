"""Compile and exercise the production footprint and collision code with MSVC.

5.0.7: the registry carries surveyed OSM footprints and a relative ownership
rule (clipped overlap must be above 0.25 m2 AND at least 30% of the smaller of
the two areas). The tests cover rotated clipping, the relative rule against a
large neighbor with a small boundary overlap, the Casa Konder / Hotel Rota do
Mar adjacency, courtyard openness, world-space contact normals and the solid
component registration count.
"""
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]


def main():
    foundation = (ROOT / "src/foundation/foundation.inc").read_text()
    start = foundation.index("static int circle_aabb_contact(")
    end = foundation.index("\nstatic ", start)
    prelude = r'''
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef unsigned int u32;
typedef struct {float x,y,z;} V3;
typedef struct {V3 c;float sx,sz,h;u32 seed;} Building;
typedef struct {V3 c;float hx,hz;int type,source;} StaticCollider;
typedef struct {V3 normal;float penetration;int collider;} Contact;
static V3 v3(float x,float y,float z){V3 v={x,y,z};return v;}
static V3 vadd(V3 a,V3 b){return v3(a.x+b.x,a.y+b.y,a.z+b.z);}
static V3 vsub(V3 a,V3 b){return v3(a.x-b.x,a.y-b.y,a.z-b.z);}
static V3 vmul(V3 a,float s){return v3(a.x*s,a.y*s,a.z*s);}
static float absf(float x){return fabsf(x);}
static float clampf(float x,float a,float b){return fminf(b,fmaxf(a,x));}
static V3 project_geo(double lat,double lon){return v3((float)((lon+48.658)*111320*.8917),0,(float)(-(lat+26.907)*111320));}
static struct {int buildingCount;Building buildings[4];} g_world;
static int g_building12_ready=1,g_building_vert_count12=256;
static int g_building_poly_count12[4]={4,0,0,0},g_building_poly_start12[4]={0,0,0,0};
static V3 g_building_verts12[256];
static int building12_find_seed(u32 seed){for(int i=0;i<g_world.buildingCount;i++)if(g_world.buildings[i].seed==seed)return i;return -1;}
#define COLLIDER_BUILDING 1
static int added=0;
static int foundation_add_aabb(float x,float z,float hx,float hz,int type,int source){added++;return added;}
'''
    tests = r'''
static int checks=0;
static void check(int ok,const char*name){checks++;if(!ok){fprintf(stderr,"FAIL %s\n",name);exit(1);}}
static V3 world(float x,float z,int id){const Landmark506*l=&g_landmarks506[id];V3 c=project_geo(l->lat,l->lon);float cs=cosf(l->yaw),sn=sinf(l->yaw);return v3(c.x+x*cs+z*sn,0,c.z-x*sn+z*cs);}
static void rectangle(float x,float z,float w,float d,int id){g_building_verts12[0]=world(x-w/2,z-d/2,id);g_building_verts12[1]=world(x+w/2,z-d/2,id);g_building_verts12[2]=world(x+w/2,z+d/2,id);g_building_verts12[3]=world(x-w/2,z+d/2,id);}
static int solid_index(int landmark,int nth){for(int i=0;i<LANDMARK506_SOLIDS;i++)if(g_landmark506_solids[i].landmark==landmark&&nth--==0)return i;return -1;}
int main(void){
 for(int id=0;id<LANDMARK506_COUNT;id++){
  const Landmark506*l=&g_landmarks506[id];
  check(l->w>5&&l->d>5&&l->ref&&l->ref[0]&&l->conf>=0&&l->conf<=2,"registry entry is complete");
  rectangle(0,0,2,2,id);check(fabsf(landmark506_overlap(g_building_verts12,4,id)-4)<.01f,"contained rotated footprint");
  rectangle(0,0,300,300,id);check(fabsf(landmark506_overlap(g_building_verts12,4,id)-l->w*l->d)<.5f,"landmark inside larger footprint");
  rectangle(l->w/2+1,0,2,2,id);check(landmark506_overlap(g_building_verts12,4,id)<.01f,"shared boundary preserves neighbor");
  rectangle(l->w/2+1.1f,0,2,2,id);check(landmark506_overlap(g_building_verts12,4,id)==0,"separated neighbor");
  rectangle(0,0,300,1,id);check(fabsf(landmark506_overlap(g_building_verts12,4,id)-l->w)<.1f,"crossing edges without contained vertices");
  check(landmark506_claims(4,4,id),"small building fully inside is claimed");
  check(!landmark506_claims(.2f,.2f,id),"sub-tolerance sliver is never claimed");
  check(!landmark506_claims(30,900,id),"large neighbor with 30 m2 boundary overlap is kept");
  check(landmark506_claims(l->w*l->d*.9f,l->w*l->d*4,id),"block polygon covering the landmark is claimed");
 }
 g_world.buildingCount=1;Building*b=&g_world.buildings[0];b->c=world(0,0,0);b->sx=100;b->sz=100;b->seed=42;
 rectangle(0,0,2,2,0);check(landmark506_owner(b)==0,"exact source ownership");
 rectangle(30,0,2,2,0);check(!landmark506_replaces(b),"exact polygon beats oversized bounding box");check(landmark506_box_overlap(b),"LOD box must retain exact geometry near landmark");
 // A 70 x 14 neighbor whose exact polygon only crosses the boundary by 2.5 m survives.
 rectangle(0,g_landmarks506[0].d/2+7-2.5f,70,14,0);check(!landmark506_replaces(b),"large neighbor crossing boundary by 2.5 m is preserved");
 rectangle(0,g_landmarks506[0].d/2-2,20,16,0);check(landmark506_owner(b)==0,"neighbor mostly inside the footprint is replaced");
 g_building12_ready=0;b->sx=2;b->sz=2;check(landmark506_owner(b)==0,"missing sidecar fallback");
 b->sx=90;b->sz=90;b->c=world(0,g_landmarks506[0].d/2+40,0);check(!landmark506_replaces(b),"missing sidecar: large box with small overlap is kept");g_building12_ready=1;
 // Casa Konder (3) and Hotel Rota do Mar (8): each cadastral polygon goes to its own entry.
 {const Landmark506*ck=&g_landmarks506[3],*ho=&g_landmarks506[8];Building*casa=&g_world.buildings[0],*hotel=&g_world.buildings[1];g_world.buildingCount=2;
  casa->c=world(0,0,3);casa->sx=casa->sz=16;casa->seed=42;g_building_poly_start12[0]=0;g_building_poly_count12[0]=4;rectangle(0,0,ck->w,ck->d,3);
  hotel->c=world(0,0,8);hotel->sx=72;hotel->sz=20;hotel->seed=43;g_building_poly_start12[1]=4;g_building_poly_count12[1]=4;
  V3*hv=g_building_verts12+4;hv[0]=world(-ho->w/2,-ho->d/2,8);hv[1]=world(ho->w/2,-ho->d/2,8);hv[2]=world(ho->w/2,ho->d/2,8);hv[3]=world(-ho->w/2,ho->d/2,8);
  check(landmark506_owner(casa)==3,"Casa Konder polygon owned by Casa Konder entry");
  check(landmark506_owner(hotel)==8,"Hotel Rota do Mar polygon owned by hotel entry, not deleted by Casa Konder");
  g_world.buildingCount=1;g_building_poly_start12[0]=0;g_building_poly_count12[0]=4;}
 Contact hit;
 // Mercado Publico (1): open courtyard, solid wings.
 {int wing=solid_index(1,1),court=0;for(int i=0;i<LANDMARK506_SOLIDS;i++){if(g_landmark506_solids[i].landmark!=1)continue;court+=landmark506_contact(-100-i,world(1.25f,3,1),.5f,&hit,7)>0;}
  check(court==0,"market courtyard has no invisible collider");check(landmark506_contact(-100-wing,world(-5.5f,5,1),.5f,&hit,7)==1,"market wing collides");}
 // Matriz nave wall: rotated contact and world-space normal.
 {int nave=solid_index(0,0);const Landmark506Solid*s=&g_landmark506_solids[nave];float wall=s->x+s->w*.5f;
  check(landmark506_contact(-100-nave,world(wall+.2f,s->z,0),.5f,&hit,9)==1,"rotated nave wall contact");
  check(fabsf(hit.penetration-.3f)<.002f&&hit.collider==9,"contact depth and identity");
  check(fabsf(hit.normal.x-cosf(g_landmarks506[0].yaw))<.001f&&fabsf(hit.normal.z+sinf(g_landmarks506[0].yaw))<.001f,"normal rotates to world space");
  check(landmark506_contact(-100-nave,world(wall+.6f,s->z+s->d*.5f+.6f,0),.5f,&hit,9)==0,"empty rotated corner remains drivable");}
 // Matriz forecourt and Centreventos service notch stay open.
 {int open=0;for(int i=0;i<LANDMARK506_SOLIDS;i++){if(g_landmark506_solids[i].landmark==0)open+=landmark506_contact(-100-i,world(0,-40,0),.5f,&hit,7)>0;if(g_landmark506_solids[i].landmark==6)open+=landmark506_contact(-100-i,world(5,55,6),.5f,&hit,7)>0;}
  check(open==0,"forecourt and service notch are drivable");}
 check(landmark506_contact(-99,world(0,0,1),.5f,&hit,9)==-1,"unknown collider keeps fallback");
 check(landmark506_contact(-100-LANDMARK506_SOLIDS,world(0,0,1),.5f,&hit,9)==-1,"out-of-range collider keeps fallback");
 for(int i=0;i<LANDMARK506_SOLIDS;i++){const Landmark506Solid*s=&g_landmark506_solids[i];check(s->landmark>=0&&s->landmark<LANDMARK506_COUNT&&s->w>0&&s->d>0,"solid references a registered landmark");}
 landmark506_colliders();check(added==LANDMARK506_SOLIDS,"all occupied components registered");
 printf("PASS %d landmark ownership / collision checks\n",checks);return 0;
}
'''
    with tempfile.TemporaryDirectory(prefix="itajai-landmarks-") as temp:
        temp = Path(temp)
        source = temp / "test.c"
        inc = (ROOT / "src/world/landmarks507.inc").as_posix()
        source.write_text(prelude + foundation[start:end] + f'\n#include "{inc}"\n' + tests)
        subprocess.run(["cl", "/nologo", "/TC", "/O2", str(source), f"/Fo{temp / 'test.obj'}", f"/Fe{temp / 'test.exe'}"], check=True, cwd=temp)
        subprocess.run([str(temp / "test.exe")], check=True)


if __name__ == "__main__":
    main()
