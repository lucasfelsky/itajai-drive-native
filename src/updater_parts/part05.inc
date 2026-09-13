static int channel12_hit(int x,int y,int l,int t,int r,int b){return x>=l&&x<r&&y>=t&&y<b;}
static void channel12_button(HDC dc,int l,int t,int r,int b,const char*text,int active,int enabled){RECT q={l,t,r,b};HBRUSH br=CreateSolidBrush(active?rgb(47,129,191):(enabled?rgb(43,49,59):rgb(31,35,42)));FillRect(dc,&q,br);DeleteObject(br);SetTextColor(dc,active?rgb(248,251,255):(enabled?rgb(193,202,214):rgb(104,111,121)));TextOutA(dc,l+14,t+9,text,slen(text));}

static void draw_ui(void){
  if(!g_hwnd)return;HDC dc=GetDC(g_hwnd);if(!dc)return;RECT r;GetClientRect(g_hwnd,&r);HBRUSH bg=CreateSolidBrush(rgb(17,20,26));FillRect(dc,&r,bg);DeleteObject(bg);HGDIOBJ font=GetStockObject(SYSTEM_FONT);SelectObject(dc,font);SetBkMode(dc,TRANSPARENT);
  SetTextColor(dc,rgb(237,241,247));const char* title="ITAJAI DRIVE // UPDATER 1.2";TextOutA(dc,28,24,title,slen(title));
  SetTextColor(dc,rgb(123,201,255));const char* sub="Stable + Renderer 5.0 Beta // SHA-256 // retry // rollback seguro";TextOutA(dc,28,48,sub,slen(sub));
  SetTextColor(dc,rgb(235,238,243));TextOutA(dc,28,86,g_status,slen(g_status));SetTextColor(dc,rgb(158,168,182));TextOutA(dc,28,110,g_detail,slen(g_detail));
  SetTextColor(dc,rgb(126,138,154));const char* cl="CANAL DE ATUALIZACAO";TextOutA(dc,28,139,cl,slen(cl));
  channel12_button(dc,28,158,236,194,"STABLE",!g_channel_beta12,!g_update_started12);
  channel12_button(dc,248,158,528,194,"RENDERER 5.0 BETA",g_channel_beta12,!g_update_started12);
  RECT track={28,220,r.right-28,240};HBRUSH tb=CreateSolidBrush(rgb(46,52,63));FillRect(dc,&track,tb);DeleteObject(tb);RECT bar=track;bar.right=bar.left+(LONG)((track.right-track.left)*(g_progress/100.0));HBRUSH pb=CreateSolidBrush(rgb(64,174,255));FillRect(dc,&bar,pb);DeleteObject(pb);
  char v[240];sprintf(v,"Instalada: %s%s%s   |   Canal: %s",g_local_version,g_remote_version[0]?"   |   Disponivel: ":"",g_remote_version[0]?g_remote_version:"",channel12_name());SetTextColor(dc,rgb(125,134,146));TextOutA(dc,28,258,v,slen(v));
  channel12_button(dc,28,292,286,330,g_update_started12?"VERIFICANDO...":"VERIFICAR / JOGAR",0,!g_update_started12);
  SetTextColor(dc,rgb(102,112,126));const char* hint="1 Stable   2 Beta   Enter verificar";TextOutA(dc,304,304,hint,slen(hint));
  ReleaseDC(g_hwnd,dc);
}

static LRESULT CALLBACK wndproc(HWND h,UINT m,WPARAM w,LPARAM l){
  if(m==0x0201&&!g_update_started12){int x=(int)(short)(l&0xffff),y=(int)(short)((l>>16)&0xffff);if(channel12_hit(x,y,28,158,236,194)){channel12_select(0);return 0;}if(channel12_hit(x,y,248,158,528,194)){channel12_select(1);return 0;}if(channel12_hit(x,y,28,292,286,330)){channel12_start();return 0;}}
  if(m==0x0100&&!g_update_started12){if(w=='1'){channel12_select(0);return 0;}if(w=='2'){channel12_select(1);return 0;}if(w==13){channel12_start();return 0;}}
  if(m==WM_CLOSE||m==WM_DESTROY){PostQuitMessage(0);return 0;}return DefWindowProcA(h,m,w,l);
}

static int init_window(void){HINSTANCE inst=GetModuleHandleA(NULL);WNDCLASSA wc;memset(&wc,0,sizeof(wc));wc.style=CS_OWNDC;wc.lpfnWndProc=wndproc;wc.hInstance=inst;wc.lpszClassName="ItajaiDriveUpdaterWindow";if(!RegisterClassA(&wc))return 0;g_hwnd=CreateWindowExA(0,wc.lpszClassName,"Itajai Drive Updater 1.2",WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_VISIBLE,180,160,660,380,NULL,NULL,inst,NULL);if(!g_hwnd)return 0;ShowWindow(g_hwnd,SW_SHOW);UpdateWindow(g_hwnd);return 1;}

void WinMainCRTStartup(void){set_working_directory();read_version();channel12_load_config();if(!init_window())ExitProcess(2);if(g_channel_beta12)set_status("Canal Renderer 5.0 Beta selecionado.","Clique VERIFICAR / JOGAR para procurar a build beta mais recente.");else set_status("Canal Stable selecionado.","Clique VERIFICAR / JOGAR para procurar apenas releases estaveis.");MSG msg;int running=1;while(running){while(PeekMessageA(&msg,NULL,0,0,PM_REMOVE)){if(msg.message==WM_QUIT){running=0;break;}TranslateMessage(&msg);DispatchMessageA(&msg);}draw_ui();Sleep(33);}if(g_update_thread12)CloseHandle(g_update_thread12);ExitProcess(0);}
