#include "graphics.h"
#include "stdio.h"
#include "stdlib.h"
#include "conio.h"
#include "time.h"
#include "dos.h"

	int xmin,ymin,xmax,ymax,x,y;
	int z,t,w,q,r,e,xx,yy,rr;
	int graphdriver,graphmode;

main()
{
	 graphdriver = CGA;
	 graphmode = CGAC0;
	 initgraph(&graphdriver,&graphmode,"");
	 cleardevice();
	 rectangle(2,2,318,198);
	 xmax = 318;      xmin = 2;
	 ymax = 198;      ymin = 2;
	 x = 3;   y = 3;
	 z = -57; t = -57;
	 q = 1; w = 1; e = 1; r = 1;
	 randomize();
	 for (rr=1;rr<=700;rr++)
	 {
		  xx = (random(60)*5)+5;
		  yy = (random(40)*5)+5;
		  line(xx,yy,xx+5,yy);
	 };
	 do {
	 delay(4);
	 if ((z<0) && (t<0))  {  e = 1; r = 1; };
	 if ((z==1) && (t==1))  {  z = 3; t = 3; };

	 putpixel(z,t,0);
	 putpixel(x,y,2);
	 x=x+q;  y=y+w;
	 z=z+e;  t=t+r;
	 if ((z==xmax)||(z==xmin)||(getpixel(z+1,t)==3)||(getpixel(z-1,t)==3)) e=e*(-1);
	 if ((x==xmax)||(x==xmin)||(getpixel(x+1,y)==3)||(getpixel(x-1,y)==3)) q=q*(-1);
	 if ((t==ymax)||(t==ymin)||(getpixel(z,t+1)==3)||(getpixel(z,t-1)==3)) r=r*(-1);
	 if ((y==ymax)||(y==ymin)||(getpixel(x,y+1)==3)||(getpixel(x,y-1)==3)) w=w*(-1);
	 }
	 while (kbhit()==0);
closegraph();
}
