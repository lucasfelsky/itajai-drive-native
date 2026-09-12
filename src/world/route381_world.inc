// -----------------------------------------------------------------------------
// 3.8.1 route graph extension.
// Keeps the detailed Itajai/Navegantes city/building bbox unchanged, but extends
// only the OSM road/signal query northwest far enough to include the real BR-101
// Itajai-Acu crossing and its connection corridor. This lets A* connect the two
// banks without importing a second city's worth of building geometry.
// -----------------------------------------------------------------------------
static int route381_fetch_host(LPCWSTR host,char**out,DWORD*outSize){
  const char*query="[out:json][timeout:75];(way[highway](-26.9580,-48.6860,-26.8840,-48.6250);way[highway](-26.9150,-48.7260,-26.8400,-48.6810);node[highway=traffic_signals](-26.9580,-48.6860,-26.8840,-48.6250);node[highway=traffic_signals](-26.9150,-48.7260,-26.8400,-48.6810);way[building](-26.9580,-48.6860,-26.8840,-48.6250););out geom;";
  char*enc=g_post_body;int encLen=percent_encode(query,enc+5,8187);enc[0]='d';enc[1]='a';enc[2]='t';enc[3]='a';enc[4]='=';encLen+=5;
  HINTERNET ses=WinHttpOpen(L"ItajaiDriveNative/3.8.1",WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,NULL,NULL,0);if(!ses)return 0;WinHttpSetTimeouts(ses,7000,12000,30000,85000);
  HINTERNET con=WinHttpConnect(ses,host,INTERNET_DEFAULT_HTTPS_PORT,0);if(!con){WinHttpCloseHandle(ses);return 0;}
  HINTERNET req=WinHttpOpenRequest(con,L"POST",L"/api/interpreter",NULL,NULL,NULL,WINHTTP_FLAG_SECURE);if(!req){WinHttpCloseHandle(con);WinHttpCloseHandle(ses);return 0;}
  BOOL ok=WinHttpSendRequest(req,L"Content-Type: application/x-www-form-urlencoded\r\n",(DWORD)-1,enc,(DWORD)encLen,(DWORD)encLen,0)&&WinHttpReceiveResponse(req,NULL);if(!ok){WinHttpCloseHandle(req);WinHttpCloseHandle(con);WinHttpCloseHandle(ses);return 0;}
  DWORD cap=96u*1024u*1024u,used=0;char*data=(char*)VirtualAlloc(NULL,(SIZE_T)cap+1,MEM_COMMIT|MEM_RESERVE,4);if(!data){WinHttpCloseHandle(req);WinHttpCloseHandle(con);WinHttpCloseHandle(ses);return 0;}
  while(1){DWORD avail=0;if(!WinHttpQueryDataAvailable(req,&avail)){ok=0;break;}if(!avail){ok=1;break;}if(used+avail>cap){ok=0;break;}DWORD got=0;if(!WinHttpReadData(req,data+used,avail,&got)){ok=0;break;}used+=got;if(!got)break;}
  data[used]=0;WinHttpCloseHandle(req);WinHttpCloseHandle(con);WinHttpCloseHandle(ses);if(!ok||used<100){VirtualFree(data,0,MEM_RELEASE);return 0;}*out=data;*outSize=used;return 1;
}
