static volatile LONG g_state=0; // 0 boot,1 checking,2 downloading,3 applying,4 current,5 updated,6 warning,7 error
static volatile LONG g_progress=0;
static volatile LONG g_file_index=0;
static volatile LONG g_file_count=0;
static char g_status[256]="Inicializando atualizador...";
static char g_detail[320]="";
static char g_local_version[32]="0.0.0";
static char g_remote_version[32]="";
static int g_auto_launch=1;
static u8 g_io_buf[65536];

static int slen(const char* s){return (int)strlen(s);} 
static void scopy(char* d,int cap,const char* s){int i=0;if(cap<=0)return;while(s&&s[i]&&i<cap-1){d[i]=s[i];i++;}d[i]=0;}
static int seq(const char* a,const char* b){int i=0;while(a[i]&&b[i]&&a[i]==b[i])i++;return a[i]==0&&b[i]==0;}
static int starts(const char* s,const char* p){int i=0;while(p[i]){if(s[i]!=p[i])return 0;i++;}return 1;}
static char lowerc(char c){return(c>='A'&&c<='Z')?(char)(c+32):c;}
static int starts_i(const char* s,const char* p){int i=0;while(p[i]){if(lowerc(s[i])!=lowerc(p[i]))return 0;i++;}return 1;}
static void set_status(const char* a,const char* b){scopy(g_status,256,a);scopy(g_detail,320,b?b:"");}
static DWORD rgb(u8 r,u8 g,u8 b){return (DWORD)(r|((u32)g<<8)|((u32)b<<16));}

static char* trim(char* s){while(*s==' '||*s=='\t'||*s=='\r')s++;char* e=s+slen(s);while(e>s&&(e[-1]==' '||e[-1]=='\t'||e[-1]=='\r'||e[-1]=='\n'))*--e=0;return s;}

static int file_exists(const char* p){DWORD a=GetFileAttributesA(p);return a!=INVALID_FILE_ATTRIBUTES && !(a&FILE_ATTRIBUTE_DIRECTORY);}
static int dir_exists(const char* p){DWORD a=GetFileAttributesA(p);return a!=INVALID_FILE_ATTRIBUTES && (a&FILE_ATTRIBUTE_DIRECTORY);}

static int ensure_parent_dirs(const char* path){char tmp[520];scopy(tmp,520,path);for(int i=0;tmp[i];i++){if(tmp[i]=='/'||tmp[i]=='\\'){char old=tmp[i];tmp[i]=0;if(tmp[0]&&!dir_exists(tmp))CreateDirectoryA(tmp,NULL);tmp[i]=old;}}return 1;}

static int read_entire_file(const char* path,char** out,u64* outSize){*out=0;*outSize=0;HANDLE h=CreateFileA(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);if(h==INVALID_HANDLE_VALUE)return 0;LARGE_INTEGER sz;if(!GetFileSizeEx(h,&sz)||sz.QuadPart<0||sz.QuadPart>16*1024*1024){CloseHandle(h);return 0;}u64 n=(u64)sz.QuadPart;char* data=(char*)VirtualAlloc(NULL,n+2,MEM_COMMIT|MEM_RESERVE,4);if(!data){CloseHandle(h);return 0;}DWORD got=0,total=0;while(total<(DWORD)n){if(!ReadFile(h,data+total,(DWORD)n-total,&got,NULL)||!got)break;total+=got;}CloseHandle(h);if(total!=(DWORD)n){VirtualFree(data,0,MEM_RELEASE);return 0;}data[n]=0;*out=data;*outSize=n;return 1;}

static int write_text_file(const char* path,const char* text){ensure_parent_dirs(path);HANDLE h=CreateFileA(path,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);if(h==INVALID_HANDLE_VALUE)return 0;DWORD n=(DWORD)slen(text),w=0;BOOL ok=WriteFile(h,text,n,&w,NULL);CloseHandle(h);return ok&&w==n;}

static void set_working_directory(void){char path[520];DWORD n=GetModuleFileNameA(NULL,path,519);path[n]=0;for(int i=(int)n-1;i>=0;i--){if(path[i]=='\\'||path[i]=='/'){path[i]=0;SetCurrentDirectoryA(path);break;}}}

