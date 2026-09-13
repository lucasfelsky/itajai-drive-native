// -----------------------------------------------------------------------------
// Updater 1.2 channel selector.
// Keeps the legacy updater transaction/rollback engine intact and only selects
// which manifest URL it receives through update_config.ini.
// -----------------------------------------------------------------------------
static char g_stable_manifest_url12[1400]="";
static char g_beta_manifest_url12[1400]="";
static int g_channel_beta12=0;
static int g_channel_config_loaded12=0;
static int g_update_started12=0;
static HANDLE g_update_thread12=0;

static void channel12_choose_manifest(void){
  const char* u=(g_channel_beta12&&g_beta_manifest_url12[0])?g_beta_manifest_url12:g_stable_manifest_url12;
  scopy(g_manifest_url,1400,u?u:"");
}

static void channel12_load_config(void){
  if(g_channel_config_loaded12)return;
  g_channel_config_loaded12=1;
  char legacy[1400]="";char* d=0;u64 n=0;
  if(read_entire_file("update_config.ini",&d,&n)){
    char* p=d;while(*p){char* line=p;while(*p&&*p!='\n')p++;if(*p)*p++=0;line=trim(line);if(!line[0]||line[0]=='#'||line[0]==';')continue;
      if(starts(line,"manifest_url="))scopy(legacy,1400,trim(line+13));
      else if(starts(line,"stable_manifest_url="))scopy(g_stable_manifest_url12,1400,trim(line+20));
      else if(starts(line,"beta_manifest_url="))scopy(g_beta_manifest_url12,1400,trim(line+18));
      else if(starts(line,"channel=")){char* v=trim(line+8);g_channel_beta12=starts_i(v,"beta")?1:0;}
      else if(starts(line,"auto_launch="))g_auto_launch=atoi(trim(line+12))?1:0;
    }
    VirtualFree(d,0,MEM_RELEASE);
  }
  if(!g_stable_manifest_url12[0]&&legacy[0])scopy(g_stable_manifest_url12,1400,legacy);
  if(!g_stable_manifest_url12[0])scopy(g_stable_manifest_url12,1400,"https://github.com/lucasfelsky/itajai-drive-native/releases/latest/download/manifest.txt");
  if(!g_beta_manifest_url12[0])scopy(g_beta_manifest_url12,1400,"https://github.com/lucasfelsky/itajai-drive-native/releases/download/renderer-beta/manifest.txt");
  channel12_choose_manifest();
}

static void channel12_save_config(void){
  channel12_load_config();channel12_choose_manifest();char b[4600];
  sprintf(b,"manifest_url=%s\r\nstable_manifest_url=%s\r\nbeta_manifest_url=%s\r\nchannel=%s\r\nauto_launch=%d\r\n",
    g_manifest_url,g_stable_manifest_url12,g_beta_manifest_url12,g_channel_beta12?"beta":"stable",g_auto_launch?1:0);
  write_text_file("update_config.ini",b);
}

static void channel12_select(int beta){
  if(g_update_started12)return;channel12_load_config();g_channel_beta12=beta?1:0;channel12_save_config();
  if(g_channel_beta12)set_status("Canal Renderer 5.0 Beta selecionado.","Clique VERIFICAR / JOGAR para instalar ou validar a beta.");
  else set_status("Canal Stable selecionado.","Clique VERIFICAR / JOGAR para usar apenas releases estaveis.");
}

static DWORD WINAPI updater_thread(LPVOID unused){
  channel12_load_config();channel12_save_config();
  return updater_thread_legacy11(unused);
}

static void channel12_start(void){
  if(g_update_started12)return;channel12_load_config();channel12_save_config();g_update_started12=1;
  set_status("Preparando verificacao...",g_channel_beta12?"Canal: Renderer 5.0 Beta":"Canal: Stable");
  DWORD tid=0;g_update_thread12=CreateThread(NULL,0,updater_thread,NULL,0,&tid);
  if(!g_update_thread12){g_update_started12=0;set_status("Nao consegui iniciar o atualizador.","Feche e tente novamente.");g_state=7;}
}

static const char* channel12_name(void){return g_channel_beta12?"Renderer 5.0 Beta":"Stable";}
