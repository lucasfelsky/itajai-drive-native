// -----------------------------------------------------------------------------
// 3.2 Internet Multiplayer transport.
// Adds public relay/rendezvous sessions around the 3.1 vehicle snapshot protocol.
// Both players keep their validated local simulation; the relay only forwards UDP.
//
// Host:
//   ItajaiDriveNative.exe --relay=203.0.113.10:27016 --internet-host
// Guest:
//   ItajaiDriveNative.exe --relay=203.0.113.10:27016 --internet-join=ABC234
//
// F9 creates an Internet room when --relay was supplied. F11 disconnects.
// The room code and relay/ping status are rendered in the HUD.
// -----------------------------------------------------------------------------

typedef struct Net320HostEnt {
  char*h_name;
  char**h_aliases;
  short h_addrtype;
  short h_length;
  char**h_addr_list;
} Net320HostEnt;
__declspec(dllimport) Net320HostEnt* WINAPI gethostbyname(const char*);

#define NET320_MAGIC 0x32524A49u /* IJR2 */
#define NET320_PROTOCOL 1
#define NET320_RELAY_PORT 27016
#define NET320_CREATE 1
#define NET320_CREATED 2
#define NET320_JOIN 3
#define NET320_JOINED 4
#define NET320_READY 5
#define NET320_DATA 6
#define NET320_PING 7
#define NET320_PONG 8
#define NET320_LEAVE 9
#define NET320_ERROR 10
#define NET320_ERR_BAD_PACKET 1
#define NET320_ERR_ROOM_NOT_FOUND 2
#define NET320_ERR_ROOM_FULL 3
#define NET320_ERR_NOT_MEMBER 4
#define NET320_STATE_OFF 0
#define NET320_STATE_CREATING 1
#define NET320_STATE_WAITING 2
#define NET320_STATE_JOINING 3
#define NET320_STATE_CONNECTED 4
#define NET320_STATE_ERROR 5
#define NET320_MODE_NONE 0
#define NET320_MODE_HOST 1
#define NET320_MODE_GUEST 2
#define NET320_VK_F9 0x78
#define NET320_VK_F11 0x7A
#define NET320_MAX_INNER 512

#pragma pack(push,1)
typedef struct {
  u32 magic;
  u16 protocol;
  u16 type;
  u32 sequence;
  u32 nonce;
} Net320Header;
typedef struct {
  Net320Header h;
  char code[6];
  u16 reserved;
} Net320CodePacket;
typedef struct {
  Net320Header h;
  char code[6];
  u16 reserved;
  u16 payloadBytes;
  u8 payload[NET320_MAX_INNER];
} Net320DataPacket;
typedef struct { Net320Header h; float clientTime; } Net320PingPacket;
typedef struct { Net320Header h; u32 reason; } Net320ErrorPacket;
#pragma pack(pop)

static int g_net320_active=0;
static int g_net320_mode=NET320_MODE_NONE;
static int g_net320_state=NET320_STATE_OFF;
static int g_net320_connected=0;
static int g_net320_relay_valid=0;
static Net301SockAddrIn g_net320_relay_addr;
static char g_net320_relay_host[96]="";
static int g_net320_relay_port=NET320_RELAY_PORT;
static char g_net320_room[7]="";
static char g_net320_join_room[7]="";
static u32 g_net320_sequence=0;
static u32 g_net320_nonce=0x32032001u;
static float g_net320_control_timer=0;
static float g_net320_snapshot_timer=0;
static float g_net320_ping_timer=0;
static float g_net320_relay_age=0;
static float g_net320_ping_ms=-1.0f;
static int g_net320_last_error=0;
static int g_net320_cmdline_checked=0;
static int g_net320_rx_packets=0,g_net320_tx_packets=0,g_net320_forwarded=0;

static const char*net320_state_name(void){
  if(g_net320_state==NET320_STATE_CREATING)return "CRIANDO SALA";
  if(g_net320_state==NET320_STATE_WAITING)return "AGUARDANDO JOGADOR";
  if(g_net320_state==NET320_STATE_JOINING)return "ENTRANDO";
  if(g_net320_state==NET320_STATE_CONNECTED)return "CONECTADO";
  if(g_net320_state==NET320_STATE_ERROR)return "ERRO";
  return "OFF";
}

