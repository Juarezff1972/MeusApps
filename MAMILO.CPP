#include <conio.h>
#include <stdio.h>
/////////////////////////////////////////////////////////////////////////////
void TelaInic(void)
{
  int i;
  gotoxy(1,1);
  textcolor(1);
  textbackground(15);
  for(i=1;i<=2000;i++)   putch(177);
  textcolor(3);
  textbackground(0);
  gotoxy(1,1);
  cprintf("ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป");
  gotoxy(1,18);
  cprintf("ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ");
  for(i=2;i<=17;i++)
  {
     gotoxy(1,i);
     cprintf("บ                                                                              บ");
  }
  textcolor(0);
  textbackground(15);
  gotoxy(1,19);
  cprintf(" Copiar      Apagar      Diretขrio      Atributos      Servidor      Visualizar ");
  gotoxy(2,19);
  textcolor(4);
  putch('C');
  gotoxy(14,19);
  putch('A');
  gotoxy(26,19);
  putch('D');
  gotoxy(42,19);
  putch('t');
  gotoxy(56,19);
  putch('S');
  gotoxy(70,19);
  putch('V');
  textcolor(3);
  textbackground(0);
  gotoxy(55,4);
  cprintf("NetBar 1.0a");
  gotoxy(51,6);
  cprintf("Copyright (C) 1994");
  gotoxy(56,8);
  cprintf("by Coyote");
  gotoxy(57,10);
  cprintf("Equipe:");
  gotoxy(50,12);
  cprintf("Aderson Cleber Pifer");
  gotoxy(48,13);
  cprintf("Anderson Ferreira Martins");
  gotoxy(48,14);
  cprintf("Gustavo Henrique Busnardo");
  gotoxy(49,15);
  cprintf("Juarez Fioravanti Filho");
}
///////////////////////////////////////////////////////////////////////
void main()
{
  TelaInic();
}