// -----------------------------------------------------------------------------
// Renderer 5.0.7: one authored model per registered Centro building, drawn in
// the registry's local frame (center, facade yaw, extents from landmarks507.inc).
// Local -Z is the street facade. This file replaces the landmark drawing that
// used to be spread over city340/city370/world503/world504/world505; nothing
// else draws these buildings. Masses stay visible to ~1.3 km, openings and trim
// are added inside 380 m, fine trim inside 170 m.
// -----------------------------------------------------------------------------
#include "architecture507.inc"
static int g_world507_buildings=0,g_world507_detail=0,g_world507_context=0;
typedef struct{V3 c;float y,yaw,cs,sn;int near,fine,dark;} Lm507;
static Lm507 lm507_frame(int id){Lm507 f;const Landmark506*l=&g_landmarks506[id];f.c=project_geo(l->lat,l->lon);f.y=terrain11_height(f.c.x,f.c.z);f.yaw=l->yaw;f.cs=(float)cos(l->yaw);f.sn=(float)sin(l->yaw);f.near=city340_near(f.c,380);f.fine=city340_near(f.c,170);f.dark=atmos280_dark();return f;}
static V3 lm507_at(const Lm507*f,float x,float y,float z){return v3(f->c.x+x*f->cs+z*f->sn,f->y+y,f->c.z-x*f->sn+z*f->cs);}
// Box whose bottom sits at local height y.
static void lm507_box(const Lm507*f,float x,float y,float z,float w,float h,float d,float r,float g,float b){draw_box(lm507_at(f,x,y+h*.5f,z),w,h,d,f->yaw,r,g,b);}
// Face frames: 0 = -Z (facade), 1 = +Z (rear), 2 = +X side, 3 = -X side. u runs
// along the wall, v is the wall plane offset, out moves away from the building.
static V3 lm507_face(const Lm507*f,int face,float u,float y,float v,float out,float*yaw){
  float x,z;if(face==0){x=u;z=v-out;*yaw=f->yaw;}else if(face==1){x=-u;z=v+out;*yaw=f->yaw+3.14159265f;}else if(face==2){x=v+out;z=u;*yaw=f->yaw-1.5707963f;}else{x=v-out;z=-u;*yaw=f->yaw+1.5707963f;}
  return lm507_at(f,x,y,z);
}
static void lm507_arch(const Lm507*f,int face,float u,float y,float v,float w,float h,float r,float g,float b){float yaw;V3 p=lm507_face(f,face,u,y,v,0,&yaw);architecture506_arch(p,w,h,yaw,r,g,b);g_world507_detail++;}
// Rectangular window: pale frame slightly proud of the wall, glazed pane in front.
static void lm507_window(const Lm507*f,int face,float u,float y,float v,float w,float h,int lit,float fr,float fg,float fb){
  float yaw;V3 p=lm507_face(f,face,u,y+h*.5f,v,.05f,&yaw);draw_box(p,w+.30f,h+.30f,.08f,yaw,fr,fg,fb);
  renderer09_set_material(MAT09_GLASS);p=lm507_face(f,face,u,y+h*.5f,v,.11f,&yaw);draw_box(p,w,h,.06f,yaw,lit?.80f:.12f,lit?.66f:.20f,lit?.34f:.24f);renderer09_set_material(MAT09_GENERIC);g_world507_detail++;
}
static void lm507_wallbox(const Lm507*f,int face,float u,float y,float v,float out,float w,float h,float d,float r,float g,float b){float yaw;V3 p=lm507_face(f,face,u,y+h*.5f,v,out,&yaw);draw_box(p,w,h,d,yaw,r,g,b);}
static void lm507_door(const Lm507*f,int face,float u,float y,float v,float w,float h){lm507_wallbox(f,face,u,y,v,.04f,w+.36f,h+.22f,.10f,.90f,.88f,.80f);lm507_wallbox(f,face,u,y,v,.09f,w,h,.08f,.27f,.21f,.16f);g_world507_detail++;}
static void lm507_text(const Lm507*f,int face,float u,float y,float v,const char*s,float r,float g,float b){float yaw;V3 p=lm507_face(f,face,u,y,v,.16f,&yaw);renderer09_suspend_shader();identity150_text3d(p,s,r,g,b);renderer09_resume_shader();}
static int lm507_lit(const Lm507*f,int k){return f->dark&&((k*7+3)%5==0);}
static void lm507_plinth(const Lm507*f,float x,float z,float w,float d,float h){lm507_box(f,x,0,z,w+.16f,h,d+.16f,.46f,.44f,.40f);}

