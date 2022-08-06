#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <malloc.h>

#include "vars.h"
#include "bin.h"
#include "linha.h"
#include "coords.h"
#include "curva.h"
#include "figura.h"
/////////////////////////////////////////////////////////////
void poligono0(unsigned x,unsigned y,unsigned lados,
               unsigned raio,unsigned cor)
{
   float x1[15],y1[15];
   float ad,ar;
   unsigned i,j,k;
   if ((lados<3) || (lados>15)) return;
   for(i=1;i<=lados;i++)
   {
      ad=i*360.0/lados;
      ar=ad*pi/180.0;
      x1[i]=raio*cos(ar);
      y1[i]=raio*sin(ar);
      y1[i]=-y1[i]/asp;
   }
   for(j=1;j<=lados-1;j++)
      for(k=j+1;k<=lados;k++)
         linha(x+x1[j],y+y1[j],x+x1[k],y+y1[k],cor);
}
//////////////////////////////////////////////////////////////
void demorp(void)
{
   unsigned i;
   unsigned mx,my;
   mx=(XRes)/2; my=(YRes)/2;
   for(i=0;i<=XRes;i++)
   {
      linha(mx,my,i,0,random(MaxCor)+1);
      linha(mx,my,i,YRes,random(MaxCor)+1);
   }
   for(i=0;i<=YRes;i++)
   {
      linha(mx,my,0,i,random(MaxCor)+1);
      linha(mx,my,XRes,i,random(MaxCor)+1);
   }
   getch();
}
//////////////////////////////////////////////////////////////
void demori(void)
{
   while(!kbhit())
      poligono0(random(XRes),random(YRes),
                random(15),random(250),random(MaxCor)+1);
   getch();
}
//////////////////////////////////////////////////////////////
void demorpm(void)
{
   int ix,iy,iix,iiy;
   int x1,x2,y1,y2;
   ix=1;
   iy=1;
   iix=1;
   iiy=1;
   x1=random(XRes);x2=random(XRes);
   y1=random(YRes);y2=random(YRes);
   while(!kbhit())
   {
      linha(x1,y1,x2,y2,random(MaxCor)+1);
      x1=x1+ix;  y1=y1+iy;
      x2=x2+iix; y2=y2+iiy;
      if (x1<=0)   ix=1;  if (x2<=0)   iix=1;
      if (y1<=0)   iy=1;  if (y2<=0)   iiy=1;
      if (x1>XRes) ix=-1; if (x2>XRes) iix=-1;
      if (y1>YRes) iy=-1; if (y2>YRes) iiy=-1;
   }
   getch();
}
//////////////////////////////////////////////////////////////
void democi(void)
{
   unsigned x1,y1,i,ii;
   x1=30;y1=30;i=1;ii=1;
   while(!kbhit())
   {
      circulo(x1,y1,30,random(MaxCor)+1);
      x1=x1+i;y1=y1+ii;
      if (x1<30) i=random(3);
      if (x1>(XRes)+30) i=-random(3);
      if (y1<30) ii=random(3);
      if (y1>(YRes)+30) ii=-random(3);
   }
   getch();
}
//////////////////////////////////////////////////////////////
void democpm(void)
{
   unsigned x1,y1,i,ii;
   while(!kbhit())
   {
      ii=random(MaxCor)+1;x1=random(XRes);y1=random(YRes);
      for(i=1;i<=random(150);i++) circulo1(x1,y1,i,ii);
   }
   getch();
}
//////////////////////////////////////////////////////////////
void demoac(void)
{
   struct time t;
   unsigned i;
   unsigned mx=(XRes)/2,my=(YRes)/2;
   gettime(&t);
   while(!kbhit())
   {
      for (i=1;i<=t.ti_hour*10;i++)
      {
         gettime(&t);
         arco(mx,my,i,t.ti_min*6,t.ti_sec*6,t.ti_hund);
      }
   }
   getch();
}
//////////////////////////////////////////////////////////////
void demosc(void)
{
   unsigned n[15],i,ii,iii,t;
   unsigned mx=(XRes)/2,my=(YRes)/2;
   while(!kbhit())
   {
      for (i=0;i<15;i++) {n[i]=random(360);}
      for(i=0;i<15;i=i+2)
      {
         t=random(150);
         iii=random(MaxCor)+1;
         for(ii=1;ii<=t;ii++)
            setorcirc(mx,my,ii,n[i],n[i+1],iii);
      }
   }
   getch();
}
//////////////////////////////////////////////////////////////
void demoepm(void)
{
   unsigned x1,y1,r,i;
   while(!kbhit())
   {
      x1=random(XRes);
      y1=random(YRes);
      r=random(YRes/2);
      for(i=1;i<=r;i=i+1) elipse1(x1,y1,i,r-i+1,random(MaxCor)+1);
   }
   getch();
}
//////////////////////////////////////////////////////////////
void demose(void)
{
   while(!kbhit())
   {
      setorelip(random(XRes),random(YRes),random(YRes),
                random(YRes),random(360),random(360),random(MaxCor)+1);
   }
   getch();
}
//////////////////////////////////////////////////////////////
void demomarca(void)
{
   unsigned char i;
   unsigned mx=(XRes)/8;
   unsigned my=(YRes)/5;
   unsigned my1=my,my2=2*my,my3=3*my,my4=4*my;
   while(!kbhit())
   {
      for (i=1;i<=7;i++)
      {
         desenha_marca(i*mx,my1,cruz    ,5*i,random(MaxCor)+1);
         desenha_marca(i*mx,my2,quadrado,5*i,random(MaxCor)+1);
         desenha_marca(i*mx,my3,xis     ,5*i,random(MaxCor)+1);
         desenha_marca(i*mx,my4,estrela ,5*i,random(MaxCor)+1);
      }
   }
   getch();

}
//////////////////////////////////////////////////////////////
void demopr(void)
{
   unsigned i,ii,iii;
   unsigned mx=(XRes)/2,my=(YRes)/2;
   while(!kbhit())
   {
      ii=random(MaxCor)+1;
      iii=random(15);
      for(i=1;i<=(YRes/2);i++) poligono_regular(mx,my,iii,i,ii);
   }
   getch();
}
//////////////////////////////////////////////////////////////
void demopoli(void)
{
   unsigned char i,ii,iii;
   while(!kbhit())
   {
      ii=random(20)+3;
      for(i=0;i<=ii-1;i++)
      {
         pline[i].x=random(XRes);
         pline[i].y=random(YRes);
      }
      numpoli=ii;
      iii=random(2);
      polilinha(numpoli,pline,iii,random(MaxCor)+1);
      delay(1000);
      polilinha(numpoli,pline,iii,0);
   }
   getch();
}
//////////////////////////////////////////////////////////////
void demobezi(void)
{
   unsigned char i,ii;
   while(!kbhit())
   {
      ii=random(4);
      for(i=0;i<=ii;i++)
      {
         pline[i].x=random(XRes);
         pline[i].y=random(YRes);
      }
      numpoli=ii;
      polilinha(numpoli,pline,0,random(MaxCor)+1);
      bezier(numpoli,pline,random(MaxCor));
      delay(1000);
      polilinha(numpoli,pline,0,0);
      bezier(numpoli,pline,0);
   }
   getch();
}
//////////////////////////////////////////////////////////////
void demospline(void)
{
   unsigned char i,ii;
   while(!kbhit())
   {
      ii=random(20)+3;
      for(i=0;i<=ii;i++)
      {
         pline[i].x=random(XRes);
         pline[i].y=random(YRes);
      }
      numpoli=ii;
      polilinha(numpoli,pline,0,random(MaxCor)+1);
      spline(numpoli,pline,random(MaxCor)+1);
      delay(1000);
      polilinha(numpoli,pline,0,0);
      spline(numpoli,pline,0);
   }
   getch();
}
//////////////////////////////////////////////////////////////
void demopmarca(void)
{
   unsigned char i,ii,iii;
   while(!kbhit())
   {
      ii=random(20)+3;
      for(i=0;i<=ii-1;i++)
      {
         pline[i].x=random(XRes);
         pline[i].y=random(YRes);
      }
      numpoli=ii;
      iii=random(2);
      polimarca(numpoli,pline,iii,quadrado,10,random(MaxCor)+1);
      delay(1000);
      polimarca(numpoli,pline,iii,quadrado,10,0);
   }
   getch();
}
//////////////////////////////////////////////////////////////
void main(void)
{
   unsigned x1,y1,x2,y2;
   unsigned i,ii;
   unsigned modo=0x105;
   vesa();
   mudamodo(modo);
   randomize;
   setcoord(XMIN,YMIN,XMAX,YMAX);
   retangulo(0,0,XRes,YRes,15); demorp();
   viewport(0,0,XRes/2,YRes/2);      limpatela(); retangulo(0,0,XRes,YRes,15); demori();
   viewport(XRes/2,0,XRes,YRes/2);   limpatela(); retangulo(0,0,XRes,YRes,15); demorpm();
   viewport(0,YRes/2,XRes/2,YRes);   limpatela(); retangulo(0,0,XRes,YRes,15); democi();
   viewport(XRes/2,YRes/2,XRes,YRes);limpatela(); retangulo(0,0,XRes,YRes,15); democpm();
   viewport(0,0,XRes/2,YRes/2);      limpatela(); retangulo(0,0,XRes,YRes,15); demoac();
   viewport(XRes/2,0,XRes,YRes/2);   limpatela(); retangulo(0,0,XRes,YRes,15); demoepm();
   viewport(0,YRes/2,XRes/2,YRes);   limpatela(); retangulo(0,0,XRes,YRes,15); demose();
   viewport(XRes/2,YRes/2,XRes,YRes);limpatela(); retangulo(0,0,XRes,YRes,15); demosc();
   viewport(0,0,XRes,YRes);
   limpatela(); retangulo(0,0,XRes,YRes,15); demomarca();
   limpatela(); retangulo(0,0,XRes,YRes,15); demopr();
   limpatela();
   retangulo(0,0,XRes,YRes,15); demopoli();
   retangulo(0,0,XRes,YRes,15); demobezi();
   retangulo(0,0,XRes,YRes,15); demospline();
   retangulo(0,0,XRes,YRes,15); demopmarca();
   mudar(0x3);
   printf("Termino Normal");
}
