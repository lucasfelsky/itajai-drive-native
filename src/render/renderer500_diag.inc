// -----------------------------------------------------------------------------
// Renderer 5.0 startup diagnostics. Keeps fallback behavior intact while leaving
// a tiny text breadcrumb beside the executable when HDR/context/post setup fails
// on a real driver. Useful because CI can compile GLSL strings but cannot execute
// them on the player's RTX driver.
// -----------------------------------------------------------------------------
static char g_renderer500_diag_path[640];
static void renderer500_diag_path(void){sprintf(g_renderer500_diag_path,"%s\\itajai_renderer500.log",g_exe_dir);}
static void renderer500_diag_write(const char*line,int reset){renderer500_diag_path();HANDLE f=CreateFileA(g_renderer500_diag_path,GENERIC_WRITE,FILE_SHARE_READ,NULL,reset?CREATE_ALWAYS:OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);if(f==INVALID_HANDLE_VALUE)return;if(!reset)SetFilePointer(f,0,NULL,FILE_END);DWORD n=0;WriteFile(f,line,(DWORD)strlen(line),&n,NULL);WriteFile(f,"\r\n",2,&n,NULL);CloseHandle(f);}
static void renderer500_diag_state(const char*stage){char b[360];sprintf(b,"%s | context33=%d shader=%d api=%d post=%u fbo=%u hdr=%d target=%dx%d scale=%.2f",stage,g_renderer500_context33,g_renderer09_shader,(p500BindFramebuffer&&p500CheckFramebufferStatus)?1:0,g_post500,g_fbo500,g_renderer500_hdr,g_renderer500_w,g_renderer500_h,g_renderer500_scale);renderer500_diag_write(b,0);}
