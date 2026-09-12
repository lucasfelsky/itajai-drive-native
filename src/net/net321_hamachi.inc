// -----------------------------------------------------------------------------
// 3.2.1 Hamachi / virtual-LAN multiplayer helper.
// Detects LogMeIn Hamachi via GetAdaptersInfo and provides an engine-native
// keyboard menu around the existing 3.1 direct UDP host/client transport.
// The game still binds UDP 27015 on all local interfaces, so Hamachi works as a
// normal virtual LAN without exposing a router port to the public Internet.
// -----------------------------------------------------------------------------

#define NET321_VK_F12 0x7B
#define NET321_VK_BACK 0x08
#define NET321_VK_ENTER 0x0D
#define NET321_VK_OEM_PERIOD 0xBE
#define NET321_VK_DECIMAL 0x6E
#define NET321_ADAPTER_BUFFER 32768
#define NET321_MAX_ADAPTER_NAME 256
#define NET321_MAX_ADAPTER_DESC 128
#define NET321_MAX_ADAPTER_ADDR 8

#define NET321_ERROR_BUFFER_OVERFLOW 111u
#define NET321_ERROR_SUCCESS 0u

typedef struct Net321IpAddrString {
  struct Net321IpAddrString*Next;
  char IpAddress[16];
  char IpMask[16];
  DWORD Context;
} Net321IpAddrString;

typedef struct Net321AdapterInfo {
  struct Net321AdapterInfo*Next;
  DWORD ComboIndex;
  char AdapterName[NET321_MAX_ADAPTER_NAME+4];
  char Description[NET321_MAX_ADAPTER_DESC+4];
  UINT AddressLength;
  BYTE Address[NET321_MAX_ADAPTER_ADDR];
  DWORD Index;
  UINT Type;
  UINT DhcpEnabled;
  Net321IpAddrString*CurrentIpAddress;
  Net321IpAddrString IpAddressList;
  Net321IpAddrString GatewayList;
  Net321IpAddrString DhcpServer;
  BOOL HaveWins;
  Net321IpAddrString PrimaryWinsServer;
  Net321IpAddrString SecondaryWinsServer;
  long long LeaseObtained;
  long long LeaseExpires;
} Net321AdapterInfo;

__declspec(dllimport) DWORD WINAPI GetAdaptersInfo(Net321AdapterInfo*,DWORD*);

static int g_net321_menu_open=0;
static int g_net321_selected=0;
static int g_net321_hamachi_found=0;
static int g_net321_hamachi_probable=0;
static char g_net321_hamachi_ip[16]="";
static char g_net321_hamachi_adapter[132]="";
static char g_net321_join_ip[32]="";
static char g_net321_status[96]="";
static float g_net321_scan_timer=0;
static int g_net321_initialized=0;

static char net321_lower(char c){if(c>='A'&&c<='Z')return (char)(c-'A'+'a');return c;}
static int net321_contains_ci(const char*s,const char*needle){
  if(!s||!needle||!needle[0])return 0;
  for(int i=0;s[i];i++){
    int j=0;while(needle[j]&&s[i+j]&&net321_lower(s[i+j])==net321_lower(needle[j]))j++;
    if(!needle[j])return 1;
  }
  return 0;
}
static int net321_ip_is_usable(const char*ip){
  if(!ip||!ip[0]||!strcmp(ip,"0.0.0.0")||!strcmp(ip,"127.0.0.1"))return 0;
  DWORD a=inet_addr(ip);return a!=0xffffffffu;
}
static int net321_ip_looks_hamachi(const char*ip){return ip&&ip[0]=='2'&&ip[1]=='5'&&ip[2]=='.';}

