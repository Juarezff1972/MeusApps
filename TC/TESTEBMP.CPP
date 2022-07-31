#include <stdio.h>
#include <string.h>
#include <graphics.h>
#include <conio.h>
#include <dos.h>
int main(void)
{
   FILE *in;
   char *aux1;
   int gd = DETECT, gm;
   int i;
   int x,y;
   int aux2,aux3;
   long maxx,maxy;
   if (!(in = fopen("anonym.bmp", "rb")) )
   {
      fprintf(stderr, "N„o posso abrir arquivo.\n");
      return 1;
   }
   for(i=1;i<=18;i++) getc(in);
   for(i=3;i>=0;i--) aux1[i]=getc(in);
   maxx=(aux1[2]*256+aux1[3])+(aux1[0]*256+aux1[1])*65536;
   for(i=3;i>=0;i--) aux1[i]=getc(in);
   maxy=(aux1[2]*256+aux1[3])+(aux1[0]*256+aux1[1])*65536;
   for(i=1;i<=28;i++) getc(in);
   initgraph(&gd,&gm,"");
   for(i=0;i<=15;i++)
   {
      aux1[1]=getc(in);
      aux1[2]=getc(in);
      aux1[3]=getc(in);
      if (i<=7) setrgbpalette(i,aux1[3]/4,aux1[2]/4,aux1[1]/4);
      if (i==8) setrgbpalette(20,aux1[3]/4,aux1[2]/4,aux1[1]/4);
      if (i>=9) setrgbpalette(48+i,aux1[3]/4,aux1[2]/4,aux1[1]/4);
      getc(in);
   }
   for(y=maxy-1;y>=0;y--)
      for(x=0;x<=maxx-1;x=x+2)
      {
         aux2=getc(in);
         aux3=(aux2/16);
         putpixel(x,y,aux3);
         aux3=(aux2-aux3)/16;
         putpixel(x+1,y,aux3);
      }
   fclose(in);
   if (kbhit()) getc(stdin);
   while (!kbhit());
   return 0;
}
