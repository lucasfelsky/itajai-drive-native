// -----------------------------------------------------------------------------
// 0.18 procedural engine + road + rain + coastal ambience.
// Gracefully disables itself when no Windows audio device is available.
// -----------------------------------------------------------------------------
#define AUDIO18_RATE 22050
#define AUDIO18_SAMPLES 2048
#define AUDIO18_BUFFERS 4
static short g_audio_pcm18[AUDIO18_BUFFERS][AUDIO18_SAMPLES];
static WAVEHDR18 g_audio_hdr18[AUDIO18_BUFFERS];
static HWAVEOUT18 g_audio_out18=NULL;
static volatile LONG g_audio_running18=0;
static volatile LONG g_audio_started18=0;
static volatile LONG g_audio_available18=0;
static float g_audio_phase18=0,g_audio_phase218=0;

static float audio18_noise(u32*state){*state=*state*1664525u+1013904223u;return (((*state>>8)&65535)/32767.5f)-1.0f;}
static void audio18_fill(short*out,int count,u32*noiseState){
  float rpm=clampf(g_engine_rpm15,750,6800),hz=rpm/60.0f*2.0f;float throttle=key_down('W')?1.0f:0.20f;float speed=absf(g_player.speed),rain=g_weather>=2?(g_weather==3?.16f:.09f):0.0f;float coast=(g_sector10>=3)?.045f:.012f;
  for(int i=0;i<count;i++){float s1=(float)sin((double)g_audio_phase18),s2=(float)sin((double)g_audio_phase218);g_audio_phase18+=6.2831853f*hz/AUDIO18_RATE;g_audio_phase218+=6.2831853f*(hz*2.02f)/AUDIO18_RATE;if(g_audio_phase18>6.2831853f)g_audio_phase18-=6.2831853f;if(g_audio_phase218>6.2831853f)g_audio_phase218-=6.2831853f;float n=audio18_noise(noiseState);float engine=(s1*.68f+s2*.32f)*(.07f+.10f*throttle);float road=n*clampf(speed/45.0f,0,1)*.055f;float wet=n*rain;float ambience=n*coast;float sample=clampf(engine+road+wet+ambience,-.82f,.82f);out[i]=(short)(sample*32767.0f);}
}
static DWORD WINAPI audio18_thread(LPVOID unused){(void)unused;WAVEFORMATEX18 fmt;memset(&fmt,0,sizeof(fmt));fmt.wFormatTag=WAVE_FORMAT_PCM18;fmt.nChannels=1;fmt.nSamplesPerSec=AUDIO18_RATE;fmt.wBitsPerSample=16;fmt.nBlockAlign=2;fmt.nAvgBytesPerSec=AUDIO18_RATE*2;if(waveOutOpen(&g_audio_out18,WAVE_MAPPER18,&fmt,0,0,0)!=0||!g_audio_out18){g_audio_available18=0;g_audio_running18=0;return 0;}g_audio_available18=1;u32 ns=0x84f23a11u;for(int b=0;b<AUDIO18_BUFFERS;b++){memset(&g_audio_hdr18[b],0,sizeof(WAVEHDR18));audio18_fill(g_audio_pcm18[b],AUDIO18_SAMPLES,&ns);g_audio_hdr18[b].lpData=(char*)g_audio_pcm18[b];g_audio_hdr18[b].dwBufferLength=AUDIO18_SAMPLES*2;if(waveOutPrepareHeader(g_audio_out18,&g_audio_hdr18[b],sizeof(WAVEHDR18))==0)waveOutWrite(g_audio_out18,&g_audio_hdr18[b],sizeof(WAVEHDR18));}
  while(g_audio_running18){for(int b=0;b<AUDIO18_BUFFERS;b++)if(g_audio_hdr18[b].dwFlags&WHDR_DONE18){audio18_fill(g_audio_pcm18[b],AUDIO18_SAMPLES,&ns);g_audio_hdr18[b].dwFlags&=~WHDR_DONE18;waveOutWrite(g_audio_out18,&g_audio_hdr18[b],sizeof(WAVEHDR18));}Sleep(8);}waveOutReset(g_audio_out18);for(int b=0;b<AUDIO18_BUFFERS;b++)waveOutUnprepareHeader(g_audio_out18,&g_audio_hdr18[b],sizeof(WAVEHDR18));waveOutClose(g_audio_out18);g_audio_out18=NULL;g_audio_available18=0;return 0;}
static void audio18_start(void){if(g_audio_started18)return;g_audio_started18=1;g_audio_running18=1;HANDLE th=CreateThread(NULL,0,audio18_thread,NULL,0,NULL);if(th)CloseHandle(th);else g_audio_running18=0;}
static void audio18_stop(void){g_audio_running18=0;Sleep(80);}
static void game_update(float dt){game_update_legacy17(dt);if(g_started&&!g_audio_started18)audio18_start();}
