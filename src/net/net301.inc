// -----------------------------------------------------------------------------
// 3.1 Multiplayer Foundation - native Winsock/UDP transport.
// First milestone intentionally supports one remote peer (2 players total),
// direct IPv4/LAN sessions, 20 Hz vehicle snapshots and visual interpolation.
// Remote vehicles are presentation-only here: no shared collision/traffic/world
// authority yet, so the validated 3.0.1 local vehicle simulation stays untouched.
//
// Launch helpers:
//   ItajaiDriveNative.exe --host
//   ItajaiDriveNative.exe --join=192.168.0.42
//   optional: --port=27015
// Runtime test keys: F6 host, F7 join localhost, F8 disconnect.
// -----------------------------------------------------------------------------

typedef unsigned long long NET301_SOCKET;
typedef struct { DWORD s_addr; } Net301InAddr;
typedef struct {
  signed short sin_family;
  WORD sin_port;
  Net301InAddr sin_addr;
  char sin_zero[8];
} Net301SockAddrIn;

__declspec(dllimport) int WINAPI WSAStartup(WORD,void*);
__declspec(dllimport) int WINAPI WSACleanup(void);
__declspec(dllimport) int WINAPI WSAGetLastError(void);
__declspec(dllimport) NET301_SOCKET WINAPI socket(int,int,int);
__declspec(dllimport) int WINAPI closesocket(NET301_SOCKET);
__declspec(dllimport) int WINAPI bind(NET301_SOCKET,const void*,int);
__declspec(dllimport) int WINAPI sendto(NET301_SOCKET,const char*,int,int,const void*,int);
__declspec(dllimport) int WINAPI recvfrom(NET301_SOCKET,char*,int,int,void*,int*);
__declspec(dllimport) int WINAPI ioctlsocket(NET301_SOCKET,LONG,DWORD*);
__declspec(dllimport) DWORD WINAPI inet_addr(const char*);
__declspec(dllimport) WORD WINAPI htons(WORD);
__declspec(dllimport) LPSTR WINAPI GetCommandLineA(void);

#define NET301_AF_INET 2
#define NET301_SOCK_DGRAM 2
#define NET301_IPPROTO_UDP 17
#define NET301_INVALID_SOCKET ((NET301_SOCKET)(~0ull))
#define NET301_SOCKET_ERROR (-1)
#define NET301_FIONBIO ((LONG)0x8004667eL)
#define NET301_DEFAULT_PORT 27015
#define NET301_MAGIC 0x31444A49u /* IJD1 */
#define NET301_PROTOCOL 1
#define NET301_PACKET_HELLO 1
#define NET301_PACKET_WELCOME 2
#define NET301_PACKET_SNAPSHOT 3
#define NET301_PACKET_BYE 4
#define NET301_MODE_OFF 0
#define NET301_MODE_HOST 1
#define NET301_MODE_CLIENT 2
#define NET301_FLAG_BRAKE 1u
#define NET301_FLAG_HANDBRAKE 2u
#define NET301_FLAG_HEADLIGHTS 4u
#define NET301_VK_F6 0x75
#define NET301_VK_F7 0x76
#define NET301_VK_F8 0x77

#pragma pack(push,1)
typedef struct {
  u32 magic;
  u16 protocol;
  u16 type;
  u32 sequence;
  u32 senderId;
} Net301Header;

typedef struct {
  Net301Header h;
  u32 nonce;
  int car;
} Net301Hello;

typedef struct {
  Net301Header h;
  u32 assignedId;
} Net301Welcome;

typedef struct {
  Net301Header h;
  float x,y,z;
  float yaw,pitch,roll;
  float speed,steer;
  int car;
  u32 flags;
} Net301Snapshot;

typedef struct { Net301Header h; } Net301Bye;
#pragma pack(pop)

typedef struct {
  int active;
  u32 playerId;
  int car;
  V3 targetP;
  V3 renderP;
  float targetYaw,renderYaw;
  float targetPitch,renderPitch;
  float targetRoll,renderRoll;
  float speed,steer;
  u32 flags;
  float age;
  u32 lastSequence;
} Net301RemotePlayer;

