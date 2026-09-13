// Renderer 5.0 optional OpenGL 3.3 compatibility context.
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
static int g_renderer500_context33=0;
static int renderer500_upgrade_context(void){
 PFN500WGLCREATECONTEXTATTRIBS create=(PFN500WGLCREATECONTEXTATTRIBS)wglGetProcAddress("wglCreateContextAttribsARB");if(!create||!g_hdc||!g_glrc)return 0;
 int a[]={WGL_CONTEXT_MAJOR_VERSION_ARB,3,WGL_CONTEXT_MINOR_VERSION_ARB,3,WGL_CONTEXT_PROFILE_MASK_ARB,WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,0};HGLRC old=g_glrc,modern=create(g_hdc,NULL,a);if(!modern)return 0;
 if(!wglMakeCurrent(g_hdc,modern)){wglDeleteContext(modern);return 0;}g_glrc=modern;wglDeleteContext(old);g_renderer500_context33=1;return 1;
}
