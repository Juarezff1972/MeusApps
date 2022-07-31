#include <graphics.h>
#include <dos.h>
#include <mem.h>
#include <conio.h>
///////////////////////////////////////////////////////////////////////
void MainMenu(void)
{
   setfillstyle(SOLID_FILL,7);
   bar(0,0,getmaxx(),10);
   bar(0,getmaxy()-10,getmaxx(),getmaxy());
   setcolor(0);
   outtextxy(1,1," -  Arquivo  Op‡”es  Sair                                                 Ajuda");
   setcolor(4);
   outtextxy(1,1," -  A        O       S                                                    A    ");
}
///////////////////////////////////////////////////////////////////////
int huge DetectVideo(void)
{
   int driver, mode, sugmode = 3;
   detectgraph(&driver, &mode);
   if (driver==9) return sugmode; else return grError;
}
///////////////////////////////////////////////////////////////////////
void Inicializa(void)
{
   int gdriver, gmode, errorcode;
   gdriver = installuserdriver("SVGA256", DetectVideo);
   gdriver = DETECT;
   initgraph(&gdriver, &gmode, "c:\\borlandc\\bgi");
}
///////////////////////////////////////////////////////////////////////
void main(void)
{
   unsigned int c;
   Inicializa();
   for(c=16;c<=24;c++)
   {
      setcolor(c);
      line((c-16),getmaxy()-(c-6),getmaxx()-(c-16),getmaxy()-(c-6));
      line(getmaxx()-(c-16),10+(c-16),getmaxx()-(c-16),getmaxy()-(c-6));
      setcolor(32-(c-16));
      line((c-16),getmaxy()-(c-6),(c-16),10+(c-16));
      line((c-16),10+(c-16),getmaxx()-(c-16),10+(c-16));
   }
   setfillstyle(SOLID_FILL,24);
   bar(8,18,getmaxx()-8,getmaxy()-18);
   MainMenu();
   while(!kbhit());
}
