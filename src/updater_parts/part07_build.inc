// Renderer beta build identity fallback.
// BUILD.txt is managed by the beta manifest. If the semantic version is unchanged
// but the gated commit id changes, force the normal transactional updater once.
static char g_beta_build501_remote[65]="";
static int build501_hex40(const char*s){if(!s)return 0;for(int i=0;i<40;i++){char c=s[i];if(!c||!((c>='0'&&c<='9')||(lowerc(c)>='a'&&lowerc(c)<='f')))return 0;}return s[40]==0;}
static void build501_url(char*out,int cap){scopy(out,cap,g_beta_manifest_url12);int n=slen(out),slash=-1;for(int i=0;i<n;i++)if(out[i]=='/'||out[i]=='\\')slash=i;if(slash>=0){out[slash+1]=0;int p=slash+1;const char*s="BUILD.txt";for(int i=0;s[i]&&p<cap-1;i++)out[p++]=s[i];out[p]=0;}}
static int build501_fetch(void){g_beta_build501_remote[0]=0;if(!g_channel_beta12)return 0;char url[1400];build501_url(url,1400);char*d=0;u64 n=0;if(!http_get_memory(url,&d,&n))return 0;scopy(g_beta_build501_remote,65,trim(d));VirtualFree(d,0,MEM_RELEASE);return build501_hex40(g_beta_build501_remote);}
static void build501_local(char*out){out[0]=0;char*d=0;u64 n=0;if(!read_entire_file("BUILD.txt",&d,&n))return;scopy(out,65,trim(d));VirtualFree(d,0,MEM_RELEASE);}
static void build501_write(void){if(!g_beta_build501_remote[0])return;char b[80];sprintf(b,"%s\r\n",g_beta_build501_remote);write_text_file("BUILD.txt",b);}

static DWORD WINAPI updater_thread(LPVOID unused){
  channel12_load_config();char local[65];build501_local(local);int changed=build501_fetch()&&!seq(local,g_beta_build501_remote);char saved[64]="";int oldAuto=g_auto_launch;
  if(changed){read_version();scopy(saved,64,g_local_version);g_auto_launch=0;write_text_file("VERSION.txt","0.0.0\r\n");}
  DWORD r=updater_thread_legacy12(unused);
  if(!changed)return r;
  if(g_state==5||g_state==4){build501_write();g_auto_launch=oldAuto;channel12_save_config();set_status("Beta atualizada.","Novo build detectado e instalado mesmo com a mesma versao.");launch_game_after(1000);}
  else {sprintf(g_version_text,"%s\r\n",saved);write_text_file("VERSION.txt",g_version_text);scopy(g_local_version,32,saved);g_auto_launch=oldAuto;channel12_save_config();if(g_state!=7)launch_game_after(1800);}
  return r;
}

static void channel12_start(void){
  if(g_update_started12)return;channel12_load_config();channel12_save_config();g_update_started12=1;
  set_status("Preparando verificacao...",g_channel_beta12?"Canal: Renderer 5.0 Beta":"Canal: Stable");
  DWORD tid=0;g_update_thread12=CreateThread(NULL,0,updater_thread,NULL,0,&tid);
  if(!g_update_thread12){g_update_started12=0;set_status("Nao consegui iniciar o atualizador.","Feche e tente novamente.");g_state=7;}
}
