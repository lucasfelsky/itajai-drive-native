// 0.9 programmable-renderer platform surface; expanded in 2.6 with VBO support.
// 5.0 extends the runtime-resolved surface with framebuffer, MRT-era texture,
// context-upgrade and post-processing entry points while keeping the legacy
// compatibility renderer available as a safe fallback.
__declspec(dllimport) void* WINAPI wglGetProcAddress(LPCSTR);
__declspec(dllimport) void APIENTRY glGenTextures(int,u32*);
__declspec(dllimport) void APIENTRY glBindTexture(u32,u32);
__declspec(dllimport) void APIENTRY glTexParameteri(u32,u32,int);
__declspec(dllimport) void APIENTRY glTexImage2D(u32,int,int,int,int,int,u32,u32,const void*);
__declspec(dllimport) void APIENTRY glDeleteTextures(int,const u32*);
__declspec(dllimport) void APIENTRY glEnableClientState(u32);
__declspec(dllimport) void APIENTRY glDisableClientState(u32);
__declspec(dllimport) void APIENTRY glVertexPointer(int,u32,int,const void*);
__declspec(dllimport) void APIENTRY glNormalPointer(u32,int,const void*);
__declspec(dllimport) void APIENTRY glTexCoordPointer(int,u32,int,const void*);
__declspec(dllimport) void APIENTRY glDrawElements(u32,int,u32,const void*);
__declspec(dllimport) void APIENTRY glDrawArrays(u32,int,int);
__declspec(dllimport) void APIENTRY glDepthMask(u8);
__declspec(dllimport) void APIENTRY glColorMask(u8,u8,u8,u8);
__declspec(dllimport) void APIENTRY glDrawBuffer(u32);
__declspec(dllimport) void APIENTRY glReadBuffer(u32);

#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_LINEAR 0x2601
#define GL_NEAREST 0x2600
#define GL_REPEAT 0x2901
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_RED 0x1903
#define GL_DEPTH_COMPONENT 0x1902
#define GL_DEPTH_COMPONENT24 0x81A6
#define GL_RGBA16F 0x881A
#define GL_RGB16F 0x881B
#define GL_HALF_FLOAT 0x140B
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_VERTEX_ARRAY 0x8074
#define GL_NORMAL_ARRAY 0x8075
#define GL_TEXTURE_COORD_ARRAY 0x8078
#define GL_FLOAT 0x1406
#define GL_UNSIGNED_INT 0x1405
#define GL_FRAMEBUFFER 0x8D40
#define GL_RENDERBUFFER 0x8D41
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_NONE 0
#define GL_DEPTH_TEXTURE_MODE 0x884B
#define GL_TEXTURE_COMPARE_MODE 0x884C
#define GL_COMPARE_R_TO_TEXTURE 0x884E
#define GL_LEQUAL_COMPARE 0x0203

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
typedef void (APIENTRY *PFN09UNIFORM2F)(int,float,float);
typedef void (APIENTRY *PFN09UNIFORM3F)(int,float,float,float);
typedef void (APIENTRY *PFN09UNIFORM4F)(int,float,float,float,float);
typedef void (APIENTRY *PFN09UNIFORMMATRIX4FV)(int,int,u8,const float*);

// VBO API used by the 2.6 modernization path. Runtime resolution preserves the
// old display-list/immediate fallback on legacy drivers.
typedef void (APIENTRY *PFN26GENBUFFERS)(int,u32*);
typedef void (APIENTRY *PFN26BINDBUFFER)(u32,u32);
typedef void (APIENTRY *PFN26BUFFERDATA)(u32,SIZE_T,const void*,u32);
typedef void (APIENTRY *PFN26DELETEBUFFERS)(int,const u32*);

// 5.0 modern presentation surface. The game still uses a compatibility profile
// for historical immediate-mode city content, but all post-processing and HDR
// targets are backed by modern framebuffer objects.
typedef HGLRC (WINAPI *PFN500WGLCREATECONTEXTATTRIBS)(HDC,HGLRC,const int*);
typedef void (APIENTRY *PFN500GENFRAMEBUFFERS)(int,u32*);
typedef void (APIENTRY *PFN500BINDFRAMEBUFFER)(u32,u32);
typedef void (APIENTRY *PFN500DELETEFRAMEBUFFERS)(int,const u32*);
typedef u32 (APIENTRY *PFN500CHECKFRAMEBUFFERSTATUS)(u32);
typedef void (APIENTRY *PFN500FRAMEBUFFERTEXTURE2D)(u32,u32,u32,u32,int);
typedef void (APIENTRY *PFN500GENRENDERBUFFERS)(int,u32*);
typedef void (APIENTRY *PFN500BINDRENDERBUFFER)(u32,u32);
typedef void (APIENTRY *PFN500RENDERBUFFERSTORAGE)(u32,u32,int,int);
typedef void (APIENTRY *PFN500FRAMEBUFFERRENDERBUFFER)(u32,u32,u32,u32);
typedef void (APIENTRY *PFN500DELETERENDERBUFFERS)(int,const u32*);
typedef void (APIENTRY *PFN500ACTIVETEXTURE)(u32);
typedef void (APIENTRY *PFN500GENERATEMIPMAP)(u32);
typedef void (APIENTRY *PFN500BLENDFUNCSEPARATE)(u32,u32,u32,u32);
typedef void (APIENTRY *PFN500GENVERTEXARRAYS)(int,u32*);
typedef void (APIENTRY *PFN500BINDVERTEXARRAY)(u32);
typedef void (APIENTRY *PFN500DELETEVERTEXARRAYS)(int,const u32*);