static const char*net320_error_name(void){
  if(g_net320_last_error==NET320_ERR_ROOM_NOT_FOUND)return "SALA NAO ENCONTRADA";
  if(g_net320_last_error==NET320_ERR_ROOM_FULL)return "SALA CHEIA";
  if(g_net320_last_error==NET320_ERR_NOT_MEMBER)return "SESSAO INVALIDA";
  if(g_net320_last_error==-320)return "RELAY NAO CONFIGURADO";
  if(g_net320_last_error==-321)return "RELAY INVALIDO";
  if(g_net320_last_error==-322)return "RELAY TIMEOUT";
  return "FALHA DE REDE";
}

static void net320_copy_room(char*d,const char*s){
  int i=0;for(;i<6&&s&&s[i];i++){char c=s[i];if(c>='a'&&c<='z')c=(char)(c-'a'+'A');d[i]=c;}for(;i<6;i++)d[i]=0;d[6]=0;
}
static int net320_room_valid(const char*c){if(!c)return 0;for(int i=0;i<6;i++){char x=c[i];int ok=(x>='A'&&x<='Z')||(x>='2'&&x<='9');if(!x||!ok)return 0;}return c[6]==0;}
static void net320_wire_room(char out[6]){for(int i=0;i<6;i++)out[i]=g_net320_room[i];}

static int net320_resolve_relay(const char*host,int port){
  if(!host||!host[0]){g_net320_last_error=-320;return 0;}
  DWORD ip=inet_addr(host);
  if(ip==0xffffffffu){Net320HostEnt*h=gethostbyname(host);if(!h||h->h_addrtype!=NET301_AF_INET||h->h_length!=4||!h->h_addr_list||!h->h_addr_list[0]){g_net320_last_error=-321;return 0;}memcpy(&ip,h->h_addr_list[0],4);}
  net301_addr_clear(&g_net320_relay_addr);g_net320_relay_addr.sin_port=htons((WORD)port);g_net320_relay_addr.sin_addr.s_addr=ip;g_net320_relay_valid=1;return 1;
}

static void net320_header(Net320Header*h,u16 type){h->magic=NET320_MAGIC;h->protocol=NET320_PROTOCOL;h->type=type;h->sequence=++g_net320_sequence;h->nonce=g_net320_nonce;}
static int net320_send_raw(const void*data,int bytes){
  if(g_net301_socket==NET301_INVALID_SOCKET||!g_net320_relay_valid)return 0;
  int r=sendto(g_net301_socket,(const char*)data,bytes,0,&g_net320_relay_addr,(int)sizeof(g_net320_relay_addr));if(r==NET301_SOCKET_ERROR){g_net301_last_error=WSAGetLastError();return 0;}g_net320_tx_packets++;return 1;
}

static void net320_send_create(void){Net320Header p;net320_header(&p,NET320_CREATE);net320_send_raw(&p,(int)sizeof(p));}
static void net320_send_code_packet(u16 type){Net320CodePacket p;memset(&p,0,sizeof(p));net320_header(&p.h,type);net320_wire_room(p.code);net320_send_raw(&p,(int)sizeof(p));}
static void net320_send_ping(void){Net320PingPacket p;memset(&p,0,sizeof(p));net320_header(&p.h,NET320_PING);p.clientTime=(float)g_time;net320_send_raw(&p,(int)sizeof(p));}

static int net320_send_inner(const void*inner,int bytes){
  if(bytes<=0||bytes>NET320_MAX_INNER||!g_net320_connected)return 0;Net320DataPacket p;memset(&p,0,sizeof(p));net320_header(&p.h,NET320_DATA);net320_wire_room(p.code);p.payloadBytes=(u16)bytes;memcpy(p.payload,inner,(unsigned long long)bytes);return net320_send_raw(&p,(int)(sizeof(Net320Header)+8+2+bytes));
}

static void net320_send_snapshot(void){
  if(!g_started||!g_net320_connected)return;Net301Snapshot p;memset(&p,0,sizeof(p));net301_fill_header(&p.h,NET301_PACKET_SNAPSHOT);p.x=g_player.body.p.x;p.y=g_player.body.p.y;p.z=g_player.body.p.z;p.yaw=g_player.body.yaw;p.pitch=g_player.pitch;p.roll=g_player.roll;p.speed=g_player.speed;p.steer=g_player.steer;p.car=g_player.car&3;p.flags=net301_local_flags();net320_send_inner(&p,(int)sizeof(p));
}

