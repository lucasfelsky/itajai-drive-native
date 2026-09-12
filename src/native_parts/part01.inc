// Itajai Drive Native 0.7.0
// Win32 + OpenGL 1.1, no browser / no external engine.
#define WIN32_LEAN_AND_MEAN 1

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long long u64;
typedef signed int     i32;
typedef long long      i64;
typedef long           LONG;
typedef unsigned long  DWORD;
typedef int            BOOL;
typedef unsigned short WORD;
typedef unsigned char  BYTE;
typedef unsigned int   UINT;
typedef long long      LPARAM;
typedef unsigned long long WPARAM;
typedef long long      LRESULT;
typedef void* HANDLE;
typedef void* HWND;
typedef void* HDC;
typedef void* HGLRC;
typedef void* HINSTANCE;
typedef void* HICON;
typedef void* HCURSOR;
typedef void* HBRUSH;
typedef void* HMENU;
typedef void* HGDIOBJ;
typedef void* HINTERNET;
typedef const char* LPCSTR;
typedef char* LPSTR;
typedef const unsigned short* LPCWSTR;
typedef void* LPVOID;
typedef const void* LPCVOID;
typedef unsigned long long SIZE_T;
typedef DWORD (*LPTHREAD_START_ROUTINE)(LPVOID);

#define WINAPI __stdcall
#define CALLBACK __stdcall
#define APIENTRY __stdcall
#define NULL ((void*)0)
#define TRUE 1
#define FALSE 0

// Win32 structs
#pragma pack(push,8)
typedef struct { LONG left,top,right,bottom; } RECT;
typedef struct { LONG x,y; } POINT;
typedef struct { HWND hwnd; UINT message; WPARAM wParam; LPARAM lParam; DWORD time; POINT pt; DWORD lPrivate; } MSG;
typedef LRESULT (CALLBACK *WNDPROC)(HWND,UINT,WPARAM,LPARAM);
typedef struct {
  UINT style; WNDPROC lpfnWndProc; int cbClsExtra; int cbWndExtra; HINSTANCE hInstance;
  HICON hIcon; HCURSOR hCursor; HBRUSH hbrBackground; LPCSTR lpszMenuName; LPCSTR lpszClassName;
} WNDCLASSA;
typedef struct {
  WORD nSize,nVersion; DWORD dwFlags; BYTE iPixelType,cColorBits,cRedBits,cRedShift,cGreenBits,cGreenShift,cBlueBits,cBlueShift,cAlphaBits,cAlphaShift,cAccumBits,cAccumRedBits,cAccumGreenBits,cAccumBlueBits,cAccumAlphaBits,cDepthBits,cStencilBits,cAuxBuffers,iLayerType,bReserved; DWORD dwLayerMask,dwVisibleMask,dwDamageMask;
} PIXELFORMATDESCRIPTOR;
typedef union { struct { DWORD LowPart; LONG HighPart; }; i64 QuadPart; } LARGE_INTEGER;
#pragma pack(pop)

// imported Win32 API
__declspec(dllimport) HINSTANCE WINAPI GetModuleHandleA(LPCSTR);
__declspec(dllimport) DWORD WINAPI GetModuleFileNameA(HINSTANCE,LPSTR,DWORD);
__declspec(dllimport) void WINAPI ExitProcess(UINT);
__declspec(dllimport) BOOL WINAPI QueryPerformanceCounter(LARGE_INTEGER*);
__declspec(dllimport) BOOL WINAPI QueryPerformanceFrequency(LARGE_INTEGER*);
__declspec(dllimport) HANDLE WINAPI CreateThread(LPVOID,SIZE_T,LPTHREAD_START_ROUTINE,LPVOID,DWORD,DWORD*);
__declspec(dllimport) BOOL WINAPI CloseHandle(HANDLE);
__declspec(dllimport) void* WINAPI VirtualAlloc(void*,SIZE_T,DWORD,DWORD);
__declspec(dllimport) BOOL WINAPI VirtualFree(void*,SIZE_T,DWORD);
__declspec(dllimport) HANDLE WINAPI CreateFileA(LPCSTR,DWORD,DWORD,LPVOID,DWORD,DWORD,HANDLE);
__declspec(dllimport) BOOL WINAPI ReadFile(HANDLE,LPVOID,DWORD,DWORD*,LPVOID);
__declspec(dllimport) BOOL WINAPI WriteFile(HANDLE,LPCVOID,DWORD,DWORD*,LPVOID);
__declspec(dllimport) BOOL WINAPI GetFileSizeEx(HANDLE,LARGE_INTEGER*);
__declspec(dllimport) BOOL WINAPI DeleteFileA(LPCSTR);

