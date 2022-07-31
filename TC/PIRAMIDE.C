#include "graphics.h"
#include "conio.h"
#include "stdlib.h"
#include "math.h"

int gd,gm,x,y,i;
float z,t,q;
main()
{
   detectgraph(&gd,&gm);
	initgraph(&gd,&gm,"c:\\tc\\bgi");
   cleardevice();
   circle(260,20,50);
   setfillstyle(SOLID_FILL,3);
   setcolor(1);
   floodfill(260,20,15);
   setcolor(3);
   for (i=1;i < 500;i++)
      {
	 x=(random(640));
	 y=(random(200));
         putpixel(x,y,15);
      };
    for (y=0;y < 199;y++)
       {
          moveto(0,199);
          setcolor(0);
          lineto(320,y);
          lineto(619,199);
       };
    line(0,198,319,0);
    line(639,0,0,198);
    t=0;
    q=1;
    do
    {
    q = q * (1.1);
    setcolor(15);
    moveto(0,199);
    lineto(320,ceil(t));
    lineto(639,199);
    t=t+q;
    }
    while (t <= 199);
    line(0,199,639,199);
    z=0;
    q=1;
    do
    {
    q=q*1.1;
    line(320,0,ceil(z),199);
    line(320,0,ceil(640-z),199);
    z=z+q;
    }
    while (z <= 320);
    line(320,0,320,199);
    do i=kbhit(); while(i == 0);
    closegraph();
}