// ---------------------------------------------------------------- Matriz ----
// Igreja Matriz do Santissimo Sacramento. OSM polygon: 60.9 x 32.6 m, apse at the
// rear, twin tower blocks flanking a recessed central bay, side chapels. Facade
// faces Praca Governador Irineu Bornhausen (NNW). Heights are estimates.
static void world507_matriz(void){
  Lm507 f=lm507_frame(0);if(!city340_near(f.c,1400))return;renderer09_set_material(0);
  const float wr=.91f,wg=.91f,wb=.88f,tr=.95f,tg=.95f,tb=.92f;
  lm507_box(&f,0,0,-20.5f,23.4f,14,15,wr,wg,wb);                       // nave body (z -28..-13)
  lm507_box(&f,.6f,0,-6.15f,31.2f,14,13.7f,wr,wg,wb);                   // transept
  lm507_box(&f,.85f,0,10.05f,30.7f,12.5f,18.7f,wr,wg,wb);               // choir and side chapels
  lm507_box(&f,0,0,21.95f,14,12.5f,5.1f,wr,wg,wb);                      // chancel
  architecture507_apse(lm507_at(&f,0,0,24.5f),6.4f,11.2f,4.6f,f.yaw,wr,wg,wb,.52f,.31f,.20f);
  architecture506_roof(lm507_at(&f,0,14,-20.5f),24,15.2f,5,f.yaw);
  architecture506_roof(lm507_at(&f,.6f,14,-6.15f),13.9f,31.6f,4.6f,f.yaw+1.5707963f);
  architecture506_roof(lm507_at(&f,.85f,12.5f,10.05f),31,18.9f,4.8f,f.yaw);
  architecture506_roof(lm507_at(&f,0,12.5f,21.95f),14,5.3f,3.2f,f.yaw);
  // Towers protrude 2.3 m in front of the recessed central bay.
  for(int s=-1;s<=1;s+=2){float x=s*8.6f;lm507_box(&f,x,0,-27.2f,6.2f,27,6.2f,tr,tg,tb);for(int k=0;k<3;k++)lm507_box(&f,x,9+k*7.2f,-27.2f,6.7f,.34f,6.7f,.80f,.80f,.76f);lm507_box(&f,x,27,-27.2f,6.9f,.5f,6.9f,.80f,.80f,.76f);
    city340_pyramid(lm507_at(&f,x,27.5f,-27.2f),6.1f,6.1f,9.5f,f.yaw,.55f,.48f,.38f);city340_cross(lm507_at(&f,x,37,-27.2f),1.8f);}
  lm507_box(&f,0,0,-27.7f,10.6f,19,.6f,tr,tg,tb);                      // central bay to the pediment base
  city340_pyramid(lm507_at(&f,0,19,-27.7f),10.6f,1.5f,3.6f,f.yaw,.90f,.90f,.87f);
  city340_cross(lm507_at(&f,0,22.6f,-27.7f),1.1f);
  if(f.near){
    for(int portal=-1;portal<=1;portal++)lm507_arch(&f,0,portal*3.1f,.25f,-28,2.45f,5.7f,.20f,.18f,.15f);
    architecture506_rose(lm507_at(&f,0,12.9f,-28.05f),1.9f,f.yaw);
    architecture506_clock(lm507_at(&f,0,17.2f,-28.05f),1.0f,f.yaw);
    for(int s=-1;s<=1;s+=2){for(int fl=0;fl<2;fl++)lm507_arch(&f,0,s*8.6f,13.3f+fl*6.9f,-30.3f,1.65f,3.4f,.13f,.20f,.23f);lm507_arch(&f,0,s*8.6f,1.0f,-30.3f,1.6f,3.2f,.20f,.18f,.15f);}
    // Nave, transept and chapel windows with buttresses between bays.
    for(int s=-1;s<=1;s+=2){int face=s>0?2:3;
      for(int k=0;k<3;k++){float z=-25.5f+k*4.6f;lm507_arch(&f,face,s>0?z:-z,4.6f,s*11.7f,2.2f,5.0f,.15f,.25f,.32f);lm507_wallbox(&f,face,(s>0?z:-z)+2.3f,0,s*11.7f,.45f,.9f,12.5f,1.0f,.78f,.79f,.74f);}
      lm507_arch(&f,face,s>0?-6.0f:6.0f,4.6f,s*15.6f,2.4f,5.4f,.15f,.25f,.32f);
      for(int k=0;k<4;k++){float z=3.0f+k*4.6f;lm507_arch(&f,face,s>0?z:-z,4.0f,s*15.4f,2.0f,4.6f,.15f,.25f,.32f);lm507_wallbox(&f,face,(s>0?z:-z)+2.3f,0,s*15.4f,.4f,.8f,11.5f,.9f,.78f,.79f,.74f);}
      lm507_arch(&f,s>0?2:3,s>0?-6.15f:6.15f,0,s*15.6f,1.5f,2.9f,.27f,.21f,.16f);   // transept side doors
    }
    // Stair turrets recorded in the cadastral outline.
    for(int s=-1;s<=1;s+=2){architecture507_drum(lm507_at(&f,s*14.0f,0,-22),2.1f,16,8,tr,tg,tb);architecture507_cone(lm507_at(&f,s*14.0f,16,-22),2.3f,2.4f,8,.52f,.31f,.20f);
      architecture507_drum(lm507_at(&f,s*16.3f,0,10.5f),2.1f,14,8,tr,tg,tb);architecture507_cone(lm507_at(&f,s*16.3f,14,10.5f),2.3f,2.2f,8,.52f,.31f,.20f);}
    for(int k=0;k<3;k++)lm507_box(&f,0,k*.16f,-28.9f-k*.45f,11.2f-k*.6f,.16f,1.0f,.65f,.64f,.58f);
    if(f.dark)for(int s=-1;s<=1;s+=2){lm507_box(&f,s*7,0,-36,.14f,.9f,.14f,.3f,.3f,.3f);lm507_box(&f,s*7,.9f,-36,.5f,.3f,.5f,1.0f,.86f,.55f);}
  }
  // Praca Governador Irineu Bornhausen in front of the facade, clear of the
  // service lane that runs along the -X flank.
  if(city340_near(f.c,700)){city360_assets_init();
    city340_octagon(lm507_at(&f,-6,.02f,-47),5.4f,.42f,.76f,.73f,.67f);city340_octagon(lm507_at(&f,-6,.45f,-47),4.55f,.12f,.32f,.62f,.70f);lm507_box(&f,-6,.5f,-47,.13f,2.25f,.13f,.80f,.88f,.92f);
    static const float trees[5][2]={{-24,-48},{14,-53},{-14,-63},{10,-66},{24,-40}};for(int i=0;i<5;i++)city360_asset(g_city360_tree,lm507_at(&f,trees[i][0],0,trees[i][1]),0,.88f,.20f,.47f,.19f);
    static const float benches[3][2]={{-18,-40},{8,-40},{-6,-58}};for(int i=0;i<3;i++)city360_asset(g_city360_bench,lm507_at(&f,benches[i][0],0,benches[i][1]),f.yaw,.92f,.35f,.27f,.20f);
    g_world507_context+=9;}
  renderer09_set_material(0);g_world507_buildings++;
}

