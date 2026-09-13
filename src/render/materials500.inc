// -----------------------------------------------------------------------------
// Renderer 5.0 material texture upgrade.
// 5.0 visual pass 1: denser facade variation + manual mip chain. The mip chain
// keeps distant road/building detail stable without requiring GL3-only helpers.
// Existing material IDs/shader contracts remain unchanged.
// -----------------------------------------------------------------------------
#define MAT500_SIZE 256
#ifndef GL_LINEAR_MIPMAP_LINEAR
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#endif
static u8 g_mat500_pixels[MAT500_SIZE*MAT500_SIZE*3];
static u8 g_mat500_mip[MAT500_SIZE*MAT500_SIZE*3];
static int g_mat500_ready=0;
static u32 mat500_hash(u32 x,u32 y,u32 s){u32 h=x*0x9e3779b1u^y*0x85ebca6bu^s;h^=h>>16;h*=0x7feb352du;h^=h>>15;return h^(h>>16);}
static int mat500_clampi(int v,int a,int b){return v<a?a:v>b?b:v;}

static void mat500_fill(int kind){
  for(int y=0;y<MAT500_SIZE;y++)for(int x=0;x<MAT500_SIZE;x++){
    u32 h=mat500_hash((u32)x,(u32)y,771u+(u32)kind*991u),h2=mat500_hash((u32)(x/4),(u32)(y/4),119u+(u32)kind);int i=(y*MAT500_SIZE+x)*3;
    if(kind==1){
      int fine=(int)(h&31),clump=(int)(h2&31);int blade=((x*7+y*13+(int)(h>>8))%43)<2;
      g_mat500_pixels[i]=(u8)mat500_clampi(31+fine/4+clump/7+(blade?5:0),0,255);
      g_mat500_pixels[i+1]=(u8)mat500_clampi(70+fine+clump/3+(blade?18:0),0,255);
      g_mat500_pixels[i+2]=(u8)mat500_clampi(29+fine/5+clump/8,0,255);
    }else if(kind==2){
      int fine=(int)(h&27),large=(int)(h2&19);int aggregate=((h>>10)&31)<4;int crack=((x*17+y*11+(int)(h>>16))%211)==0;int oil=((x-172)*(x-172)+(y-84)*(y-84)<260&&((h>>7)&7)>2);int v=38+fine/2+large/3+(aggregate?8:0)-(crack?19:0)-(oil?5:0);
      g_mat500_pixels[i]=(u8)mat500_clampi(v,0,255);g_mat500_pixels[i+1]=(u8)mat500_clampi(v+1,0,255);g_mat500_pixels[i+2]=(u8)mat500_clampi(v+2,0,255);
    }else{
      // One 256px tile carries an 8x8 facade bay atlas instead of repeating one
      // identical window. Per-bay hashing varies glazing, blinds, blank walls and
      // subtle balcony rails while low-frequency noise breaks the flat box look.
      int cx=x>>5,cy=y>>5,bx=x&31,by=y&31;u32 cell=mat500_hash((u32)cx,(u32)cy,0x500facu);
      int low=(int)(mat500_hash((u32)(x>>4),(u32)(y>>4),0x51u)&15)-7;
      int fine=(int)(h&7)-3;int warm=(int)((cell>>9)&7);int blank=((cell>>4)&15)==0;
      int pane=!blank&&bx>=6&&bx<=25&&by>=6&&by<=24;
      int frame=!blank&&((bx>=4&&bx<=6)||(bx>=25&&bx<=27)||(by>=4&&by<=6)||(by>=24&&by<=26));
      int mullion=pane&&(((cell>>13)&1)?(bx==15||bx==16):(by==15||by==16));
      int sill=!blank&&by>=26&&by<=28&&bx>=4&&bx<=27;
      int balcony=((cell>>17)&7)==1&&by>=24&&by<=28&&bx>=2&&bx<=29;
      int joint=(bx==0||by==0);
      int r=139+low+fine+warm,g=142+low+fine+warm/2,b=143+low+fine;
      if(joint){r-=16;g-=16;b-=15;}
      if(pane){int glass=42+(int)((cell>>20)&19)+(by-6)/4;r=glass-5;g=glass+8;b=glass+20;}
      if(mullion||frame){r=167+low;g=169+low;b=168+low;}
      if(sill){r=116+low;g=118+low;b=118+low;}
      if(balcony){r=83+fine;g=87+fine;b=89+fine;}
      // A few bays carry light curtain/blind variation so towers no longer read
      // as a single stamped texture in daylight.
      if(pane&&((cell>>7)&7)==2&&((by-6)%5)<2){r+=20;g+=19;b+=15;}
      g_mat500_pixels[i]=(u8)mat500_clampi(r,0,255);g_mat500_pixels[i+1]=(u8)mat500_clampi(g,0,255);g_mat500_pixels[i+2]=(u8)mat500_clampi(b,0,255);
    }
  }
}

static void mat500_upload(u32 tex,int kind){
  if(!tex)return;mat500_fill(kind);glBindTexture(GL_TEXTURE_2D,tex);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,MAT500_SIZE,MAT500_SIZE,0,GL_RGB,GL_UNSIGNED_BYTE,g_mat500_pixels);
  int sw=MAT500_SIZE,sh=MAT500_SIZE,level=1;u8*src=g_mat500_pixels;u8*dst=g_mat500_mip;
  while(sw>1||sh>1){
    int dw=sw>1?sw>>1:1,dh=sh>1?sh>>1:1;
    for(int y=0;y<dh;y++)for(int x=0;x<dw;x++)for(int c=0;c<3;c++){
      int sx=x*2,sy=y*2,sx1=(sx+1<sw)?sx+1:sx,sy1=(sy+1<sh)?sy+1:sy;
      int a=src[(sy*sw+sx)*3+c],b=src[(sy*sw+sx1)*3+c],d=src[(sy1*sw+sx)*3+c],e=src[(sy1*sw+sx1)*3+c];
      dst[(y*dw+x)*3+c]=(u8)((a+b+d+e+2)>>2);
    }
    glTexImage2D(GL_TEXTURE_2D,level,GL_RGB,dw,dh,0,GL_RGB,GL_UNSIGNED_BYTE,dst);
    u8*tmp=src;src=dst;dst=tmp;sw=dw;sh=dh;level++;
  }
}
static void materials500_init(void){if(g_mat500_ready)return;g_mat500_ready=1;mat500_upload(g_tex_grass09,1);mat500_upload(g_tex_road09,2);mat500_upload(g_tex_build09,3);glBindTexture(GL_TEXTURE_2D,0);}
