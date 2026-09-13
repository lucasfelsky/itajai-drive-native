// Renderer 5.0 framebuffer state and entry points.
static PFN500GENFRAMEBUFFERS p500GenFramebuffers=NULL;
static PFN500BINDFRAMEBUFFER p500BindFramebuffer=NULL;
static PFN500DELETEFRAMEBUFFERS p500DeleteFramebuffers=NULL;
static PFN500CHECKFRAMEBUFFERSTATUS p500CheckFramebufferStatus=NULL;
static PFN500FRAMEBUFFERTEXTURE2D p500FramebufferTexture2D=NULL;
static PFN500ACTIVETEXTURE p500ActiveTexture=NULL;
static u32 g_fbo500=0,g_color500=0,g_depth500=0;
static int g_renderer500_ready=0,g_renderer500_w=0,g_renderer500_h=0,g_renderer500_hdr=0;
static float g_renderer500_scale=1.0f,g_renderer500_last_fps=60.0f;
static void* renderer500_proc2(const char*a,const char*b){void*p=wglGetProcAddress(a);if(!p&&b)p=wglGetProcAddress(b);return p;}
static int renderer500_load_api(void){
 p500GenFramebuffers=(PFN500GENFRAMEBUFFERS)renderer500_proc2("glGenFramebuffers","glGenFramebuffersEXT");
 p500BindFramebuffer=(PFN500BINDFRAMEBUFFER)renderer500_proc2("glBindFramebuffer","glBindFramebufferEXT");
 p500DeleteFramebuffers=(PFN500DELETEFRAMEBUFFERS)renderer500_proc2("glDeleteFramebuffers","glDeleteFramebuffersEXT");
 p500CheckFramebufferStatus=(PFN500CHECKFRAMEBUFFERSTATUS)renderer500_proc2("glCheckFramebufferStatus","glCheckFramebufferStatusEXT");
 p500FramebufferTexture2D=(PFN500FRAMEBUFFERTEXTURE2D)renderer500_proc2("glFramebufferTexture2D","glFramebufferTexture2DEXT");
 p500ActiveTexture=(PFN500ACTIVETEXTURE)wglGetProcAddress("glActiveTexture");
 return p500GenFramebuffers&&p500BindFramebuffer&&p500DeleteFramebuffers&&p500CheckFramebufferStatus&&p500FramebufferTexture2D&&p500ActiveTexture;
}