__declspec(dllimport) unsigned short WINAPI RegisterClassA(const WNDCLASSA*);
__declspec(dllimport) HWND WINAPI CreateWindowExA(DWORD,LPCSTR,LPCSTR,DWORD,int,int,int,int,HWND,HMENU,HINSTANCE,LPVOID);
__declspec(dllimport) BOOL WINAPI ShowWindow(HWND,int);
__declspec(dllimport) BOOL WINAPI UpdateWindow(HWND);
__declspec(dllimport) BOOL WINAPI PeekMessageA(MSG*,HWND,UINT,UINT,UINT);
__declspec(dllimport) BOOL WINAPI TranslateMessage(const MSG*);
__declspec(dllimport) LRESULT WINAPI DispatchMessageA(const MSG*);
__declspec(dllimport) LRESULT WINAPI DefWindowProcA(HWND,UINT,WPARAM,LPARAM);
__declspec(dllimport) void WINAPI PostQuitMessage(int);
__declspec(dllimport) BOOL WINAPI GetClientRect(HWND,RECT*);
__declspec(dllimport) HDC WINAPI GetDC(HWND);
__declspec(dllimport) int WINAPI ReleaseDC(HWND,HDC);
__declspec(dllimport) short WINAPI GetAsyncKeyState(int);
__declspec(dllimport) BOOL WINAPI SetWindowTextA(HWND,LPCSTR);
__declspec(dllimport) BOOL WINAPI ClientToScreen(HWND,POINT*);
__declspec(dllimport) BOOL WINAPI SetCursorPos(int,int);
__declspec(dllimport) int WINAPI ShowCursor(BOOL);
__declspec(dllimport) HWND WINAPI SetCapture(HWND);
__declspec(dllimport) BOOL WINAPI ReleaseCapture(void);

__declspec(dllimport) int WINAPI ChoosePixelFormat(HDC,const PIXELFORMATDESCRIPTOR*);
__declspec(dllimport) BOOL WINAPI SetPixelFormat(HDC,int,const PIXELFORMATDESCRIPTOR*);
__declspec(dllimport) BOOL WINAPI SwapBuffers(HDC);
__declspec(dllimport) HGDIOBJ WINAPI GetStockObject(int);
__declspec(dllimport) HGDIOBJ WINAPI SelectObject(HDC,HGDIOBJ);

// WinHTTP
__declspec(dllimport) HINTERNET WINAPI WinHttpOpen(LPCWSTR,DWORD,LPCWSTR,LPCWSTR,DWORD);
__declspec(dllimport) HINTERNET WINAPI WinHttpConnect(HINTERNET,LPCWSTR,WORD,DWORD);
__declspec(dllimport) HINTERNET WINAPI WinHttpOpenRequest(HINTERNET,LPCWSTR,LPCWSTR,LPCWSTR,LPCWSTR,const LPCWSTR*,DWORD);
__declspec(dllimport) BOOL WINAPI WinHttpSendRequest(HINTERNET,LPCWSTR,DWORD,LPVOID,DWORD,DWORD,u64);
__declspec(dllimport) BOOL WINAPI WinHttpReceiveResponse(HINTERNET,LPVOID);
__declspec(dllimport) BOOL WINAPI WinHttpQueryDataAvailable(HINTERNET,DWORD*);
__declspec(dllimport) BOOL WINAPI WinHttpReadData(HINTERNET,LPVOID,DWORD,DWORD*);
__declspec(dllimport) BOOL WINAPI WinHttpSetTimeouts(HINTERNET,int,int,int,int);
__declspec(dllimport) BOOL WINAPI WinHttpCloseHandle(HINTERNET);

// msvcrt - only basic math/string formatting. The game itself uses Win32 allocation/file IO.
__declspec(dllimport) double __cdecl sin(double);
__declspec(dllimport) double __cdecl cos(double);
__declspec(dllimport) double __cdecl sqrt(double);
__declspec(dllimport) double __cdecl atan2(double,double);
__declspec(dllimport) double __cdecl atof(const char*);
__declspec(dllimport) int __cdecl atoi(const char*);
__declspec(dllimport) int __cdecl sprintf(char*,const char*,...);
__declspec(dllimport) unsigned long long __cdecl strlen(const char*);
__declspec(dllimport) char* __cdecl strstr(const char*,const char*);
__declspec(dllimport) void* __cdecl memcpy(void*,const void*,unsigned long long);
__declspec(dllimport) void* __cdecl memset(void*,int,unsigned long long);

// OpenGL 1.1
__declspec(dllimport) HGLRC WINAPI wglCreateContext(HDC);
__declspec(dllimport) BOOL WINAPI wglMakeCurrent(HDC,HGLRC);
__declspec(dllimport) BOOL WINAPI wglDeleteContext(HGLRC);
__declspec(dllimport) BOOL WINAPI wglUseFontBitmapsA(HDC,DWORD,DWORD,DWORD);
__declspec(dllimport) void APIENTRY glClearColor(float,float,float,float);
__declspec(dllimport) void APIENTRY glClear(u32);