// -----------------------------------------------------------------------------
// SHA-256
// -----------------------------------------------------------------------------
typedef struct {u32 h[8];u64 bits;u8 buf[64];u32 used;} SHA256;
static u32 ror32(u32 x,u32 n){return(x>>n)|(x<<(32-n));}
static const u32 K256[64]={
0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};
static void sha_init(SHA256* s){s->h[0]=0x6a09e667;s->h[1]=0xbb67ae85;s->h[2]=0x3c6ef372;s->h[3]=0xa54ff53a;s->h[4]=0x510e527f;s->h[5]=0x9b05688c;s->h[6]=0x1f83d9ab;s->h[7]=0x5be0cd19;s->bits=0;s->used=0;}
static void sha_block(SHA256* s,const u8* p){u32 w[64];for(int i=0;i<16;i++)w[i]=((u32)p[i*4]<<24)|((u32)p[i*4+1]<<16)|((u32)p[i*4+2]<<8)|p[i*4+3];for(int i=16;i<64;i++){u32 a=w[i-15],b=w[i-2];u32 s0=ror32(a,7)^ror32(a,18)^(a>>3);u32 s1=ror32(b,17)^ror32(b,19)^(b>>10);w[i]=w[i-16]+s0+w[i-7]+s1;}u32 a=s->h[0],b=s->h[1],c=s->h[2],d=s->h[3],e=s->h[4],f=s->h[5],g=s->h[6],h=s->h[7];for(int i=0;i<64;i++){u32 S1=ror32(e,6)^ror32(e,11)^ror32(e,25);u32 ch=(e&f)^((~e)&g);u32 t1=h+S1+ch+K256[i]+w[i];u32 S0=ror32(a,2)^ror32(a,13)^ror32(a,22);u32 maj=(a&b)^(a&c)^(b&c);u32 t2=S0+maj;h=g;g=f;f=e;e=d+t1;d=c;c=b;b=a;a=t1+t2;}s->h[0]+=a;s->h[1]+=b;s->h[2]+=c;s->h[3]+=d;s->h[4]+=e;s->h[5]+=f;s->h[6]+=g;s->h[7]+=h;}
static void sha_update(SHA256* s,const u8* p,u32 n){s->bits+=(u64)n*8;while(n){u32 take=64-s->used;if(take>n)take=n;memcpy(s->buf+s->used,p,take);s->used+=take;p+=take;n-=take;if(s->used==64){sha_block(s,s->buf);s->used=0;}}}
static void sha_final(SHA256* s,u8 out[32]){u32 u=s->used;s->buf[u++]=0x80;if(u>56){while(u<64)s->buf[u++]=0;sha_block(s,s->buf);u=0;}while(u<56)s->buf[u++]=0;u64 b=s->bits;for(int i=7;i>=0;i--)s->buf[u++]=(u8)(b>>(i*8));sha_block(s,s->buf);for(int i=0;i<8;i++){out[i*4]=(u8)(s->h[i]>>24);out[i*4+1]=(u8)(s->h[i]>>16);out[i*4+2]=(u8)(s->h[i]>>8);out[i*4+3]=(u8)s->h[i];}}
static void hex32(const u8 in[32],char out[65]){static const char* h="0123456789abcdef";for(int i=0;i<32;i++){out[i*2]=h[in[i]>>4];out[i*2+1]=h[in[i]&15];}out[64]=0;}
static int hex_eq_i(const char* a,const char* b){for(int i=0;i<64;i++){if(!a[i]||!b[i]||lowerc(a[i])!=lowerc(b[i]))return 0;}return a[64]==0||a[64]=='\r'||a[64]=='\n';}

// -----------------------------------------------------------------------------
// URL + download
// -----------------------------------------------------------------------------
typedef struct {int secure;WORD port;char host[256];char path[1400];} ParsedURL;
static ParsedURL g_parsed_url;
static unsigned short g_whost[256];
static unsigned short g_wpath[1400];
