// -----------------------------------------------------------------------------
// 3.5 Building Visual Rework
// Layers Brazilian/coastal architectural cues over exact cadastral footprints.
// Geometry remains GeoItajai/OSM-driven; this pass is visual only.
// -----------------------------------------------------------------------------
static int g_city350_houses=0,g_city350_towers=0,g_city350_shops=0,g_city350_sheds=0,g_city350_civic=0;

typedef struct {V3 mid,along,out;float len,yaw;} City350Front;
static int city350_front(Building*b,City350Front*out){
  int id=building12_find_seed(b->seed);if(id<0)return 0;int n=g_building_poly_count12[id],s=g_building_poly_start12[id];if(n<3||s<0||s+n>g_building_vert_count12)return 0;float best=0;V3 ba=v3(0,0,0),bc=v3(0,0,0);
  for(int i=0;i<n;i++){V3 a=g_building_verts12[s+i],c=g_building_verts12[s+(i+1)%n],d=vsub(c,a);float l=vlen(d);if(l>best){best=l;ba=a;bc=c;}}
  if(best<2.0f)return 0;V3 d=vsub(bc,ba),along=vmul(d,1.0f/best),mid=vmul(vadd(ba,bc),.5f),outv=vnorm(vsub(mid,b->c));if(vlen(outv)<.2f)outv=v3(along.z,0,-along.x);out->mid=mid;out->along=along;out->out=outv;out->len=best;out->yaw=(float)atan2((double)along.x,(double)along.z);return 1;
}
static void city350_gable(Building*b,City350Front*f,float r,float g,float bl){
  float base=terrain11_height(b->c.x,b->c.z)+b->h+.04f,w=clampf(f->len*.94f,3.0f,26.0f),depth=clampf((b->sx+b->sz)*.32f,3.5f,18.0f),rise=clampf(depth*.30f,1.0f,4.5f);V3 c=v3(b->c.x,base,b->c.z),a=f->along,o=f->out;V3 p0=vadd(vadd(c,vmul(a,-w*.5f)),vmul(o,-depth*.5f)),p1=vadd(vadd(c,vmul(a,w*.5f)),vmul(o,-depth*.5f)),p2=vadd(vadd(c,vmul(a,w*.5f)),vmul(o,depth*.5f)),p3=vadd(vadd(c,vmul(a,-w*.5f)),vmul(o,depth*.5f)),r0=vadd(c,vmul(a,-w*.5f)),r1=vadd(c,vmul(a,w*.5f));r0.y+=rise;r1.y+=rise;
  glColor3f(r,g,bl);glBegin(GL_QUADS);glVertex3f(p0.x,p0.y,p0.z);glVertex3f(p1.x,p1.y,p1.z);glVertex3f(r1.x,r1.y,r1.z);glVertex3f(r0.x,r0.y,r0.z);glVertex3f(r0.x,r0.y,r0.z);glVertex3f(r1.x,r1.y,r1.z);glVertex3f(p2.x,p2.y,p2.z);glVertex3f(p3.x,p3.y,p3.z);glEnd();glBegin(GL_TRIANGLES);glVertex3f(p0.x,p0.y,p0.z);glVertex3f(r0.x,r0.y,r0.z);glVertex3f(p3.x,p3.y,p3.z);glVertex3f(p1.x,p1.y,p1.z);glVertex3f(p2.x,p2.y,p2.z);glVertex3f(r1.x,r1.y,r1.z);glEnd();g_draw_calls+=2;
}
static void city350_house(Building*b,City350Front*f){
  if(!city340_near(b->c,190.0f))return;float base=terrain11_height(b->c.x,b->c.z),q=((b->seed>>6)&7)/7.0f;city350_gable(b,f,.42f+.20f*q,.18f+.09f*q,.11f+.06f*q);
  float doorW=clampf(f->len*.28f,2.2f,4.2f);V3 p=vadd(f->mid,vmul(f->out,.10f));draw_box(v3(p.x,base+1.25f,p.z),doorW,2.45f,.14f,f->yaw,.24f,.25f,.24f);
  if(f->len>8.0f){float rem=(f->len-doorW)*.30f;for(int s=-1;s<=1;s+=2){V3 w=vadd(p,vmul(f->along,s*(doorW*.5f+rem*.62f)));draw_box(v3(w.x,base+1.55f,w.z),clampf(rem,1.0f,2.0f),1.15f,.11f,f->yaw,.18f,.28f,.30f);}}
  if((b->seed&1u)==0u){V3 tank=vadd(b->c,vmul(f->out,-minf(2.2f,b->sz*.15f)));draw_box(v3(tank.x,base+b->h+1.0f,tank.z),1.25f,1.65f,1.25f,0,.58f,.62f,.60f);}g_city350_houses++;
}
static void city350_tower(Building*b,City350Front*f,int sector){
  if(!city340_near(b->c,230.0f))return;float base=terrain11_height(b->c.x,b->c.z);int floors=(int)(b->h/3.15f);if(floors<4)return;if(floors>13)floors=13;float bw=clampf(f->len*.72f,4.2f,13.0f),glass=(sector==1||sector==4||sector==5||sector==6)?.55f:.34f;
  for(int fl=1;fl<floors;fl+=2){float y=base+fl*3.15f;V3 p=vadd(f->mid,vmul(f->out,.46f));draw_box(v3(p.x,y,p.z),bw,.14f,.92f,f->yaw,.66f,.68f,.68f);draw_box(v3(p.x,y+.55f,p.z),bw*.96f,.78f,.055f,f->yaw,.20f+glass*.20f,.34f+glass*.20f,.40f+glass*.25f);}
  V3 col=vadd(f->mid,vmul(f->out,.13f));draw_box(v3(col.x,base+b->h*.53f,col.z),clampf(f->len*.18f,1.4f,3.8f),b->h*.84f,.09f,f->yaw,.17f,.29f,.35f);
  draw_box(v3(b->c.x,base+b->h+.55f,b->c.z),clampf(b->sx*.24f,2.2f,5.0f),1.0f,clampf(b->sz*.24f,2.2f,5.0f),0,.55f,.57f,.57f);g_city350_towers++;
}
static void city350_shop(Building*b,City350Front*f){
  if(!city340_near(b->c,205.0f))return;float base=terrain11_height(b->c.x,b->c.z);V3 p=vadd(f->mid,vmul(f->out,.13f));float w=clampf(f->len*.86f,3.5f,16.0f);
  draw_box(v3(p.x,base+1.35f,p.z),w,2.45f,.13f,f->yaw,.10f,.20f,.24f);for(int k=-2;k<=2;k++){V3 q=vadd(p,vmul(f->along,k*w*.18f));draw_box(v3(q.x,base+1.35f,q.z),.08f,2.35f,.16f,f->yaw,.68f,.69f,.67f);}
  V3 aw=vadd(p,vmul(f->out,.72f));draw_box(v3(aw.x,base+2.75f,aw.z),w*.94f,.14f,1.45f,f->yaw,.50f+.18f*((b->seed>>4)&1),.13f,.10f);
  draw_box(v3(p.x,base+3.20f,p.z),w*.92f,.58f,.16f,f->yaw,.10f,.30f,.43f);g_city350_shops++;
}
static void city350_shed(Building*b,City350Front*f){
  if(!city340_near(b->c,260.0f))return;float base=terrain11_height(b->c.x,b->c.z),w=clampf(f->len*.80f,4.0f,22.0f);V3 p=vadd(f->mid,vmul(f->out,.12f));int doors=(int)(w/5.0f);if(doors<1)doors=1;if(doors>5)doors=5;
  for(int k=0;k<doors;k++){float t=(k+.5f)/doors-.5f;V3 q=vadd(p,vmul(f->along,t*w));draw_box(v3(q.x,base+2.05f,q.z),clampf(w/doors*.66f,2.7f,4.4f),3.85f,.13f,f->yaw,.20f,.22f,.23f);}
  for(int k=-1;k<=1;k++){V3 q=vadd(b->c,vmul(f->along,k*minf(5.0f,f->len*.22f)));draw_box(v3(q.x,base+b->h+.36f,q.z),2.8f,.70f,1.35f,f->yaw,.47f,.49f,.48f);}g_city350_sheds++;
}
static void city350_civic(Building*b,City350Front*f){
  if(!city340_near(b->c,220.0f))return;float base=terrain11_height(b->c.x,b->c.z);V3 p=vadd(f->mid,vmul(f->out,.12f));float w=clampf(f->len*.76f,5.0f,15.0f);draw_box(v3(p.x,base+.38f,p.z),w,.76f,.22f,f->yaw,.70f,.68f,.61f);for(int k=-2;k<=2;k++){V3 q=vadd(p,vmul(f->along,k*w*.18f));draw_box(v3(q.x,base+2.2f,q.z),.34f,3.5f,.34f,f->yaw,.78f,.76f,.70f);}g_city350_civic++;
}
static void city350_draw(void){
  g_city350_houses=g_city350_towers=g_city350_shops=g_city350_sheds=g_city350_civic=0;if(!g_stream_ready08)return;renderer09_set_material(0);
  for(int i=0;i<g_stream_building_count08;i++){Building*b=&g_stream_buildings08[i].b;if(!city340_near(b->c,270.0f))continue;City350Front f;if(!city350_front(b,&f))continue;int cls=urban10_building_class(b),sector=urban10_sector_at(b->c,NULL);if(cls==BLD10_HOUSE)city350_house(b,&f);else if(cls==BLD10_APARTMENT)city350_tower(b,&f,sector);else if(cls==BLD10_COMMERCIAL)city350_shop(b,&f);else if(cls==BLD10_INDUSTRIAL)city350_shed(b,&f);else if(cls==BLD10_CIVIC)city350_civic(b,&f);}
  renderer09_set_material(0);
}
static void city350_overlay(void){if(!g_debug||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;settings_draw_2d_begin();char b[220];hud_panel(20,(float)g_h-198,610,28);sprintf(b,"CITY 3.5 // casas %d | torres %d | comercio %d | galpoes %d | civicos %d",g_city350_houses,g_city350_towers,g_city350_shops,g_city350_sheds,g_city350_civic);gl_text(34,(float)g_h-177,b,.70f,.83f,.72f);settings_draw_2d_end();}
