// -----------------------------------------------------------------------------
// Persistent settings implementation. Loaded after init_paths() and saved beside
// the executable. Updater does not manage this file.
// -----------------------------------------------------------------------------
#define CAMERA_SENS_DEFAULT .00170f
#define CAMERA_SENS_MIN     .00055f
#define CAMERA_SENS_MAX     .00420f
#define CAMERA_SENS_STEP    .00015f

static void settings_defaults(void){g_camera_sensitivity=CAMERA_SENS_DEFAULT;g_camera_invert_y=0;}
static void settings_build_path(void){sprintf(g_settings_path,"%s\\itajai_settings.ini",g_exe_dir);}
static void settings_save(void){
  char buf[256];int n=sprintf(buf,"# Itajai Drive local settings\r\ncamera_sensitivity=%.6f\r\ncamera_invert_y=%d\r\n",g_camera_sensitivity,g_camera_invert_y?1:0);if(n>0)file_write_all(g_settings_path,buf,(DWORD)n);g_settings_dirty=0;
}
static void settings_load(void){
  settings_build_path();settings_defaults();char*data=NULL;DWORD size=0;if(!file_read_all(g_settings_path,&data,&size)){settings_save();return;}
  const char*p=strstr(data,"camera_sensitivity=");if(p){p+=19;g_camera_sensitivity=clampf((float)atof(p),CAMERA_SENS_MIN,CAMERA_SENS_MAX);}p=strstr(data,"camera_invert_y=");if(p){p+=16;g_camera_invert_y=atoi(p)?1:0;}VirtualFree(data,0,MEM_RELEASE);g_settings_dirty=0;
}
static int settings_sensitivity_percent(void){return (int)(g_camera_sensitivity/.00310f*100.0f+.5f);}
static void settings_adjust_sensitivity(int dir){g_camera_sensitivity=clampf(g_camera_sensitivity+CAMERA_SENS_STEP*(float)dir,CAMERA_SENS_MIN,CAMERA_SENS_MAX);g_settings_dirty=1;settings_save();}
static void settings_restore_defaults(void){settings_defaults();g_settings_dirty=1;settings_save();}
static void settings_open_menu(void){if(g_camera_captured)camera_release_mouse();g_settings_menu_open=1;g_settings_selected=0;}
static void settings_close_menu(void){if(g_settings_dirty)settings_save();g_settings_menu_open=0;}
static void settings_menu_update(void){
  update_key_edges();
  if(key_tap(VK_ESCAPE)){settings_close_menu();return;}
  int up=key_tap(VK_UP)||key_tap('W'),down=key_tap(VK_DOWN)||key_tap('S');if(up){g_settings_selected--;if(g_settings_selected<0)g_settings_selected=4;}if(down){g_settings_selected++;if(g_settings_selected>4)g_settings_selected=0;}
  int left=key_tap(VK_LEFT)||key_tap('A'),right=key_tap(VK_RIGHT)||key_tap('D');
  if(g_settings_selected==0){if(left)settings_adjust_sensitivity(-1);if(right)settings_adjust_sensitivity(1);}else if(g_settings_selected==1&&(left||right)){g_camera_invert_y=!g_camera_invert_y;g_settings_dirty=1;settings_save();}
  if(key_tap(VK_RETURN)){
    if(g_settings_selected==0)settings_adjust_sensitivity(1);
    else if(g_settings_selected==1){g_camera_invert_y=!g_camera_invert_y;g_settings_dirty=1;settings_save();}
    else if(g_settings_selected==2)settings_restore_defaults();
    else if(g_settings_selected==3)settings_close_menu();
    else if(g_settings_selected==4){if(g_settings_dirty)settings_save();PostQuitMessage(0);}
  }
}
