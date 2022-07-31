#include <process.h>
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#include <math.h>
#include <alloc.h>

   int xmin,ymin,xmax,ymax,x,y,z,t,w,q,r,e,xx,yy,rr,graphdriver,graphmode;
   unsigned char c;
   int raio,angulo,tiro,disparo,angshot,explosao,pontos;
   char far *canhao[40];
   char far *alvo;

void main()
{
	 graphdriver = CGA;
	 graphmode = CGAC1;
	 initgraph(&graphdriver,&graphmode,"");
	 raio=12;
	 tiro=13;
	 pontos=0;
	 explosao=0;
	 for (angulo=15;angulo<176;angulo=angulo+4)
	 {
		delay(150);
		setfillstyle(SOLID_FILL,0);
		bar(145,169,171,197);
		setlinestyle(SOLID_LINE,0,NORM_WIDTH);
		rectangle(0,0,319,199);
		settextstyle(TRIPLEX_FONT,HORIZ_DIR,6);
		outtextxy(55,40,"HUNTING");
		outtextxy(108,90,"BALL");
		setcolor(2);
		setfillstyle(SOLID_FILL,2);
		bar(150,190,170,197);
		setcolor(3);
		arc(160,189,0,180,4);
		line(156,189,164,189);
		setfillstyle(SOLID_FILL,3);
		floodfill(160,188,3);
		setlinestyle(SOLID_LINE,0,THICK_WIDTH);
		setcolor(3);
		line(160,189,(160+(raio*cos(angulo*3.1415926 / 180))),(189+(-raio*sin(angulo*3.1415926 / 180))/1.1));
		canhao[(angulo-14)/4]=(char far*)farmalloc((unsigned long)
		imagesize(145,169,171,197));
		getimage(145,169,171,197,canhao[(angulo-14)/4]);
	 };
	 q = 1; w = 1; c = 0;
	 x=10; y=10;
	 pontos=0;
	 setlinestyle(SOLID_LINE,0,NORM_WIDTH);
	 setcolor(2);
	 circle(x,y,3);
	 setfillstyle(SOLID_FILL,2);
	 floodfill(x,y,2);
	 alvo=(char far*)farmalloc((unsigned long)imagesize(x-4,y-3,x+4,y+3));
	 getimage(x-4,y-3,x+4,y+3,alvo);
	 xmax = 316;      xmin = 4;
	 ymax = 198;      ymin = 2;
	 if (kbhit() != 0) c=getch();
	 while (kbhit()==0)
	 {
		putimage(x-4,y-3,alvo,OR_PUT);
		delay(10);
		putimage(x-4,y-3,alvo,COPY_PUT);
		x=x+q;  y=y+w;
		if ((x==xmax-3)|(x==xmin+3)|(getpixel(x+4,y)==3)|(getpixel(x-4,y)==3)) q=q*(-1);
		if ((y==ymax-3)|(y==ymin+3)|(getpixel(x,y+3)==3)|(getpixel(x,y-3)==3)) w=w*(-1);
	 };
comecar:
	 setcolor(3);
	 setlinestyle(SOLID_LINE,0,NORM_WIDTH);
	 cleardevice();
	 rectangle(2,2,318,198);
    q = 1; w = 1; c = 0;
    z = 0; t = 0;
    raio=12;
    angulo=90;
    tiro=13;
    disparo=0;
    randomize();
    x=random(290)+10;
    y=random(170)+10;
    for (rr=1;rr<1001;++rr)
    {
	xx = (random(60)*5)+15;
	yy = (random(40)*10)+20;
        line(xx,yy,xx+5,yy);
    };
    setcolor(2);
    putimage(145,169,canhao[(angulo-14)/4],COPY_PUT);
    setcolor(0);
    putimage(x-4,y-3,alvo,COPY_PUT);
    while (c != 27)
    {
      setcolor(2);
      putimage(x-4,y-3,alvo,OR_PUT);
      gotoxy(2,23);
      printf("%d\n",pontos);
      if (kbhit() != 0)
      {
	c=getch();
	if (c==27) exit(pontos);
      };
      if ((t < ymin)|(z < xmin)|(z > xmax))
	  {
	    disparo=0;
	    tiro=13;
	    setcolor(3);
	    rectangle(2,2,318,198);
	   };
      if ((c==32)|(disparo==1))
	{
	  setcolor(0);
	  if (c==32) { angshot=angulo; sound(440); delay(2); nosound(); };
	  circle(z,t,1);
	  z=(160+(tiro*cos(angshot*3.1415926 / 180)));
	  t=(189+(-tiro*sin(angshot*3.1415926 / 180))/1.1);
	  tiro=tiro+7;
	  setcolor(1);
	  circle(z,t,1);
	  disparo=1;
      };
      setcolor(0);
      putimage(145,169,canhao[(angulo-14)/4],COPY_PUT);
      if (c==75) angulo=angulo+4;
      if (c==77) angulo=angulo-4;
      if (angulo<15) angulo=15;
      if (angulo>165) angulo=165;
      putimage(145,169,canhao[(angulo-14)/4],COPY_PUT);
      c=0;
      setcolor(0);
      putimage(x-4,y-3,alvo,COPY_PUT);
      x=x+q;  y=y+w;
      if ((x==xmax-3)|(x==xmin+3)|(getpixel(x+4,y)==3)|(getpixel(x-4,y)==3)) q=q*(-1);
      if ((y==ymax-4)|(y==ymin+4)|(getpixel(x,y+3)==3)|(getpixel(x,y-3)==3)) w=w*(-1);
      if (getpixel(z,t)==2)
      {
	 for (explosao=0;explosao<12;explosao=explosao+2)
	 {
	    setcolor(3);
	    circle(x,y,explosao);
	    sound(70*explosao);
	    delay(2);
	    nosound();
	    delay(700);
	 };
         for (explosao=0;explosao<12;explosao=explosao+2)
	 {
	    setcolor(0);
	    circle(x,y,explosao);
	    sound(840+70*explosao);
	    delay(2);
	    nosound();
	    delay(700);
	 };
	 pontos=pontos+1;
	 goto comecar;
      };
    };
    closegraph();
}
/*
lib\bola.lib
bola2.c
*/