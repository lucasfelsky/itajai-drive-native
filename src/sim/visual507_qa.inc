// Visual QA entry point (compiled only with ITAJAI_VISUAL_QA; never in public builds).
// ITAJAI_LANDMARK_QA selects the 5.0.7 landmark checkpoint: nine facade views,
// rear views, night views, an oblique, a distant LOD view and a continuous
// street-level traversal of the worked Centro stretch with frame-time statistics.
// Without it the 5.0.5 fixed street/vehicle scenes are used.
#ifndef ITAJAI_LANDMARK_QA
#include "visual505_qa.inc"
#else
__declspec(dllimport) void __stdcall glReadPixels(int,int,int,int,unsigned int,unsigned int,void*);
#define QA507_SCENES 17
#define QA507_ROUTE 16
static int g_qa507_frame=0,g_qa507_scene=0;
static int qa507_id(void){int s=g_qa507_scene;if(s<9)return s;if(s==9||s==12||s==15)return 0;if(s==10||s==13)return 2;if(s==11)return 1;if(s==14)return 6;return 5;}
static void qa507_frame(int id,Lm507*f){f->c=project_geo(g_landmarks506[id].lat,g_landmarks506[id].lon);f->y=terrain11_height(f->c.x,f->c.z);f->yaw=g_landmarks506[id].yaw;f->cs=(float)cos(f->yaw);f->sn=(float)sin(f->yaw);f->near=f->fine=1;f->dark=0;}
// Traversal: facade forecourts of the worked buildings, in street order from
// Praca Vidal Ramos to the Matriz. Straight segments at 1.6 m, 14 m/s (50 km/h);
// it measures streaming and draw load along the stretch, not vehicle physics.
static const int g_qa507_route_ids[8]={5,4,3,8,1,6,2,0};
static double g_qa507_route_t=0,g_qa507_route_dt_max=0,g_qa507_route_time=0;static int g_qa507_route_frames=0,g_qa507_route_slow=0,g_qa507_route_shots=0,g_qa507_route_started=0;static float g_qa507_route_len=0;static LARGE_INTEGER g_qa507_route_last;
static V3 qa507_waypoint(int k){Lm507 f;qa507_frame(g_qa507_route_ids[k],&f);const Landmark506*l=&g_landmarks506[g_qa507_route_ids[k]];V3 p=lm507_at(&f,0,0,-(l->d*.5f+14));p.y=terrain11_height(p.x,p.z)+1.6f;return p;}
static void qa507_route_position(double dist,V3*pos,float*yaw){double acc=0;for(int k=0;k<7;k++){V3 a=qa507_waypoint(k),b=qa507_waypoint(k+1);V3 d=vsub(b,a);d.y=0;float len=vlen(d);if(dist<=acc+len||k==6){float t=len>0?(float)((dist-acc)/len):0;if(t>1)t=1;if(t<0)t=0;*pos=vadd(a,vmul(vsub(b,a),t));pos->y=terrain11_height(pos->x,pos->z)+1.6f;*yaw=(float)atan2(d.x,d.z);return;}acc+=len;}}
static void qa505_pose(void){
  int id=qa507_id();Lm507 f;qa507_frame(id,&f);const Landmark506*l=&g_landmarks506[id];
  // QA frames must never idle: keep the simulation loop active so the exact
  // footprint sidecar loads and the window-inactive Sleep path is not taken.
  g_window_active500=1;g_player.pitch=g_player.roll=g_player.speed=0;g_player.car=0;g_player.camera=0;g_weather=0;g_photo70=1;g_photo70_hide_hud=1;g_hour=(g_qa507_scene==12||g_qa507_scene==13)?21:15;
  if(g_qa507_scene==QA507_ROUTE){
    if(!g_qa507_route_started){g_qa507_route_started=1;g_qa507_route_len=0;for(int k=0;k<7;k++){V3 d=vsub(qa507_waypoint(k+1),qa507_waypoint(k));d.y=0;g_qa507_route_len+=vlen(d);}QueryPerformanceCounter(&g_qa507_route_last);g_qa507_route_t=0;}
    else{LARGE_INTEGER now,freq;QueryPerformanceCounter(&now);QueryPerformanceFrequency(&freq);double dt=(double)(now.QuadPart-g_qa507_route_last.QuadPart)/(double)freq.QuadPart;g_qa507_route_last=now;if(dt>.25)dt=.25;g_qa507_route_time+=dt;g_qa507_route_frames++;if(dt>g_qa507_route_dt_max)g_qa507_route_dt_max=dt;if(dt>1.0/50.0)g_qa507_route_slow++;g_qa507_route_t+=14.0*dt;}
    V3 pos;float yaw;qa507_route_position(g_qa507_route_t,&pos,&yaw);g_player.body.p=pos;g_player.body.yaw=yaw;
  }else{
    float back=l->d*.5f+(g_qa507_scene==15?450.0f:22.0f);g_player.body.p=lm507_at(&f,0,0,g_qa507_scene==9||g_qa507_scene==10||g_qa507_scene==11?back:-back);g_player.body.p.y=terrain11_height(g_player.body.p.x,g_player.body.p.z)+.66f;g_player.body.yaw=0;
  }
  sim07_stream_update(g_player.body.p,1);stream08_refresh(g_player.body.p,1);
}
static void qa505_camera(void){
  int id=qa507_id();Lm507 f;qa507_frame(id,&f);const Landmark506*l=&g_landmarks506[id];
  // Street camera: the eye stands in the street/square in front of the facade
  // (inside the gap to the opposite block), slightly off-axis for a 3/4 view.
  // Per-building eye distance from the facade = the real street/square gap in
  // front of it (surveyed), so the camera never stands inside the opposite block.
  static const float camDist[LANDMARK506_COUNT]={29,12,14,12,11,13,60,29,55},camH[LANDMARK506_COUNT]={15,10,11,9,11,8,24,10,20};
  float dist=camDist[id],eyeH=camH[id],tgtH=id==0?11:id==6?8:id==2?6:id==4?6:4;
  V3 target=lm507_at(&f,0,tgtH,0),eye;
  int s=g_qa507_scene;
  if(s==9||s==10||s==11)eye=lm507_at(&f,l->w*.45f,eyeH,l->d*.5f+dist);
  else if(s==14)eye=lm507_at(&f,-(l->w*.5f+dist*.9f),eyeH*1.4f,-(l->d*.5f+dist*.7f));
  else if(s==15)eye=lm507_at(&f,-100,60,-450);
  else if(s==QA507_ROUTE){V3 fwd=v3((float)sin(g_player.body.yaw),0,(float)cos(g_player.body.yaw));eye=vsub(g_player.body.p,vmul(fwd,6.5f));eye.y=g_player.body.p.y+1.4f;target=vadd(g_player.body.p,vmul(fwd,12));target.y=g_player.body.p.y+.6f;}
  else eye=lm507_at(&f,-l->w*.45f,eyeH,-(l->d*.5f+dist));
  glMatrixMode(GL_MODELVIEW);glLoadIdentity();V3 fw=vnorm(vsub(target,eye)),side=vnorm(vcross(fw,v3(0,1,0))),up=vcross(side,fw);
  float mat[16]={side.x,up.x,-fw.x,0,side.y,up.y,-fw.y,0,side.z,up.z,-fw.z,0,-vdot(side,eye),-vdot(up,eye),vdot(fw,eye),1};glMultMatrixf(mat);g_renderer_camera_eye09=eye;renderer09_begin_world(side,up,fw);
}
static void qa507_audit(void){
  static char report[24000];char path[640];int n=0,removed=0,ghosts=0,solids=0,per[LANDMARK506_COUNT];memset(per,0,sizeof(per));
  for(int i=0;i<g_world.buildingCount;i++){const Building*b=&g_world.buildings[i];int id=landmark506_owner(b);if(id<0)continue;removed++;per[id]++;float lm=g_landmarks506[id].w*g_landmarks506[id].d;if(n<20000)n+=sprintf(report+n,"replace=%s source=%d seed=%u size=%.1fx%.1f box_area=%.0f landmark_area=%.0f%s\n",g_landmarks506[id].name,i,b->seed,b->sx,b->sz,b->sx*b->sz,lm,b->sx*b->sz>lm*2.2f?" CHECK:large-source":"");}
  for(int i=0;i<g_static_collider_count;i++){StaticCollider*c=&g_static_colliders[i];if(c->type!=COLLIDER_BUILDING)continue;if(c->source<=-100)solids++;else if(c->source>=0&&landmark506_replaces(&g_world.buildings[c->source]))ghosts++;}
  for(int k=0;k<LANDMARK506_COUNT;k++)n+=sprintf(report+n,"landmark=%s ref=%s conf=%d replaced=%d\n",g_landmarks506[k].name,g_landmarks506[k].ref,g_landmarks506[k].conf,per[k]);
  n+=sprintf(report+n,"removed=%d ghosts=%d solids=%d total=%d exact=%d\n",removed,ghosts,solids,g_world.buildingCount,g_building12_ready);sprintf(path,"%s\\qa507-ownership.txt",g_exe_dir);file_write_all(path,report,n);if(ghosts||solids!=LANDMARK506_SOLIDS||!removed)ExitProcess(5);
}
static void qa507_shot(const char*name,float fps){
  unsigned char*data=(unsigned char*)VirtualAlloc(NULL,g_w*g_h*4,MEM_COMMIT|MEM_RESERVE,4),*out=(unsigned char*)VirtualAlloc(NULL,g_w*g_h*3+128,MEM_COMMIT|MEM_RESERVE,4);if(!data||!out)ExitProcess(2);
  glReadPixels(0,0,g_w,g_h,GL_RGBA,GL_UNSIGNED_BYTE,data);char path[640];sprintf(path,"%s\\qa507-%s.ppm",g_exe_dir,name);int n=sprintf((char*)out,"P6\n%d %d\n255\n",g_w,g_h);
  for(int y=g_h-1;y>=0;y--)for(int x=0;x<g_w;x++)for(int k=0;k<3;k++)out[n++]=data[(y*g_w+x)*4+k];if(!file_write_all(path,(char*)out,n))ExitProcess(3);VirtualFree(data,0,MEM_RELEASE);VirtualFree(out,0,MEM_RELEASE);
  char metrics[320];n=sprintf(metrics,"scene=%s fps=%.1f draws=%d assets=%d authored=%d openings=%d generic_facades=%d size=%dx%d\n",name,fps,g_draw_calls,g_asset_count08,g_world507_buildings,g_world507_detail,g_world505_facades,g_w,g_h);sprintf(path,"%s\\qa507-metrics-%s.txt",g_exe_dir,name);file_write_all(path,metrics,n);
}
static void qa505_capture(float fps){
  char name[64];
  if(g_qa507_scene==QA507_ROUTE){
    if(g_qa507_route_frames<2)return;
    double frac=g_qa507_route_len>0?g_qa507_route_t/g_qa507_route_len:1;
    if(g_qa507_route_shots<3&&frac>=.25*(g_qa507_route_shots+1)){sprintf(name,"route-%d",g_qa507_route_shots++);qa507_shot(name,fps);}
    if(g_qa507_route_t>=g_qa507_route_len){char path[640],txt[400];double avg=g_qa507_route_frames/maxf((float)g_qa507_route_time,.001f);int n=sprintf(txt,"route_m=%.0f frames=%d seconds=%.1f avg_fps=%.1f min_fps=%.1f frames_below_50fps=%d size=%dx%d\n",g_qa507_route_len,g_qa507_route_frames,g_qa507_route_time,avg,1.0/maxf((float)g_qa507_route_dt_max,.001f),g_qa507_route_slow,g_w,g_h);sprintf(path,"%s\\qa507-route.txt",g_exe_dir);file_write_all(path,txt,n);ExitProcess(0);}
    return;
  }
  if(++g_qa507_frame<60)return;if(g_qa507_scene==0)qa507_audit();
  sprintf(name,"%d",g_qa507_scene);qa507_shot(name,fps);g_qa507_frame=0;g_qa507_scene++;
}
#endif