// ------------------------------------------------------- Mercado Publico ----
// Antigo Mercado Publico (1917, straightened art-deco lines after 1936). OSM
// polygon w298841666 (21.3 x 39.0 m, Rua Felix Busso Asseburg 25) between the
// street and Av. Prefeito Paulo Bauer, north of Rua Olimpio Miranda Junior. The
// taller south pavilion faces Praca Felix Busso Asseburg; the body is a ring of
// wings around the courtyard, which stays open. Identification: position +
// address (polygon carries no name) -> confidence 1.
static void world507_mercado(void){
  Lm507 f=lm507_frame(1);if(!city340_near(f.c,1100))return;renderer09_set_material(0);
  const float yr=.80f,yg=.63f,yb=.26f,tr=.93f,tg=.91f,tb=.84f;
  lm507_box(&f,0,0,-12.8f,21.4f,8.4f,13.4f,yr,yg,yb);                  // south pavilion
  lm507_box(&f,-5.5f,0,5,5,5.4f,22.2f,yr,yg,yb);lm507_box(&f,8,0,5,5,5.4f,22.2f,yr,yg,yb);lm507_box(&f,1.25f,0,14.1f,18.5f,5.4f,4,yr,yg,yb);
  lm507_box(&f,0,8.4f,-12.8f,21.8f,.9f,13.8f,tr,tg,tb);                // platibanda
  lm507_box(&f,0,9.3f,-19.3f,6.4f,1.1f,.6f,tr,tg,tb);                   // stepped centre of the parapet
  architecture507_hip(lm507_at(&f,0,9.3f,-12.8f),13.0f,21.4f,2.6f,f.yaw+1.5707963f,.50f,.30f,.20f);
  architecture506_roof(lm507_at(&f,-5.5f,5.4f,5),5.3f,22.4f,1.5f,f.yaw);architecture506_roof(lm507_at(&f,8,5.4f,5),5.3f,22.4f,1.5f,f.yaw);architecture506_roof(lm507_at(&f,1.25f,5.4f,14.1f),4.3f,18.7f,1.4f,f.yaw+1.5707963f);
  static const float pil[4]={-8.6f,-3.4f,3.4f,8.6f};for(int k=0;k<4;k++)lm507_wallbox(&f,0,pil[k],0,-19.5f,.12f,.40f,8.4f,.26f,tr,tg,tb);  // vertical rhythm between the arches
  lm507_wallbox(&f,0,0,0,-19.5f,.06f,21.6f,.38f,.14f,.62f,.42f,.22f);   // plinth band
  if(f.near){
    lm507_arch(&f,0,0,.18f,-19.5f,3.8f,6.0f,.15f,.22f,.15f);
    for(int s=-1;s<=1;s+=2)lm507_arch(&f,0,s*7.2f,.18f,-19.5f,2.0f,4.3f,.13f,.29f,.19f);
    for(int s=-1;s<=1;s+=2)lm507_wallbox(&f,0,s*7.2f,2.75f,-19.5f,.6f,2.5f,.20f,1.2f,.10f,.34f,.20f);   // green awnings
    lm507_text(&f,0,-2.6f,7.1f,-19.5f,"MERCADO PUBLICO",.97f,.94f,.84f);
    // Arcaded long sides along Felix Busso Asseburg (+X) and Paulo Bauer (-X).
    for(int s=-1;s<=1;s+=2){int face=s>0?2:3;for(int k=0;k<7;k++){float z=-16.5f+k*3.0f;float v=s>0?(z<-6.1f?10.7f:10.5f):(z<-6.1f?10.7f:8.0f);lm507_arch(&f,face,s>0?z:-z,.18f,s*v,1.8f,z<-6.1f?4.2f:3.4f,.13f,.29f,.19f);}}
    for(int k=0;k<5;k++)lm507_arch(&f,1,-7+k*3.5f,.18f,16.1f,1.8f,3.4f,.13f,.29f,.19f);
    // Courtyard: paving and the small central fountain of the old market.
    lm507_box(&f,1.25f,0,3,8.3f,.06f,17.8f,.70f,.67f,.60f);city340_octagon(lm507_at(&f,1.25f,.06f,3),1.6f,.5f,.72f,.70f,.64f);lm507_box(&f,1.25f,.56f,3,.12f,1.1f,.12f,.75f,.80f,.84f);
    // Sidewalk tables on the square side, kept off the asphalt of Olimpio Miranda.
    for(int k=-2;k<=2;k++){V3 q=lm507_at(&f,k*3.6f,0,-22.3f);world503_cafe_set(q,f.yaw,(u32)(k+9)*741u);}
  }
  if(city340_near(f.c,700)){city360_assets_init();for(int k=-2;k<=2;k+=2)city360_asset(g_city360_palm,lm507_at(&f,k*9.0f,0,-42),f.yaw,.92f,.22f,.50f,.22f);for(int s=-1;s<=1;s+=2)city360_asset(g_city360_bench,lm507_at(&f,s*5.5f,0,-37),f.yaw+3.14159265f,.9f,.35f,.27f,.20f);g_world507_context+=5;}
  renderer09_set_material(0);g_world507_buildings++;
}

