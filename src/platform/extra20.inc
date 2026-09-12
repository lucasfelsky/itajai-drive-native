// -----------------------------------------------------------------------------
// 0.20 OpenGL 1.1 static mesh display-list support.
// -----------------------------------------------------------------------------
#ifndef GL_COMPILE
#define GL_COMPILE 0x1300
#endif
__declspec(dllimport) u32 APIENTRY glGenLists(int);
__declspec(dllimport) void APIENTRY glNewList(u32,u32);
__declspec(dllimport) void APIENTRY glEndList(void);
__declspec(dllimport) void APIENTRY glCallList(u32);
__declspec(dllimport) void APIENTRY glDeleteLists(u32,int);
