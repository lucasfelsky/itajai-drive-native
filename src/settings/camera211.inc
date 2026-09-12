// -----------------------------------------------------------------------------
// 2.11 chase-camera input wrapper.
// Records genuine mouse motion so the gameplay layer can temporarily yield auto
// follow to the player, then smoothly return to the driving-oriented camera.
// -----------------------------------------------------------------------------
static float g_camera_mouse_idle211=999.0f;
static int g_camera_mouse_override211=0;

static void camera_mouse_move(HWND h,LPARAM l){
  if(!g_camera_captured)return;
  RECT r;if(!GetClientRect(h,&r))return;
  int cx=(r.right-r.left)/2,cy=(r.bottom-r.top)/2;
  int x=(int)(short)(l&0xffff),y=(int)(short)((l>>16)&0xffff);
  int dx=x-cx,dy=y-cy;
  if(dx!=0||dy!=0){g_camera_mouse_idle211=0.0f;g_camera_mouse_override211=1;}
  camera_mouse_move_legacy071(h,l);
}
