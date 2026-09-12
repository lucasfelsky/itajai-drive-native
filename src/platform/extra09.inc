// 0.9 programmable-renderer platform surface.
__declspec(dllimport) void* WINAPI wglGetProcAddress(LPCSTR);
__declspec(dllimport) void APIENTRY glGenTextures(int,u32*);
__declspec(dllimport) void APIENTRY glBindTexture(u32,u32);
__declspec(dllimport) void APIENTRY glTexParameteri(u32,u32,int);
__declspec(dllimport) void APIENTRY glTexImage2D(u32,int,int,int,int,int,u32,u32,const void*);
__declspec(dllimport) void APIENTRY glDeleteTextures(int,const u32*);

#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_LINEAR 0x2601
#define GL_REPEAT 0x2901
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82

// OpenGL 2.x functions are resolved at runtime so machines that only expose
// the legacy path can still launch and use the fixed-function fallback.
typedef u32 (APIENTRY *PFN09CREATESHADER)(u32);
typedef void (APIENTRY *PFN09SHADERSOURCE)(u32,int,const char* const*,const int*);
typedef void (APIENTRY *PFN09COMPILESHADER)(u32);
typedef void (APIENTRY *PFN09GETSHADERIV)(u32,u32,int*);
typedef void (APIENTRY *PFN09DELETESHADER)(u32);
typedef u32 (APIENTRY *PFN09CREATEPROGRAM)(void);
typedef void (APIENTRY *PFN09ATTACHSHADER)(u32,u32);
typedef void (APIENTRY *PFN09LINKPROGRAM)(u32);
typedef void (APIENTRY *PFN09GETPROGRAMIV)(u32,u32,int*);
typedef void (APIENTRY *PFN09USEPROGRAM)(u32);
typedef int (APIENTRY *PFN09GETUNIFORMLOCATION)(u32,const char*);
typedef void (APIENTRY *PFN09UNIFORM1I)(int,int);
typedef void (APIENTRY *PFN09UNIFORM1F)(int,float);
typedef void (APIENTRY *PFN09UNIFORM3F)(int,float,float,float);
