// -----------------------------------------------------------------------------
// 0.18 minimal WinMM declarations for procedural native audio.
// -----------------------------------------------------------------------------
typedef void* HWAVEOUT18;
typedef UINT MMRESULT18;
#pragma pack(push,8)
typedef struct { WORD wFormatTag,nChannels; DWORD nSamplesPerSec,nAvgBytesPerSec; WORD nBlockAlign,wBitsPerSample,cbSize; } WAVEFORMATEX18;
typedef struct WAVEHDR18_TAG { char* lpData; DWORD dwBufferLength,dwBytesRecorded; u64 dwUser; DWORD dwFlags,dwLoops; struct WAVEHDR18_TAG* lpNext; u64 reserved; } WAVEHDR18;
#pragma pack(pop)
#define WAVE_FORMAT_PCM18 1
#define WAVE_MAPPER18 ((UINT)0xffffffffu)
#define WHDR_DONE18 0x00000001u
__declspec(dllimport) MMRESULT18 WINAPI waveOutOpen(HWAVEOUT18*,UINT,const WAVEFORMATEX18*,u64,u64,DWORD);
__declspec(dllimport) MMRESULT18 WINAPI waveOutPrepareHeader(HWAVEOUT18,WAVEHDR18*,UINT);
__declspec(dllimport) MMRESULT18 WINAPI waveOutUnprepareHeader(HWAVEOUT18,WAVEHDR18*,UINT);
__declspec(dllimport) MMRESULT18 WINAPI waveOutWrite(HWAVEOUT18,WAVEHDR18*,UINT);
__declspec(dllimport) MMRESULT18 WINAPI waveOutReset(HWAVEOUT18);
__declspec(dllimport) MMRESULT18 WINAPI waveOutClose(HWAVEOUT18);
__declspec(dllimport) void WINAPI Sleep(DWORD);
