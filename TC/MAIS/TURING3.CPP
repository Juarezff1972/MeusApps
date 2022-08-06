#include <ctype.h>
#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//////////////////////////////////////////////////////////////////////////////
#define MaxFita 60
#define MaxPassos 200
//////////////////////////////////////////////////////////////////////////////
typedef struct
{
   unsigned Button;
   unsigned x;
   unsigned y;
} Status;
char Fita [MaxFita+1];
char Estados [MaxPassos+1];
char Conteudo [MaxPassos+1];
char EstadoCorrente;
unsigned char TamGram;
char MaxEstados=9;
struct
   {
      char Op;
      char It;
   } Funcao[MaxPassos];
signed int i,ii;
unsigned ButtonNum;
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
            textcolor(YELLOW); textbackground(BLACK);
            if (sizeof(Fita)>=31)
               for(i=0;i<=31;i++)
               {
                  gotoxy (31+i,9); putch(Fita[i]);
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
            textcolor(YELLOW); textbackground(BLACK);
            for(i=0;i<=31;i++)
            {
               gotoxy(31+i,9); putch(Fita[i+copo]);
            }
            i=0;
            while(1)
            {
               if (kbhit()) c=getch();
               if ((c==27)||(c==13)) break;
               if ((c=='#')||(c==32)) {Fita[i+copo]='#';i++;}
               if ((c>='a')&&(c<='s')){Fita[i+copo]=c;i++;}
               if (c==0) c=getch();
               if (c==77) i++;
               if (c==75) i--;
               c=1;
               if (i<0) { copo--; i=0; }
               if (copo<0) copo=0;
               if (i>31) { copo++; i=31; }
               if (i+copo>MaxFita) copo--;
               textcolor(YELLOW); textbackground(BLACK);
               for(bolo=0;bolo<=31;bolo++)
               {
                  if (bolo==i)
                  {
                     textcolor(RED); textbackground(DARKGRAY);
                  }
                  else
                  {
                     textcolor(YELLOW); textbackground(BLACK);
                  }
                  gotoxy(31+bolo,9); putch(Fita[bolo+copo]);
               }
               textcolor(RED); textbackground(DARKGRAY);
               gotoxy(31+i,9); putch(Fita[i+copo]);
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
void InicializaFita(void)
{
   char aux;
   char buffer[4096];
   unsigned i;
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
void Inicializa(void)
{
   char aux[4];
   char buffer[224];
   char folha,raiz;
   char c;
   int i=0;
   textcolor(YELLOW);
   textbackground(BLUE);
   gettext(28,11,55,14,buffer);
   gotoxy(28,11); cprintf("ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿");
   gotoxy(28,12); cprintf("³ Entre com o n£mero de    ³");
   gotoxy(28,13); cprintf("³ linhas da gram tica:     ³");
   gotoxy(28,14); cprintf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ");
   textcolor(LIGHTRED);
   gotoxy(51,13);
   while(1)
   {
      c=getch();
      if (c==13) break;
      if (c==27)
      {
        for(i=0;i<=3;i++) aux[i]=0;
        i=0;
        gotoxy(51,13);
        cprintf("   ");
      }
      if ((c>=48)&&(c<=57))
      {
          aux[i]=c;
          gotoxy(51+i,13);
          putch(aux[i]);
          i++;
          if(i==3)
          {
             if(atoi(aux)<=MaxPassos) break;
             delay(350);
             for(i=0;i<=3;i++) aux[i]=0;
             i=0;
             gotoxy(51,13);
             cprintf("   ");
          }
      }
   }
   delay(350);
   aux[i]=0;
   TamGram=(atoi(aux)-1);
   if (TamGram==255)
   {
      Cursor(6,7);
      asm{
          mov ax,0x0003
          int 0x10
      }
      exit(768);
   }
   textcolor(YELLOW);
   EstadoCorrente=0;
   puttext(28,11,55,14,buffer);
}
//////////////////////////////////////////////////////////////////////////////
void Executa(void)
{
   char nextstep=0;
   unsigned int count=0;
   ii=0;
   textcolor(YELLOW);textbackground(BLUE);
   Cursor(14,15);
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
             gotoxy(2,3);cprintf("q=%d",EstadoCorrente);
             gotoxy(10+ii,3); putch(Fita[ii]);
             gotoxy(10+ii,3);
             if (Funcao[i].Op==20)  goto Termina;
             count++;
             nextstep=1;
	      }
	      if ((ii<0)||(ii>MaxFita)) goto Termina;
	      delay(50);
      }
      if (count>=500*TamGram)
      {
         gotoxy(10,4); cprintf(" Loop! ");
         delay(500);
         goto Termina;
      }
      if (nextstep==0)
      {
         gotoxy(10,4); cprintf(" Travamento! ");
         delay(500);
         goto Termina;
      }
      if (nextstep==1) nextstep=0;
   }
   Termina: ;
   textcolor(LIGHTCYAN);
   gotoxy(10,4);cprintf("ÍÍÍÍÍÍÍÍÍÍÍÍÍ");
   Cursor(25,0);
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
void MostraGram(unsigned char folha,unsigned char raiz)
{
   unsigned char tronco;
   gotoxy(8,9+folha); cprintf("%d",Estados[folha+raiz]);
   tronco=Conteudo[folha+raiz];
   gotoxy(23,9+folha); cprintf("%c",tronco);
   if (Funcao[folha+raiz].Op==20) tronco='h';
   if (Funcao[folha+raiz].Op<=9) tronco=Funcao[folha+raiz].Op+48;
   gotoxy(46,9+folha); cprintf("(%c",tronco);
   if (Funcao[folha+raiz].It==20) tronco='r';
   if (Funcao[folha+raiz].It==30) tronco='l';
   if (Funcao[folha+raiz].It<=19) tronco=Funcao[folha+raiz].It+96;
   if (tronco==96) tronco='#';
   gotoxy(48,9+folha); cprintf(",%c)",tronco);
}
//////////////////////////////////////////////////////////////////////////////
void EntraGram(unsigned char i)
{
   char sopro[4096];
   char c;
   gettext(1,1,79,25,sopro);
   textcolor(YELLOW);textbackground(BLUE);
   gotoxy(8,11); cprintf("ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿");
   gotoxy(8,12); cprintf("³  Estado (q):                   ³");
   gotoxy(8,13); cprintf("³  Conte£do (s):                 ³");
   gotoxy(8,14); cprintf("³  Fun‡„o do estado (f(q)):      ³");
   gotoxy(8,15); cprintf("³  Fun‡„o do conte£do (f(s)):    ³");
   gotoxy(8,16); cprintf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ");
   Cursor(13,14);
   while(1)
   {
      gotoxy(38,12); c=getche();
      if (c==27) goto acaba;
      if ((c>=48)&&(c<=57)) { Estados[i]=c-48; break; }
   }
   while(1)
   {
      gotoxy(38,13); c=getche();
      if (c==27) goto acaba;
      if (c==32) {gotoxy(38,13);putch('#');}
      if ((c==32)||(c==35)) { Conteudo[i]=35; break; }
      if ((c>=97)&&(c<=115)) { Conteudo[i]=c; break; }
   }
   while(1)
   {
      gotoxy(38,14); c=getche();
      if (c==27) goto acaba;
      if (c==104) {Funcao[i].Op=20;break;}
      if ((c>=48)&&(c<=57)) { Funcao[i].Op=c-48; break; }
   }
   while(1)
   {
      gotoxy(38,15); c=getche();
      if (c==32) {gotoxy(38,15);putch('#');}
      if (c==27) goto acaba;
      if (c==114) {Funcao[i].It=20;break;}
      if (c==108) {Funcao[i].It=30;break;}
      if ((c==32)||(c==35)) {Funcao[i].It=0;break;}
      if ((c>=97)&&(c<=115)) { Funcao[i].It=c-96; break; }
   }
acaba:
   Cursor(25,0);
   puttext(1,1,79,25,sopro);
}
//////////////////////////////////////////////////////////////////////////////
void Salvar(char nome[8])
{
   FILE *fp;
   char filename[13];
   unsigned i;
   for (i=0;i<=7;i++) filename[i]=nome[i];
   filename[8]='.';filename[9]='T';filename[10]='U';filename[11]='R';filename[12]=0;
   if (!(fp=fopen(filename,"wb")))
	{
      clrscr();
		printf(" \n\nO arquivo n„o pode ser aberto\n");
		exit(2);
	};
   for(i=0;i<=MaxFita;i++) putc(Fita[i],fp);
   for(i=0;i<=MaxPassos;i++)
   {
      putc(Estados[i],fp);
      putc(Conteudo[i],fp);
      putc(Funcao[i].Op,fp);
      putc(Funcao[i].It,fp);
   }
   putc(EstadoCorrente,fp);putc(TamGram,fp);putc(MaxEstados,fp);
   fclose(fp);
}
//////////////////////////////////////////////////////////////////////////////
void Ler(char nome[8])
{
   FILE *fp;
   char filename[13];
   unsigned i;
   for (i=0;i<=7;i++) filename[i]=nome[i];
   filename[8]='.';filename[9]='T';filename[10]='U';filename[11]='R';filename[12]=0;
   if (!(fp=fopen(filename,"rb")))
	{
      clrscr();
		printf(" \n\nO arquivo n„o pode ser aberto\n");
		exit(2);
	};
   for(i=0;i<=MaxFita;i++) Fita[i]=getc(fp);
   for(i=0;i<=MaxPassos;i++)
   {
      Estados[i]=getc(fp);
      Conteudo[i]=getc(fp);
      Funcao[i].Op=getc(fp);
      Funcao[i].It=getc(fp);
   }
   EstadoCorrente=getc(fp);TamGram=getc(fp);MaxEstados=getc(fp);
   fclose(fp);
}
//////////////////////////////////////////////////////////////////////////////
void File(void)
{
   char vento[4096];
   char nome[8]="TURING01";
   char i;
   unsigned char c;
   int folha,caule;
   unsigned vidro;
   Status coisa;
   gettext(1,1,79,25,vento);
   textcolor(YELLOW);  textbackground(CYAN);
   gotoxy(8,11); cprintf("ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿");
   gotoxy(8,12); cprintf("³                                         ³");
   gotoxy(8,13); cprintf("³                                         ³");
   gotoxy(8,14); cprintf("³                                         ³");
   gotoxy(8,15); cprintf("³                                         ³");
   gotoxy(8,16); cprintf("³                                         ³");
   gotoxy(8,17); cprintf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ");
   gotoxy(10,15); cprintf("Nome: ");
   textcolor(BLACK);  textbackground(WHITE);
   gotoxy(10,13); cprintf(" Salvar ");
   gotoxy(25,13); cprintf("   Ler  ");
   gotoxy(40,13); cprintf(" In¡cio ");
   textcolor(BLACK); textbackground(CYAN);
   gotoxy(11,14); cprintf("ßßßßßßßß");gotoxy(18,13);cprintf("Ü");
   gotoxy(26,14); cprintf("ßßßßßßßß");gotoxy(33,13);cprintf("Ü");
   gotoxy(41,14); cprintf("ßßßßßßßß");gotoxy(48,13);cprintf("Ü");
   textcolor(YELLOW); textbackground(BLACK);
   gotoxy(16,15); cprintf("          ");
   for (i=0;i<=7;i++) { gotoxy(17+i,15); putch(nome[i]); }
   ShowCursor();
   while(1)
   {
      coisa=GetStatus();
      if (coisa.Button==2) {puttext(1,1,79,25,vento);break;}
      if ((coisa.y==15)&&(coisa.x>=16)&&(coisa.x<=25)&&(coisa.Button==1))
      {
          textcolor(YELLOW); textbackground(BLACK);
          gotoxy(16,15); cprintf("          ");
          Cursor(14,15);
          gotoxy(17,15);
          i=0;
          HideCursor();
          while(1)
          {
              c=toupper(getch());
              if ((c==27)||(c==13)) break;
              if ((c=='!')||(c=='-')||((c>=48)&&(c<=57))||(c=='@')
               ||((c>='A')&&(c<='Z'))||((c>=35)&&(c<=41))||(c>=128))
              {
                 nome[i]=c;
                 gotoxy(17+i,15);putch(c);
                 i++;
              }
              if (i>7) break;
          }
          ShowCursor();
          Cursor(25,0);
      }
      if (coisa.y==13)
      {
         if ((coisa.x>=10)&&(coisa.x<=17)&&(coisa.Button==1))
         {
            HideCursor();
            textbackground(CYAN);gotoxy(10,13);putch(32);
            textcolor(BLACK);  textbackground(WHITE);
            gotoxy(11,13); cprintf(" Salvar ");
            textbackground(CYAN);gotoxy(10,14);cprintf("         ");
            Salvar(nome);
            textcolor(BLACK);  textbackground(WHITE);
            gotoxy(10,13); cprintf(" Salvar ");
            textcolor(BLACK); textbackground(CYAN);
            gotoxy(11,14); cprintf("ßßßßßßßß");gotoxy(18,13);cprintf("Ü");
            puttext(1,1,79,25,vento);
            ShowCursor();break;
         }
         if ((coisa.x>=25)&&(coisa.x<=32)&&(coisa.Button==1))
         {
            HideCursor();
            textbackground(CYAN);gotoxy(25,13);putch(32);
            textcolor(BLACK);  textbackground(WHITE);
            gotoxy(26,13); cprintf("   Ler  ");
            textbackground(CYAN);gotoxy(25,14);cprintf("         ");
            puttext(1,1,79,25,vento);
            for(vidro=0;vidro<=MaxPassos;vidro++)
            {
               Estados[vidro]=0;
               Conteudo[vidro]=35;
               Funcao[vidro].It=0;
               Funcao[vidro].Op=0;
            }
            for(vidro=0;vidro<=MaxFita;vidro++) Fita[vidro]='#';
            textbackground(BLUE);textcolor(YELLOW);
            window(7,9,9,22);clrscr();window(1,1,79,25);
            window(22,9,24,22);clrscr();window(1,1,79,25);
            window(45,9,51,22);clrscr();window(1,1,79,25);
            Ler(nome);
            caule=0;
            for(folha=0;folha<=TamGram;folha++)
            if (folha<=13) MostraGram(folha,caule);
            gotoxy(64,7);putch(216);
            folha=0;
            textbackground(DARKGRAY);textcolor(WHITE);
            MostraGram(folha,caule);
            gotoxy(10,3);textbackground(BLUE);textcolor(WHITE);cprintf("%s",Fita);
            ShowCursor();
            break;
         }
         if ((coisa.x>=40)&&(coisa.x<=47)&&(coisa.Button==1))
         {
            HideCursor();
            textbackground(CYAN);gotoxy(40,13);putch(32);
            textcolor(BLACK);  textbackground(WHITE);
            gotoxy(41,13); cprintf(" In¡cio ");
            textbackground(CYAN);gotoxy(40,14);cprintf("         ");
            puttext(1,1,79,25,vento);
            for(vidro=0;vidro<=MaxPassos;vidro++)
            {
               Estados[vidro]=0;
               Conteudo[vidro]=35;
               Funcao[vidro].It=0;
               Funcao[vidro].Op=0;
            }
            for(vidro=0;vidro<=MaxFita;vidro++) Fita[vidro]='#';
            textbackground(BLUE);textcolor(YELLOW);
            window(7,9,9,22);clrscr();window(1,1,79,25);
            window(22,9,24,22);clrscr();window(1,1,79,25);
            window(45,9,51,22);clrscr();window(1,1,79,25);
            Inicializa();
            caule=0;
            for(folha=0;folha<=TamGram;folha++)
            if (folha<=13) MostraGram(folha,caule);
            gotoxy(64,7);putch(216);
            folha=0;
            textbackground(DARKGRAY);textcolor(WHITE);
            MostraGram(folha,caule);
            gotoxy(10,3);textbackground(BLUE);textcolor(WHITE);cprintf("%s",Fita);
            ShowCursor();
            break;
         }
      }
   }
   HideCursor();
}
//////////////////////////////////////////////////////////////////////////////
void Ajuda(void)
{
   FILE *fp;
   char *linhas[100];
   unsigned i=0;
   Status coisa;
   textcolor(YELLOW);textbackground(BLUE);
   gotoxy(8,11); cprintf("ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿");
   gotoxy(8,12); cprintf("³                                                         ³");
   gotoxy(8,13); cprintf("³                                                         ³");
   gotoxy(8,14); cprintf("³                                                         ³");
   gotoxy(8,15); cprintf("³                                                         ³");
   gotoxy(8,16); cprintf("³                                                         ³");
   gotoxy(8,17); cprintf("³                                                         ³");
   gotoxy(8,18); cprintf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ");
   textcolor(LIGHTRED);
   if ((fp=fopen("TURING.HLP","rt"))==NULL)
   {
      gotoxy(10,13);cprintf("TURING.HLP n„o foi encontrado.");
      gotoxy(10,14);cprintf("Aperte algo para continuar.");
      getch();
      return;
   }
   while (!feof(fp))
   {
      fgets(linhas[i],54,fp);
//      gotoxy(10,12+i);puts(linhas[i]);
      i++;
   }
   while(1)
   {
      coisa=GetStatus();
      if (coisa.Button==2) break;
   }
   fclose(fp);
}
//////////////////////////////////////////////////////////////////////////////
void Tela(void)
{
   unsigned i;
   Status coisa;
   char *texto;
   char buffer1[144];
   char buffer999[928];
   char c;
   int folha,raiz,tronco,caule;
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
      gotoxy(15,i);putch(179);
      gotoxy(29,i);putch(179);
      gotoxy(64,i);putch(216);
   }
   for(i=3;i<=62;i++)
   {
      gotoxy(i,6);  putch(196);
      gotoxy(i,23); putch(196);
      gotoxy(i,8);  putch(196);
   }
   gotoxy(64,23); putch(207); gotoxy(64,6); putch(209);
   gotoxy(63,8); putch(180);gotoxy(2,8);putch(195);
   gotoxy(15,23);putch(193);gotoxy(29,23);putch(193);
   gotoxy(15,6);putch(194);gotoxy(29,6);putch(194);
   gotoxy(15,8);putch(197);gotoxy(29,8);putch(197);
   gotoxy(8,7);putch('q');gotoxy(23,7);putch('s');
   gotoxy(45,7);cprintf("f(q,s)");
   DrawButton();
   Inicializa();
   ShowCursor();
   Cursor(25,0);
   caule=0;
   for(folha=0;folha<=TamGram;folha++)
      if (folha<=13) MostraGram(folha,caule);
   gotoxy(64,7);putch(216);
   folha=0;
   textbackground(DARKGRAY);textcolor(WHITE);
   MostraGram(folha,caule);
   gotoxy(10,3);textbackground(BLUE);textcolor(WHITE);cprintf("%s",Fita);
   while(1)
   {
      if (kbhit())
      {
         c=getch();
         textcolor(YELLOW);textbackground(BLUE);
         for(i=7;i<=22;i++) {gotoxy(64,i);putch(216);}
         if (c==0) c=getch();
         if (c==72) folha--;
         if (c==80) folha++;
         if (folha<0) {folha=0;caule--;}
         if (TamGram<=13) if (folha>TamGram) folha=TamGram;
         if (TamGram>13) if (folha>13) {folha=13;caule++;};
         if (caule<0) caule=0;
         if (folha+caule>TamGram) caule--;
         HideCursor();
         if (c==13) EntraGram(folha+caule);
         textbackground(BLUE);textcolor(YELLOW);
         gotoxy(64,7+(int)((float)(((float)folha+(float)caule)/(float)TamGram)*15.0));
         putch(219);
         for(raiz=0;raiz<=TamGram;raiz++)
            if (raiz<=13) MostraGram(raiz,caule);
         textbackground(DARKGRAY);textcolor(WHITE);
         MostraGram(folha,caule);
         ShowCursor();
         c=1;
      }
      coisa=GetStatus();
      if ((coisa.x==64)&&(coisa.y==23)&&(coisa.Button==1)) ungetch(80);
      if ((coisa.x==64)&&(coisa.y== 6)&&(coisa.Button==1)) ungetch(72);
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
         if ((coisa.y==17)&&(coisa.Button==1))
         {
            HideCursor();
            textcolor(BLACK); textbackground(BLUE);
            gotoxy(68,17); putch(32);
            gotoxy(68,18); cprintf("           ");
            gotoxy(69,17);
            textbackground(WHITE);cprintf(" Executar ");
            Executa();
            DrawButton();
            ShowCursor();
         }
         if ((coisa.y==13)&&(coisa.Button==1))
         {
            HideCursor();
            textcolor(BLACK); textbackground(BLUE);
            gotoxy(68,13); putch(32);
            gotoxy(68,14); cprintf("           ");
            gotoxy(69,13);
            textbackground(WHITE);cprintf(" Arquivo  ");
            File();
            DrawButton();
            ShowCursor();
         }
         if ((coisa.y==15)&&(coisa.Button==1))
         {
            HideCursor();
            textcolor(BLACK); textbackground(BLUE);
            gotoxy(68,15); putch(32);
            gotoxy(68,16); cprintf("           ");
            gotoxy(69,15);
            textbackground(WHITE);cprintf("   Ajuda  ");
            gettext(8,11,68,18,buffer999);
            Ajuda();
            puttext(8,11,68,18,buffer999);
            DrawButton();
            ShowCursor();
            if(coisa.Button==2) break;
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
   unsigned vidro;
   for(vidro=0;vidro<=MaxPassos;vidro++)
   {
      Estados[vidro]=0;
      Conteudo[vidro]=35;
      Funcao[vidro].It=0;
      Funcao[vidro].Op=0;
   }
   for(vidro=0;vidro<=MaxFita;vidro++) Fita[vidro]='#';
   InitMouse();
   Tela();
}
