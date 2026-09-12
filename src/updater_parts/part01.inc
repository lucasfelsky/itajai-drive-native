// Itajai Drive Updater 1.1.0
// Native Win32 + WinHTTP updater. No browser, no .NET, no external runtime.
#define WIN32_LEAN_AND_MEAN 1

typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed int i32;
typedef long LONG;
typedef unsigned long DWORD;
typedef int BOOL;
typedef unsigned short WORD;
typedef unsigned int UINT;
typedef long long LPARAM;
typedef unsigned long long WPARAM;
typedef long long LRESULT;
typedef void* HANDLE;
typedef void* HWND;
typedef void* HDC;
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
#define NULL ((void*)0)
#define TRUE 1
#define FALSE 0

#pragma pack(push,8)
typedef struct { LONG left,top,right,bottom; } RECT;
typedef struct { LONG x,y; } POINT;
typedef struct { HWND hwnd; UINT message; WPARAM wParam; LPARAM lParam; DWORD time; POINT pt; DWORD lPrivate; } MSG;
typedef LRESULT (CALLBACK *WNDPROC)(HWND,UINT,WPARAM,LPARAM);
typedef struct { UINT style; WNDPROC lpfnWndProc; int cbClsExtra; int cbWndExtra; HINSTANCE hInstance; HICON hIcon; HCURSOR hCursor; HBRUSH hbrBackground; LPCSTR lpszMenuName; LPCSTR lpszClassName; } WNDCLASSA;
typedef struct { DWORD LowPart; LONG HighPart; } LARGE_INTEGER_PARTS;
typedef union { LARGE_INTEGER_PARTS p; long long QuadPart; } LARGE_INTEGER;
#pragma pack(pop)

// KERNEL32
__declspec(dllimport) HINSTANCE WINAPI GetModuleHandleA(LPCSTR);
__declspec(dllimport) DWORD WINAPI GetModuleFileNameA(HINSTANCE,LPSTR,DWORD);
__declspec(dllimport) BOOL WINAPI SetCurrentDirectoryA(LPCSTR);
__declspec(dllimport) void WINAPI ExitProcess(UINT);
__declspec(dllimport) HANDLE WINAPI CreateThread(LPVOID,SIZE_T,LPTHREAD_START_ROUTINE,LPVOID,DWORD,DWORD*);
__declspec(dllimport) BOOL WINAPI CloseHandle(HANDLE);
__declspec(dllimport) void WINAPI Sleep(DWORD);
__declspec(dllimport) void* WINAPI VirtualAlloc(void*,SIZE_T,DWORD,DWORD);
__declspec(dllimport) BOOL WINAPI VirtualFree(void*,SIZE_T,DWORD);
__declspec(dllimport) HANDLE WINAPI CreateFileA(LPCSTR,DWORD,DWORD,LPVOID,DWORD,DWORD,HANDLE);
__declspec(dllimport) BOOL WINAPI ReadFile(HANDLE,LPVOID,DWORD,DWORD*,LPVOID);
__declspec(dllimport) BOOL WINAPI WriteFile(HANDLE,LPCVOID,DWORD,DWORD*,LPVOID);
__declspec(dllimport) BOOL WINAPI GetFileSizeEx(HANDLE,LARGE_INTEGER*);
__declspec(dllimport) BOOL WINAPI DeleteFileA(LPCSTR);
__declspec(dllimport) BOOL WINAPI CreateDirectoryA(LPCSTR,LPVOID);
__declspec(dllimport) BOOL WINAPI MoveFileExA(LPCSTR,LPCSTR,DWORD);
__declspec(dllimport) DWORD WINAPI GetFileAttributesA(LPCSTR);
__declspec(dllimport) UINT WINAPI WinExec(LPCSTR,UINT);

// USER32
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
__declspec(dllimport) int WINAPI FillRect(HDC,const RECT*,HBRUSH);