static NET301_SOCKET g_net301_socket=NET301_INVALID_SOCKET;
static Net301SockAddrIn g_net301_peer_addr;
static int g_net301_peer_valid=0;
static int g_net301_mode=NET301_MODE_OFF;
static int g_net301_connected=0;
static int g_net301_wsa=0;
static int g_net301_port=NET301_DEFAULT_PORT;
static char g_net301_join_ip[64]="127.0.0.1";
static u32 g_net301_local_id=0;
static u32 g_net301_sequence=0;
static u32 g_net301_nonce=0x4a170301u;
static float g_net301_send_timer=0;
static float g_net301_hello_timer=0;
static int g_net301_last_error=0;
static int g_net301_rx_packets=0,g_net301_tx_packets=0,g_net301_dropped_packets=0;
static int g_net301_cmdline_checked=0;
static Net301RemotePlayer g_net301_remote;

static const char*net301_mode_name(void){return g_net301_mode==NET301_MODE_HOST?"HOST":g_net301_mode==NET301_MODE_CLIENT?"CLIENT":"OFF";}

static void net301_addr_clear(Net301SockAddrIn*a){memset(a,0,sizeof(*a));a->sin_family=NET301_AF_INET;}
static int net301_addr_equal(const Net301SockAddrIn*a,const Net301SockAddrIn*b){return a->sin_family==b->sin_family&&a->sin_port==b->sin_port&&a->sin_addr.s_addr==b->sin_addr.s_addr;}

static void net301_remote_reset(void){memset(&g_net301_remote,0,sizeof(g_net301_remote));}

static void net301_close_socket(void){
  if(g_net301_socket!=NET301_INVALID_SOCKET){closesocket(g_net301_socket);g_net301_socket=NET301_INVALID_SOCKET;}
}

static void net301_shutdown(void){
  if(g_net301_socket!=NET301_INVALID_SOCKET&&g_net301_peer_valid){
    Net301Bye p;memset(&p,0,sizeof(p));p.h.magic=NET301_MAGIC;p.h.protocol=NET301_PROTOCOL;p.h.type=NET301_PACKET_BYE;p.h.sequence=++g_net301_sequence;p.h.senderId=g_net301_local_id;
    sendto(g_net301_socket,(const char*)&p,(int)sizeof(p),0,&g_net301_peer_addr,(int)sizeof(g_net301_peer_addr));
  }
  net301_close_socket();
  if(g_net301_wsa){WSACleanup();g_net301_wsa=0;}
  g_net301_mode=NET301_MODE_OFF;g_net301_connected=0;g_net301_peer_valid=0;g_net301_local_id=0;g_net301_send_timer=0;g_net301_hello_timer=0;
  net301_remote_reset();
}

static int net301_socket_open(void){
  if(!g_net301_wsa){u8 wsa[512];memset(wsa,0,sizeof(wsa));if(WSAStartup(0x0202,wsa)!=0){g_net301_last_error=WSAGetLastError();return 0;}g_net301_wsa=1;}
  g_net301_socket=socket(NET301_AF_INET,NET301_SOCK_DGRAM,NET301_IPPROTO_UDP);
  if(g_net301_socket==NET301_INVALID_SOCKET){g_net301_last_error=WSAGetLastError();return 0;}
  DWORD nonblock=1;if(ioctlsocket(g_net301_socket,NET301_FIONBIO,&nonblock)==NET301_SOCKET_ERROR){g_net301_last_error=WSAGetLastError();net301_close_socket();return 0;}
  return 1;
}

static int net301_start_host(int port){
  net301_shutdown();g_net301_port=port>0?port:NET301_DEFAULT_PORT;
  if(!net301_socket_open())return 0;
  Net301SockAddrIn local;net301_addr_clear(&local);local.sin_port=htons((WORD)g_net301_port);local.sin_addr.s_addr=0;
  if(bind(g_net301_socket,&local,(int)sizeof(local))==NET301_SOCKET_ERROR){g_net301_last_error=WSAGetLastError();net301_shutdown();return 0;}
  g_net301_mode=NET301_MODE_HOST;g_net301_local_id=1;g_net301_sequence=0;g_net301_nonce^=rng_u32();return 1;
}

