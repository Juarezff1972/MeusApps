#include <ctype.h>
#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//////////////////////////////////////////////////////////////////////////////
#define MaxFita 60
#define MaxPassos 20
//////////////////////////////////////////////////////////////////////////////
typedef struct
{
   unsigned Button;
   unsigned x;
   unsigned y;
} Status;
char Fita [MaxFita];
char Estados [MaxPassos];
char Conteudo [MaxPassos];
char EstadoCorrente;
char TamGram;
char MaxEstados=9;
struct
   {
      char Op;
      char It;
   } Funcao[MaxPassos];
signed int i,ii;
unsigned ButtonNum;
//////////////////////////////////////////////////////////////////////////////
void Cursor(unsigned inicio,unsigned fim);
char InitMouse(void);
void ShowCursor(void);
void HideCursor(void);
Status GetStatus(void);
void Desenha(void);
void InicializaFita(void);
void EscreveFita(void);
void Inicializa(void);
void Executa(void);
void DrawButton(void);
void Tela(void);
//////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////
char InitMouse(void)
{
   _AX=0;
   geninterrupt(0x33);
   ButtonNum=_BX;
   return _AX;
}
//////////////////////////////////////////////////////////////////////////////
void ShowCursor(void)
{
   _AX=1;
   geninterrupt(0x33);
}
//////////////////////////////////////////////////////////////////////////////
void HideCursor(void)
{
   _AX=2;
   geninterrupt(0x33);
}
//////////////////////////////////////////////////////////////////////////////
Status GetStatus(void)
{
   Status aux;
   _AX=3;
   geninterrupt(0x33);
   aux.Button=_BX;
   aux.x=_CX;
   aux.y=_DX;
   aux.x=1+(aux.x/8);
   aux.y=1+(aux.y/8);
   return aux;
}
//////////////////////////////////////////////////////////////////////////////
void Desenha(void)
{
   unsigned char i;
   unsigned char t[4096];
   unsigned aux1,aux2;
   FILE *asci;
   if ((asci = fopen("CHAR437", "rb"))== NULL)
   {
      fprintf(stderr, "N„o posso abrir arquivo CHAR437.\n");
      exit(1);
   }
   fread(t,4096,1,asci);
   fclose(asci);
   t[3008]=255;   t[3009]=129;   t[3010]=129;   t[3011]=129;   t[3012]=129;
   t[3013]=161;   t[3014]=177;   t[3015]=185;   t[3016]=189;   t[3017]=185;
   t[3018]=177;   t[3019]=161;   t[3020]=129;   t[3021]=129;   t[3022]=129;
   t[3023]=254;   t[3200]=255;   t[3201]=129;   t[3202]=129;   t[3203]=129;
   t[3204]=129;   t[3205]=133;   t[3206]=141;   t[3207]=157;   t[3208]=189;
   t[3209]=157;   t[3210]=141;   t[3211]=133;   t[3212]=129;   t[3213]=129;
   t[3214]=129;   t[3215]=255;   t[3280]=255;   t[3281]=0;     t[3282]=0;
   t[3283]=0;     t[3284]=0;     t[3285]=0;     t[3286]=0;     t[3287]=0;
   t[3288]=0;     t[3289]=0;     t[3290]=0;     t[3291]=0;     t[3292]=0;
   t[3293]=0;     t[3294]=0;     t[3295]=255;   t[3344]=255;   t[3345]=129;
   t[3346]=153;   t[3347]=153;   t[3348]=189;   t[3349]=189;   t[3350]=255;
   t[3351]=255;   t[3352]=153;   t[3353]=153;   t[3354]=153;   t[3355]=153;
   t[3356]=153;   t[3357]=129;   t[3358]=129;   t[3359]=255;   t[3312]=255;
   t[3313]=129;   t[3314]=129;   t[3315]=129;   t[3316]=153;   t[3317]=153;
   t[3318]=153;   t[3319]=153;   t[3320]=255;   t[3321]=255;   t[3322]=189;
   t[3323]=189;   t[3324]=153;   t[3325]=153;   t[3326]=129;   t[3327]=255;
   t[3456]=129;   t[3457]=129;   t[3458]=129;   t[3459]=129;   t[3460]=129;
   t[3461]=129;   t[3462]=129;   t[3463]=129;   t[3464]=129;   t[3465]=129;
   t[3466]=129;   t[3467]=129;   t[3468]=129;   t[3469]=129;   t[3470]=129;
   t[3471]=129;
   aux1=FP_SEG(&t);
   aux2=FP_OFF(&t);
   asm{
      push ax
      push bx
      push cx
      push dx
      push es
      push bp
      mov ax,0x1100
      mov cx,256
      mov dx,0
      mov bx,0x1000
      push aux1
      pop es
      push aux2
      pop bp
      int 0x10
      pop bp
      pop es
      pop dx
      pop cx
      pop bx
      pop ax
   }
}
//////////////////////////////////////////////////////////////////////////////
void InicializaFita(void)
{
   char aux;
   char buffer[4096];
   unsigned i;
   for(i=0;i<=MaxFita;i++)
   {
      aux=0;
      if (aux==0) Fita[i]='#'; else Fita[i]=96+aux;
   }
   gettext(1,1,79,25,buffer);
   textcolor(YELLOW); textbackground(BLUE);
   window(5,5,65,11); clrscr(); window(1,1,79,25);
   gotoxy(5,5); putch(218); gotoxy(5,11); putch(192);
   gotoxy(65,5); putch(191); gotoxy(65,11); putch(217);
   for(i=6;i<=10;i++) { gotoxy(5,i); putch(179); gotoxy(65,i); putch(179); }
   for(i=11;i<=69;i++) { gotoxy(i-5,5); putch(196); gotoxy(i-5,11); putch(196); }
   textcolor(BLACK); textbackground(WHITE);
   gotoxy(7,7); cprintf("  Randomizar  ");
   gotoxy(7,9); cprintf("    Editar    ");
   textcolor(BLACK); textbackground(BLUE);
   for(i=0;i<=2;i=i+2)
   {
      gotoxy(7,8+i);
      cprintf(" ßßßßßßßßßßßßßß");
      gotoxy(21,7+i);
      cprintf("Ü");
   }
   textcolor(LIGHTRED);
   gotoxy(30,7); cprintf("Tamanho da fita: %d",MaxFita);
   textcolor(YELLOW); textbackground(BLACK);
   gotoxy(30,9); cprintf("                                  ");
   if (sizeof(Fita)>=31)
      for(i=0;i<=31;i++)
      {
         gotoxy (31+i,9);
         cprintf("%c",Fita[i]);
      }
   ShowCursor();
   EscreveFita();
   HideCursor();
   puttext(1,1,79,25,buffer);
   gotoxy(10,3);
   textbackground(BLUE);
   textcolor(WHITE);
   cprintf("%s",Fita);
}
//////////////////////////////////////////////////////////////////////////////
void EscreveFita(void)
{
   int aux;
   signed i,bolo,copo=0;
   char c=1;
   Status coisa;
   while(1)
   {
      coisa=GetStatus();
      if ((coisa.x>=7)&&(coisa.x<=21))
      {
         if ((coisa.y==7)&&(coisa.Button==1))
         {
            HideCursor();
            textcolor(BLACK); textbackground(BLUE);
            gotoxy(7,7); putch(32);
            gotoxy(8,8); cprintf("              ");
            gotoxy(8,7);
            textbackground(WHITE); cprintf("  Randomizar  ");
            randomize();
            for(i=0;i<=MaxFita;i++)
            {
               aux=random(19);
               if (aux==0) Fita[i]='#'; else Fita[i]=96+aux;
            }
            textcolor(YELLOW);
            textbackground(BLACK);
            if (sizeof(Fita)>=31)
               for(i=0;i<=31;i++)
               {
                  gotoxy (31+i,9);
                  cprintf("%c",Fita[i]);
               }
            textcolor(BLACK); textbackground(WHITE);
            gotoxy(7,7); cprintf("  Randomizar  ");
            gotoxy(7,9); cprintf("    Editar    ");
            textcolor(BLACK); textbackground(BLUE);
            for(i=0;i<=2;i=i+2)
            {
               gotoxy(7,8+i);
               cprintf(" ßßßßßßßßßßßßßß");
               gotoxy(21,7+i);
               cprintf("Ü");
            }
            ShowCursor();
         }
         if ((coisa.y==9)&&(coisa.Button==1))
         {
            HideCursor();
            textcolor(BLACK); textbackground(BLUE);
            gotoxy(7,9); putch(32);
            gotoxy(8,10); cprintf("              ");
            gotoxy(8,9);
            textbackground(WHITE); cprintf("    Editar    ");
            for(i=0;i<=31;i++)
            {
               gotoxy(31+i,9); putch(Fita[i+copo]);
            }
            i=0;
            while(1)
            {
               if (kbhit()) c=getch();
               if (c==27) break;
               if (c=='#') {Fita[i+copo]='#';i++;}
               if ((c>='a')&&(c<='s')){Fita[i+copo]=c;i++;}
               if (c==0) c=getch();
               if (c==75) i++;
               if (c==77) i--;
               if (i<0) { copo--; i=0; }
               if (i>31) { copo++; i=31; }
               for(bolo=0;bolo<=31;bolo++)
               {
                  gotoxy(31+bolo,9); putch(Fita[bolo+copo]);
               }
            }
            textcolor(BLACK); textbackground(WHITE);
            gotoxy(7,7); cprintf("  Randomizar  ");
            gotoxy(7,9); cprintf("    Editar    ");
            textcolor(BLACK); textbackground(BLUE);
            for(i=0;i<=2;i=i+2)
            {
               gotoxy(7,8+i);
               cprintf(" ßßßßßßßßßßßßßß");
               gotoxy(21,7+i);
               cprintf("Ü");
            }
            ShowCursor();
         }
      }
      if(coisa.Button==2) break;
   }

}
//////////////////////////////////////////////////////////////////////////////
void Inicializa(void)
{
   char aux;
   int i=0;
   cprintf("\n\rEntre com o n£mero de linhas da gram tica: ");
   scanf("%d",&aux);
   TamGram=aux-1;
   for(i=0;i<=TamGram;i++)
   {
      cprintf("\n\rq %d:",i);
      Estados[i]=getche(); Estados[i]=Estados[i]-48;
      cprintf("\n\rå %d:",i);
      Conteudo[i]=getche();
      cprintf("\n\rë(q) %d:",i);
      Funcao[i].Op=getche();
      if(Funcao[i].Op=='h') Funcao[i].Op=20;
      cprintf("\n\rë(å) %d:",i);
      Funcao[i].It=getche();
      if ((tolower(Funcao[i].Op)>=48)&&(tolower(Funcao[i].Op)<=57))
         Funcao[i].Op=Funcao[i].Op-48;
      if ((tolower(Funcao[i].It)>=97)&&(tolower(Funcao[i].It)<=107))
         Funcao[i].It=Funcao[i].It-96;
      if (Funcao[i].It=='#') Funcao[i].It=0;
      if(tolower(Funcao[i].It)=='r') Funcao[i].It=20;
      if(tolower(Funcao[i].It)=='l') Funcao[i].It=30;
   }
   EstadoCorrente=0;
}
//////////////////////////////////////////////////////////////////////////////
void Executa(void)
{
   char nextstep=0;
   unsigned int count=0;
   ii=0;
   textcolor(YELLOW);
   while(1)
   {
      for(i=0;i<=TamGram;i++)
      {
  	      if ((Estados[i]==EstadoCorrente)&&(Fita[ii]==Conteudo[i]))
	      {
	          if (Funcao[i].It==30) ii--;
	          if (Funcao[i].It==20) ii++;
	          if (Funcao[i].It==0) Fita[ii]='#'; else
                if (Funcao[i].It<20) Fita[ii]=96+Funcao[i].It;
	          if (Funcao[i].Op!=20) EstadoCorrente=Funcao[i].Op;
	          if (Funcao[i].Op==20)  goto Termina;
             gotoxy(2+ii,2); putch(Fita[ii]);
	          gotoxy(2,5); cprintf("Estado atual:   %d",EstadoCorrente);
	          gotoxy(2,6); cprintf("Valor da Fita:  %c",Fita[ii]);
             gotoxy(2+ii,2);
             count++;
             nextstep=1;
	      }
	      if ((ii<0)||(ii>=MaxFita)) goto Termina;
	      delay(100);
      }
      if (count>=100*TamGram)
      {
         gotoxy(2,7); cprintf("Loop!");
         goto Termina;
      }
      if (nextstep==0)
      {
         gotoxy(2,7); cprintf("Travamento!");
         goto Termina;
      }
      if (nextstep==1) nextstep=0;
   }
   Termina: ;
}
//////////////////////////////////////////////////////////////////////////////
void DrawButton(void)
{
   unsigned i;
   textcolor(BLACK); textbackground(BLUE);
   for(i=0;i<=12;i=i+2)
   {
      gotoxy(68,8+i);
      cprintf(" ßßßßßßßßßß");
      gotoxy(78,7+i);
      cprintf("Ü");
   }
   textcolor(BLACK);   textbackground(WHITE);
   gotoxy(68, 7); cprintf(" Alfabeto ");
   gotoxy(68, 9); cprintf(" Estados  ");
   gotoxy(68,11); cprintf("   Fita   ");
   gotoxy(68,13); cprintf(" Arquivo  ");
   gotoxy(68,15); cprintf("   Ajuda  ");
   gotoxy(68,17); cprintf(" Executar ");
   gotoxy(68,19); cprintf("   Sair   ");
}
//////////////////////////////////////////////////////////////////////////////
void Tela(void)
{
   unsigned i;
   Status coisa;
   char *texto;
   char buffer1[72];
   char c;
   clrscr();
   Desenha();
   Cursor(0x19,0x00);
   for(i=0;i<=2000;i++)
   {
      pokeb(0xb800,2*i,0xb0);      pokeb(0xb800,2*i+1,0x1b);
   }
   for(i=0;i<=79;i++)
   {
      pokeb(0xb800,2*i,0x20);        pokeb(0xb800,2*i+1,0xff);
      pokeb(0xb800,2*(i+80),0xc4);   pokeb(0xb800,2*(i+80)+1,0x1b);
      pokeb(0xb800,2*(i+160),0x20);  pokeb(0xb800,2*(i+160)+1,0x1b);
      pokeb(0xb800,2*(i+240),0xcd);  pokeb(0xb800,2*(i+240)+1,0x1b);
      pokeb(0xb800,2*(i+1920),0x20); pokeb(0xb800,2*(i+1920)+1,0xff);
   }
   textcolor(LIGHTCYAN);   textbackground(BLUE);
   gotoxy(1,4); putch(200);
   gotoxy(80,4);putch(188);
   textcolor(BLACK);   textbackground(WHITE);
   texto="M quina de Turing";
   gotoxy(40-strlen(texto)/2,1);
   cprintf("%s",texto);
   texto="Copyright(C)1994 by Coyote";
   gotoxy(40-strlen(texto)/2,25);
   cprintf("%s",texto);
   textcolor(YELLOW);   textbackground(BLUE);
   window(66,6,79,20); clrscr(); window(1,1,79,25);
   window(2,6,64,23);  clrscr(); window(1,1,79,25);
   gotoxy(2,6); putch(218); gotoxy(2,23);putch(192);
   gotoxy(63,6); putch(191); gotoxy(63,23);putch(217);
   for(i=7;i<=22;i++)
   {
      gotoxy(2,i); putch(179);
      gotoxy(63,i);putch(179);
      gotoxy(64,i);putch(216);
   }
   for(i=3;i<=62;i++) { gotoxy(i,6); putch(196); gotoxy(i,23); putch(196);}
   gotoxy(64,23); putch(207); gotoxy(64,6); putch(209);
   DrawButton();
   ShowCursor();
   while(1)
   {
      coisa=GetStatus();
      if ((coisa.x>=68)&&(coisa.x<=77))
      {
         if ((coisa.y==19)&&(coisa.Button==1))
         {
            HideCursor();
            textcolor(BLACK); textbackground(BLUE);
            gotoxy(68,19); putch(32);
            gotoxy(68,20); cprintf("           ");
            gotoxy(69,19);
            textbackground(WHITE);cprintf("   Sair   ");
            delay(100);
            Cursor(6,7);
            asm{
                mov ax,0x0003
                int 0x10
            }
            exit(768);
            break;
         }
         if ((coisa.y==7)&&(coisa.Button==1))
         {
            HideCursor();
            textcolor(BLACK); textbackground(BLUE);
            gotoxy(68,7); putch(32);
            gotoxy(68,8); cprintf("           ");
            gotoxy(69,7);
            textbackground(WHITE);cprintf(" Alfabeto ");
            textcolor(YELLOW); textbackground(BLUE);
            gettext(28,11,51,13,buffer1);
            window(28,11,51,13); clrscr(); window(1,1,79,25);
            gotoxy(28,11); cprintf("ÚÄÄÄÄÄÄ Alfabeto ÄÄÄÄÄÄ¿");
            gotoxy(28,12); cprintf("³ #abcdefghijklmnopqrs ³");
            gotoxy(28,13); cprintf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ");
            ShowCursor();
            while(1)
            {
               coisa=GetStatus();
               if (kbhit())
               {
                  c=getch();
                  break;
               }
               if (coisa.Button==2) break;
            }
            HideCursor();
            puttext(28,11,51,13,buffer1);
            DrawButton();
            ShowCursor();
         }
         if ((coisa.y==9)&&(coisa.Button==1))
         {
            HideCursor();
            textcolor(BLACK); textbackground(BLUE);
            gotoxy(68,9); putch(32);
            gotoxy(68,10); cprintf("           ");
            gotoxy(69,9);
            textbackground(WHITE);cprintf(" Estados  ");
            textcolor(YELLOW); textbackground(BLUE);
            gettext(28,11,51,13,buffer1);
            window(28,11,51,13); clrscr(); window(1,1,79,25);
            gotoxy(28,11); cprintf("ÚÄÄÄÄÄÄ Estados ÄÄÄÄÄÄÄ¿");
            gotoxy(28,12); cprintf("³ N§ de estados: 9     ³");
            gotoxy(28,13); cprintf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ");
            ShowCursor();
            while(1)
            {
               coisa=GetStatus();
               gotoxy(45,12); putch(MaxEstados+48);
               if (kbhit())
               {
                  c=getch();
                  if ((c>=48)&&(c<=57)) MaxEstados=c-48;
                  if ((c==27)||(c==13)) break;
               }
               if (coisa.Button==2) break;
            }
            HideCursor();
            puttext(28,11,51,13,buffer1);
            DrawButton();
            ShowCursor();
         }
         if ((coisa.y==11)&&(coisa.Button==1))
         {
            HideCursor();
            textcolor(BLACK); textbackground(BLUE);
            gotoxy(68,11); putch(32);
            gotoxy(68,12); cprintf("           ");
            gotoxy(69,11);
            textbackground(WHITE);cprintf("   Fita   ");
            InicializaFita();
            DrawButton();
            ShowCursor();
         }
      }
   }
/*   Inicializa();
   Executa();*/
   HideCursor();
   Cursor(6,7);
   asm{
      mov ax,0x0003
      int 0x10
   }
}
//////////////////////////////////////////////////////////////////////////////
void main(void)
{
   InitMouse();
   Tela();
}
