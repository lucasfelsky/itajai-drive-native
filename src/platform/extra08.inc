// 0.8 platform/OpenGL extensions that are still part of Win32/OpenGL 1.1.
__declspec(dllimport) BOOL WINAPI SetFilePointerEx(HANDLE,LARGE_INTEGER,LARGE_INTEGER*,DWORD);
__declspec(dllimport) void APIENTRY glNormal3f(float,float,float);
__declspec(dllimport) void APIENTRY glTexCoord2f(float,float);
#define FILE_BEGIN 0
#define GL_TRIANGLES 0x0004