// ------------------------------------------------- Palacio Marcos Konder ----
// Museu Historico de Itajai (1921-1925, eclectic, round-arched openings, high
// basement, three towers). OSM polygon w1097529372: 51.2 x 20.9 m. Main entrance
// (OSM entrance=main) on the narrow Rua Hercilio Luz end; the long flank runs
// along Av. Coronel Marcos Konder (local -X), where the third tower stands.
static void world507_palacio(void){
  Lm507 f=lm507_frame(2);if(!city340_near(f.c,1300))return;renderer09_set_material(0);
  const float br=.84f,bg=.63f,bb=.25f,tr=.94f,tg=.91f,tb=.80f;
  lm507_plinth(&f,-.95f,-13.65f,15.9f,18.3f,1.3f);
  lm507_box(&f,-.95f,0,-13.65f,15.9f,10.4f,18.3f,br,bg,bb);              // main block
  lm507_box(&f,1.85f,0,1,6.5f,7.4f,11,br,bg,bb);                        // link
  lm507_box(&f,-2.7f,0,15.55f,12.8f,8.0f,19.9f,br,bg,bb);               // rear wing
  lm507_box(&f,7.1f,0,13.45f,6.8f,6.4f,12.3f,br,bg,bb);                 // annex
  lm507_box(&f,-.95f,10.4f,-13.65f,16.5f,.55f,18.9f,tr,tg,tb);lm507_box(&f,-.95f,5.1f,-13.65f,16.2f,.26f,18.6f,tr,tg,tb);
  lm507_box(&f,-2.7f,8.0f,15.55f,13.3f,.4f,20.4f,tr,tg,tb);lm507_box(&f,1.85f,7.4f,1,6.9f,.35f,11.4f,tr,tg,tb);lm507_box(&f,7.1f,6.4f,13.45f,7.2f,.35f,12.7f,tr,tg,tb);
  architecture507_hip(lm507_at(&f,-.95f,10.95f,-13.65f),15.9f,18.3f,3.4f,f.yaw,.42f,.27f,.16f);
  architecture507_hip(lm507_at(&f,-2.7f,8.4f,15.55f),12.8f,19.9f,2.6f,f.yaw,.42f,.27f,.16f);
  architecture507_hip(lm507_at(&f,7.1f,6.75f,13.45f),6.8f,12.3f,1.8f,f.yaw,.42f,.27f,.16f);
  // Three towers: two flanking the Hercilio Luz facade, one on the avenue flank.
  static const float towers[3][2]={{6.3f,-22.4f},{-6.6f,-22.8f},{-7.6f,-5.2f}};
  for(int t=0;t<3;t++){V3 b=lm507_at(&f,towers[t][0],0,towers[t][1]);architecture507_drum(b,2.6f,14.6f,12,br,bg,bb);architecture507_drum(lm507_at(&f,towers[t][0],14.6f,towers[t][1]),2.95f,.7f,12,tr,tg,tb);architecture507_cone(lm507_at(&f,towers[t][0],15.3f,towers[t][1]),2.9f,3.6f,12,.40f,.27f,.15f);}
  if(f.near){
    for(int t=0;t<3;t++){float dir=towers[t][1]<-10?-1.0f:1.0f;for(int fl=0;fl<3;fl++){float yaw=f.yaw+(dir<0?0:3.14159265f);V3 p=lm507_at(&f,towers[t][0],2.0f+fl*4.1f,towers[t][1]+dir*2.6f);architecture506_arch(p,1.1f,2.3f,yaw,.14f,.24f,.25f);}}
    // Facade between the towers: raised portal with stair, flanking windows, upper arcade.
    lm507_arch(&f,0,.5f,1.3f,-22.8f,2.4f,4.2f,.27f,.23f,.19f);
    for(int k=0;k<3;k++)lm507_box(&f,.5f,k*.33f,-23.6f-k*.5f,4.2f-k*.4f,.33f,.9f,.62f,.60f,.54f);
    for(int s=-1;s<=1;s+=2)lm507_arch(&f,0,.5f+s*3.3f,1.6f,-22.8f,1.3f,2.6f,lm507_lit(&f,s+1)?.70f:.14f,.24f,.25f);
    for(int k=-1;k<=1;k++)lm507_arch(&f,0,.5f+k*3.3f,6.2f,-22.8f,1.3f,2.7f,lm507_lit(&f,k+5)?.70f:.14f,.24f,.25f);
    lm507_text(&f,0,-2.4f,9.6f,-22.8f,"MUSEU HISTORICO",.98f,.94f,.82f);
    // Avenue flank (-X) and the opposite flank: two floors of arched bays with pilasters.
    for(int s=-1;s<=1;s+=2){int face=s>0?2:3;float v=s>0?7.0f:8.9f;
      for(int k=0;k<4;k++){float z=-20.0f+k*4.2f,u=s>0?z:-z;for(int fl=0;fl<2;fl++)lm507_arch(&f,face,u,1.6f+fl*4.6f,s*v,1.3f,2.6f,lm507_lit(&f,k*2+fl+s)?.70f:.14f,.24f,.25f);lm507_wallbox(&f,face,u+2.1f,1.3f,s*v,.08f,.30f,9.1f,.18f,tr,tg,tb);}
      for(int k=0;k<4;k++){float z=7.5f+k*4.6f,u=s>0?z:-z,vv=s>0?3.7f:9.1f;for(int fl=0;fl<2;fl++)lm507_window(&f,face,u,1.2f+fl*3.4f,s*vv,1.2f,2.0f,lm507_lit(&f,k+fl*3+s),tr,tg,tb);}
    }
    for(int k=0;k<3;k++)for(int fl=0;fl<2;fl++)lm507_window(&f,1,-7.8f+k*3.6f,1.2f+fl*3.4f,25.5f,1.2f,2.0f,lm507_lit(&f,k+fl),tr,tg,tb);
    if(f.dark)for(int s=-1;s<=1;s+=2){lm507_box(&f,.5f+s*5.5f,0,-27,.16f,5.4f,.16f,.3f,.3f,.3f);lm507_box(&f,.5f+s*5.5f,5.4f,-27,.55f,.35f,.55f,1.0f,.82f,.45f);}
  }
  // Garden strip on the Hercilio Luz side and lamps at the portal.
  if(city340_near(f.c,700)){city360_assets_init();for(int s=-1;s<=1;s+=2)city360_asset(g_city360_lamp,lm507_at(&f,.5f+s*9.0f,0,-29),f.yaw,.92f,.27f,.29f,.29f);for(int k=0;k<3;k++)city360_asset(g_city360_tree,lm507_at(&f,13.5f,0,-16+k*9.0f),0,.80f,.20f,.46f,.18f);city360_asset(g_city360_bench,lm507_at(&f,12.8f,0,-20),f.yaw+1.5707963f,.86f,.32f,.24f,.18f);g_world507_context+=6;}
  renderer09_set_material(0);g_world507_buildings++;
}