static void net320_reset_runtime(void){
  g_net320_active=0;g_net320_mode=NET320_MODE_NONE;g_net320_state=NET320_STATE_OFF;g_net320_connected=0;g_net320_room[0]=0;g_net320_control_timer=0;g_net320_snapshot_timer=0;g_net320_ping_timer=0;g_net320_relay_age=0;g_net320_ping_ms=-1;g_net301_connected=0;net301_remote_reset();
}

static void net320_shutdown(void){
  if(g_net320_active&&g_net301_socket!=NET301_INVALID_SOCKET&&g_net320_relay_valid&&g_net320_room[0])net320_send_code_packet(NET320_LEAVE);
  net301_close_socket();if(g_net301_wsa){WSACleanup();g_net301_wsa=0;}net320_reset_runtime();
}

static int net320_open_transport(void){
  net301_shutdown();if(!g_net320_relay_valid){g_net320_last_error=-320;g_net320_state=NET320_STATE_ERROR;return 0;}if(!net301_socket_open()){g_net320_state=NET320_STATE_ERROR;return 0;}g_net320_active=1;g_net320_sequence=0;g_net320_nonce^=rng_u32();g_net320_relay_age=0;return 1;
}

static int net320_start_host(void){
  if(!net320_open_transport())return 0;g_net320_mode=NET320_MODE_HOST;g_net320_state=NET320_STATE_CREATING;g_net301_local_id=1;g_net320_control_timer=0;g_net320_room[0]=0;return 1;
}
static int net320_start_guest(const char*room){
  char r[7];net320_copy_room(r,room);if(!net320_room_valid(r)){g_net320_last_error=NET320_ERR_ROOM_NOT_FOUND;g_net320_state=NET320_STATE_ERROR;return 0;}if(!net320_open_transport())return 0;g_net320_mode=NET320_MODE_GUEST;g_net320_state=NET320_STATE_JOINING;g_net301_local_id=2;net320_copy_room(g_net320_room,r);g_net320_control_timer=0;return 1;
}

static int net320_packet_valid(const Net320Header*h,int bytes){return h&&bytes>=(int)sizeof(Net320Header)&&h->magic==NET320_MAGIC&&h->protocol==NET320_PROTOCOL;}
static void net320_receive(void){
  if(!g_net320_active||g_net301_socket==NET301_INVALID_SOCKET)return;for(int n=0;n<96;n++){
    u8 buffer[768];Net301SockAddrIn from;int fromLen=(int)sizeof(from);int got=recvfrom(g_net301_socket,(char*)buffer,(int)sizeof(buffer),0,&from,&fromLen);if(got==NET301_SOCKET_ERROR)break;if(!net301_addr_equal(&from,&g_net320_relay_addr))continue;if(got<(int)sizeof(Net320Header))continue;
    Net320Header*h=(Net320Header*)buffer;if(!net320_packet_valid(h,got))continue;g_net320_rx_packets++;g_net320_relay_age=0;
    if(h->type==NET320_CREATED&&got>=(int)sizeof(Net320CodePacket)&&g_net320_mode==NET320_MODE_HOST){Net320CodePacket*p=(Net320CodePacket*)buffer;for(int i=0;i<6;i++)g_net320_room[i]=p->code[i];g_net320_room[6]=0;g_net320_state=NET320_STATE_WAITING;continue;}
    if(h->type==NET320_JOINED&&got>=(int)sizeof(Net320CodePacket)&&g_net320_mode==NET320_MODE_GUEST){g_net320_state=NET320_STATE_WAITING;continue;}
    if(h->type==NET320_READY&&got>=(int)sizeof(Net320CodePacket)){g_net320_state=NET320_STATE_CONNECTED;g_net320_connected=1;g_net301_connected=1;g_net320_snapshot_timer=0;continue;}
    if(h->type==NET320_PONG&&got>=(int)sizeof(Net320PingPacket)){Net320PingPacket*p=(Net320PingPacket*)buffer;g_net320_ping_ms=maxf(0,((float)g_time-p->clientTime)*1000.0f);continue;}
    if(h->type==NET320_ERROR&&got>=(int)sizeof(Net320ErrorPacket)){Net320ErrorPacket*p=(Net320ErrorPacket*)buffer;g_net320_last_error=(int)p->reason;g_net320_state=NET320_STATE_ERROR;g_net320_connected=0;continue;}
    if(h->type==NET320_LEAVE){g_net320_connected=0;g_net301_connected=0;net301_remote_reset();g_net320_state=g_net320_mode==NET320_MODE_HOST?NET320_STATE_WAITING:NET320_STATE_ERROR;if(g_net320_mode==NET320_MODE_GUEST)g_net320_last_error=NET320_ERR_ROOM_NOT_FOUND;continue;}
    if(h->type==NET320_DATA){int base=(int)sizeof(Net320Header)+8;if(got<base+2)continue;u16 payloadBytes=*(u16*)(buffer+base);int payloadStart=base+2;if(payloadBytes<=0||payloadBytes>NET320_MAX_INNER||payloadStart+payloadBytes!=got)continue;Net301Header*inner=(Net301Header*)(buffer+payloadStart);if(!net301_packet_valid(inner,payloadBytes))continue;if(inner->senderId==g_net301_local_id)continue;if(inner->type==NET301_PACKET_SNAPSHOT&&payloadBytes>=(u16)sizeof(Net301Snapshot)){g_net320_forwarded++;net301_apply_snapshot((Net301Snapshot*)inner);}continue;}
  }
}

