#include <stdio.h>
#include <graphics.h>
#include <process.h>
#include <bios.h>
#include <alloc.h>
#include <dos.h>

void keyboard(void);
void quitpgm(void);
void graphicssetup(void);
void noise(int hertz,int duration);

int t1=1,t2=1,x1=220,y1=100,x2=270,y2=120,sx=220,sy=100,sxmove=3,
    symove=-1,sx1,sy1;
int c0=0,c1=1,c2=2,c3=3,c4=4,c5=5,c6=6,c7=7,c8=8,c9=9,c10=10,c11=11,
    c12=12,c13=13,c14=14,c15=15,modeflag=0;
int hz=450;
char far *gr_array1;

main()
{

  graphicssetup();
  setcolor(c7);
  circle(x1+25,y1+10,22);
  setfillstyle(SOLID_FILL,c4);
  floodfill(x1+25,y1+10,c7);
  gr_array1=(char far*)farmalloc((unsigned long)imagesize(x1,y1,x2,y2));
  getimage(x1,y1,x2,y2,gr_array1);
  setfillstyle(SOLID_FILL,c0);
  bar(x1,y1,x2,y2);
  setcolor(c7);
  rectangle(0,10,639,189);
  moveto(319,188);
  lineto(319,110);
  putimage(sx,sy,gr_array1,COPY_PUT);
  animate:
  if (sx1<=268) goto leftarena;
  if (sx1>=320) goto rightarena;
  goto inbetween;

  leftarena:
  if (sx1>=266) {
    if (sy1>=81) {
      noise(hz,300);
      sxmove=-3;
    }
  }
  if (sx1<=3) {
    noise(hz,300);
    sxmove=3;
  }
  if (sy1>=168) {
    noise(hz,300);
    symove=-1;
  }
  if (sy1<=11) {
    noise(hz,300);
    symove=1;
  }
  sx1=sx+sxmove;
  sy1=sy+symove;
  putimage(sx1,sy1,gr_array1,COPY_PUT);
  sx=sx1;
  sy=sy1;
  keyboard();
  goto animate;

  rightarena:
  if (sx1>=586) {
    noise(hz,300);
    sxmove=-3;
  }
  if (sx1<=322) {
      if (sy1>=81){
      noise(hz,300);
      sxmove=3;
      }
  }
  if (sy1>=168) {
    noise(hz,300);
    symove=-1;
  }
  if (sy1<=11) {
    noise(hz,300);
    symove=1;
  }
  sx1=sx+sxmove;
  sy1=sy+symove;
  putimage(sx1,sy1,gr_array1,COPY_PUT);
  sx=sx1;
  sy=sy1;
  keyboard();
  goto animate;

  inbetween:
  if (sy1<=11) {
    noise(hz,300);
    symove=1;
  }
  if (sy1>=89) {
    noise(hz,300);
    symove=-1;
  }
  sx1=sx+sxmove;
  sy1=sy+symove;
  putimage(sx1,sy1,gr_array1,COPY_PUT);
  sx=sx1;
  sy=sy1;
  keyboard();
  goto animate;
  quitpgm();
  }

  void keyboard(void)
  {
    union u_type {
      int a;
      char b[3];
    }
    keystroke;
    char inkey=0;
    if (bioskey(1)==0) return;
    keystroke.a=bioskey(0);
    inkey=keystroke.b[0];
    switch (inkey) {
      case 27  : quitpgm();
      case 46  : sxmove=3; return;
      case 44  : sxmove=-3; return;
      case 97  : symove=-1; return;
      case 122 : symove=1; return;
      default  : return;
    }
  }

  void quitpgm(void)
  {
    cleardevice();
    restorecrtmode();
    exit(0);
  }
  void noise(int hertz,int duration)
  {
    int t1=1,highbyte=0,lowbyte=0;
    short count=0;
    unsigned char oldport=0,newport=0;
    if (hertz<40) return;
    if (hertz>4660)return;
    count=1193180L/hertz;
    highbyte=count/256;
    lowbyte=count-(highbyte*256);
    outportb(0x43,0xb6);
    outportb(0x42,lowbyte);
    outportb(0x42,highbyte);
    oldport=inportb(0x61);
    newport=(oldport | 0x03);
    outportb(0x61,newport);
    for (t1=1;t1<=duration;t1++);
    outportb(0x61,oldport);
    return;
  }
  void graphicssetup(void)
  {
    int adapter,mode;
    detectgraph(&adapter,&mode);
    goto cgamode;
    cgamode:
    adapter=CGA;
    mode=CGAHI;
    initgraph(&adapter,&mode,"");
    t1=1;t2=1;x1=220;y1=100;x2=270;y2=120;sx=220;sy=100;sxmove=3;
    symove=-1;sx1;sy1;
    hz=450;
    c0=0;c1=1;c2=1;c3=1;c4=1;c5=1;c6=1;c7=1;c8=1;c9=1;c10=1;c11=1;
    c12=1;c13=1;c14=1;c15=1;modeflag=4;
    setcolor(c7);
    return;
  }
