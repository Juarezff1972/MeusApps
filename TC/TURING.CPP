#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//////////////////////////////////////////////////////////////////////////////
#define MaxFita 60
#define MaxPassos 20
//////////////////////////////////////////////////////////////////////////////
char Fita [MaxFita];
char Estados [MaxPassos];
char Conteudo [MaxPassos];
char EstadoCorrente;
char TamGram;
struct
   {
      char Op;
      char It;
   } Funcao[MaxPassos];

signed int i,ii;
//////////////////////////////////////////////////////////////////////////////
void InicializaFita(void)
{
   char aux;
   gotoxy(2,2);
   randomize();
   for(i=0;i<=MaxFita;i++)
   {
//      aux=random(2);
      aux=1;
      if (aux==0) Fita[i]='#';
      if (aux==1) Fita[i]='a';
      if (aux==2) Fita[i]='b';
      textcolor(aux+LIGHTCYAN);
      putch(Fita[i]);
   }
}
//////////////////////////////////////////////////////////////////////////////
void InicializaGramatica(void) // Funcao[i].Op: 0 = q0, 1 = q1, 20 = h
{                              // Funcao[i].It: 0 = '#' 1 = 'a' 20 = R 30 = L
   Estados[ 0]=0; Conteudo[ 0]='a'; Funcao[ 0].Op=1; Funcao[ 0].It=0;
   Estados[ 1]=0; Conteudo[ 1]='#'; Funcao[ 1].Op=20;Funcao[ 1].It=0;
   Estados[ 2]=1; Conteudo[ 2]='a'; Funcao[ 2].Op=0; Funcao[ 2].It=1;
   Estados[ 3]=1; Conteudo[ 3]='#'; Funcao[ 3].Op=0; Funcao[ 3].It=20;
   EstadoCorrente=0;
   TamGram=3;
}
//////////////////////////////////////////////////////////////////////////////
void Executa(void)
{
   ii=0;
   gotoxy(2,2);
   textcolor(YELLOW);
   while(1)
   {
      for(i=0;i<=TamGram;i++)
      {
         if ((Estados[i]==EstadoCorrente)&&(Fita[ii]==Conteudo[i]))
         {
            if (Funcao[i].It==30) ii--;
            if (Funcao[i].It==20) ii++;
            if (Funcao[i].It==2)  Fita[ii]='b';
            if (Funcao[i].It==1)  Fita[ii]='a';
            if (Funcao[i].It==0)  Fita[ii]='#';
            if (Funcao[i].Op!=20) EstadoCorrente=Funcao[i].Op;
            if (Funcao[i].Op==20)  goto Termina;
         }
         gotoxy(2+ii,2); putch(Fita[ii]);
         gotoxy(2,5); cprintf("Estado atual:   %d",EstadoCorrente);
         gotoxy(2,6); cprintf("Valor da Fita:  %c",Fita[ii]);
         if ((ii<0)||(ii>=MaxFita)) goto Termina;
         delay(100);
      }
   }
   Termina: ;
}
//////////////////////////////////////////////////////////////////////////////
void main(void)
{
   clrscr();
   InicializaFita();
   InicializaGramatica();
   Executa();
}