// GDI32
__declspec(dllimport) HBRUSH WINAPI CreateSolidBrush(DWORD);
__declspec(dllimport) BOOL WINAPI DeleteObject(HGDIOBJ);
__declspec(dllimport) DWORD WINAPI SetTextColor(HDC,DWORD);
__declspec(dllimport) int WINAPI SetBkMode(HDC,int);
__declspec(dllimport) BOOL WINAPI TextOutA(HDC,int,int,LPCSTR,int);
__declspec(dllimport) HGDIOBJ WINAPI GetStockObject(int);
__declspec(dllimport) HGDIOBJ WINAPI SelectObject(HDC,HGDIOBJ);

// WINHTTP. Redirects remain enabled by WinHTTP's default request policy; GitHub's
// /releases/latest/download URL therefore resolves without a browser dependency.
__declspec(dllimport) HINTERNET WINAPI WinHttpOpen(LPCWSTR,DWORD,LPCWSTR,LPCWSTR,DWORD);
__declspec(dllimport) HINTERNET WINAPI WinHttpConnect(HINTERNET,LPCWSTR,WORD,DWORD);
__declspec(dllimport) HINTERNET WINAPI WinHttpOpenRequest(HINTERNET,LPCWSTR,LPCWSTR,LPCWSTR,LPCWSTR,const LPCWSTR*,DWORD);
__declspec(dllimport) BOOL WINAPI WinHttpSendRequest(HINTERNET,LPCWSTR,DWORD,LPVOID,DWORD,DWORD,u64);
__declspec(dllimport) BOOL WINAPI WinHttpReceiveResponse(HINTERNET,LPVOID);
__declspec(dllimport) BOOL WINAPI WinHttpQueryDataAvailable(HINTERNET,DWORD*);
__declspec(dllimport) BOOL WINAPI WinHttpReadData(HINTERNET,LPVOID,DWORD,DWORD*);
__declspec(dllimport) BOOL WINAPI WinHttpSetTimeouts(HINTERNET,int,int,int,int);
__declspec(dllimport) BOOL WINAPI WinHttpCloseHandle(HINTERNET);

// UCRT imports
__declspec(dllimport) int __cdecl sprintf(char*,const char*,...);
__declspec(dllimport) unsigned long long __cdecl strlen(const char*);
__declspec(dllimport) void* __cdecl memcpy(void*,const void*,unsigned long long);
__declspec(dllimport) void* __cdecl memset(void*,int,unsigned long long);
__declspec(dllimport) int __cdecl atoi(const char*);

#define CS_OWNDC 0x0020
#define WS_OVERLAPPED 0x00000000L
#define WS_CAPTION 0x00C00000L
#define WS_SYSMENU 0x00080000L
#define WS_MINIMIZEBOX 0x00020000L
#define WS_VISIBLE 0x10000000L
#define WM_DESTROY 0x0002
#define WM_CLOSE 0x0010
#define WM_QUIT 0x0012
#define PM_REMOVE 0x0001
#define SW_SHOW 5
#define MEM_COMMIT 0x1000
#define MEM_RESERVE 0x2000
#define MEM_RELEASE 0x8000
#define GENERIC_READ 0x80000000u
#define GENERIC_WRITE 0x40000000u
#define FILE_SHARE_READ 0x00000001
#define OPEN_EXISTING 3
#define CREATE_ALWAYS 2
#define FILE_ATTRIBUTE_NORMAL 0x80
#define FILE_ATTRIBUTE_DIRECTORY 0x10
#define INVALID_FILE_ATTRIBUTES 0xffffffffu
#define INVALID_HANDLE_VALUE ((HANDLE)(long long)-1)
#define INTERNET_DEFAULT_HTTP_PORT 80
#define INTERNET_DEFAULT_HTTPS_PORT 443
#define WINHTTP_FLAG_SECURE 0x00800000
#define WINHTTP_ACCESS_TYPE_DEFAULT_PROXY 0
#define MOVEFILE_REPLACE_EXISTING 0x1
#define MOVEFILE_WRITE_THROUGH 0x8
#define TRANSPARENT 1
#define SYSTEM_FONT 13

int _fltused=0;

static HWND g_hwnd=0;