// ----------------------------------------------------------- Casa Konder ----
// Casa Konder (1887, Reinhold Roenick, Rua Lauro Muller 83). OSM polygon
// w1098072184: 15.7 x 14.5 m, single storey on a high basement; facade on Lauro
// Muller (local -Z, west). Hotel Rota do Mar stands behind (+Z).
static void world507_casa_konder(void){
  Lm507 f=lm507_frame(3);if(!city340_near(f.c,1050))return;renderer09_set_material(0);
  const float wr=.90f,wg=.82f,wb=.60f,tr=.95f,tg=.93f,tb=.86f;
  lm507_plinth(&f,0,0,14.5f,15.7f,1.1f);lm507_box(&f,0,0,0,14.5f,5.2f,15.7f,wr,wg,wb);lm507_box(&f,0,5.2f,0,15.1f,.36f,16.3f,tr,tg,tb);
  architecture507_hip(lm507_at(&f,0,5.56f,0),15.3f,16.5f,3.6f,f.yaw,.40f,.28f,.18f);
  lm507_box(&f,3.2f,8.4f,2.5f,.9f,1.5f,.9f,.62f,.40f,.30f);          // chimney
  if(f.near){
    for(int s=-1;s<=1;s+=2)lm507_wallbox(&f,0,s*7.1f,1.1f,-7.85f,.03f,.5f,4.1f,.22f,tr,tg,tb);    // quoins
    lm507_door(&f,0,-1.6f,1.2f,-7.85f,1.3f,2.9f);for(int k=0;k<3;k++)lm507_box(&f,-1.6f,k*.3f,-8.6f-k*.45f,2.6f-k*.3f,.3f,.8f,.58f,.55f,.50f);
    static const float ux[3]={-4.8f,1.6f,4.8f};for(int k=0;k<3;k++){lm507_window(&f,0,ux[k],1.9f,-7.85f,1.15f,2.2f,lm507_lit(&f,k),tr,tg,tb);for(int s=-1;s<=1;s+=2)lm507_wallbox(&f,0,ux[k]+s*.95f,1.9f,-7.85f,.05f,.42f,2.2f,.06f,.18f,.36f,.30f);}
    for(int side=0;side<2;side++){int face=side?3:2;float v=side?-7.25f:7.25f;for(int k=0;k<3;k++)lm507_window(&f,face,-4.5f+k*4.5f,1.9f,v,1.15f,2.2f,lm507_lit(&f,k+side*3+1),tr,tg,tb);}
    for(int k=0;k<2;k++)lm507_window(&f,1,-2.5f+k*5.0f,1.9f,7.85f,1.15f,2.2f,0,tr,tg,tb);
    lm507_box(&f,0,6.3f,-5.4f,2.6f,1.7f,1.4f,wr,wg,wb);city340_pyramid(lm507_at(&f,0,8.0f,-5.4f),2.9f,1.7f,.9f,f.yaw,.40f,.28f,.18f);lm507_window(&f,0,0,6.5f,-6.1f,.8f,1.0f,lm507_lit(&f,9),tr,tg,tb);  // dormer
  }
  renderer09_set_material(0);g_world507_buildings++;
}

