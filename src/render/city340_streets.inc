// -----------------------------------------------------------------------------
// 3.4 named-street identity. Uses real OSM road names already present in the
// world graph so the visual treatment follows the actual Itajai layout.
// -----------------------------------------------------------------------------
static int g_city340_named_segments=0,g_city340_street_signs=0,g_city340_cycle_segments=0;
static void city340_road_quad(StreamRoad08*s,float offset,float width,float r,float g,float b){
  V3 d=vsub(s->b,s->a);float len=vlen(d);if(len<1.0f)return;V3 rt=v3(d.z/len,0,-d.x/len);float a0=offset-width*.5f,a1=offset+width*.5f;V3 p0=vadd(s->a,vmul(rt,a0)),p1=vadd(s->a,vmul(rt,a1)),p2=vadd(s->b,vmul(rt,a1)),p3=vadd(s->b,vmul(rt,a0));float y0=terrain11_height(s->a.x,s->a.z)+.091f,y1=terrain11_height(s->b.x,s->b.z)+.091f;glColor3f(r,g,b);glBegin(GL_QUADS);glVertex3f(p0.x,y0,p0.z);glVertex3f(p1.x,y0,p1.z);glVertex3f(p2.x,y1,p2.z);glVertex3f(p3.x,y1,p3.z);glEnd();g_draw_calls++;
}
static void city340_crosswalk(StreamRoad08*s,RoadWay*r){
  V3 d=vsub(s->b,s->a);float len=vlen(d);if(len<11.0f)return;V3 f=vmul(d,1.0f/len),rt=v3(f.z,0,-f.x),m=vmul(vadd(s->a,s->b),.5f);float yaw=(float)atan2((double)f.x,(double)f.z);for(int k=-3;k<=3;k++){V3 q=vadd(m,vmul(rt,k*1.25f));float y=terrain11_height(q.x,q.z)+.105f;draw_box(v3(q.x,y,q.z),.72f,.025f,minf(3.1f,r->width*.28f),yaw,.88f,.87f,.80f);} }
static void city340_named_sign(V3 p,const char*label,float yaw){
  float y=terrain11_height(p.x,p.z);draw_box(v3(p.x,y+1.7f,p.z),.12f,3.4f,.12f,yaw,.42f,.44f,.43f);draw_box(v3(p.x,y+3.15f,p.z),4.8f,.78f,.14f,yaw,.06f,.27f,.48f);renderer09_suspend_shader();identity150_text3d(v3(p.x-2.0f,y+3.22f,p.z-.10f),label,.96f,.96f,.91f);renderer09_resume_shader();g_city340_street_signs++;
}
static void city340_draw_streets(void){
  g_city340_named_segments=0;g_city340_street_signs=0;g_city340_cycle_segments=0;if(!g_stream_ready08)return;renderer09_set_material(0);
  for(int i=0;i<g_stream_road_count08;i++){StreamRoad08*s=&g_stream_roads08[i];if(s->road<0||s->road>=g_world.roadCount)continue;RoadWay*r=&g_world.roads[s->road];V3 m=vmul(vadd(s->a,s->b),.5f);if(!city340_near(m,520.0f))continue;u32 h=roads130_hash(s);
    int beira=city330_contains_ci(r->name,"Victor Konder")||city330_contains_ci(r->name,"Beira-Rio")||city330_contains_ci(r->name,"Beira Rio");
    int uruguai=city330_contains_ci(r->name,"Uruguai");int centro=city330_contains_ci(r->name,"Herc")||city330_contains_ci(r->name,"Marcos Konder")||city330_contains_ci(r->name,"Sete de Setembro");
    if(beira){float side=(h&1u)?1.0f:-1.0f;city340_road_quad(s,side*(r->width*.5f+1.15f),1.85f,.55f,.12f,.10f);g_city340_cycle_segments++;g_city340_named_segments++;if((h%13u)==0u){V3 d=vsub(s->b,s->a);float len=vlen(d);if(len>1){V3 f=vmul(d,1.0f/len),rt=v3(f.z,0,-f.x),p=vadd(m,vmul(rt,side*(r->width*.5f+3.8f)));city340_named_sign(p,"BEIRA-RIO",(float)atan2((double)f.x,(double)f.z));}}}
    else if(uruguai){g_city340_named_segments++;if((h%11u)==2u)city340_crosswalk(s,r);if((h%17u)==4u){V3 d=vsub(s->b,s->a);float len=vlen(d);if(len>1){V3 f=vmul(d,1.0f/len),rt=v3(f.z,0,-f.x),p=vadd(m,vmul(rt,r->width*.5f+2.4f));city340_named_sign(p,"RUA URUGUAI",(float)atan2((double)f.x,(double)f.z));}}}
    else if(centro){g_city340_named_segments++;if((h%15u)==3u)city340_crosswalk(s,r);}
  }
}
