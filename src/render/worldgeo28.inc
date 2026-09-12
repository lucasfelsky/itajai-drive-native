// -----------------------------------------------------------------------------
// 2.8 World Geometry 2.0.
// Near-field roof silhouettes, rooftop equipment, podiums and curbside greenery
// break up repeated OSM extrusions without changing the source footprint cache.
// -----------------------------------------------------------------------------
static int g_world28_roofs=0,g_world28_mech=0,g_world28_green=0,g_world28_podium=0;
static float world28_hash01(u32 s){s^=s>>16;s*=0x7feb352du;s^=s>>15;s*=0x846ca68bu;s^=s>>16;return (s&65535u)/65535.0f;}
static void world28_roof_for(const Building*b){
  float dx=b->c.x-g_player.body.p.x,dz=b->c.z-g_player.body.p.z;if(dx*dx+dz*dz>245.0f*245.0f)return;int cls=urban10_building_class(b);float base=terrain11_height(b->c.x,b->c.z),top=base+b->h;float q=world28_hash01(b->seed^0x51a3u);
  renderer09_set_material(MAT09_BUILDING);
  if(cls==BLD10_HOUSE&&b->sx>3.2f&&b->sz>3.2f){float h=.55f+.55f*q;draw_box(v3(b->c.x,top+h*.35f,b->c.z),b->sx*.86f,h,b->sz*.72f,0,.46f+.12f*q,.28f+.08f*q,.20f+.05f*q);g_world28_roofs++;}
  else if(cls==BLD10_APARTMENT||b->h>22.0f){float sx=minf(7.5f,b->sx*.38f),sz=minf(6.5f,b->sz*.34f);draw_box(v3(b->c.x,top+.32f,b->c.z),sx,.62f,sz,0,.54f,.56f,.58f);g_world28_roofs++;if((b->seed&3u)!=0){float ox=(q-.5f)*minf(4.0f,b->sx*.22f);draw_box(v3(b->c.x+ox,top+.82f,b->c.z),1.25f,.72f,1.05f,0,.36f,.39f,.41f);g_world28_mech++;}}
  else if(cls==BLD10_INDUSTRIAL&&b->sx>8.0f&&b->sz>8.0f){int vents=1+(int)((b->seed>>5)&3u);for(int k=0;k<vents;k++){float t=(k+1.0f)/(vents+1.0f);float x=b->c.x-b->sx*.30f+t*b->sx*.60f;draw_box(v3(x,top+.35f,b->c.z),.72f,.70f,.72f,0,.30f,.32f,.33f);g_world28_mech++;}}
  if((cls==BLD10_COMMERCIAL||cls==BLD10_APARTMENT)&&b->h>9.0f&&b->sx>7.0f&&b->sz>7.0f&&dx*dx+dz*dz<170.0f*170.0f){draw_box(v3(b->c.x,base+.46f,b->c.z),b->sx*1.055f,.88f,b->sz*1.045f,0,.46f,.48f,.49f);g_world28_podium++;}
  renderer09_set_material(0);
}
static void world28_greenery(void){
  g_world28_green=0;if(!g_stream_ready08)return;int tree=asset08_find("urban_tree"),palm=asset08_find("coastal_palm");if(tree<0)return;int stride=g_lod20_tier>=2?5:3;
  for(int i=0;i<g_stream_road_count08;i+=stride){StreamRoad08*s=&g_stream_roads08[i];V3 d=vsub(s->b,s->a);float len=vlen(d);if(len<16||len>130)continue;V3 m=vmul(vadd(s->a,s->b),.5f);float dx=m.x-g_player.body.p.x,dz=m.z-g_player.body.p.z;if(dx*dx+dz*dz>210.0f*210.0f)continue;u32 h=roads130_hash(s);if((h%5u)>1u)continue;V3 dir=vnorm(d),rr=v3(dir.z,0,-dir.x);float rw=(s->road>=0&&s->road<g_world.roadCount)?g_world.roads[s->road].width:7.0f;float side=(h&1u)?1.0f:-1.0f;V3 p=vadd(m,vmul(rr,side*(rw*.5f+2.2f)));p.y=terrain11_height(p.x,p.z);int sec=urban10_sector_at(p,NULL);int id=(sec>=4&&palm>=0)?palm:tree;float sc=.78f+((h>>8)&31u)/100.0f;asset08_draw_instance(id,p,(float)((h>>16)&255u)*.0246f,sc,.30f,.54f,.28f);g_world28_green++;}
}
static void world28_draw(void){g_world28_roofs=g_world28_mech=g_world28_podium=0;if(g_stream_ready08)for(int i=0;i<g_stream_building_count08;i++)world28_roof_for(&g_stream_buildings08[i].b);world28_greenery();}
