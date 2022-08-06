#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
///////////////////////////////////////////////////////////////////////
#define MaxFita 60
#define Coord 200,100,600,250
///////////////////////////////////////////////////////////////////////
char *Alfabeto="#abcdefghi";
char Fita [MaxFita];
char buffer[4000];
///////////////////////////////////////////////////////////////////////
unsigned ButtonNum;
typedef struct
{
   unsigned Button;
   unsigned x;
   unsigned y;
} Status;
unsigned int mousepointer[32] = {
  0xf07f, 0xc000, 0x8701, 0x8710, 0xc000, 0xb051, 0x8a80, 0x8000, 
  0x8011, 0x8000, 0x8010, 0x8001, 0x8010, 0x800f, 0xc01f, 0xf07f, 
  0x0f80, 0x3fff, 0x78fe, 0x78ef, 0x3fff, 0x4fae, 0x757f, 0x7fff, 
  0x7fee, 0x7fff, 0x7fef, 0x7ffe, 0x7fef, 0x7ff0, 0x3fe0, 0x0f80
};
/////////////////////////////////////////////////////////////
char InitMouse(void)
{
   _AX=0;
   geninterrupt(0x33);
   ButtonNum=_BX;
   return _AX;
}
/////////////////////////////////////////////////////////////
void ShowCursor(void)
{
   _AX=1;
   geninterrupt(0x33);
}
/////////////////////////////////////////////////////////////
void HideCursor(void)
{
   _AX=2;
   geninterrupt(0x33);
}
/////////////////////////////////////////////////////////////
Status GetStatus(void)
{
   Status aux;
   _AX=3;
   geninterrupt(0x33);
   aux.Button=_BX;
   aux.x=_CX;
   aux.y=_DX;
   return aux;
}
/////////////////////////////////////////////////////////////
void SetMouseShape( int spotx, int spoty, char *data )
{
  struct REGPACK reg;
  reg.r_ax = 0x09;
  reg.r_bx = spotx;
  reg.r_cx = spoty;
  reg.r_dx = FP_OFF( data );
  reg.r_es = FP_SEG( data );
  intr(0x33, &reg);
}
///////////////////////////////////////////////////////////////////////
void InicializaFita(void)
{
   char aux;
   unsigned i;
   randomize();
   for(i=0;i<=MaxFita;i++)
   {
     aux=random(strlen(Alfabeto));
//      aux=1;
      Fita[i]=Alfabeto[aux];
   }
}
///////////////////////////////////////////////////////////////////////
void Arquivo(void)
{
   void *buffer;
   int qqc;
   qqc=imagesize(Coord);
   buffer=malloc(qqc);
   getimage(Coord,buffer);
   setfillstyle(SOLID_FILL,MAGENTA);
   bar(Coord);
   setcolor(LIGHTBLUE);
   rectangle(Coord);
   setcolor(YELLOW);
   outtextxy(210,110,"O Arquivo, no momento, encontra-se");
   outtextxy(210,135,"no Almoxarifado...");
   getch();
   putimage(200,100,buffer,COPY_PUT);
   free(buffer);
}
///////////////////////////////////////////////////////////////////////
void Alfa(void)
{
   void *buffer;
   int qqc;
   qqc=imagesize(Coord);
   buffer=malloc(qqc);
   getimage(Coord,buffer);
   setfillstyle(SOLID_FILL,MAGENTA);
   bar(Coord);
   setcolor(LIGHTBLUE);
   rectangle(Coord);
   setcolor(YELLOW);
   outtextxy(210,110,"Depois do AlfaBeto vem o GamaDelto.");
   getch();
   putimage(200,100,buffer,COPY_PUT);
   free(buffer);
}
///////////////////////////////////////////////////////////////////////
void Estados(void)
{
   void *buffer;
   int qqc;
   qqc=imagesize(Coord);
   buffer=malloc(qqc);
   getimage(Coord,buffer);
   setfillstyle(SOLID_FILL,MAGENTA);
   bar(Coord);
   setcolor(LIGHTBLUE);
   rectangle(Coord);
   setcolor(YELLOW);
   outtextxy(210,110,"Os Estados do Sul s„o Paran  e");
   outtextxy(210,135,"Rio Grande do Sul. Santa Catarina");
   outtextxy(210,160,"‚ uma extens„o da Argentina!");
   getch();
   putimage(200,100,buffer,COPY_PUT);
   free(buffer);
}
///////////////////////////////////////////////////////////////////////
void Ajuda(void)
{
   void *buffer;
   int qqc;
   qqc=imagesize(Coord);
   buffer=malloc(qqc);
   getimage(Coord,buffer);
   setfillstyle(SOLID_FILL,MAGENTA);
   bar(Coord);
   setcolor(LIGHTBLUE);
   rectangle(Coord);
   setcolor(YELLOW);
   outtextxy(210,110,"O cursor ‚ um rolo de papel higiˆnico...");
   getch();
   putimage(200,100,buffer,COPY_PUT);
   free(buffer);
}
///////////////////////////////////////////////////////////////////////
int menu(void)
{
   void *item;
   unsigned Morango;
   Status rato;
   char c;
   rato=GetStatus();
   if (kbhit()) c=getch();
   if (c==27) return 1;
   if (rato.Button==1)
   {
      if ((rato.x>=714)&&(rato.x<=774))
      {
         if ((rato.y>=474)&&(rato.y<=504))
         {
            return 1;
         }
      }
      if ((rato.x>=704)&&(rato.x<=789))
      {
         Morango=imagesize(704,45,784,85);
         item=malloc(Morango);
         if ((rato.y>=45)&&(rato.y<=85))
         {
            HideCursor();
            getimage(704,45,784,85,item);
            putimage(704,45,item,NOT_PUT);
            Arquivo();
            putimage(704,45,item,COPY_PUT);
            ShowCursor();
         }
         if ((rato.y>=95)&&(rato.y<=135))
         {
            HideCursor();
            getimage(704,95,784,135,item);
            putimage(704,95,item,NOT_PUT);
            Alfa();
            putimage(704,95,item,COPY_PUT);
            ShowCursor();
         }
         if ((rato.y>=145)&&(rato.y<=185))
         {
            HideCursor();
            getimage(704,145,784,185,item);
            putimage(704,145,item,NOT_PUT);
            Estados();
            putimage(704,145,item,COPY_PUT);
            ShowCursor();
         }
         if ((rato.y>=195)&&(rato.y<=235))
         {
            HideCursor();
            getimage(704,195,784,235,item);
            putimage(704,195,item,NOT_PUT);
            Ajuda();
            putimage(704,195,item,COPY_PUT);
            ShowCursor();
         }
         free(item);
      }
   }
   return 0;
}
///////////////////////////////////////////////////////////////////////
int LerGrade(void)
{
   FILE *in;
   char *aux4;
   unsigned char *aux1;
   int i;
   signed long x,y;
   unsigned aux2,aux3;
   long maxx,maxy;
   char aux5[12];
   {
      if (!(in = fopen("turgrid2.bmp", "rb")) )
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
   }
   InicializaFita();
   strcpy(aux4,Alfabeto);aux4=strcat(aux4," }");aux4=strcat("ä = { ",aux4);
   setcolor(14);
   settextstyle(TRIPLEX_FONT,HORIZ_DIR,1);
   outtextxy(61,31,aux4);
   outtextxy(61,475,Fita);
   setcolor(0);
   aux4=0;
   for(i=0;i<=19;i++)
   {
      if (i<10)
      {
         itoa(i,aux4,10);
         outtextxy( 61,101+(18*i),aux4);
      }
      if (i>9)
      {
         itoa(i-10,aux4,10);
         outtextxy( 61,101+(18*i),aux4);
      }
      *aux1=Alfabeto[0];    outtextxy(181,101+(18*i),aux1);
      aux5[0]='(';      aux5[1]=' ';
      aux5[2]=*aux4;
      aux5[3]=',';
      aux5[4]=*aux1;
      aux5[5]=' ';      aux5[6]=')';
      aux5[7]=0;
      outtextxy(241,101+(18*i),aux5);
   }
   ShowCursor();
   while(menu()!=1);
   HideCursor();
   return 0;
}
///////////////////////////////////////////////////////////////////////
int huge detectEGA(void)
{
   int driver, mode, sugmode = 4;

   detectgraph(&driver, &mode);
   if (driver==9) return sugmode; else return grError;
}
///////////////////////////////////////////////////////////////////////
void Inicializa(void)
{
   int gdriver, gmode, errorcode;
   gdriver = installuserdriver("SVGA16", detectEGA);
   gdriver = DETECT;
   initgraph(&gdriver, &gmode, "");
}
///////////////////////////////////////////////////////////////////////
void main(void)
{
   int x,y;
   Inicializa();
   InitMouse();
   SetMouseShape(4,0,(char *)mousepointer);
   LerGrade();
   closegraph();
}