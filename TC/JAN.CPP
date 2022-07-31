#include <conio.h>
#include <stdio.h>
#include <ctype.h>
/////////////////////////////////////////////////////////////////////
float mat[7][8];
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
//////////////////////////////////////////////////////////////////////
void EscreveMatriz(signed char x,signed char y)
{
   char buffer[4000];
   float aux;
   gettext(2,2,66,12,buffer);
   gotoxy(21,6);   cprintf("ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");         // Ú 218
   gotoxy(21,8);   cprintf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");         // Ä 196
   gotoxy(46,6);   cprintf("¿");         // À 192  ¿ 191
   gotoxy(46,8);   cprintf("Ù");         // Ù 217
   gotoxy(21,7);   cprintf("³");    // ³ 179
   gotoxy(46,7);   cprintf("³");
   gotoxy(22,7);
   cprintf("Entre com o valor: ");
//   scanf("%f",&aux);
   cscanf("%10f",&aux);
   mat[x][y]=aux;
   puttext(2,2,66,12,buffer);
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
   gotoxy(1,1);   cprintf("ÚÄÄ");         // Ú 218
   gotoxy(1,13);  cprintf("ÀÄÄ");         // Ä 196
   gotoxy(67,1);  cprintf("ÄÄ¿");         // À 192  ¿ 191
   gotoxy(67,13); cprintf("ÄÄÙ");         // Ù 217
   for(i=2;i<=12;i++)
   {
      gotoxy(1,i);      cprintf("³");    // ³ 179
      gotoxy(69,i);     cprintf("³");
   }
   for(x=0;x<=6;x++)
      for(y=0;y<=5;y++)
      {
         mat[x][y]=0.0;
         gotoxy(5+(9*x),2+(2*y));
         cprintf("%6.5f",mat[x][y]);
      }
   c=0;x=0;y=0;
   while(c!=27)
   {
      gotoxy(5+(9*x),2+(2*y));
      textbackground(RED);
      textcolor(YELLOW);
      cprintf("%6.5f",mat[x][y]);
      textbackground(BLUE);
      textcolor(YELLOW);
      c=getch();
      if (c==0) c=getch();
      for(x1=0;x1<=6;x1++)
      for(y1=0;y1<=5;y1++)
      {
         gotoxy(5+(9*x1),2+(2*y1));
         cprintf("%6.5f",mat[x1][y1]);
      }
      switch(c)
      {
         case 72 : y--; if (y<0) y=5; break;
         case 80 : y++; if (y>5) y=0; break;
         case 75 : x--; if (x<0) x=6; break;
         case 77 : x++; if (x>6) x=0; break;
         case 13 : EscreveMatriz(x,y);break;
      }
   }
   window(1,1,79,24);
   Cursor(6,7);
}
