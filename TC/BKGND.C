#include <dos.h>
#include <stdio.h>
#include <graphics.h>

void keyboard(void);
void quit_pgm(void);
void graphics_setup(void);void coords(void);

float sx=0,sy=0,x_res=0,y_res=0,sx1=0,sy1=0,sx2=0,sy2=0;
int c0=0,c1=1,c2=2,c3=3,c4=4,c5=5,c6=6,c7=7,c8=8,c9=9,c10=10,c11=11,c12=12,c13=13,c14=14,c15=15,mode_flag=0;
int t=1,t1=1;
float x=52,y=32,x1=52,h1=80,v1=30,h2=81,v2=31,h3=90,v3=35;
int bg=2,shadow=0,hilite=10;

main(){
graphics_setup();

bg=c2;shadow=c0;hilite=c10;
sx=0;sy=24;coords();sx1=sx;sy1=sy;
sx=639,sy=454;coords();sx2=sx;sy2=sy;
bar(sx1,sy1,sx2,sy2);
sx=619;sy=41;coords();moveto(sx,sy);setcolor(hilite);
sx=619,sy=437;coords();lineto(sx,sy);
sx=620,sy=41;coords();moveto(sx,sy);
sx=620,sy=437;coords();lineto(sx,sy);
sx=19;sy=437;coords();lineto(sx,sy);
setcolor(shadow);sx=19;sy=41;coords();lineto(sx,sy);
sx=20;sy=437;coords();moveto(sx,sy);
sx=20;sy=41;coords();lineto(sx,sy);
sx=620,sy=41;coords();lineto(sx,sy);

for (t1=1;t1<=4;t1++){
for (t=1;t<=6;t++){
moveto(x,y);setcolor(hilite);lineto(x+h1,y);
moveto(x+h2,y+1);lineto(x+h2,y+v1);lineto(x,y+v1);
setcolor(hilite);lineto(x,y);moveto(x+1,y+v1);
lineto(x+1,y);x=x+h3;};
x=x1;y=y+v3;};

keyboard();
quit_pgm();
}

void keyboard(void){
union u_type {int a;char b[3];} keystroke;
int get_keystroke(void);

do keystroke.a=get_keystroke();
while (keystroke.b[0]!=27);
}

int get_keystroke(void){
union regs regs;regs.h.ah;return int86(0x16,&regs,&regs);}

void quit_pgm(void){
cleardevice();restorecrtmode();exit(0);}

void graphics_setup(void){
int graphics_adapter,graphics_mode;
detectgraph(&graphics_adapter,&graphics_mode);
if (graphics_adapter==CGA) goto cga_mode;
if (graphics_adapter==MCGA) goto mcga_mode;
goto abort_message;

cga_mode:
graphics_adapter=cga;graphics_mode=cgac3;
initgraph(&graphics_adapter,&graphics_mode,"");
x_res=320;y_res=200;c0=0;c2=2;c7=3;c10=3;mode_flag=4;setcolor(c7);
x=26;y=32;x1=26;h1=40;v1=30;h2=41;v2=31;h3=45;c3=35;
outtextxy(0,192,"320x200 4-color CGA mode.");
outtextxy(88,0,"SCULPTURED BACKGROUND");
return;

mcga_mode:
graphics_adapter=mcga;graphics_mode=mcgac3;
initgraph(&graphics_adapter,&graphics_mode,"");
x_res=320;y_res=200;c0=0;c2=2;c7=3;c10=3;mode_flag=4;setcolor(c7);
x=26;y=32;x1=26;h1=40;v1=30;h2=41;v2=31;h3=45;c3=35;
outtextxy(0,192,"320x200 4-color CGA mode.");
outtextxy(88,0,"SCULPTURED BACKGROUND");
return;

abort_message:
printf("\n\nUnable to proceed.\n");
printf("Requires CGA or MCGA adapter\n");
printf("   with appropriate monitor.\n");
exit(0);
}

void coords(void){
sx=sx*(x_res/640);sy=sy*(y_res/480);return;}
