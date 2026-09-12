// -----------------------------------------------------------------------------
// 3.9 OSM world scope query.
// Roads/signals cover the entire Itajai administrative area plus the deliberate
// Navegantes/airport/BR-470/BR-101 extension. Heavy building geometry remains
// on the proven detailed urban bbox; municipal cadastral enrichment still owns
// the dense building pass.
// -----------------------------------------------------------------------------
static int world390_fetch_host(LPCWSTR host,char**out,DWORD*outSize){
  const char*query="[out:json][timeout:95];area[boundary=administrative][admin_level=8][name~\"^Itaja\"]->.itajai;(way[highway](area.itajai);node[highway=traffic_signals](area.itajai);way[highway](-26.9350,-48.7450,-26.8350,-48.6200);node[highway=traffic_signals](-26.9350,-48.7450,-26.8350,-48.6200);way[highway](-26.9580,-48.6860,-26.8840,-48.6250);node[highway=traffic_signals](-26.9580,-48.6860,-26.8840,-48.6250);way[building](-26.9580,-48.6860,-26.8840,-48.6250););out geom;";
  char*enc=g_post_body;int encLen=percent_encode(query,enc+5,8187);enc[0]='d';enc[1]='a';enc[2]='t';enc[3]='a';enc[4]='=';encLen+=5;
  HINTERNET ses=WinHttpOpen(L"ItajaiDriveNative/3.9.0",WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,NULL,NULL,0);if(!ses)return 0;WinHttpSetTimeouts(ses,7000,12000,35000,100000);
  HINTERNET con=WinHttpConnect(ses,host,INTERNET_DEFAULT_HTTPS_PORT,0);if(!con){WinHttpCloseHandle(ses);return 0;}
  HINTERNET req=WinHttpOpenRequest(con,L"POST",L"/api/interpreter",NULL,NULL,NULL,WINHTTP_FLAG_SECURE);if(!req){WinHttpCloseHandle(con);WinHttpCloseHandle(ses);return 0;}
  BOOL ok=WinHttpSendRequest(req,L"Content-Type: application/x-www-form-urlencoded\r\n",(DWORD)-1,enc,(DWORD)encLen,(DWORD)encLen,0)&&WinHttpReceiveResponse(req,NULL);if(!ok){WinHttpCloseHandle(req);WinHttpCloseHandle(con);WinHttpCloseHandle(ses);return 0;}
  DWORD cap=128u*1024u*1024u,used=0;char*data=(char*)VirtualAlloc(NULL,(SIZE_T)cap+1,MEM_COMMIT|MEM_RESERVE,4);if(!data){WinHttpCloseHandle(req);WinHttpCloseHandle(con);WinHttpCloseHandle(ses);return 0;}
  while(1){DWORD avail=0;if(!WinHttpQueryDataAvailable(req,&avail)){ok=0;break;}if(!avail){ok=1;break;}if(used+avail>cap){ok=0;break;}DWORD got=0;if(!WinHttpReadData(req,data+used,avail,&got)){ok=0;break;}used+=got;if(!got)break;}
  data[used]=0;WinHttpCloseHandle(req);WinHttpCloseHandle(con);WinHttpCloseHandle(ses);if(!ok||used<100){VirtualFree(data,0,MEM_RELEASE);return 0;}*out=data;*outSize=used;return 1;
}