// ---------------------------------------------------------- Casa Malburg ----
// Casa Malburg (1912-1915, Roenick, hanseatic eclectic, three storeys, steep
// imported-tile roof). OSM polygon w1106146084: 16.9 x 15.9 m; the facade faces
// Rua Pedro Ferreira (local -Z, south-west).
static void world507_casa_malburg(void){
  Lm507 f=lm507_frame(4);if(!city340_near(f.c,1000))return;renderer09_set_material(0);
  const float wr=.86f,wg=.74f,wb=.52f,tr=.94f,tg=.92f,tb=.86f;float H=11.6f;
  lm507_plinth(&f,0,0,15.9f,16.9f,.9f);lm507_box(&f,0,0,0,15.9f,H,16.9f,wr,wg,wb);
  for(int fl=1;fl<=2;fl++)lm507_box(&f,0,.9f+fl*3.55f,0,16.2f,.26f,17.2f,tr,tg,tb);lm507_box(&f,0,H,0,16.6f,.5f,17.6f,tr,tg,tb);
  architecture507_hip(lm507_at(&f,0,H+.5f,0),16.4f,17.4f,4.6f,f.yaw,.30f,.28f,.31f);
  for(int s=-1;s<=1;s+=2)lm507_box(&f,s*4.5f,H+2.6f,3.0f,.8f,1.6f,.8f,.48f,.34f,.28f);
  if(f.near){
    for(int s=-1;s<=1;s+=2){lm507_wallbox(&f,0,s*7.7f,.9f,-8.45f,.04f,.6f,H-.9f,.26f,tr,tg,tb);lm507_wallbox(&f,1,s*7.7f,.9f,8.45f,.04f,.6f,H-.9f,.26f,tr,tg,tb);}
    static const float ux[4]={-5.4f,-1.8f,1.8f,5.4f};
    for(int k=0;k<4;k++){if(k==1)lm507_arch(&f,0,ux[k],.9f,-8.45f,1.7f,3.2f,.27f,.21f,.16f);else lm507_arch(&f,0,ux[k],1.5f,-8.45f,1.5f,2.9f,lm507_lit(&f,k)?.72f:.14f,.24f,.26f);
      for(int fl=1;fl<=2;fl++)lm507_window(&f,0,ux[k],1.6f+fl*3.55f,-8.45f,1.3f,2.3f,lm507_lit(&f,k+fl*4),tr,tg,tb);}
    lm507_wallbox(&f,0,0,4.9f,-8.45f,.6f,5.2f,.18f,1.2f,tr,tg,tb);lm507_wallbox(&f,0,0,5.08f,-8.45f,1.15f,5.0f,.9f,.07f,.42f,.40f,.38f);    // first-floor balcony
    for(int k=0;k<3;k++)lm507_box(&f,-1.8f,k*.3f,-9.1f-k*.45f,3.0f-k*.3f,.3f,.8f,.58f,.55f,.50f);
    for(int side=0;side<2;side++){int face=side?3:2;float v=side?-7.95f:7.95f;for(int k=0;k<4;k++)for(int fl=0;fl<=2;fl++)lm507_window(&f,face,-5.7f+k*3.8f,1.6f+fl*3.55f,v,1.3f,2.3f,lm507_lit(&f,k+fl*4+side*2),tr,tg,tb);}
    for(int k=0;k<4;k++)for(int fl=0;fl<=2;fl++)lm507_window(&f,1,ux[k],1.6f+fl*3.55f,8.45f,1.3f,2.3f,0,tr,tg,tb);
    for(int s=-1;s<=1;s+=2){lm507_box(&f,s*3.6f,H+1.0f,-6.3f,2.0f,1.8f,1.6f,wr,wg,wb);city340_pyramid(lm507_at(&f,s*3.6f,H+2.8f,-6.3f),2.3f,1.9f,.9f,f.yaw,.30f,.28f,.31f);lm507_window(&f,0,s*3.6f,H+1.2f,-7.1f,.8f,1.1f,lm507_lit(&f,s+7),tr,tg,tb);}   // dormers
  }
  renderer09_set_material(0);g_world507_buildings++;
}

// ------------------------------------------------- Imaculada Conceicao ----
// Igreja da Imaculada Conceicao, "Igrejinha Velha" (masonry 1837-1840, oldest
// standing building). OSM polygon w681517239: 28.2 x 14.2 m with the chancel
// narrowing at the rear. Entrance node faces Praca Vidal Ramos (local -Z, ESE).
static void world507_imaculada(void){
  Lm507 f=lm507_frame(5);if(!city340_near(f.c,1100))return;renderer09_set_material(0);
  const float wr=.93f,wg=.92f,wb=.87f,tr=.52f,tg=.58f,tb=.66f;
  lm507_box(&f,0,0,-4.0f,14.2f,8.4f,20.2f,wr,wg,wb);                      // nave (z -14.1..6.1)
  lm507_box(&f,0,0,10.1f,13.6f,7.6f,8.0f,wr,wg,wb);                       // chancel
  architecture506_roof(lm507_at(&f,0,8.4f,-4.0f),14.6f,20.6f,3.3f,f.yaw);architecture506_roof(lm507_at(&f,0,7.6f,10.1f),14.0f,8.4f,3.0f,f.yaw);
  lm507_box(&f,0,0,-12.2f,4.4f,13.0f,4.4f,wr,wg,wb);                      // single front tower
  lm507_box(&f,0,13.0f,-12.2f,4.8f,.4f,4.8f,tr,tg,tb);city340_pyramid(lm507_at(&f,0,13.4f,-12.2f),4.6f,4.6f,3.4f,f.yaw,.52f,.31f,.20f);city340_cross(lm507_at(&f,0,16.8f,-12.2f),1.2f);
  city340_pyramid(lm507_at(&f,0,8.4f,-13.9f),14.2f,.9f,3.3f,f.yaw,wr,wg,wb);  // gable face
  if(f.near){
    for(int s=-1;s<=1;s+=2)lm507_wallbox(&f,0,s*6.8f,0,-14.1f,.04f,.5f,8.4f,.2f,tr,tg,tb);
    lm507_wallbox(&f,0,0,8.3f,-14.1f,.05f,14.4f,.3f,.2f,tr,tg,tb);
    lm507_arch(&f,0,0,.15f,-14.4f,2.0f,3.7f,.27f,.21f,.16f);
    lm507_window(&f,0,0,6.6f,-14.4f,1.0f,1.6f,lm507_lit(&f,2),tr,tg,tb);
    for(int s=-1;s<=1;s+=2)lm507_window(&f,0,s*4.6f,3.6f,-14.1f,1.2f,2.3f,lm507_lit(&f,s+4),tr,tg,tb);
    lm507_arch(&f,0,0,9.6f,-14.4f,1.4f,2.4f,.10f,.10f,.12f);           // belfry opening
    for(int side=0;side<2;side++){int face=side?3:2;float v=side?-7.1f:7.1f;for(int k=0;k<4;k++){float z=-9.0f+k*4.6f;lm507_arch(&f,face,side?-z:z,3.2f,v,1.3f,2.7f,lm507_lit(&f,k+side*4)?.72f:.15f,.25f,.32f);}}
    for(int k=0;k<3;k++)lm507_box(&f,0,k*.15f,-14.8f-k*.4f,5.0f-k*.4f,.15f,.7f,.62f,.60f,.54f);
    if(f.dark)for(int s=-1;s<=1;s+=2){lm507_box(&f,s*5,0,-18,.14f,.9f,.14f,.3f,.3f,.3f);lm507_box(&f,s*5,.9f,-18,.5f,.3f,.5f,1.0f,.86f,.55f);}
  }
  // Praca Vidal Ramos: paved square, Marco Zero marker, trees and benches in front.
  if(city340_near(f.c,750)){city360_assets_init();lm507_box(&f,0,0,-33,30,.08f,22,.48f,.47f,.43f);
    lm507_box(&f,6,.08f,-40,1.6f,.5f,1.6f,.66f,.64f,.58f);lm507_box(&f,6,.58f,-40,.7f,3.2f,.7f,.72f,.70f,.64f);
    for(int i=-2;i<=2;i++){city360_asset(g_city360_tree,lm507_at(&f,i*6.5f,0,-28-(i&1)*9.0f),0,.84f,.19f,.45f,.18f);if(i<2)city360_asset(g_city360_bench,lm507_at(&f,i*6.5f+3.2f,0,-36),f.yaw+3.14159265f,.88f,.33f,.25f,.18f);}g_world507_context+=10;}
  renderer09_set_material(0);g_world507_buildings++;
}

