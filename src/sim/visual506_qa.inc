// Landmark checkpoint captures: eight front views, rear, night and distant LOD.
__declspec(dllimport) void __stdcall glReadPixels(int,int,int,int,unsigned int,unsigned int,void*);
static int g_qa506_frame=0,g_qa506_scene=0;
static int qa506_id(void){return g_qa506_scene<8?g_qa506_scene:g_qa506_scene==8?0:g_qa506_scene==9?2:0;}
static void qa505_pose(void){
  const Landmark506*l=&g_landmarks506[qa506_id()];V3 p=project_geo(l->lat,l->lon);g_player.body.p=city340_rot(p,0,-60,l->yaw);g_player.body.p.y=terrain11_height(p.x,p.z)+.66f;
  g_player.body.yaw=0;g_player.pitch=g_player.roll=g_player.speed=0;g_player.car=0;g_player.camera=0;g_hour=g_qa506_scene==10?21:15;g_weather=0;g_photo70=1;g_photo70_hide_hud=1;
  sim07_stream_update(g_player.body.p,1);stream08_refresh(g_player.body.p,1);
}
static void qa505_camera(void){
  const Landmark506*l=&g_landmarks506[qa506_id()];V3 p=project_geo(l->lat,l->lon);float y=terrain11_height(p.x,p.z),dist=qa506_id()==0?78:qa506_id()==6?60:34;
  V3 target=v3(p.x,y+(qa506_id()==0?12:7),p.z),eye=city340_rot(v3(p.x,y,p.z),-7,g_qa506_scene==8?80:-dist,l->yaw);eye.y=y+13;
  if(g_qa506_scene>=3&&g_qa506_scene<=5){eye=city340_rot(v3(p.x,y+7,p.z),0,-20,l->yaw);target.y=y+4;}
  if(g_qa506_scene==9){eye=city340_rot(v3(p.x,y+20,p.z),40,60,l->yaw);}if(g_qa506_scene==11){eye=city340_rot(v3(p.x,y+60,p.z),-100,-450,l->yaw);g_player.body.p=eye;stream08_refresh(eye,1);}
  glMatrixMode(GL_MODELVIEW);glLoadIdentity();V3 fw=vnorm(vsub(target,eye)),side=vnorm(vcross(fw,v3(0,1,0))),up=vcross(side,fw);
  float mat[16]={side.x,up.x,-fw.x,0,side.y,up.y,-fw.y,0,side.z,up.z,-fw.z,0,-vdot(side,eye),-vdot(up,eye),vdot(fw,eye),1};glMultMatrixf(mat);g_renderer_camera_eye09=eye;renderer09_begin_world(side,up,fw);
}
static void qa506_audit(void){
  static char report[16000];char path[640];int n=0,removed=0,ghosts=0,solids=0;
  for(int i=0;i<g_world.buildingCount;i++){int id=landmark506_owner(&g_world.buildings[i]);if(id<0)continue;removed++;if(n<14000)n+=sprintf(report+n,"replace=%s source=%d seed=%u size=%.1fx%.1f\n",g_landmarks506[id].name,i,g_world.buildings[i].seed,g_world.buildings[i].sx,g_world.buildings[i].sz);}
  for(int i=0;i<g_static_collider_count;i++){StaticCollider*c=&g_static_colliders[i];if(c->type!=COLLIDER_BUILDING)continue;if(c->source<=-100)solids++;else if(c->source>=0&&landmark506_replaces(&g_world.buildings[c->source]))ghosts++;}
  n+=sprintf(report+n,"removed=%d ghosts=%d solids=%d total=%d exact=%d\n",removed,ghosts,solids,g_world.buildingCount,g_building12_ready);sprintf(path,"%s\\qa506-ownership.txt",g_exe_dir);file_write_all(path,report,n);if(ghosts||solids!=LANDMARK506_SOLIDS||!removed)ExitProcess(5);
}
static void qa505_capture(float fps){
  if(++g_qa506_frame<60)return;if(g_qa506_scene==0)qa506_audit();
  unsigned char*data=(unsigned char*)VirtualAlloc(NULL,g_w*g_h*4,MEM_COMMIT|MEM_RESERVE,4),*out=(unsigned char*)VirtualAlloc(NULL,g_w*g_h*3+128,MEM_COMMIT|MEM_RESERVE,4);if(!data||!out)ExitProcess(2);
  glReadPixels(0,0,g_w,g_h,GL_RGBA,GL_UNSIGNED_BYTE,data);char path[640];sprintf(path,"%s\\qa506-%d.ppm",g_exe_dir,g_qa506_scene);int n=sprintf((char*)out,"P6\n%d %d\n255\n",g_w,g_h);
  for(int y=g_h-1;y>=0;y--)for(int x=0;x<g_w;x++)for(int k=0;k<3;k++)out[n++]=data[(y*g_w+x)*4+k];if(!file_write_all(path,(char*)out,n))ExitProcess(3);VirtualFree(data,0,MEM_RELEASE);VirtualFree(out,0,MEM_RELEASE);
  char metrics[256];n=sprintf(metrics,"scene=%d fps=%.1f draws=%d assets=%d generic_facades=%d\n",g_qa506_scene,fps,g_draw_calls,g_asset_count08,g_world505_facades);sprintf(path,"%s\\qa506-metrics-%d.txt",g_exe_dir,g_qa506_scene);file_write_all(path,metrics,n);
  g_qa506_frame=0;if(++g_qa506_scene==12)ExitProcess(0);
}
