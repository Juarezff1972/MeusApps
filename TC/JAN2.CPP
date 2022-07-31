#include <conio.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>
/////////////////////////////////////////////////////////////////////
float mat[7][8];
float x[7];
int i,j,k;
unsigned n=7,m=6;   // 7, 6
float c;
/////////////////////////////////////////////////////////////////////
void Cursor(unsigned inicio,unsigned fim)
{
   asm{
      mov    dx,0x3D4
      mov    ax,0xA
      out    dx,ax
      inc    dx
      mov    ax,inicio
      out    dx,ax
      dec    dx
      mov    ax,0xB
      out    dx,ax
      inc    dx
      mov    ax,fim
      out    dx,ax
   }
}
////////////////////////////////////////////////////////////////////
void sombra(int x1,int y1,int x2,int y2)
{
   int x,y;
   char z,t;
   for(x=x1;x<=(x2-3);x++)
   {
		gotoxy(x,y2);
      _AH=8;
      geninterrupt(0x10);
      z=_AH;
      t=_AL;
      if ((z==8)||(z==0)) z=0; else z=8;
      if (t==219) z=0;
      _CX=1;
      _BX=z;
      _AL=t;
      _AH=9;
      geninterrupt(0x10);
   }
   for(x=(x1-2);x<=(x1-1);x++)
      for(y=(y1+1);y<=y2;y++)
      {
         gotoxy(x,y);
         _AH=8;
         geninterrupt(0x10);
         z=_AH;
         t=_AL;
         if ((z==8)||(z==0)) z=0; else z=8;
         if (t==219) z=0;
         _CX=1;
         _BX=z;
         _AL=t;
         _AH=9;
         geninterrupt(0x10);
      }
}

////////////////////////////////////////////////////////////////////
void resolve(void)
{
 for(k=0;k<m-1;k++)
  {
   i=k+1;
   while(i!=m)
    {
     c=(mat[i][k]/mat[k][k]);
     for(j=0;j<n;j++)  mat[i][j]=(mat[i][j]-(c*mat[k][j]));
     i=i+1;
    }
  }
}
//////////////////////////////////////////////////////////////////////////
void resposta(void)
{
 float sum[11];
 int i,j;
 for(i=0;i<10;i++) sum[i]=0;
 for(j=0;j<11;j++) x[j]=0;
 for(i=m;i>0;i--)
 {
  for(j=n;j>0;j--)
  {
   sum[i-1]=sum[i-1]+(mat[i-1][j-1]*x[j-1]);
  }
  x[i-1]=(mat[i-1][m]-sum[i-1])/mat[i-1][i-1];
 }
}
//////////////////////////////////////////////////////////////////////
float LeTeclado(int x,int y)
{
   char c=0;
   char i;
   char aux[13];
   float retval;
trator:
   ;
   aux[0]='+';
   for(i=1;i<=13;i++) aux[i]=0;
   i=1;
   while(c!=13)
   {
      c=getch();
      if (c==44) c=46;
      if (((c>=48)&&(c<=57))||(c==46)) {  aux[i]=c; i++; }
      if (c==45) aux[0]='-';
      if (c==43) aux[0]='+';
      if (c==8)  { aux[i-1]=0; i--; gotoxy(x,y); cprintf("            "); }
      if (i>11) break;
      gotoxy(x,y); cprintf("%s",aux);
   }
   aux[i]=0;
   retval=atof(aux);
   if (fabs(retval)>=1000.0)
   {
      gotoxy(x,y); cprintf("            "); gotoxy(x,y);
      c=0;
      goto trator;
   }
   return retval;
}
//////////////////////////////////////////////////////////////////////
void EscreveMatriz(signed char x,signed char y)
{
   char buffer[4000];
   float aux;
   gettext(2,2,66,12,buffer);
   textbackground(LIGHTGRAY);
   textcolor(YELLOW);
   sombra(21,6,53,9);
   gotoxy(21,6);   cprintf("ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");         // Ú 218
   gotoxy(21,8);   cprintf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");         // Ä 196
   gotoxy(53,6);   cprintf("¿");         // À 192  ¿ 191
   gotoxy(53,8);   cprintf("Ù");         // Ù 217
   gotoxy(21,7);   cprintf("³");    // ³ 179
   gotoxy(53,7);   cprintf("³");
   gotoxy(22,7);
   cprintf("Entre com o valor:             ");
   gotoxy(41,7);
   Cursor(14,15);
   aux=LeTeclado(41,7);
   Cursor(0x19,0x00);
   mat[x][y]=aux;
   puttext(2,2,66,12,buffer);
   textbackground(BLUE);
   textcolor(YELLOW);
}
//////////////////////////////////////////////////////////////////////
void DrawMatrix(char n,char m)
{
   int i;
   signed char x,y;
   gotoxy(1,1);   cprintf("ÚÄÄ");         // Ú 218
   gotoxy(1,(2*m)+1);  cprintf("ÀÄÄ");         // Ä 196
   gotoxy((9*n)+4,1);  cprintf("ÄÄ¿");         // À 192  ¿ 191
   gotoxy((9*n)+4,(2*m)+1); cprintf("ÄÄÙ");         // Ù 217
   for(i=2;i<=(2*m);i++)
   {
      gotoxy(1,i);      cprintf("³");    // ³ 179
      gotoxy((9*n)+6,i);     cprintf("³");
   }
   for(x=0;x<=n-1;x++)
      for(y=0;y<=m-1;y++)
      {
         mat[x][y]=0.0;
	      gotoxy(5+(9*x),2+(2*y));
	      cprintf("%.3f",mat[x][y]);
      }
}
//////////////////////////////////////////////////////////////////////
void main(void)
{
   int i;
   char c;
   signed char x,y,x1,y1;
   Cursor(0x19,0x00);
   textbackground(BLACK);
   clrscr();
   window(6,3,75,15);
   textbackground(BLUE);
   textcolor(YELLOW);
   clrscr();
   DrawMatrix(n,m);
   c=0;x=0;y=0;
   while(c!=27)
   {
      gotoxy(5+(9*x),2+(2*y));
	   cprintf("        ");
      gotoxy(5+(9*x),2+(2*y));
      textbackground(RED);
      textcolor(YELLOW);
      cprintf("%.3f",mat[x][y]);
      textbackground(BLUE);
      textcolor(YELLOW);
      c=getch();
      if (c==0) c=getch();
      for(x1=0;x1<=n-1;x1++)
      for(y1=0;y1<=m-1;y1++)
      {
	      gotoxy(5+(9*x1),2+(2*y1));
	      cprintf("%.3f",mat[x1][y1]);
      }
      switch(c)
      {
	      case 72 : y--; if (y<0) y=m-1; break;
	      case 80 : y++; if (y>m-1) y=0; break;
	      case 75 : x--; if (x<0) x=n-1; break;
	      case 77 : x++; if (x>n-1) x=0; break;
	      case 13 : EscreveMatriz(x,y);break;
	      case 67 : resolve();resposta();break;
      }
   }
   window(1,1,79,24);
   Cursor(14,15);
}
