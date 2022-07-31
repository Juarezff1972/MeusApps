#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <stdlib.h>
#include <math.h>
//////////////////////////////////////////////////////////////////////
unsigned ButtonNum;
typedef struct
{
   unsigned Button;
   unsigned x;
   unsigned y;
} Status;
void *up,*down,*left,*right;      // 0,2,3,1
void *Caminho;
/////////////////////////////////////////////////////////////
char InitMouse(void)   // Inicializa o Mouse, verifica se ta instalado
{                      // e quantos botoes tem
   _AX=0;
   geninterrupt(0x33);
   ButtonNum=_BX;
   return _AX;
}
/////////////////////////////////////////////////////////////
void ShowCursor(void)     // Mostra o cursor do mouse, nao importa
{                         // em qual modo de video esta
   _AX=1;
   geninterrupt(0x33);
}
/////////////////////////////////////////////////////////////
void HideCursor(void)           // Esconde o cursor do mouse
{
   _AX=2;
   geninterrupt(0x33);
}
/////////////////////////////////////////////////////////////
Status GetStatus(void)         // Retorna as coordenadas do mouse e
{                              // se tem 1 ou + botoes apertados e
   Status aux;                 // quais sao eles.
   _AX=3;
   geninterrupt(0x33);
   aux.Button=_BX;
   aux.x=_CX;
   aux.y=_DX;
   return aux;
}
/////////////////////////////////////////////////////////////
void Editar(void)              // Nao faz nada.
{
}
/////////////////////////////////////////////////////////////
void Found(char a)
{
   void *item;
   unsigned size;
   if (a==1)
   {
      size=imagesize(550,376,609,387);
      item=malloc(size);
      getimage(550,376,609,387,item);
      for(size=0;size<=5;size++)
      {
         putimage(550,376,item,NOT_PUT);
         delay(100);
         putimage(550,376,item,COPY_PUT);
         delay(100);
      }
   }
   if (a==2)
   {
      size=imagesize(540,393,621,404);
      item=malloc(size);
      getimage(540,393,621,404,item);
      for(size=0;size<=5;size++)
      {
         putimage(540,393,item,NOT_PUT);
         delay(100);
         putimage(540,393,item,COPY_PUT);
         delay(100);
      }
   }
   free(item);
}
/////////////////////////////////////////////////////////////
int Procurar(int x,int y,int dir)
{
   unsigned char aux;
   char rtncode=0;
   putimage((x-549)*10,(y-319)*10,Caminho,COPY_PUT);
   putpixel((x-549)*10+5,(y-319)*10+5,15);
   if (dir==0) putimage((x-549)*10,(y-319)*10,up,COPY_PUT);
   if (dir==1) putimage((x-549)*10,(y-319)*10,right,COPY_PUT);
   if (dir==2) putimage((x-549)*10,(y-319)*10,down,COPY_PUT);
   if (dir==3) putimage((x-549)*10,(y-319)*10,left,COPY_PUT);
   aux=getpixel(x+1,y);
   delay(50);
   if ((aux==0)||(aux==11))
   {
      if (aux==11) return 2;
      putpixel(x,y,1);
      putimage((x-549)*10,(y-319)*10,Caminho,COPY_PUT);
      putpixel((x-549)*10+5,(y-319)*10+5,15);
      rtncode=Procurar(x+1,y,1);
      if (rtncode==2) return rtncode;
      if (dir==0) putimage((x-549)*10,(y-319)*10,down,COPY_PUT);
      if (dir==1) putimage((x-549)*10,(y-319)*10,left,COPY_PUT);
      if (dir==2) putimage((x-549)*10,(y-319)*10,up,COPY_PUT);
      if (dir==3) putimage((x-549)*10,(y-319)*10,right,COPY_PUT);
   }
   aux=getpixel(x,y+1);
   if ((aux==0)||(aux==11))
   {
      if (aux==11) return 2;
      putpixel(x,y,1);
      putimage((x-549)*10,(y-319)*10,Caminho,COPY_PUT);
      putpixel((x-549)*10+5,(y-319)*10+5,15);
      rtncode=Procurar(x,y+1,2);
      if (rtncode==2) return rtncode;
      if (dir==0) putimage((x-549)*10,(y-319)*10,down,COPY_PUT);
      if (dir==1) putimage((x-549)*10,(y-319)*10,left,COPY_PUT);
      if (dir==2) putimage((x-549)*10,(y-319)*10,up,COPY_PUT);
      if (dir==3) putimage((x-549)*10,(y-319)*10,right,COPY_PUT);
   }
   aux=getpixel(x-1,y);
   if ((aux==0)||(aux==11))
   {
      if (aux==11) return 2;
      putpixel(x,y,1);
      putimage((x-549)*10,(y-319)*10,Caminho,COPY_PUT);
      putpixel((x-549)*10+5,(y-319)*10+5,15);
      rtncode=Procurar(x-1,y,3);
      if (rtncode==2) return rtncode;
      if (dir==0) putimage((x-549)*10,(y-319)*10,down,COPY_PUT);
      if (dir==1) putimage((x-549)*10,(y-319)*10,left,COPY_PUT);
      if (dir==2) putimage((x-549)*10,(y-319)*10,up,COPY_PUT);
      if (dir==3) putimage((x-549)*10,(y-319)*10,right,COPY_PUT);
   }
   aux=getpixel(x,y-1);
   if ((aux==0)||(aux==11))
   {
      if (aux==11) return 2;
      putpixel(x,y,1);
      putimage((x-549)*10,(y-319)*10,Caminho,COPY_PUT);
      putpixel((x-549)*10+5,(y-319)*10+5,15);
      rtncode=Procurar(x,y-1,0);
      if (rtncode==2) return rtncode;
      if (dir==0) putimage((x-549)*10,(y-319)*10,down,COPY_PUT);
      if (dir==1) putimage((x-549)*10,(y-319)*10,left,COPY_PUT);
      if (dir==2) putimage((x-549)*10,(y-319)*10,up,COPY_PUT);
      if (dir==3) putimage((x-549)*10,(y-319)*10,right,COPY_PUT);
   }
   putpixel(x,y,2);
   delay(50);
   putimage((x-549)*10,(y-319)*10,Caminho,COPY_PUT);
   return rtncode;
}
/////////////////////////////////////////////////////////////
int Menu(void)
{
   int x,y,z;
   char c;
   Status mouse;
   unsigned size;
   char item;
   int ratox=539,ratoy=60;
   void *Botoes;
   void *Parede,*Rato,*Saida;
   size=imagesize(539,30,549,40);
   Parede=malloc(size);   getimage(539,30,549,40,Parede);
   Caminho=malloc(size);  getimage(539,60,549,70,Caminho);
   Rato=malloc(size);     getimage(539,90,549,100,Rato);
   Saida=malloc(size);    getimage(539,120,549,130,Saida);
   c=0;
   ShowCursor();
   while (c!=27)
   {
      if (kbhit()) c=getch();
      mouse=GetStatus();
      if (mouse.Button==2)
      {
         for(x=51;x>=2;x--)
            for(y=45;y>=2;y--)
            {
               HideCursor();
               if (getpixel(x+549,y+319)==9) putimage(10*x,10*y,Parede,COPY_PUT);
               if (getpixel(x+549,y+319)==0) putimage(10*x,10*y,Caminho,COPY_PUT);
               if (getpixel(x+549,y+319)==15) putimage(10*x,10*y,Rato,COPY_PUT);
               if (getpixel(x+549,y+319)==11) putimage(10*x,10*y,Saida,COPY_PUT);
               ShowCursor();
            }
      }
      if (mouse.Button==1)
      {
         if ((mouse.x>=539)&&(mouse.x<=549))
         {
            if ((mouse.y>=30)&&(mouse.y<=40)) item='p';
            if ((mouse.y>=60)&&(mouse.y<=70)) item='c';
            if ((mouse.y>=90)&&(mouse.y<=100)) item='r';
            if ((mouse.y>=120)&&(mouse.y<=130)) item='s';
         }
         if ((mouse.x>=20)&&(mouse.x<=519))
         {
            if ((mouse.y>=20)&&(mouse.y<=459))
            {
               x=10*(mouse.x/10);
               y=10*(mouse.y/10);
               HideCursor();
               if (item=='p')
               {
                  putimage(x,y,Parede,COPY_PUT);
                  putpixel(x/10+549,y/10+319,9);
               }
               if (item=='c')
               {
                  putimage(x,y,Caminho,COPY_PUT);
                  putpixel(x/10+549,y/10+319,0);
               }
               if (item=='r')
               {
                  putimage(ratox,ratoy,Caminho,COPY_PUT);
                  if((ratox!=539)&&(ratoy!=60))
                     putpixel(ratox/10+549,ratoy/10+319,0);
                  putimage(x,y,Rato,COPY_PUT);
                  putpixel(x/10+549,y/10+319,15);
                  ratox=x;ratoy=y;
               }
               if (item=='s')
               {
                  putimage(x,y,Saida,COPY_PUT);
                  putpixel(x/10+549,y/10+319,11);
               }
               ShowCursor();
            }
         }
         if ((mouse.x>=535)&&(mouse.x<=623))
         {
            if ((mouse.y>=246)&&(mouse.y<=284))
            {
               c=27;
               HideCursor();
               size=imagesize(535,246,623,284);
               Botoes=malloc(size);
               getimage(535,246,623,284,Botoes);
               putimage(535,246,Botoes,NOT_PUT);
               free(Botoes);
               ShowCursor();
            }
            if ((mouse.y>=196)&&(mouse.y<=234))
            {
               HideCursor();
               size=imagesize(535,196,623,234);
               Botoes=malloc(size);
               getimage(535,196,623,234,Botoes);
               putimage(535,196,Botoes,NOT_PUT);
               Editar();
               delay(100);
               putimage(535,196,Botoes,COPY_PUT);
               free(Botoes);
               ShowCursor();
            }
            if ((mouse.y>=146)&&(mouse.y<=184))
            {
               HideCursor();
               size=imagesize(535,146,623,184);
               Botoes=malloc(size);
               getimage(535,146,623,184,Botoes);
               putimage(535,146,Botoes,NOT_PUT);
               size=imagesize(579,410,589,419);
               up=malloc(size);down=malloc(size);
               left=malloc(size);right=malloc(size);
               getimage(579,410,589,419,up);
               getimage(579,450,589,459,down);
               getimage(559,430,569,439,left);
               getimage(599,430,609,439,right);
               z=Procurar(ratox/10+549,ratoy/10+319,1);
               if (z==2) Found(1); else Found(2);
               free(up);free(down);free(left);free(right);
               delay(100);
               putimage(535,146,Botoes,COPY_PUT);
               free(Botoes);
               ShowCursor();
               return 1;
            }
         }
      }
   };
   HideCursor();
   free(Parede);
   free(Caminho);
   free(Rato);
   free(Saida);
   if (c==27) return 5;
   return 0;
}
/////////////////////////////////////////////////////////////
int LerGrade(void)
{
   FILE *in;
   unsigned char *aux1;
   int i;
   int x,y;
   int aux2,aux3;
   long maxx,maxy;
   do
   {
      HideCursor();
      if (!(in = fopen("labgrid2.bmp", "rb")) )
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
      {
         for(x=0;x<=maxx-1;x=x+2)
         {
            aux2=getc(in);
            aux3=(aux2/16);
            putpixel(x,y,aux3);
            aux3=aux2-aux3*16;
            putpixel(x+1,y,aux3);
         }
      }
      fclose(in);
      if (kbhit()) getc(stdin);
      ShowCursor();
   }
   while (Menu()!=5);
   for(y=0;y<=479;y++)
   {
      setcolor(0);
      line(0,y,639,y);
      delay(10);
   }
   return 0;
}
///////////////////////////////////////////////////////////////////////
int LerAbertura(void)
{
   FILE *in;
   unsigned char *aux1;
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
   {
      for(x=0;x<=maxx-1;x=x+2)
      {
         aux2=getc(in);
         aux3=(aux2/16);
         putpixel(x,y,aux3);
         aux3=aux2-aux3*16;
         putpixel(x+1,y,aux3);
      }
   }
   fclose(in);
   if (kbhit()) getc(stdin);
   setcolor(15);
   while (!kbhit());
   return 0;
}
///////////////////////////////////////////////////////////////////////
void Inicializa(void)
{
   int gdriver, gmode, errorcode;
   gdriver=DETECT;
   initgraph(&gdriver, &gmode, "");
}
/////////////////////////////////////////////////////////////////////
void main(void)
{
   int x,y;
   Inicializa();
   LerAbertura();
   InitMouse();
   LerGrade();
   closegraph();
}