static void net321_detect_hamachi(void){
  g_net321_hamachi_found=0;g_net321_hamachi_probable=0;g_net321_hamachi_ip[0]=0;g_net321_hamachi_adapter[0]=0;
  static u8 buffer[NET321_ADAPTER_BUFFER];DWORD bytes=(DWORD)sizeof(buffer);
  DWORD r=GetAdaptersInfo((Net321AdapterInfo*)buffer,&bytes);if(r!=NET321_ERROR_SUCCESS)return;
  char fallbackIp[16]="";char fallbackDesc[132]="";
  for(Net321AdapterInfo*a=(Net321AdapterInfo*)buffer;a;a=a->Next){
    int named=net321_contains_ci(a->Description,"hamachi")||net321_contains_ci(a->AdapterName,"hamachi")||net321_contains_ci(a->Description,"logmein");
    for(Net321IpAddrString*ip=&a->IpAddressList;ip;ip=ip->Next){
      if(!net321_ip_is_usable(ip->IpAddress))continue;
      if(named){str_copy(g_net321_hamachi_ip,ip->IpAddress,(int)sizeof(g_net321_hamachi_ip));str_copy(g_net321_hamachi_adapter,a->Description,(int)sizeof(g_net321_hamachi_adapter));g_net321_hamachi_found=1;return;}
      if(!fallbackIp[0]&&net321_ip_looks_hamachi(ip->IpAddress)){str_copy(fallbackIp,ip->IpAddress,(int)sizeof(fallbackIp));str_copy(fallbackDesc,a->Description,(int)sizeof(fallbackDesc));}
    }
  }
  if(fallbackIp[0]){str_copy(g_net321_hamachi_ip,fallbackIp,(int)sizeof(g_net321_hamachi_ip));str_copy(g_net321_hamachi_adapter,fallbackDesc,(int)sizeof(g_net321_hamachi_adapter));g_net321_hamachi_found=1;g_net321_hamachi_probable=1;}
}

static void net321_set_status(const char*s){str_copy(g_net321_status,s?s:"",(int)sizeof(g_net321_status));}
static void net321_disconnect_all(void){if(g_net320_active)net320_shutdown();net301_shutdown();net321_set_status("DESCONECTADO");}
static void net321_host(void){
  if(g_net320_active)net320_shutdown();
  if(net301_start_host(NET301_DEFAULT_PORT)){net321_set_status("HOST ATIVO // AGUARDANDO JOGADOR");}
  else net321_set_status("FALHA AO CRIAR HOST");
}
static void net321_join(void){
  if(!net321_ip_is_usable(g_net321_join_ip)){net321_set_status("IP INVALIDO");return;}
  if(g_net320_active)net320_shutdown();
  if(net301_start_client(g_net321_join_ip,NET301_DEFAULT_PORT)){net321_set_status("CONECTANDO...");}
  else net321_set_status("FALHA AO CONECTAR");
}

static void net321_input_ip(void){
  int len=(int)strlen(g_net321_join_ip);
  if(key_tap(NET321_VK_BACK)&&len>0){g_net321_join_ip[len-1]=0;return;}
  if(len>=30)return;
  for(int k=0;k<=9;k++)if(key_tap('0'+k)){g_net321_join_ip[len]=(char)('0'+k);g_net321_join_ip[len+1]=0;return;}
  if((key_tap(NET321_VK_OEM_PERIOD)||key_tap(NET321_VK_DECIMAL))&&len>0&&g_net321_join_ip[len-1]!='.'){g_net321_join_ip[len]='.';g_net321_join_ip[len+1]=0;}
}

static void net321_toggle_menu(void){
  g_net321_menu_open=!g_net321_menu_open;
  if(g_net321_menu_open){if(g_camera_captured)camera_release_mouse();net321_detect_hamachi();g_net321_scan_timer=3.0f;}
}

static const char*net321_direct_state(void){
  if(g_net320_active)return "INTERNET RELAY ATIVO";
  if(g_net301_mode==NET301_MODE_HOST)return g_net301_connected?"HOST // CONECTADO":"HOST // AGUARDANDO";
  if(g_net301_mode==NET301_MODE_CLIENT)return g_net301_connected?"CLIENTE // CONECTADO":"CLIENTE // CONECTANDO";
  return "OFFLINE";
}

static void net321_update(float dt){
  if(!g_net321_initialized){g_net321_initialized=1;net321_detect_hamachi();if(g_net321_hamachi_found)net321_set_status("HAMACHI PRONTO");}
  if(key_tap(NET321_VK_F12)){net321_toggle_menu();return;}
  if(!g_net321_menu_open)return;
  g_player.body.vel=v3(0,0,0);g_player.body.angularVel=0;g_player.speed=0;
  g_net321_scan_timer-=dt;if(g_net321_scan_timer<=0){net321_detect_hamachi();g_net321_scan_timer=3.0f;}
  if(key_tap(VK_UP)||key_tap('W')){g_net321_selected=(g_net321_selected+2)%3;}
  if(key_tap(VK_DOWN)||key_tap('S')){g_net321_selected=(g_net321_selected+1)%3;}
  if(g_net321_selected==1)net321_input_ip();
  if(key_tap(NET321_VK_ENTER)){
    if(g_net321_selected==0)net321_host();
    else if(g_net321_selected==1)net321_join();
    else net321_disconnect_all();
  }
  if(key_tap(NET301_VK_F8))net321_disconnect_all();
}