static int net301_start_client(const char*ip,int port){
  net301_shutdown();g_net301_port=port>0?port:NET301_DEFAULT_PORT;if(ip&&ip[0])str_copy(g_net301_join_ip,ip,(int)sizeof(g_net301_join_ip));
  if(!net301_socket_open())return 0;
  net301_addr_clear(&g_net301_peer_addr);g_net301_peer_addr.sin_port=htons((WORD)g_net301_port);g_net301_peer_addr.sin_addr.s_addr=inet_addr(g_net301_join_ip);
  if(g_net301_peer_addr.sin_addr.s_addr==0xffffffffu){g_net301_last_error=-301;net301_shutdown();return 0;}
  g_net301_peer_valid=1;g_net301_mode=NET301_MODE_CLIENT;g_net301_local_id=2;g_net301_sequence=0;g_net301_nonce^=rng_u32();g_net301_hello_timer=0;return 1;
}

static void net301_fill_header(Net301Header*h,u16 type){h->magic=NET301_MAGIC;h->protocol=NET301_PROTOCOL;h->type=type;h->sequence=++g_net301_sequence;h->senderId=g_net301_local_id;}

static int net301_send_to_peer(const void*data,int len){
  if(g_net301_socket==NET301_INVALID_SOCKET||!g_net301_peer_valid)return 0;
  int r=sendto(g_net301_socket,(const char*)data,len,0,&g_net301_peer_addr,(int)sizeof(g_net301_peer_addr));
  if(r==NET301_SOCKET_ERROR){g_net301_last_error=WSAGetLastError();return 0;}g_net301_tx_packets++;return 1;
}

static void net301_send_hello(void){Net301Hello p;memset(&p,0,sizeof(p));net301_fill_header(&p.h,NET301_PACKET_HELLO);p.nonce=g_net301_nonce;p.car=g_player.car&3;net301_send_to_peer(&p,(int)sizeof(p));}
static void net301_send_welcome(void){Net301Welcome p;memset(&p,0,sizeof(p));net301_fill_header(&p.h,NET301_PACKET_WELCOME);p.assignedId=2;net301_send_to_peer(&p,(int)sizeof(p));}

static u32 net301_local_flags(void){
  u32 f=0;if(key_down('S')&&g_player.speed>.25f)f|=NET301_FLAG_BRAKE;if(key_down(VK_SPACE))f|=NET301_FLAG_HANDBRAKE;if(g_hour<6||g_hour>=18||g_weather>=2)f|=NET301_FLAG_HEADLIGHTS;return f;
}

static void net301_send_snapshot(void){
  if(!g_started||!g_net301_connected)return;Net301Snapshot p;memset(&p,0,sizeof(p));net301_fill_header(&p.h,NET301_PACKET_SNAPSHOT);
  p.x=g_player.body.p.x;p.y=g_player.body.p.y;p.z=g_player.body.p.z;p.yaw=g_player.body.yaw;p.pitch=g_player.pitch;p.roll=g_player.roll;p.speed=g_player.speed;p.steer=g_player.steer;p.car=g_player.car&3;p.flags=net301_local_flags();net301_send_to_peer(&p,(int)sizeof(p));
}

static void net301_apply_snapshot(const Net301Snapshot*p){
  if(!p)return;if(g_net301_remote.active&&p->h.sequence<=g_net301_remote.lastSequence){g_net301_dropped_packets++;return;}
  int first=!g_net301_remote.active;g_net301_remote.active=1;g_net301_remote.playerId=p->h.senderId;g_net301_remote.car=p->car&3;g_net301_remote.targetP=v3(p->x,p->y,p->z);g_net301_remote.targetYaw=p->yaw;g_net301_remote.targetPitch=p->pitch;g_net301_remote.targetRoll=p->roll;g_net301_remote.speed=p->speed;g_net301_remote.steer=p->steer;g_net301_remote.flags=p->flags;g_net301_remote.age=0;g_net301_remote.lastSequence=p->h.sequence;
  if(first){g_net301_remote.renderP=g_net301_remote.targetP;g_net301_remote.renderYaw=p->yaw;g_net301_remote.renderPitch=p->pitch;g_net301_remote.renderRoll=p->roll;}
}

static int net301_packet_valid(const Net301Header*h,int bytes){return h&&bytes>=(int)sizeof(Net301Header)&&h->magic==NET301_MAGIC&&h->protocol==NET301_PROTOCOL;}

