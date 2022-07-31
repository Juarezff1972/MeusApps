#include<conio.h>
#include<dos.h>
////////////////////////////////////////////////////////////////////
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
unsigned char ReadCRT(int x,int y,char func)
{
   gotoxy(x,y);
   _AX=0x0800;
   _BX=0x0000;
   geninterrupt(0x10);
   if (func) return _AH;
   return _AL;
}
////////////////////////////////////////////////////////////////////
void DesenhaTela(void)
{
   int x,y;
   clrscr();
   textcolor(YELLOW);
   for(x=1;x<=79;x++)
   {
      gotoxy(x,1);    cprintf("*");
      gotoxy(x,24);   cprintf("*");
   }
   for(y=1;y<=24;y++)
   {
      gotoxy(1,y);    cprintf("*");
      gotoxy(79,y);   cprintf("*");
   }
   for(y=1;y<=19;y++)
   {
      gotoxy(65,y);
      cprintf("*");
   }
   gotoxy(65,9);    cprintf("**********");
   gotoxy(65,15);   cprintf("**********");
   for(y=9;y<=15;y++)
   {
      gotoxy(25,y);
      cprintf("*");
   }
   gotoxy(20,9);    cprintf("***********");
   gotoxy(20,15);   cprintf("***********");
   textcolor(LIGHTRED); gotoxy(50,24); cprintf("+");
}
////////////////////////////////////////////////////////////////////
char move(int x,int y)
{
   unsigned char aux;
   char rtncode=0;
   gotoxy(x,y);
   textcolor(LIGHTCYAN);
   cprintf("#");
   delay(50);
   aux=ReadCRT(x+1,y,0);
   if ((aux==32)||(aux==43))
   {
      if (aux==43) return 2;
      gotoxy(x,y); cprintf("ú");
      rtncode=move(x+1,y);
      if (rtncode==2) return rtncode;
      gotoxy(x,y); cprintf("#");
   }
   aux=ReadCRT(x,y-1,0);
   if ((aux==32)||(aux==43))
   {
      if (aux==43) return 2;
      gotoxy(x,y); cprintf("ú");
      rtncode=move(x,y-1);
      if (rtncode==2) return rtncode;
      gotoxy(x,y); cprintf("#");
   }
   aux=ReadCRT(x,y+1,0);
   if ((aux==32)||(aux==43))
   {
      if (aux==43) return 2;
      gotoxy(x,y); cprintf("ú");
      rtncode=move(x,y+1);
      if (rtncode==2) return rtncode;
      gotoxy(x,y); cprintf("#");
   }
   aux=ReadCRT(x-1,y,0);
   if ((aux==32)||(aux==43))
   {
      if (aux==43) return 2;
      gotoxy(x,y); cprintf("ú");
      rtncode=move(x-1,y);
      if (rtncode==2) return rtncode;
      gotoxy(x,y); cprintf("#");
   }
   delay(50);
   textcolor(LIGHTMAGENTA);
   gotoxy(x,y);cprintf("ú");
   textcolor(LIGHTCYAN);
   return rtncode;
}
////////////////////////////////////////////////////////////////////
void main(void)
{
   char aux;
   Cursor(0x19,0x00);
   DesenhaTela();
   aux=move(2,2);
   if (aux==2)
   {
      textcolor(WHITE);
      gotoxy(1,24); cprintf("Encontrou a saida.");
   }
   else
   {
      textcolor(WHITE);
      gotoxy(1,24); cprintf("N„o encontrou a saida.");
   }
   Cursor(0x6,0x7);
}