// Compile with /DITAJAI_VISUAL_QA for repeatable, HUD-free framebuffer evidence.
// This file is absent from normal beta execution; never reads gameplay input.
__declspec(dllimport) void __stdcall glReadPixels(int,int,int,int,unsigned int,unsigned int,void*);
static int g_qa505_frame=0,g_qa505_scene=0;
static void qa505_pose(void){
  static const double lat[4]={-26.90712,-26.90615,-26.90930,-26.90712};
  static const double lon[4]={-48.65920,-48.65460,-48.65340,-48.65920};
  g_player.body.p=city340_geo(lat[g_qa505_scene],lon[g_qa505_scene]);g_player.body.p.y=terrain11_height(g_player.body.p.x,g_player.body.p.z)+.66f;
  g_player.body.yaw=0;g_player.pitch=g_player.roll=g_player.speed=0;g_player.car=0;g_player.camera=0;g_hour=15;g_weather=0;g_photo70=1;g_photo70_hide_hud=1;
  sim07_stream_update(g_player.body.p,1);stream08_refresh(g_player.body.p,1);
  const char*name=g_qa505_scene==1?"Lauro":g_qa505_scene==2?"Victor Konder":"Herc";float best=1e12f;V3 anchor=g_player.body.p;
  for(int i=0;i<g_stream_road_count08;i++){StreamRoad08*r=&g_stream_roads08[i];if(r->road<0||r->road>=g_world.roadCount||!city330_contains_ci(g_world.roads[r->road].name,name))continue;V3 m=vmul(vadd(r->a,r->b),.5f),d=vsub(m,anchor);float ds=d.x*d.x+d.z*d.z;if(ds<best){best=ds;g_player.body.p=m;V3 along=vsub(r->b,r->a);g_player.body.yaw=(float)atan2(along.x,along.z);}}
  g_player.body.p.y=terrain11_height(g_player.body.p.x,g_player.body.p.z)+.66f;

}
static void qa505_camera(void){
  V3 target=vadd(g_player.body.p,v3(0,.5f,0)),eye=city340_rot(target,g_qa505_scene==3?3.5f:-1.0f,g_qa505_scene==3?4.6f:-8.0f,g_player.body.yaw);eye.y+=g_qa505_scene==3?1.1f:2.2f;
  glMatrixMode(GL_MODELVIEW);glLoadIdentity();V3 fw=vnorm(vsub(target,eye)),side=vnorm(vcross(fw,v3(0,1,0))),up=vcross(side,fw);
  float mat[16]={side.x,up.x,-fw.x,0,side.y,up.y,-fw.y,0,side.z,up.z,-fw.z,0,-vdot(side,eye),-vdot(up,eye),vdot(fw,eye),1};glMultMatrixf(mat);g_renderer_camera_eye09=eye;renderer09_begin_world(side,up,fw);
}
static void qa505_capture(float fps){
  if(++g_qa505_frame<90)return;
  unsigned char*data=(unsigned char*)VirtualAlloc(NULL,g_w*g_h*4,MEM_COMMIT|MEM_RESERVE,4);if(!data)ExitProcess(2);
  unsigned char*out=(unsigned char*)VirtualAlloc(NULL,g_w*g_h*3+128,MEM_COMMIT|MEM_RESERVE,4);if(!out)ExitProcess(2);
  glReadPixels(0,0,g_w,g_h,GL_RGBA,GL_UNSIGNED_BYTE,data);char path[640];sprintf(path,"%s\\qa505-%d.ppm",g_exe_dir,g_qa505_scene);int n=sprintf((char*)out,"P6\n%d %d\n255\n",g_w,g_h);
  for(int y=g_h-1;y>=0;y--)for(int x=0;x<g_w;x++)for(int k=0;k<3;k++)out[n++]=data[(y*g_w+x)*4+k];
  if(!file_write_all(path,(char*)out,n))ExitProcess(3);VirtualFree(data,0,MEM_RELEASE);VirtualFree(out,0,MEM_RELEASE);
  sprintf(path,"%s\\qa505-metrics-%d.txt",g_exe_dir,g_qa505_scene);char metrics[256];int len=sprintf(metrics,"scene=%d fps=%.1f boxes=%d facades=%d scenes=%d draws=%d assets=%d\n",g_qa505_scene,fps,g_world505_boxes,g_world505_facades,g_world505_scenes,g_draw_calls,g_asset_count08);file_write_all(path,metrics,len);
  g_qa505_frame=0;if(++g_qa505_scene==4)ExitProcess(0);
}