static void net320_parse_relay_arg(const char*arg){
  char host[96];int i=0;while(arg&&arg[i]&&arg[i]!=' '&&arg[i]!='\t'&&arg[i]!='\"'&&arg[i]!=':'&&i<95){host[i]=arg[i];i++;}host[i]=0;int port=NET320_RELAY_PORT;if(arg&&arg[i]==':'){int p=atoi(arg+i+1);if(p>0&&p<65536)port=p;}if(host[0]){str_copy(g_net320_relay_host,host,(int)sizeof(g_net320_relay_host));g_net320_relay_port=port;net320_resolve_relay(host,port);}
}
static void net320_parse_cmdline(void){
  if(g_net320_cmdline_checked)return;g_net320_cmdline_checked=1;LPSTR cmd=GetCommandLineA();if(!cmd)return;const char*relay=strstr(cmd,"--relay=");if(relay)net320_parse_relay_arg(relay+8);const char*join=strstr(cmd,"--internet-join=");if(join){join+=16;char code[7];int i=0;while(i<6&&join[i]&&join[i]!=' '&&join[i]!='\t'&&join[i]!='\"'){code[i]=join[i];i++;}code[i]=0;net320_start_guest(code);return;}if(strstr(cmd,"--internet-host"))net320_start_host();
}

static void net320_update(float dt){
  net320_parse_cmdline();
  if(key_tap(NET320_VK_F11)){net320_shutdown();return;}
  if(key_tap(NET320_VK_F9)&&!g_net320_active){net320_start_host();}
  if(!g_net320_active)return;
  g_net320_relay_age+=dt;net320_receive();
  g_net320_control_timer-=dt;g_net320_ping_timer-=dt;
  if(g_net320_state==NET320_STATE_CREATING&&g_net320_control_timer<=0){net320_send_create();g_net320_control_timer=.75f;}
  else if(g_net320_state==NET320_STATE_JOINING&&g_net320_control_timer<=0){net320_send_code_packet(NET320_JOIN);g_net320_control_timer=.75f;}
  if(g_net320_ping_timer<=0){net320_send_ping();g_net320_ping_timer=2.0f;}
  if(g_net320_connected){g_net320_snapshot_timer-=dt;if(g_net320_snapshot_timer<=0){net320_send_snapshot();g_net320_snapshot_timer=.05f;}net301_interpolate(dt);}
  if(g_net320_relay_age>7.0f&&g_net320_state!=NET320_STATE_ERROR){g_net320_last_error=-322;g_net320_state=NET320_STATE_ERROR;g_net320_connected=0;g_net301_connected=0;net301_remote_reset();}
}
