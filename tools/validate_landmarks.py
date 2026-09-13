"""Compile and exercise the production footprint and collision code with MSVC."""
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
static struct {int buildingCount;Building buildings[2];} g_world;
static int g_building12_ready=1,g_building_vert_count12=4;
static int g_building_poly_count12[2]={4,0},g_building_poly_start12[2]={0,0};
static V3 g_building_verts12[128];
static int building12_find_seed(u32 seed){return seed==42?0:-1;}
#define COLLIDER_BUILDING 1
static int added=0;
static int foundation_add_aabb(float x,float z,float hx,float hz,int type,int source){added++;return added;}
'''
    tests = r'''
static int checks=0;
static void check(int ok,const char*name){checks++;if(!ok){fprintf(stderr,"FAIL %s\n",name);exit(1);}}
static V3 world(float x,float z,int id){const Landmark506*l=&g_landmarks506[id];V3 c=project_geo(l->lat,l->lon);float cs=cosf(l->yaw),sn=sinf(l->yaw);return v3(c.x+x*cs+z*sn,0,c.z-x*sn+z*cs);}
static void rectangle(float x,float z,float w,float d,int id){g_building_verts12[0]=world(x-w/2,z-d/2,id);g_building_verts12[1]=world(x+w/2,z-d/2,id);g_building_verts12[2]=world(x+w/2,z+d/2,id);g_building_verts12[3]=world(x-w/2,z+d/2,id);}
int main(void){
 for(int id=0;id<LANDMARK506_COUNT;id++){
  rectangle(0,0,2,2,id);check(fabsf(landmark506_overlap(g_building_verts12,4,id)-4)<.01f,"contained rotated footprint");
  rectangle(0,0,300,300,id);check(fabsf(landmark506_overlap(g_building_verts12,4,id)-g_landmarks506[id].w*g_landmarks506[id].d)<.1f,"landmark inside larger footprint");
  rectangle(g_landmarks506[id].w/2+1,0,2,2,id);check(landmark506_overlap(g_building_verts12,4,id)<.01f,"shared boundary preserves neighbor");
  rectangle(g_landmarks506[id].w/2+1.1f,0,2,2,id);check(landmark506_overlap(g_building_verts12,4,id)==0,"separated neighbor");
  rectangle(0,0,300,1,id);check(fabsf(landmark506_overlap(g_building_verts12,4,id)-g_landmarks506[id].w)<.1f,"crossing edges without contained vertices");
 }
 g_world.buildingCount=1;Building*b=&g_world.buildings[0];b->c=world(0,0,0);b->sx=100;b->sz=100;b->seed=42;
 rectangle(0,0,2,2,0);check(landmark506_owner(b)==0,"exact source ownership");
 rectangle(30,0,2,2,0);check(!landmark506_replaces(b),"exact polygon beats oversized bounding box");check(landmark506_box_overlap(b),"LOD box must retain exact geometry near landmark");
 g_building12_ready=0;b->sx=2;b->sz=2;check(landmark506_owner(b)==0,"missing sidecar fallback");g_building12_ready=1;
 Contact hit;V3 center=world(0,0,1);int count=0;
 for(int i=4;i<8;i++)count+=landmark506_contact(-100-i,center,.5f,&hit,7)>0;
 check(count==0,"market courtyard has no invisible collider");
 check(landmark506_contact(-104,world(0,-7,1),.5f,&hit,7)==1,"market wall collides");
 check(landmark506_contact(-100,world(9.7f,0,0),.5f,&hit,9)==1,"rotated nave wall contact");
 check(fabsf(hit.penetration-.3f)<.002f&&hit.collider==9,"contact depth and identity");
 check(fabsf(hit.normal.x-cosf(-.1f))<.001f&&fabsf(hit.normal.z+sinf(-.1f))<.001f,"normal rotates to world space");
 check(landmark506_contact(-100,world(9.9f,17.4f,0),.5f,&hit,9)==0,"empty rotated corner remains drivable");
 check(landmark506_contact(-99,center,.5f,&hit,9)==-1,"unknown collider keeps fallback");
 landmark506_colliders();check(added==16,"all occupied components registered");
 printf("PASS %d landmark ownership / collision checks\n",checks);return 0;
}
'''
    with tempfile.TemporaryDirectory(prefix="itajai-landmarks-") as temp:
        temp = Path(temp)
        source = temp / "test.c"
        inc = (ROOT / "src/world/landmarks506.inc").as_posix()
        source.write_text(prelude + foundation[start:end] + f'\n#include "{inc}"\n' + tests)
        subprocess.run(["cl", "/nologo", "/TC", "/O2", str(source), f"/Fo{temp / 'test.obj'}", f"/Fe{temp / 'test.exe'}"], check=True, cwd=temp)
        subprocess.run([str(temp / "test.exe")], check=True)


if __name__ == "__main__":
    main()
