// Centro 5.0.5: cadastral facades and road-aligned, deterministic street scenes.
// The box stream is submitted in ONE GL batch. Hard caps bound CPU/vertex cost.
#define WORLD505_BOX_CAP 1800
static int g_world505_boxes=0,g_world505_facades=0,g_world505_scenes=0;
static void world505_box(V3 p,float w,float h,float d,float yaw,float r,float g,float b){
  if(g_world505_boxes>=WORLD505_BOX_CAP)return;
  static const int faces[6][4]={{4,5,6,7},{1,0,3,2},{0,4,7,3},{5,1,2,6},{3,7,6,2},{0,1,5,4}};
  static const float normals[6][3]={{0,0,1},{0,0,-1},{-1,0,0},{1,0,0},{0,1,0},{0,-1,0}};
  float c=(float)cos(yaw),s=(float)sin(yaw);V3 q[8];
  for(int k=0;k<8;k++){float x=((k==1||k==2||k==5||k==6)?.5f:-.5f)*w,y=((k==2||k==3||k==6||k==7)?.5f:-.5f)*h,z=(k>=4?.5f:-.5f)*d;q[k]=v3(p.x+x*c+z*s,p.y+y,p.z-x*s+z*c);}
  glColor3f(r,g,b);for(int f=0;f<6;f++){glNormal3f(normals[f][0]*c+normals[f][2]*s,normals[f][1],-normals[f][0]*s+normals[f][2]*c);for(int k=0;k<4;k++){V3 a=q[faces[f][k]];glTexCoord2f(k==1||k==2?1.0f:0,k>=2?1.0f:0);glVertex3f(a.x,a.y,a.z);}}
  g_world505_boxes++;
}
static V3 world505_at(City350Front*f,float x,float y,float z){V3 p=vadd(f->mid,vadd(vmul(f->along,x),vmul(f->out,z)));p.y=y;return p;}
static void world505_detail(City350Front*f,float x,float y,float z,float w,float h,float d,float r,float g,float b){world505_box(world505_at(f,x,y,z),w,h,d,f->yaw,r,g,b);}
typedef struct {int used;u32 seed;City350Front front;} World505FrontCache;
static World505FrontCache g_world505_front_cache[256];
static int world505_front(Building*b,City350Front*out){
  World505FrontCache*cached=&g_world505_front_cache[b->seed&255u];for(int probe=0;probe<256;probe++){World505FrontCache*q=&g_world505_front_cache[(b->seed+probe)&255u];if(!q->used){cached=q;break;}if(q->seed==b->seed){*out=q->front;return 1;}}
  int id=building12_find_seed(b->seed);if(id<0)return 0;int start=g_building_poly_start12[id],count=g_building_poly_count12[id];if(count<3||start<0||start+count>g_building_vert_count12)return 0;
  float best=1e12f;int found=0;
  for(int edge=0;edge<count;edge++){
    V3 a=g_building_verts12[start+edge],z=g_building_verts12[start+(edge+1)%count],d=vsub(z,a);float len=vlen(d);if(len<3)continue;
    City350Front f;f.mid=vmul(vadd(a,z),.5f);f.along=vmul(d,1/len);f.out=v3(-f.along.z,0,f.along.x);if(vdot(f.out,vsub(f.mid,b->c))<0)f.out=vmul(f.out,-1);f.len=len;f.yaw=(float)atan2(-f.along.z,f.along.x);
    for(int r=0;r<g_stream_road_count08;r++){
      StreamRoad08*road=&g_stream_roads08[r];V3 v=vsub(road->b,road->a),rel=vsub(f.mid,road->a);float l2=v.x*v.x+v.z*v.z;if(l2<1)continue;
      float t=clampf((rel.x*v.x+rel.z*v.z)/l2,0,1);V3 to=vsub(vadd(road->a,vmul(v,t)),f.mid);float dist=to.x*to.x+to.z*to.z;
      if(vdot(to,f.out)<=0||dist>100*100)continue;float score=dist+12.0f/maxf(len,1);
      if(score<best){best=score;*out=f;found=1;}
    }
  }
  if(found){cached->used=1;cached->seed=b->seed;cached->front=*out;}return found;
}
static V3 g_world505_label_p[10];static int g_world505_label_id[10],g_world505_labels=0;
static void world505_facade(Building*b,int secondary){
  City350Front f;if(!world505_front(b,&f))return;if(secondary){City350Front other;if(!city350_front(b,&other)||absf(vdot(other.along,f.along))>.95f)return;f=other;}
  // Width is local X, not local Z. Keep exact cadastral wall direction.
  f.yaw=(float)atan2(-f.along.z,f.along.x);
  f.out=v3(-f.along.z,0,f.along.x);if(vdot(f.out,vsub(f.mid,b->c))<0)f.out=vmul(f.out,-1);
  float y=terrain11_height(b->c.x,b->c.z),w=minf(f.len-.25f,36.0f),height=b->h;
  if(w<3||height<3)return;u32 hash=b->seed*1664525u+1013904223u;
  int near=city340_near(b->c,105),mid=city340_near(b->c,230),style=(hash>>9)%5;
  static const float colors[8][3]={{.72f,.64f,.48f},{.63f,.72f,.68f},{.73f,.57f,.48f},{.78f,.77f,.69f},{.53f,.65f,.68f},{.69f,.67f,.56f},{.74f,.68f,.62f},{.62f,.65f,.60f}};
  const float*col=colors[(hash>>3)&7];int tower=height>=14;
  world505_detail(&f,0,y+height*.5f,.13f,w,height,.16f,col[0],col[1],col[2]);
  world505_detail(&f,0,y+height-.14f,.30f,w+.15f,.28f,.38f,.82f,.81f,.73f);
  if(!mid){g_world505_facades++;return;}
  int bays=(int)(w/(tower?3.6f:3.1f));if(bays<1)bays=1;if(bays>5)bays=5;float bw=w/bays;
  int floors=(int)(height/3.05f);if(floors>12)floors=12;
  // Ground floors have individually recessed shop fronts and awning variants.
  for(int k=0;k<bays;k++){
    float x=-w*.5f+(k+.5f)*bw,t=((hash>>(k%12))&3)*.035f;
    world505_detail(&f,x,y+1.4f,.245f,bw-.20f,2.55f,.06f,.085f+t,.15f+t,.18f+t);
    world505_detail(&f,x-bw*.5f+.08f,y+1.45f,.36f,.15f,2.9f,.24f,.76f,.74f,.66f);
    world505_detail(&f,x,y+2.95f,.46f,bw-.08f,.48f,.32f,style==0?.46f:.12f,style==1?.32f:.20f,.19f);
    if(near){
      world505_detail(&f,x,y+1.32f,.34f,.065f,2.45f,.07f,.66f,.68f,.64f);
      world505_detail(&f,x+.14f,y+1.16f,.405f,.025f,.34f,.045f,.72f,.74f,.72f);
      world505_detail(&f,x,y+.21f,.41f,bw-.26f,.20f,.32f,.37f,.37f,.34f);
      world505_detail(&f,x,y+2.65f,.90f,bw-.16f,.12f,1.30f,style==2?.55f:.17f,style==3?.36f:.22f,.19f);
      if(style==2||style==3)for(int stripe=0;stripe<3;stripe++)world505_detail(&f,x-bw*.30f+stripe*bw*.30f,y+2.72f,.91f,bw*.12f,.018f,1.28f,.75f,.71f,.60f);
      if((k&1)==0)world505_detail(&f,x,y+.65f,.29f,bw*.55f,.42f,.045f,.51f,.43f,.30f);
    }
  }
  for(int fl=1;fl<floors;fl+=(tower?2:(near?1:2))){
    float yy=y+fl*3.05f+1.45f;
    for(int k=0;k<bays;k++){
      float x=-w*.5f+(k+.5f)*bw;int lit=atmos280_dark()&&((hash+fl*31+k*13)%5==0);
      world505_detail(&f,x,yy,.245f,bw*.57f,tower?1.6f:1.85f,.07f,lit?.65f:.13f,lit?.52f:.23f,lit?.31f:.27f);
      if(near){world505_detail(&f,x,yy,.31f,.055f,1.8f,.05f,.72f,.71f,.64f);world505_detail(&f,x,yy-.94f,.34f,bw*.68f,.11f,.30f,.79f,.77f,.67f);}
      if(near&&!tower&&((hash+k+fl)%4==0)){world505_detail(&f,x+bw*.35f,yy-.5f,.47f,.52f,.38f,.39f,.64f,.64f,.59f);world505_detail(&f,x+bw*.35f,yy-.5f,.675f,.36f,.20f,.02f,.29f,.31f,.30f);}
    }
    if(tower){world505_detail(&f,0,yy-.94f,.70f,w*.93f,.16f,1.35f,.73f,.74f,.70f);world505_detail(&f,0,yy-.43f,1.30f,w*.91f,.78f,.065f,.27f,.38f,.40f);}
    else if(style<3)world505_detail(&f,0,yy+1.10f,.29f,w,.14f,.26f,.81f,.78f,.67f);
  }
  if(near&&!tower){
    for(int side=-1;side<=1;side+=2)world505_detail(&f,side*(w*.5f-.18f),y+height*.5f,.32f,.28f,height-.30f,.28f,.83f,.80f,.71f);
    if(style==0||style==4)world505_detail(&f,0,y+height+.25f,.10f,w*.34f,.65f,.38f,col[0],col[1],col[2]);
    world505_detail(&f,w*.32f,y+.48f,.34f,w*.12f,.15f,.025f,col[0]*.77f,col[1]*.77f,col[2]*.77f);
  }
  if(near&&g_world505_labels<10){g_world505_label_p[g_world505_labels]=world505_at(&f,-w*.28f,y+3.02f,.64f);g_world505_label_id[g_world505_labels++]=(hash>>8)%8;}
  g_world505_facades++;
}
static void world505_buildings(void){
  // Nearest first so streaming order cannot starve the player's own block.
  int ids[64],count=0;float ds[64];
  for(int i=0;i<g_stream_building_count08;i++){Building*b=&g_stream_buildings08[i].b;if(landmark506_replaces(b))continue;if(!world504_core(b->c)||!city340_near(b->c,420))continue;V3 d=vsub(b->c,g_player.body.p);float dist=d.x*d.x+d.z*d.z;int k=count;if(k==64){if(dist>=ds[63])continue;k=63;}else count++;while(k>0&&ds[k-1]>dist){if(k<64){ds[k]=ds[k-1];ids[k]=ids[k-1];}k--;}ds[k]=dist;ids[k]=i;}
  for(int i=0;i<count&&g_world505_boxes<1250;i++){Building*b=&g_stream_buildings08[ids[i]].b;world505_facade(b,0);if(i<10&&city340_near(b->c,150))world505_facade(b,1);}
}
static void world505_bench(V3 p,float yaw){
  world505_box(v3(p.x,p.y+.46f,p.z),1.65f,.12f,.52f,yaw,.37f,.27f,.18f);
  V3 back=city340_rot(p,0,.24f,yaw);world505_box(v3(back.x,p.y+.80f,back.z),1.65f,.47f,.07f,yaw,.41f,.31f,.21f);
  for(int s=-1;s<=1;s+=2){V3 q=city340_rot(p,s*.60f,0,yaw);world505_box(v3(q.x,p.y+.23f,q.z),.08f,.46f,.44f,yaw,.16f,.18f,.17f);}
}
static void world505_cafe(V3 p,float yaw,u32 seed){
  world505_box(v3(p.x,p.y+.74f,p.z),.85f,.08f,.85f,yaw,.58f,.48f,.33f);world505_box(v3(p.x,p.y+.37f,p.z),.08f,.74f,.08f,yaw,.23f,.25f,.24f);
  for(int s=-1;s<=1;s+=2){V3 q=city340_rot(p,s*.72f,0,yaw);world505_box(v3(q.x,p.y+.44f,q.z),.39f,.08f,.41f,yaw,.38f,.28f,.18f);world505_box(v3(q.x,p.y+.24f,q.z),.30f,.40f,.30f,yaw,.24f,.25f,.23f);}
  world505_box(v3(p.x,p.y+1.50f,p.z),.04f,1.5f,.04f,yaw,.41f,.40f,.36f);
  world505_box(v3(p.x,p.y+2.22f,p.z),2.15f,.10f,2.15f,yaw,(seed&1)?.72f:.23f,(seed&1)?.66f:.37f,.30f);
}
static void world505_streets(void){
  int count=0;
  for(int i=0;i<g_stream_road_count08&&count<28;i++){
    StreamRoad08*s=&g_stream_roads08[i];if(s->road<0||s->road>=g_world.roadCount)continue;RoadWay*r=&g_world.roads[s->road];
    V3 delta=vsub(s->b,s->a),m=vmul(vadd(s->a,s->b),.5f);float len=vlen(delta);if(len<12||!world504_core(m)||!city340_near(m,240))continue;
    int hero=city330_contains_ci(r->name,"Hercilio Luz")||city330_contains_ci(r->name,"Herc\303\255lio Luz");
    int water=city330_contains_ci(r->name,"Victor Konder")||city330_contains_ci(r->name,"Beira");
    int central=hero||water||city330_contains_ci(r->name,"Felipe Schmidt")||city330_contains_ci(r->name,"Lauro")||city330_contains_ci(r->name,"Marcos Konder");if(!central)continue;
    float yaw=(float)atan2(delta.x,delta.z),width=maxf(r->width,6.0f),y=terrain11_height(m.x,m.z);u32 hash=roads130_hash(s);count++;
    // Fit each composition into the segment interior, clear of junctions.
    float span=minf(len-8,34),edge=width*.5f+1.3f;
    for(int side=-1;side<=1;side+=2){V3 q=city340_rot(v3(m.x,y,m.z),side*edge,0,yaw);
      world505_box(v3(q.x,y+.14f,q.z),2.35f,.12f,span,yaw,.59f,.57f,.51f);
      V3 curb=city340_rot(v3(m.x,y,m.z),side*(width*.5f+.08f),0,yaw);world505_box(v3(curb.x,y+.16f,curb.z),.18f,.22f,span,yaw,.66f,.64f,.57f);
      if(city340_near(m,95))for(int k=-3;k<=3;k++){V3 seam=city340_rot(q,0,k*span/8,yaw);world505_box(v3(seam.x,y+.207f,seam.z),2.28f,.008f,.026f,yaw,.39f,.39f,.35f);}
    }
    float side=water?city360_water_side(s,edge):((hash&1)?1.0f:-1.0f);V3 p=city340_rot(v3(m.x,y+.2f,m.z),side*(edge+.25f),0,yaw);
    if(hero||water){world505_bench(p,yaw);V3 q=city340_rot(p,0,4.2f,yaw);world505_box(v3(q.x,q.y+.30f,q.z),1.05f,.60f,1.05f,yaw,.46f,.45f,.38f);world505_box(v3(q.x,q.y+.68f,q.z),.95f,.35f,.95f,yaw,.22f,.35f,.19f);if(city340_near(m,120)){V3 cafe=city340_rot(p,0,-5,yaw);world505_cafe(cafe,yaw,hash);}}
    else {world505_box(v3(p.x,p.y+.50f,p.z),.46f,1,.46f,yaw,.20f,.30f,.27f);V3 q=city340_rot(p,0,3,yaw);world505_box(v3(q.x,q.y+.54f,q.z),.62f,1.08f,.32f,yaw,.52f,.54f,.49f);}
    if(water){V3 q=city340_rot(v3(m.x,y,m.z),side*(edge+3.7f),0,yaw);world505_box(v3(q.x,y+.20f,q.z),2.3f,.12f,span,yaw,.57f,.55f,.48f);world505_box(v3(q.x,y+1.13f,q.z),.07f,.07f,span,yaw,.40f,.43f,.43f);for(int k=-2;k<=2;k++){V3 post=city340_rot(q,0,k*span/5,yaw);world505_box(v3(post.x,y+.67f,post.z),.06f,.92f,.06f,yaw,.39f,.41f,.40f);}}
    g_world505_scenes++;
  }
}
static void world505_events(void){
  V3 p=city340_geo(-26.90959,-48.65287);if(!city340_near(p,700))return;p.y=terrain11_height(p.x,p.z);float yaw=-.03f;
  // Ribbed hall roof, glazed entrance bays, entrance stairs and forecourt.
  for(int k=-5;k<=5;k++){V3 q=city340_rot(p,k*4.15f,-1,yaw);world505_box(v3(q.x,p.y+10.35f,q.z),.22f,.36f,32,yaw,.78f,.80f,.78f);}
  for(int k=-4;k<=4;k++){V3 q=city340_rot(p,k*3.35f,-16.05f,yaw);world505_box(v3(q.x,p.y+3.0f,q.z),.10f,5.1f,.14f,yaw,.67f,.70f,.69f);}
  for(int k=0;k<3;k++){V3 q=city340_rot(p,0,-19.5f-k*.65f,yaw);world505_box(v3(q.x,p.y+.15f*(3-k),q.z),32,.30f*(3-k),.7f,yaw,.62f,.62f,.57f);}
  if(city340_near(p,240))for(int k=-3;k<=3;k++){V3 q=city340_rot(p,k*5.4f,-33,yaw);world505_bench(q,yaw);}
}
static void world505_draw(void){
  g_world505_boxes=g_world505_facades=g_world505_scenes=g_world505_labels=0;if(!g_stream_ready08)return;
  renderer09_set_material(MAT09_GENERIC);glBegin(GL_QUADS);world505_streets();world505_events();world505_buildings();glEnd();g_draw_calls++;renderer09_set_material(MAT09_GENERIC);
  static const char*labels[]={"CAFE CENTRAL","LIVRARIA","MODA & CIA","PADARIA","OTICA","FARMACIA","CASA & LAR","LANCHONETE"};
  renderer09_suspend_shader();for(int i=0;i<g_world505_labels;i++)identity150_text3d(g_world505_label_p[i],labels[g_world505_label_id[i]],.91f,.88f,.76f);renderer09_resume_shader();
}
