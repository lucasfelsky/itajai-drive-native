// -----------------------------------------------------------------------------
// 1.8 procedural vehicle audio + road + rain + coastal ambience.
// 2.14 extends the same dependency-free WinMM backend with tyre/wind/surface and
// shift/impact transient layers while keeping graceful no-device fallback.
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
static float g_audio_phase18=0,g_audio_phase218=0,g_audio_phase318=0,g_audio_exhaust_lp180=0;
static float g_audio_engine_level180=0,g_audio_road_level180=0;
static const char*g_audio_voice180[4]={"UNO 4C SOFT","GOL 4C RASP","HB20 3C PULSE","RENEGADE 4C DEEP"};

static float audio18_noise(u32*state){*state=*state*1664525u+1013904223u;return (((*state>>8)&65535)/32767.5f)-1.0f;}
static float audio180_wrap(float p){while(p>6.2831853f)p-=6.2831853f;return p;}
#include "audio214_mix.inc"
static void audio18_fill(short*out,int count,u32*noiseState){
  int car=g_player.car&3;static const float firing[4]={2.0f,2.0f,1.5f,2.0f};static const float bass[4]={.66f,.56f,.72f,.78f};static const float rasp[4]={.16f,.29f,.22f,.12f};static const float intake[4]={.14f,.17f,.25f,.12f};static const float mech[4]={.06f,.09f,.07f,.05f};
  float rpm=clampf(g_engine_rpm15,750,6800),hz=rpm/60.0f*firing[car];float throttle=key_down('W')?1.0f:0.16f;float speed=absf(g_player.speed),rain=g_weather>=2?(g_weather==3?.15f:.085f):0.0f;float coast=(g_sector10>=3)?.040f:.010f;float gearLoad=clampf((float)g_gear15/5.0f,.18f,1.0f);float engineAmp=.058f+.105f*throttle+.020f*gearLoad;
  g_audio_engine_level180=engineAmp;g_audio_road_level180=clampf(speed/45.0f,0,1)*.052f;
  for(int i=0;i<count;i++){
    float s1=(float)sin((double)g_audio_phase18),s2=(float)sin((double)g_audio_phase218),s3=(float)sin((double)g_audio_phase318);float n=audio18_noise(noiseState);
    float pulse=car==2?(s1*.70f+s2*.19f+s3*.11f):(s1*bass[car]+s2*(.28f+rasp[car]*.18f)+s3*(.06f+rasp[car]*.22f));float rawExhaust=pulse*(.72f+.28f*throttle)+n*rasp[car]*.10f;float lpRate=car==3?.075f:car==0?.105f:.14f;g_audio_exhaust_lp180+=(rawExhaust-g_audio_exhaust_lp180)*lpRate;float exhaust=g_audio_exhaust_lp180*bass[car]+rawExhaust*(1.0f-bass[car])*.55f;
    float induction=(float)sin((double)(g_audio_phase218*.51f+g_audio_phase318*.13f))*intake[car]*throttle;float mechanical=n*mech[car]*(.25f+.75f*clampf(rpm/6200.0f,0,1));float engine=(exhaust+induction+mechanical)*engineAmp;
    float road=n*g_audio_road_level180;float wet=n*rain;float ambience=n*coast;float extra=audio214_extra(n,speed,throttle);float sample=clampf(engine+road+wet+ambience+extra,-.86f,.86f);out[i]=(short)(sample*32767.0f);
    g_audio_phase18=audio180_wrap(g_audio_phase18+6.2831853f*hz/AUDIO18_RATE);g_audio_phase218=audio180_wrap(g_audio_phase218+6.2831853f*(hz*(car==1?2.11f:2.02f))/AUDIO18_RATE);g_audio_phase318=audio180_wrap(g_audio_phase318+6.2831853f*(hz*(car==2?3.03f:3.12f))/AUDIO18_RATE);
  }
}
static DWORD WINAPI audio18_thread(LPVOID unused){(void)unused;WAVEFORMATEX18 fmt;memset(&fmt,0,sizeof(fmt));fmt.wFormatTag=WAVE_FORMAT_PCM18;fmt.nChannels=1;fmt.nSamplesPerSec=AUDIO18_RATE;fmt.wBitsPerSample=16;fmt.nBlockAlign=2;fmt.nAvgBytesPerSec=AUDIO18_RATE*2;if(waveOutOpen(&g_audio_out18,WAVE_MAPPER18,&fmt,0,0,0)!=0||!g_audio_out18){g_audio_available18=0;g_audio_running18=0;return 0;}g_audio_available18=1;u32 ns=0x84f23a11u;for(int b=0;b<AUDIO18_BUFFERS;b++){memset(&g_audio_hdr18[b],0,sizeof(WAVEHDR18));audio18_fill(g_audio_pcm18[b],AUDIO18_SAMPLES,&ns);g_audio_hdr18[b].lpData=(char*)g_audio_pcm18[b];g_audio_hdr18[b].dwBufferLength=AUDIO18_SAMPLES*2;if(waveOutPrepareHeader(g_audio_out18,&g_audio_hdr18[b],sizeof(WAVEHDR18))==0)waveOutWrite(g_audio_out18,&g_audio_hdr18[b],sizeof(WAVEHDR18));}
  while(g_audio_running18){for(int b=0;b<AUDIO18_BUFFERS;b++)if(g_audio_hdr18[b].dwFlags&WHDR_DONE18){audio18_fill(g_audio_pcm18[b],AUDIO18_SAMPLES,&ns);g_audio_hdr18[b].dwFlags&=~WHDR_DONE18;waveOutWrite(g_audio_out18,&g_audio_hdr18[b],sizeof(WAVEHDR18));}Sleep(8);}waveOutReset(g_audio_out18);for(int b=0;b<AUDIO18_BUFFERS;b++)waveOutUnprepareHeader(g_audio_out18,&g_audio_hdr18[b],sizeof(WAVEHDR18));waveOutClose(g_audio_out18);g_audio_out18=NULL;g_audio_available18=0;return 0;}
static void audio18_start(void){if(g_audio_started18)return;g_audio_started18=1;g_audio_running18=1;HANDLE th=CreateThread(NULL,0,audio18_thread,NULL,0,NULL);if(th)CloseHandle(th);else g_audio_running18=0;}
static void audio18_stop(void){g_audio_running18=0;Sleep(80);}
static void game_update(float dt){game_update_legacy17(dt);if(g_started&&!g_audio_started18)audio18_start();}
