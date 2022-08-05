#include <ctype.h>
#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//////////////////////////////////////////////////////////////////////////////
typedef struct
{
   unsigned Button;
   unsigned x;
   unsigned y;
} Status;
signed int i,ii;
unsigned ButtonNum;
char refeicoes[5], bebidas[5],sobremesas[5];
char refeicao=0, bebida=0,sobremesa=0;
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
   gotoxy(68, 7); cprintf(" Rel¢gio  ");
   gotoxy(68, 9); cprintf("  Gar‡om  ");
   gotoxy(68,11); cprintf("  Pre‡os  ");
   gotoxy(68,13); cprintf("  Conta   ");
   gotoxy(68,15); cprintf(" Cadastro ");
   gotoxy(68,17); cprintf("  Enviar  ");
   gotoxy(68,19); cprintf("   Sair   ");
}
//////////////////////////////////////////////////////////////////////////////
void DesenhaCardapio()
{
   textcolor(LIGHTRED);
   gotoxy(3, 7);cprintf("      REFEICOES                          BEBIDAS");
   textcolor(WHITE);
   gotoxy(3, 9);cprintf(" ( ) Charuto a la Anderson       ( ) Skol");
   gotoxy(3,10);cprintf(" ( ) Pizza de Yoplight de Uva    ( ) Cerveja decente");
   gotoxy(3,11);cprintf(" ( ) Bife frito a la Aderson     ( ) Chopp reciclado");
   gotoxy(3,12);cprintf(" ( ) Rodizio de pneus assados    ( ) Vodka amarela");
   gotoxy(3,13);cprintf(" ( ) Boi a passarinho            ( ) Pinga sem alcool");
   textcolor(LIGHTRED);
   gotoxy(3,15);cprintf("      SOBREMESAS ");
   textcolor(WHITE);
   gotoxy(3,17);cprintf(" ( ) Sorvete com sal        ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿ ");
   gotoxy(3,18);cprintf(" ( ) Pudim Gustavo          ³    Escolha suas opcoes    ³ ");
   gotoxy(3,19);cprintf(" ( ) Frape de feijao        ³    clicando o mouse nos   ³ ");
   gotoxy(3,20);cprintf(" ( ) Biscoito a la Juarez   ³    itens desejados        ³ ");
   gotoxy(3,21);cprintf(" ( ) Suspiro apaixonado     ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ ");
}
//////////////////////////////////////////////////////////////////////////////
void Enviar(void)
{
   if (refeicoes[0]==1)  refeicao=1;
   if (refeicoes[1]==1)  refeicao=refeicao+2;
   if (refeicoes[2]==1)  refeicao=refeicao+4;
   if (refeicoes[3]==1)  refeicao=refeicao+8;
   if (refeicoes[4]==1)  refeicao=refeicao+16;
   if (bebidas[0]==1)  bebida=1;
   if (bebidas[1]==1)  bebida=bebida+2;
   if (bebidas[2]==1)  bebida=bebida+4;
   if (bebidas[3]==1)  bebida=bebida+8;
   if (bebidas[4]==1)  bebida=bebida+16;
   if (sobremesas[0]==1)  sobremesa=1;
   if (sobremesas[1]==1)  sobremesa=sobremesa+2;
   if (sobremesas[2]==1)  sobremesa=sobremesa+4;
   if (sobremesas[3]==1)  sobremesa=sobremesa+8;
   if (sobremesas[4]==1)  sobremesa=sobremesa+16;
}
//////////////////////////////////////////////////////////////////////////////
void Conta(void)
{
}
//////////////////////////////////////////////////////////////////////////////
void Tela(void)
{
   unsigned i;
   Status coisa;
   struct  time prato;
   char *texto;
   char buffer1[144];
   char buffer999[928];
   char c;
   int raiz,tronco;
   char rx[5],bx[5],sx[5],ry[5],by[5],sy[5];
   for (i=0;i<=4;i++)
   {
      refeicoes[i]=0;
      bebidas[i]=0;
      sobremesas[i]=0;
      rx[i]=5;  bx[i]=37;
      ry[i]=i+9;by[i]=i+9;
      sx[i]=5;  sy[i]=17+i;
   }
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
   texto="Net BAR";
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
   for(i=3;i<=62;i++)
   {
      gotoxy(i,6);  putch(196);
      gotoxy(i,23); putch(196);
   }
   gotoxy(64,23); putch(207); gotoxy(64,6); putch(209);
   DesenhaCardapio();
   DrawButton();
   ShowCursor();
   Cursor(25,0);
   textbackground(DARKGRAY);textcolor(WHITE);
   while(!0)
   {
      if (kbhit()) c=getch();
      coisa=GetStatus();
      for(i=0;i<=4;i++)
      {
         if (((coisa.x==rx[i])&&(coisa.y==ry[i]))&&(coisa.Button==1))
         {
             HideCursor();
             textcolor(LIGHTMAGENTA);
             textbackground(BLUE);
             gotoxy(coisa.x,coisa.y);
             if (refeicoes[i]==0)
             {
                putch('X');
                refeicoes[i]=1;
             }
             else
             if (refeicoes[i]==1)
             {
                putch(' ');
                refeicoes[i]=0;
             }
             textcolor(WHITE);
             ShowCursor();
         }
         if (((coisa.x==bx[i])&&(coisa.y==by[i]))&&(coisa.Button==1))
         {
             HideCursor();
             textcolor(LIGHTMAGENTA);
             textbackground(BLUE);
             gotoxy(coisa.x,coisa.y);
             if (bebidas[i]==0)
             {
                putch('X');
                bebidas[i]=1;
             }
             else
             if (bebidas[i]==1)
             {
                putch(' ');
                bebidas[i]=0;
             }
             textcolor(WHITE);
             ShowCursor();
         }
         if (((coisa.x==sx[i])&&(coisa.y==sy[i]))&&(coisa.Button==1))
         {
             HideCursor();
             textcolor(LIGHTMAGENTA);
             textbackground(BLUE);
             gotoxy(coisa.x,coisa.y);
             if (sobremesas[i]==0)
             {
                putch('X');
                sobremesas[i]=1;
             }
             else
             if (sobremesas[i]==1)
             {
                putch(' ');
                sobremesas[i]=0;
             }
             textcolor(WHITE);
             ShowCursor();
         }
      }
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
            textbackground(WHITE);cprintf("  Enviar  ");
            Enviar();
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
            textbackground(WHITE);cprintf("  Conta   ");
            Conta();
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
            textbackground(WHITE);cprintf(" Cadastro ");
            gettext(8,11,68,18,buffer999);
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
            textbackground(WHITE);cprintf(" Rel¢gio  ");
            textcolor(YELLOW); textbackground(BLACK);
            gettext(28,11,51,13,buffer1);
            window(28,11,51,13); clrscr(); window(1,1,79,25);
            gotoxy(28,11); cprintf("ÚÄÄÄÄÄÄ Rel¢gio ÄÄÄÄÄÄ¿");
            gotoxy(28,12); cprintf("³                     ³");
            gotoxy(28,13); cprintf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ");
            ShowCursor();
            while(1)
            {
               coisa=GetStatus();
               gettime(&prato);
               gotoxy(34,12);
               cprintf(" %2d:%02d:%02d ",prato.ti_hour,prato.ti_min,prato.ti_sec);
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
            textbackground(WHITE);cprintf("  Gar‡om  ");
            textcolor(YELLOW); textbackground(BLACK);
            gettext(28,11,51,13,buffer1);
            window(28,11,51,13); clrscr(); window(1,1,79,25);
            gotoxy(28,11); cprintf("ÚÄÄÄÄÄÄ Gar‡om ÄÄÄÄÄÄÄ¿");
            gotoxy(28,12); cprintf("³ Nao implementado... ³");
            gotoxy(28,13); cprintf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ");
            ShowCursor();
            while(1)
            {
               coisa=GetStatus();
               if (kbhit())
               {
                  c=getch();
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
            textbackground(WHITE);cprintf("  Pre‡os  ");
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
   InitMouse();
   Tela();
}
