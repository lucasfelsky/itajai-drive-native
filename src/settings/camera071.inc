// 0.7.1 mouse-look override: persistent sensitivity + optional inverted Y.
static void camera_mouse_move(HWND h,LPARAM l){
  if(!g_camera_captured)return;RECT r;if(!GetClientRect(h,&r))return;int cx=(r.right-r.left)/2,cy=(r.bottom-r.top)/2;int x=(int)(short)(l&0xffff),y=(int)(short)((l>>16)&0xffff);int dx=x-cx,dy=y-cy;if(dx==0&&dy==0)return;
  g_camera_yaw_target=sim07_wrap_angle(g_camera_yaw_target+dx*g_camera_sensitivity);float ysign=g_camera_invert_y?-1.0f:1.0f;g_camera_pitch_target=clampf(g_camera_pitch_target+dy*g_camera_sensitivity*.78f*ysign,-.55f,.58f);camera_recenter(h);
}
