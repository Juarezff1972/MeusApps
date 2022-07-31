#include <dos.h>
#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>
/*------------------------------------------------------------------*/
void keyboard(void);
void quit_pgm(void);
void GraphicsSetup(void); void coords(void);

float sx=0,sy=0,xRes=0,yRes=0,sx1=0,sy1=0,sx2=0,sy2=0;
int   c0=0,c1=1,c2=2,c3=3,c4=4,c5=5,c6=6,c7=7,c8=8,c9=9,c10=10,
		c11=11,c12=12,c13=13,c14=14,c15=15,ModeFlag=0;
int   t=1,t1=1;
float x=52,y=32,x1=52,h1=80,v1=30,h2=81,v2=31,h3=90,v3=35;
int   bg=2,shadow=0,hilite=10;
/*------------------------------------------------------------------*/
void main(void)
{
GraphicsSetup();
bg=c2;shadow=c0;hilite=c10;
setfillstyle(SOLID_FILL,bg);
sx=0;sy=24;coords();sx1=sx;sy1=sy;
sx=639;sy=454;coords();sx2=sx;sy2=sy;
bar(sx1,sy1,sx2,sy2);
sx=619; sy=41;  coords(); moveto(sx,sy);
setcolor(hilite);
sx=619; sy=437; coords(); lineto(sx,sy);
sx=620; sy=41;  coords(); moveto(sx,sy);
sx=620; sy=437; coords(); lineto(sx,sy);
sx=19;  sy=437; coords(); lineto(sx,sy);
setcolor(shadow);
sx=19;  sy=41;  coords(); lineto(sx,sy);
sx=20;  sy=437; coords(); moveto(sx,sy);
sx=20;  sy=41;  coords(); lineto(sx,sy);
sx=620; sy=41;  coords(); lineto(sx,sy);

for (t1=1;t1<=4;t1++){
for (t=1 ;t <=6; t++){
moveto(x,y);      setcolor(hilite);  lineto(x+h1,y);
setcolor(shadow); moveto(x+h1,y+1);  lineto(x+h1,y+v1);
moveto(x+h2,y+1); lineto(x+h2,y+v1); lineto(x,y+v1);
setcolor(hilite); lineto(x,y);       moveto(x+1,y+v1);
lineto(x+1,y);    x=x+h3;};
x=x1;y=y+v3;};

keyboard();
quit_pgm();
}
/*------------------------------------------------------------------*/
void keyboard(void){
union u_type {int a;char b[3];} keystroke;
int get_keystroke(void);

do keystroke.a=get_keystroke();
while (keystroke.b[0]!=27);
}

int get_keystroke(void){
union REGS regs; regs.h.ah=0; return int86(0x16,&regs,&regs);}
/*------------------------------------------------------------------*/
void quit_pgm(void){
cleardevice();restorecrtmode();exit(0);}
/*------------------------------------------------------------------*/
void GraphicsSetup(void){
int GraphicsAdapter, GraphicsMode;
detectgraph(&GraphicsAdapter,&GraphicsMode);
if (GraphicsAdapter==VGA) goto VGAmode;
if (GraphicsAdapter==CGA) goto CGAmode;
if (GraphicsAdapter==EGAHI) goto EGA_ECDmode;
if (GraphicsAdapter==EGALO) goto EGA_SCDmode;
if (GraphicsAdapter==MCGA) goto MCGAmode;
goto AbortMessage;

VGAmode:
GraphicsAdapter=VGA; GraphicsMode=VGAHI;
initgraph(&GraphicsAdapter,&GraphicsMode,"");
xRes=640; yRes=480; ModeFlag=1; setcolor(c7);
x=52; y=77; x1=52; h1=80; v1=72; h2=81; v2=73; h3=90; v3=84;
outtextxy(0,472,"640x480 16-color VGA mode.");
outtextxy(160,0,"USING C TO CREATE A SCULPTURED BACKGROUND");
return;

EGA_ECDmode:
GraphicsAdapter=EGA; GraphicsMode=EGAHI;
initgraph(&GraphicsAdapter,&GraphicsMode,"c:\\tc\\bgi");
xRes=640; yRes=350; ModeFlag=2; setcolor(c7);
x=52; y=56; x1=52; h1=80; v1=53; h2=81; v2=54; h3=90; v3=61;
outtextxy(0,342,"640x350 16-color EGA mode.");
outtextxy(160,0,"USING C TO CREATE A SCULPTURED BACKGROUND");
return;

EGA_SCDmode:
GraphicsAdapter=EGA; GraphicsMode=EGALO;
initgraph(&GraphicsAdapter,&GraphicsMode,"c:\\tc\\bgi");
xRes=640; yRes=200; ModeFlag=3; setcolor(c7);
x=52; y=32; x1=52; h1=80; v1=30; h2=81; v2=31; h3=90; v3=35;
outtextxy(0,192,"640x200 16-color EGA mode.");
outtextxy(160,0,"USING C TO CREATE A SCULPTURED BACKGROUND");
return;

CGAmode:
GraphicsAdapter=CGA; GraphicsMode=CGAC3;
initgraph(&GraphicsAdapter,&GraphicsMode,"c:\\tc\\bgi");
xRes=320; yRes=200; ModeFlag=4; setcolor(c7);
x=26; y=32; x1=26; h1=40; v1=30; h2=41; v2=31; h3=45; v3=35;
c0=0; c2=2; c7=3; c10=3;
outtextxy(0,192,"320x200 4-color CGA mode.");
outtextxy(88,0,"SCULPTURED BACKGROUND");
return;

MCGAmode:
GraphicsAdapter=MCGA; GraphicsMode=MCGAC3;
initgraph(&GraphicsAdapter,&GraphicsMode,"c:\\tc\\bgi");
xRes=320; yRes=200; ModeFlag=4; setcolor(c7);
x=26; y=32; x1=26; h1=40; v1=30; h2=41; v2=31; h3=45; v3=35;
c0=0; c2=2; c7=3; c10=3;
outtextxy(0,192,"320x200 4-color MCGA mode.");
outtextxy(88,0,"SCULPTURED BACKGROUND");
return;

AbortMessage:
printf("\n\nUnable to proceed.\n");
printf("Requires VGA, EGA, MCGA or CGA adapter\n");
printf("   with appropriate monitor.\n");
exit(0);
}
/*------------------------------------------------------------------*/
void coords(void){
sx=sx*(xRes/640); sy=sy*(yRes/480); return; }
/*------------------------------------------------------------------*/