static void net301_receive(void){
  if(g_net301_socket==NET301_INVALID_SOCKET)return;for(int n=0;n<64;n++){
    u8 buffer[256];Net301SockAddrIn from;int fromLen=(int)sizeof(from);int got=recvfrom(g_net301_socket,(char*)buffer,(int)sizeof(buffer),0,&from,&fromLen);if(got==NET301_SOCKET_ERROR)break;if(got<(int)sizeof(Net301Header)){g_net301_dropped_packets++;continue;}
    Net301Header*h=(Net301Header*)buffer;if(!net301_packet_valid(h,got)){g_net301_dropped_packets++;continue;}g_net301_rx_packets++;
    if(g_net301_mode==NET301_MODE_HOST&&h->type==NET301_PACKET_HELLO&&got>=(int)sizeof(Net301Hello)){
      g_net301_peer_addr=from;g_net301_peer_valid=1;g_net301_connected=1;net301_send_welcome();continue;
    }
    if(!g_net301_peer_valid||!net301_addr_equal(&from,&g_net301_peer_addr)){g_net301_dropped_packets++;continue;}
    if(g_net301_mode==NET301_MODE_CLIENT&&h->type==NET301_PACKET_WELCOME&&got>=(int)sizeof(Net301Welcome)){Net301Welcome*w=(Net301Welcome*)buffer;g_net301_local_id=w->assignedId;g_net301_connected=1;continue;}
    if(h->type==NET301_PACKET_SNAPSHOT&&got>=(int)sizeof(Net301Snapshot)){if(h->senderId!=g_net301_local_id){g_net301_connected=1;net301_apply_snapshot((Net301Snapshot*)buffer);}continue;}
    if(h->type==NET301_PACKET_BYE){g_net301_connected=0;g_net301_peer_valid=g_net301_mode==NET301_MODE_CLIENT?1:0;net301_remote_reset();continue;}
  }
}

static float net301_angle_delta(float from,float to){float d=to-from;while(d>3.14159265f)d-=6.2831853f;while(d<-3.14159265f)d+=6.2831853f;return d;}
static void net301_interpolate(float dt){
  if(!g_net301_remote.active)return;g_net301_remote.age+=dt;if(g_net301_remote.age>3.0f){g_net301_remote.active=0;g_net301_connected=0;if(g_net301_mode==NET301_MODE_HOST)g_net301_peer_valid=0;return;}
  float a=clampf(dt*12.0f,0,1);g_net301_remote.renderP.x=lerpf(g_net301_remote.renderP.x,g_net301_remote.targetP.x,a);g_net301_remote.renderP.y=lerpf(g_net301_remote.renderP.y,g_net301_remote.targetP.y,a);g_net301_remote.renderP.z=lerpf(g_net301_remote.renderP.z,g_net301_remote.targetP.z,a);g_net301_remote.renderYaw+=net301_angle_delta(g_net301_remote.renderYaw,g_net301_remote.targetYaw)*a;g_net301_remote.renderPitch=lerpf(g_net301_remote.renderPitch,g_net301_remote.targetPitch,a);g_net301_remote.renderRoll=lerpf(g_net301_remote.renderRoll,g_net301_remote.targetRoll,a);
}

static void net301_parse_cmdline(void){
  if(g_net301_cmdline_checked)return;g_net301_cmdline_checked=1;LPSTR cmd=GetCommandLineA();if(!cmd)return;const char*portArg=strstr(cmd,"--port=");if(portArg){int p=atoi(portArg+7);if(p>0&&p<65536)g_net301_port=p;}
  const char*join=strstr(cmd,"--join=");if(join){join+=7;char ip[64];int i=0;while(join[i]&&join[i]!=' '&&join[i]!='\t'&&join[i]!='\"'&&i<63){ip[i]=join[i];i++;}ip[i]=0;if(ip[0])net301_start_client(ip,g_net301_port);return;}
  if(strstr(cmd,"--host"))net301_start_host(g_net301_port);
}

static void net301_update(float dt){
  net301_parse_cmdline();
  if(key_tap(NET301_VK_F8)){net301_shutdown();return;}
  if(key_tap(NET301_VK_F6))net301_start_host(g_net301_port);
  if(key_tap(NET301_VK_F7))net301_start_client("127.0.0.1",g_net301_port);
  if(g_net301_mode==NET301_MODE_OFF)return;
  net301_receive();
  if(g_net301_mode==NET301_MODE_CLIENT&&!g_net301_connected){g_net301_hello_timer-=dt;if(g_net301_hello_timer<=0){net301_send_hello();g_net301_hello_timer=.50f;}}
  if(g_net301_connected){g_net301_send_timer-=dt;if(g_net301_send_timer<=0){net301_send_snapshot();g_net301_send_timer=.05f;}}
  net301_interpolate(dt);
}