// ---------------------------------------------------------- Centreventos ----
// Centreventos Itajai, Av. Ministro Victor Konder 303. OSM polygon w197483217:
// 127.1 x 94.9 m. Curved glazed front toward the avenue (local -Z), side wings,
// service notch on the river end (+Z). Roof is a segmental vault (estimate).
static void world507_centreventos(void){
  Lm507 f=lm507_frame(6);if(!city340_near(f.c,1600))return;renderer09_set_material(0);
  const float wr=.80f,wg=.82f,wb=.82f;
  lm507_box(&f,3.5f,0,-7.5f,75,13,99,wr,wg,wb);                           // hall (z -57..42)
  lm507_box(&f,30.65f,0,52.75f,20.7f,13,21.5f,wr,wg,wb);lm507_box(&f,-22.3f,0,52.75f,23.4f,13,21.5f,wr,wg,wb);
  lm507_box(&f,44.2f,0,14.5f,6.4f,7,97,.72f,.74f,.74f);lm507_box(&f,-40.7f,0,15,13.4f,7,96,.72f,.74f,.74f);
  architecture507_vault(lm507_at(&f,3.5f,13,-7.5f),75,99,5.2f,f.yaw,.88f,.89f,.88f);
  for(int k=-2;k<=2;k++)lm507_box(&f,3.5f+k*15.0f,13,-7.5f,.5f,5.8f,99.4f,.70f,.72f,.72f);   // vault ribs
  // Curved entrance front from the cadastral outline, glazed dark blue.
  static const float curve[14]={41,-59,28.8f,-58.6f,17.4f,-62.2f,5.5f,-63.6f,-5.9f,-61.5f,-13.8f,-57.1f,-33,-56.4f};
  renderer09_set_material(MAT09_GLASS);architecture507_wall(lm507_at(&f,0,0,0),curve,7,0,11.5f,f.yaw,.08f,.25f,.39f);renderer09_set_material(0);
  architecture507_wall(lm507_at(&f,0,0,0),curve,7,11.5f,14.2f,f.yaw,wr,wg,wb);
  for(int i=0;i<7;i++)lm507_box(&f,curve[i*2],0,curve[i*2+1],.45f,14.2f,.45f,.86f,.87f,.85f);
  lm507_box(&f,5.5f,14.2f,-62.6f,30,.9f,2.4f,.86f,.87f,.85f);
  if(f.near){
    for(int i=0;i<6;i++){float x0=curve[i*2],z0=curve[i*2+1],x1=curve[i*2+2],z1=curve[i*2+3];for(int k=1;k<4;k++){float t=k*.25f;lm507_box(&f,x0+(x1-x0)*t,0,z0+(z1-z0)*t-.1f,.12f,11.5f,.3f,.66f,.70f,.70f);}}
    for(int k=-2;k<=2;k++)lm507_box(&f,5.5f+k*2.2f,0,-64.0f,1.9f,3.4f,.14f,.10f,.14f,.16f);               // entrance doors
    for(int k=0;k<3;k++)lm507_box(&f,5.5f,.15f*(2-k),-65.0f-k*.7f,34,.30f*(3-k),.7f,.62f,.62f,.57f);    // steps
    lm507_text(&f,0,-5.5f,12.3f,-63.8f,"CENTREVENTOS ITAJAI",.94f,.96f,.95f);
    for(int k=0;k<9;k++){lm507_window(&f,2,-50+k*11.0f,8.5f,47.4f,5.5f,2.4f,lm507_lit(&f,k),.70f,.72f,.72f);lm507_window(&f,3,50-k*11.0f,8.5f,-47.4f,5.5f,2.4f,lm507_lit(&f,k+3),.70f,.72f,.72f);}
    lm507_box(&f,4.85f,0,63.3f,30.9f,2.2f,.3f,.60f,.62f,.62f);         // service yard fence
  }
  if(city340_near(f.c,900)){city360_assets_init();for(int k=-4;k<=4;k+=2)city360_asset(g_city360_palm,lm507_at(&f,5.5f+k*7.0f,0,-74),f.yaw,.86f,.20f,.48f,.21f);for(int k=-3;k<=3;k++){lm507_box(&f,5.5f+k*6.0f,0,-80,.10f,7.4f,.10f,.70f,.72f,.70f);}for(int k=-2;k<=2;k++)city360_asset(g_city360_lamp,lm507_at(&f,5.5f+k*12.0f,0,-70),f.yaw,.95f,.27f,.30f,.31f);if(city340_near(f.c,300))for(int k=-3;k<=3;k++){V3 q=lm507_at(&f,5.5f+k*5.4f,0,-86);world505_bench(q,f.yaw);}g_world507_context+=17;}
  renderer09_set_material(0);g_world507_buildings++;
}

// -------------------------------------------------------- Pier Turistico ----
// Pavilion at Pier Guilherme Asseburg (Pier Turistico). No cadastral building;
// kept as the 5.0.6 approximation, now in the shared frame.
static void world507_pier(void){
  Lm507 f=lm507_frame(7);if(!city340_near(f.c,1200))return;renderer09_set_material(0);
  lm507_box(&f,0,0,0,31,6.8f,12,.74f,.77f,.77f);lm507_box(&f,0,1.0f,-6.18f,25,4.5f,.3f,.08f,.22f,.31f);lm507_box(&f,0,6.8f,-2,34,.32f,17,.63f,.65f,.64f);
  if(f.near){lm507_text(&f,0,-10.2f,5.2f,-6.2f,"TERMINAL DE PASSAGEIROS",.94f,.96f,.94f);for(int k=-2;k<=2;k++)lm507_window(&f,0,k*5.0f,1.4f,-6.18f,2.6f,2.4f,lm507_lit(&f,k+2),.70f,.72f,.72f);}
  renderer09_set_material(0);g_world507_buildings++;
}

// ------------------------------------------------------ Hotel Rota do Mar ----
// Historic hotel block beside Casa Konder (OSM w1106146082, 70.8 x 14.0 m, two
// storeys). Pilaster bumps in the outline mark the river-facing facade (-Z).
// The west block is set back so it meets Casa Konder's wall instead of
// overlapping it as the two cadastral polygons do.
static void world507_hotel(void){
  Lm507 f=lm507_frame(8);if(!city340_near(f.c,1000))return;renderer09_set_material(0);
  const float wr=.92f,wg=.90f,wb=.84f,tr=.62f,tg=.70f,tb=.66f;
  lm507_box(&f,.8f,0,0,47.4f,8.6f,14,wr,wg,wb);lm507_box(&f,-29.15f,0,-.2f,12.5f,8.6f,8.7f,wr,wg,wb);lm507_box(&f,29.95f,0,-3.85f,10.9f,4.4f,6.3f,wr,wg,wb);
  lm507_box(&f,.8f,8.6f,0,47.9f,.45f,14.5f,tr,tg,tb);lm507_box(&f,-29.15f,8.6f,-.2f,13.0f,.45f,9.2f,tr,tg,tb);lm507_box(&f,29.95f,4.4f,-3.85f,11.3f,.35f,6.7f,tr,tg,tb);
  architecture507_hip(lm507_at(&f,.8f,9.05f,0),14.3f,47.9f,3.2f,f.yaw+1.5707963f,.50f,.30f,.20f);
  architecture507_hip(lm507_at(&f,-29.15f,9.05f,-.2f),9.0f,12.9f,2.2f,f.yaw+1.5707963f,.50f,.30f,.20f);
  architecture507_hip(lm507_at(&f,29.95f,4.75f,-3.85f),6.6f,11.2f,1.6f,f.yaw+1.5707963f,.50f,.30f,.20f);
  lm507_box(&f,.8f,4.3f,0,47.6f,.22f,14.2f,tr,tg,tb);
  if(f.near){
    for(int k=0;k<10;k++){float u=-20.5f+k*4.6f;lm507_arch(&f,0,u,.2f,-7,1.8f,3.5f,lm507_lit(&f,k)?.72f:.14f,.24f,.26f);lm507_window(&f,0,u,5.2f,-7,1.4f,2.1f,lm507_lit(&f,k+5),tr,tg,tb);if(k<9)lm507_wallbox(&f,0,u+2.3f,.2f,-7,.05f,.34f,8.4f,.16f,tr,tg,tb);}
    for(int k=0;k<3;k++)lm507_wallbox(&f,0,-11.5f+k*9.2f,4.9f,-7,.5f,3.6f,.16f,1.0f,tr,tg,tb);          // balconies
    lm507_text(&f,0,-4.0f,7.3f,-7,"HOTEL",.20f,.26f,.30f);
    for(int k=0;k<2;k++)lm507_window(&f,0,-31.5f+k*4.6f,1.4f,-4.55f,1.4f,2.1f,lm507_lit(&f,k+8),tr,tg,tb);for(int k=0;k<2;k++)lm507_window(&f,0,-31.5f+k*4.6f,5.2f,-4.55f,1.4f,2.1f,0,tr,tg,tb);
    for(int k=0;k<2;k++)lm507_window(&f,0,26.5f+k*4.4f,1.2f,-7,1.6f,1.8f,0,tr,tg,tb);
    for(int k=0;k<10;k++)for(int fl=0;fl<2;fl++)lm507_window(&f,1,-20.5f+k*4.6f,1.4f+fl*3.8f,7,1.3f,2.0f,lm507_lit(&f,k+fl*2+1),tr,tg,tb);
    for(int k=0;k<2;k++)for(int fl=0;fl<2;fl++)lm507_window(&f,3,-2.0f+k*4.0f,1.4f+fl*3.8f,-35.4f,1.3f,2.0f,0,tr,tg,tb);
  }
  renderer09_set_material(0);g_world507_buildings++;
}

static void world507_draw(void){
  g_world507_buildings=g_world507_detail=g_world507_context=0;
  world507_matriz();world507_mercado();world507_palacio();world507_casa_konder();world507_casa_malburg();world507_imaculada();world507_centreventos();world507_pier();world507_hotel();renderer09_set_material(MAT09_GENERIC);
}
static void world507_overlay(void){if(!g_debug||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();char b[200];sprintf(b,"WORLD 5.0.7 // authored buildings %d | openings %d | context props %d",g_world507_buildings,g_world507_detail,g_world507_context);gl_text(34,(float)g_h-376,b,.66f,.84f,.76f);settings_draw_2d_end();}
