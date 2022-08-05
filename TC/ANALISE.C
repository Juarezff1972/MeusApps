/*
      Alunos: (equipe Coyote)  Anderson Ferreira Martins
                               Gustavo Henrique Busnardo
                               Juarez Fioravanti Filho
                               Ricardo Cividanes da Silva
*/
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <alloc.h>
#include <process.h>
///////////////////////////////////////////////////////////////////////
#define VERDADE        1
#define FALSO          0
#define MAIS           0
#define MENOS          1
#define VEZES          2
#define DIVIDIR        3
#define EXP            4
#define MODULO         5
#define APAREN         6
#define FPAREN         7
#define NUM            8
#define VAR            9
#define FUNC           10
#define EOLN           11
#define RUIM           12
#define TAMMAXNOMEFUNC 5
#define CALCTAMPILHA   20
#define MSGERROPILHA   "\n\rERRO: estouro de pilha.\n\r"
#define MAXCOLS        1
#define MAXLINHAS      1
///////////////////////////////////////////////////////////////////////
struct REGTOKEN
{
   char estado;
   union
   {
      double valor;
      struct
      {
         int lin, col;
      } c;
      char nomefunc[TAMMAXNOMEFUNC + 1];
   } x;
};
///////////////////////////////
struct REGVAR
{
   union
   {
      char texto[8];
      double valor;
   } v;
};
///////////////////////////////
static struct REGTOKEN pilha[CALCTAMPILHA], token_atual;
int topo_pilha, tipo_token, erro;
char *entrada, isformula;
char *funcao,*func,*fiter;
int *auxptr;
int found=0,pritm;
double xx,yy,zz;
int sx,sy,ca;
double prec,scale;
unsigned precalcy1[201];
typedef struct REGVAR *VARPTR;
VARPTR var[MAXCOLS][MAXLINHAS];
///////////////////////////////////////////////////////////////////////
int matherr(struct exception *e)
{
   e->retval = HUGE_VAL;
   return(1);
}
///////////////////////////////////////////////////////////////////////
void tela_inicio(char what)
{
   unsigned i=1,j,k;
   char c=1;
   gotoxy(1,7+what);
   cprintf("Entre com a fun‡„o de vari vel ' x ' :");
   gotoxy(1,9+what);  putch('[');
   gotoxy(64,9+what); putch(']');
   gotoxy(3,9+what);
   funcao[0]=32;
   funcao[i]=0;
   funcao[i+1]=0;
   while(1)
   {
      if (kbhit()) c=getch();
      if (c==13) break;
      if (c==27)
      {
         free(func);
         free(fiter);
         free(auxptr);
         free(funcao);
         exit(0);
      }
      if (c>31)
      {
         if (funcao[i]==0)
         {
            funcao[i]=c;
            gotoxy(2+i,9+what);
            putch(funcao[i]);
            funcao[i+1]=0;
            i++;
         }
         else
         if (strlen(funcao)<61)
         {
            j=strlen(funcao);
            for(k=j;k>=i;k--)
            {
               funcao[k]=funcao[k-1];
               gotoxy(2+k,9+what);
               putch(funcao[k]);
            }
            funcao[j+1]=0;
            funcao[i]=c;
            gotoxy(2+i,9+what);
            putch(funcao[i]);
            i++;
         }
      }
      if (c==8)
      {
         if (funcao[i]==0)
         {
            i--;
            funcao[i]=0;
            gotoxy(2+i,9+what);
            putch(32);
         }
         else
         {
            i--;
            j=strlen(funcao);
            for(k=i;k<=j;k++)
            {
               funcao[k]=funcao[k+1];
               gotoxy(2+k,9+what);putch(funcao[k]);
            }
            funcao[j]=0;
         }
      }
      if (c==0) c=getch();
      if (c==77) {if(funcao[i]==0) i--; i++;}
      if (c==75) i--;
      if (c==79) i=strlen(funcao);
      if (c==83)
      for(k=i;k<=strlen(funcao);k++)
      {
         funcao[k]=funcao[k+1];
         gotoxy(2+k,9+what);putch(funcao[k]);
      }
      c=1;
      if(i<1)  i=1;
      if(i>60) i=60;
      gotoxy(2+i,9+what);
   }
}
//////////////////////////////////////////////////////////////////////
int formula_inicio(char **entrada, int *col, int *lin)
{
   int tam, maxtam = 2;
   char *inicio, numstring[10];
   if (!isalpha(**entrada)) return(FALSO);
   *col = *((*entrada)++) - 'A';
   if (isalpha(**entrada))
   {
      *col *= 26;
      *col += *((*entrada)++) - 'A' + 26;
   }
   if (*col >= MAXCOLS) return(FALSO);
   inicio = *entrada;
   for (tam = 0; tam < maxtam; tam++)
   {
      if (!isdigit(*((*entrada)++)))
      {
         (*entrada)--;
         break;
      }
   }
   if (tam == 0) return(FALSO);
   strncpy(numstring, inicio, tam);
   numstring[tam] = 0;
   *lin = atoi(numstring) - 1;
   if ((*lin >= MAXLINHAS) || (*lin == -1)) return(FALSO);
   return(VERDADE);
}
///////////////////////////////////////////////////////////////////////
int isfunc(char *s)
{
   int tam = strlen(s);
   if (strncmp(s, entrada, tam) == 0)
   {
      strncpy(token_atual.x.nomefunc, entrada, tam);
      token_atual.x.nomefunc[tam] = 0;
      entrada += tam;
      return(VERDADE);
   }
   return(FALSO);
}
///////////////////////////////////////////////////////////////////////
int prox_token(void)
{
   char *inicio, numstring[160];
   int decimal, tam, numtam;
   while (*entrada == ' ') entrada++;
   if (*entrada == 0) return(EOLN);
   if (strchr("0123456789.", *entrada))
   {
      inicio = entrada;
      tam = 0;
      decimal = FALSO;
      while ((isdigit(*entrada)) || ((*entrada == '.') && (!decimal)))
      {
         if (*entrada == '.') decimal = VERDADE;
         entrada++;
         tam++;
      }
      if ((tam == 1) && (inicio[0] == '.')) return(RUIM);
      if (*entrada == 'E')
      {
         entrada++;
         tam++;
         if (strchr("+-", *entrada) != NULL)
         {
            entrada++;
            tam++;
         }
         numtam = 0;
         while ((isdigit(*entrada)) && (++numtam <= 3))
         {
            entrada++;
            tam++;
         }
      }
      strncpy(numstring, inicio, tam);
      numstring[tam] = 0;
      token_atual.x.valor = atof(numstring);
      if (errno == ERANGE) return(RUIM);
      return(NUM);
   }
   else if (isalpha(*entrada))
   {
      if (isfunc("ABS")   || isfunc("ACOS")  || isfunc("ASEN") ||
          isfunc("ATAN")  || isfunc("COSH")  || isfunc("COS")  ||
          isfunc("EA1P")  || isfunc("LOG10") || isfunc("LOG")  ||
          isfunc("POT10") || isfunc("ARD")   || isfunc("SENH") ||
          isfunc("SEN")   || isfunc("RAIZQ") || isfunc("QUAD") ||
          isfunc("TANH")  || isfunc("TAN")   || isfunc("TRUNC"))
          return(FUNC);
      if (formula_inicio(&entrada,&token_atual.x.c.col,&token_atual.x.c.lin))
      {
         isformula = VERDADE;
         return(VAR);
      }
      else return(RUIM);
   }
   else switch(*(entrada++))
   {
      case '+' : return(MAIS);
      case '-' : return(MENOS);
      case '*' : return(VEZES);
      case '/' : return(DIVIDIR);
      case '^' : return(EXP);
      case ':' : return(MODULO);
      case '(' : return(APAREN);
      case ')' : return(FPAREN);
      default  : return(RUIM);
   }
}
///////////////////////////////////////////////////////////////////////
void push(struct REGTOKEN *token)
{
   if (topo_pilha == CALCTAMPILHA - 1)
   {
      cprintf(MSGERROPILHA);
      erro = VERDADE;
   }
   else pilha[++topo_pilha] = *token;
}
///////////////////////////////////////////////////////////////////////
struct REGTOKEN pop(void)
{
   return(pilha[topo_pilha--]);
}
///////////////////////////////////////////////////////////////////////
int vai_pra_estado(int producao)
{
   int estado = pilha[topo_pilha].estado;
   if (producao <= 3)
   {
      switch(estado)
      {
         case 0 : return(1);
         case 9 : return(19);
         case 20 : return(28);
      }
   }
   else if (producao <= 6)
   {
      switch(estado)
      {
         case 0 :
         case 9 :
         case 20 : return(2);
         case 12 : return(21);
         case 13 : return(22);
      }
   }
   else if (producao <= 8)
   {
      switch(estado)
      {
         case 0 :
         case 9 :
         case 12 :
         case 13 :
         case 20 : return(3);
         case 14 : return(23);
         case 15 : return(24);
         case 16 : return(25);
      }
   }
   else if (producao <= 10)
   {
      switch(estado)
      {
         case 0 :
         case 9 :
         case 12 :
         case 13 :
         case 14 :
         case 15 :
         case 16 :
         case 20 : return(4);
      }
   }
   else if (producao <= 12)
   {
      switch(estado)
      {
         case 0 :
         case 9 :
         case 12 :
         case 13 :
         case 14 :
         case 15 :
         case 16 :
         case 20 : return(6);
         case 5 : return(17);
      }
   }
   else
   {
      switch(estado)
      {
         case 0 :
         case 5 :
         case 9 :
         case 12 :
         case 13 :
         case 14 :
         case 15 :
         case 16 :
         case 20 : return(8);
      }
   }
   return(30);
}
///////////////////////////////////////////////////////////////////////
double varvalor(int col, int lin)
{
   if (var[col][lin] == NULL) return(0);
   return(var[col][lin]->v.valor);
}
///////////////////////////////////////////////////////////////////////
void desloca(int estado)
{
   token_atual.estado = estado;
   push(&token_atual);
   tipo_token = prox_token();
}
///////////////////////////////////////////////////////////////////////
void reduza(int reducao)
{
   struct REGTOKEN token1, token2;
   int counter;
   switch (reducao)
   {
      case 1 :
           token1 = pop();
           pop();
           token2 = pop();
           token_atual.x.valor = token1.x.valor + token2.x.valor;
      break;
      case 2 :
           token1 = pop();
           pop();
           token2 = pop();
           token_atual.x.valor = token2.x.valor - token1.x.valor;
      break;
      case 4 :
           token1 = pop();
           pop();
           token2 = pop();
           token_atual.x.valor = token1.x.valor * token2.x.valor;
      break;
      case 5 :
           token1 = pop();
           pop();
           token2 = pop();
           if (fabs(token1.x.valor) == 0) token_atual.x.valor = HUGE_VAL;
           else token_atual.x.valor = token2.x.valor / token1.x.valor;
      break;
      case 7 :
           token1 = pop();
           pop();
           token2 = pop();
           token_atual.x.valor = pow(token2.x.valor, token1.x.valor);
      break;
      case 9 :
           token1 = pop();
           pop();
           token_atual.x.valor = -token1.x.valor;
      break;
      case 11 :
           token1 = pop();
           pop();
           token2 = pop();
           if (fabs(token1.x.valor) == 0) token_atual.x.valor = HUGE_VAL;
           else token_atual.x.valor=(double)((long)token2.x.valor % (long)token1.x.valor);
      break;
      case 13 :
           token_atual = pop();
           token_atual.x.valor=varvalor(token_atual.x.c.col,token_atual.x.c.lin);
      break;
      case 14 :
           pop();
           token_atual = pop();
           pop();
      break;
      case 16 :
           pop();
           token_atual = pop();
           pop();
           token1 = pop();
           if (strcmp(token1.x.nomefunc, "ABS") == 0)
              token_atual.x.valor = fabs(token_atual.x.valor);
           else
           if (strcmp(token1.x.nomefunc, "ACOS") == 0)
              token_atual.x.valor = acos(token_atual.x.valor);
           else
           if (strcmp(token1.x.nomefunc, "ASEN") == 0)
              token_atual.x.valor = asin(token_atual.x.valor);
           else
           if (strcmp(token1.x.nomefunc, "ATAN") == 0)
              token_atual.x.valor = atan(token_atual.x.valor);
           else
           if (strcmp(token1.x.nomefunc, "COSH") == 0)
              token_atual.x.valor = cosh(token_atual.x.valor);
           else
           if (strcmp(token1.x.nomefunc, "COS") == 0)
              token_atual.x.valor = cos(token_atual.x.valor);
           else
           if (strcmp(token1.x.nomefunc, "EA1P") == 0)
              token_atual.x.valor = exp(token_atual.x.valor);
           else
           if (strcmp(token1.x.nomefunc, "LOG10") == 0)
              token_atual.x.valor = log10(token_atual.x.valor);
           else
           if (strcmp(token1.x.nomefunc, "LOG") == 0)
              token_atual.x.valor = log(token_atual.x.valor);
           else
           if (strcmp(token1.x.nomefunc, "ARD") == 0)
              token_atual.x.valor = (int)(token_atual.x.valor + 0.5);
           else
           if (strcmp(token1.x.nomefunc, "POT10") == 0)
              token_atual.x.valor = pow10(token_atual.x.valor);
           else
           if (strcmp(token1.x.nomefunc, "SENH") == 0)
              token_atual.x.valor = sinh(token_atual.x.valor);
           else
           if (strcmp(token1.x.nomefunc, "SEN") == 0)
              token_atual.x.valor = sin(token_atual.x.valor);
           else
           if (strcmp(token1.x.nomefunc, "RAIZQ") == 0)
              token_atual.x.valor = sqrt(token_atual.x.valor);
           else
           if (strcmp(token1.x.nomefunc, "QUAD") == 0)
              token_atual.x.valor *= token_atual.x.valor;
           else
           if (strcmp(token1.x.nomefunc, "TANH") == 0)
              token_atual.x.valor = tanh(token_atual.x.valor);
           else
           if (strcmp(token1.x.nomefunc, "TAN") == 0)
              token_atual.x.valor = tan(token_atual.x.valor);
           else
           if (strcmp(token1.x.nomefunc, "TRUNC") == 0)
              token_atual.x.valor = (int)token_atual.x.valor;
      break;
      case 3 :
      case 6 :
      case 8 :
      case 10 :
      case 12 :
      case 15 :
           token_atual = pop();
      break;
   }
   token_atual.estado = vai_pra_estado(reducao);
   push(&token_atual);
}
///////////////////////////////////////////////////////////////////////
double calcula(char *s)
{
   struct REGTOKEN firsttoken;
   char aceito = FALSO;
   char copia[120];
   erro = FALSO;
   isformula = FALSO;
   entrada = copia;
   strupr(strcpy(copia, s));
   topo_pilha = -1;
   firsttoken.estado = 0;
   firsttoken.x.valor = 0;
   push(&firsttoken);
   tipo_token = prox_token();
   do
   {
      switch (pilha[topo_pilha].estado)
      {
         case  0 :
         case  9 :
         case 12 :
         case 13 :
         case 14 :
         case 15 :
         case 16 :
         case 20 :
              if (tipo_token == NUM) desloca(10);
              else if (tipo_token == VAR) desloca(7);
                   else if (tipo_token == FUNC) desloca(11);
                        else if (tipo_token == MENOS) desloca(5);
                             else if (tipo_token == APAREN) desloca(9);
                                  else erro = VERDADE;
         break;
         case 1 :
              if (tipo_token == EOLN) aceito = VERDADE;
              else if (tipo_token == MAIS) desloca(12);
                   else if (tipo_token == MENOS) desloca(13);
                        else erro = VERDADE;
                        break;
         case 2 :
              if (tipo_token == VEZES) desloca(14);
              else if (tipo_token == DIVIDIR) desloca(15);
                   else reduza(3);
         break;
         case 3 :
              reduza(6);
         break;
         case 4 :
              if (tipo_token == EXP) desloca(16);
              else reduza(8);
         break;
         case 5 :
              if (tipo_token == NUM) desloca(10);
              else if (tipo_token == VAR) desloca(7);
                   else if (tipo_token == FUNC) desloca(11);
                        else if (tipo_token == APAREN) desloca(9);
                             else erro = VERDADE;
         break;
         case 6 :
              reduza(10);
         break;
         case 7 :
              if (tipo_token == MODULO) desloca(18);
              else reduza(13);
         break;
         case 8 :
              reduza(12);
         break;
         case 10 :
              reduza(15);
         break;
         case 11 :
              if (tipo_token == APAREN) desloca(20);
              else erro = VERDADE;
         break;
         case 17 :
              reduza(9);
         break;
         case 18 :
              if (tipo_token == VAR) desloca(26);
              else erro = VERDADE;
         break;
         case 19 :
              if (tipo_token == MAIS) desloca(12);
              else if (tipo_token == MENOS) desloca(13);
                   else if (tipo_token == FPAREN) desloca(27);
                        else erro = VERDADE;
         break;
         case 21 :
              if (tipo_token == VEZES) desloca(14);
              else if (tipo_token == DIVIDIR) desloca(15);
                   else reduza(1);
         break;
         case 22 :
              if (tipo_token == VEZES) desloca(14);
              else if (tipo_token == DIVIDIR) desloca(15);
                   else reduza(2);
         break;
         case 23 :
              reduza(4);
         break;
         case 24 :
              reduza(5);
         break;
         case 25 :
              reduza(7);
         break;
         case 26 :
              reduza(11);
         break;
         case 27 :
              reduza(14);
         break;
         case 28 :
              if (tipo_token == MAIS) desloca(12);
              else if (tipo_token == MENOS) desloca(13);
                   else if (tipo_token == FPAREN) desloca(29);
                        else erro = VERDADE;
         break;
         case 29 :
              reduza(16);
         break;
         case 30 :
              erro = VERDADE;
         break;
      }
   }
   while ((!aceito) && (!erro));
   if (erro) return(0);
   strcpy(s, copia);
   return(pilha[topo_pilha].x.valor);
}
///////////////////////////////////////////////////////////////////////
double ff(double x)
{
   double tmp;
   var[0][0]->v.valor=x;
   tmp=calcula(func);
   return tmp;
}
///////////////////////////////////////////////////////////////////////
double fi(double x)
{
   double tmp;
   var[0][0]->v.valor=x;
   tmp=calcula(fiter);
   return tmp;
}
///////////////////////////////////////////////////////////////////////
int sinal(double x)
{
   if (x>0) return 1;
   if (x<0) return -1;
   return 0;
}
///////////////////////////////////////////////////////////////////////
int cmpr(double first,double second)
{
   double sign;
   char frst[60];
   sprintf(frst,"%.*f",pritm,(first-second));
   sign=fabs(atof(frst));
   if(sign==0.0) return 0; else return 1;
}
///////////////////////////////////////////////////////////////////////
void calc3d(double x,double y,double z)
{
   x=(-1)*x;z=z-350.0;if(z==0)z=1;
   sx=1200.0*x/z;sy=1200.0*y/z;
   sx=sx+399.0;sy=sy+299.0;sx=sx*(319.0/799.0);sy=sy*(199.0/599.0);
}
///////////////////////////////////////////////////////////////////////
void putpixel(unsigned x,unsigned y,unsigned char cor)
{
   unsigned offst;
   if (((x>0)&&(x<320))&&((y>0)&&(y<200)))
   {
      offst=precalcy1[(unsigned char)y]+((unsigned)x);
      asm{
          mov ax,0xa000
          mov es,ax
          mov bx,offst
          mov dh,cor
          mov byte ptr es:[bx],dh
      }
   }
}
///////////////////////////////////////////////////////////////////////
void sombra(int x,int y)
{
   unsigned offst;
   unsigned char cor;
   x=x-((200-y)/3);
   calc3d(((double)x-160.0)/1.5,-90.0,(double)y-200.0);
   offst=precalcy1[(unsigned char)sy]+((unsigned)sx);
   asm{
       mov ax,0xa000
       mov es,ax
       mov bx,offst
       mov dh,byte ptr es:[bx]
       mov cor,dh
   }
   if (cor==12) putpixel(sx,sy,4);
   if (cor==11) putpixel(sx,sy,3);
}
///////////////////////////////////////////////////////////////////////
void vline(unsigned bock,int chis,int ipslon,unsigned cor)
{
   int swapvar;
   if (chis>ipslon) {  swapvar=chis; chis=ipslon; ipslon=swapvar; }
   for (swapvar=chis;swapvar<=ipslon;swapvar++)
   {
      putpixel(bock,swapvar,cor);
      sombra(bock,swapvar);
   }
}
///////////////////////////////////////////////////////////////////////
void plotfunc(int x,unsigned char cor,char _)
{
   double f,tmp;
   unsigned char glitch;
   f=ff(x*scale);
   tmp=((199.0-((f/scale)/3.0))-99.0);
   if (abs(f/3.0)<=99)
   {
      if (_==1)
         vline(x+159,(int)tmp,100,cor);
      else
      {
         putpixel(x+159,(int)tmp,cor);
         sombra(x+159,(int)tmp);
      }
   }
   putpixel(x+159,100,cor-1);
}
///////////////////////////////////////////////////////////////////////
double raizbisec(double a,double b)
{
   double x,y,q,w,e,k;
   if (a>b) {  y=a; a=b; b=y; }
   x=(a+b)/2;
   q=ff(a);
   w=ff(b);
   e=ff(x);
   plotfunc(x/scale,10,1);
   if ((cmpr(x,a)==0)&&(fabs(e)<=prec)) { found=1; return x; }
   if (cmpr(x,a)==0) { found=0; return x; }
   if (fabs(e)<=prec) { found=1; return x; }
   if ((sinal(q)!=sinal(e))&&(found!=1)) return raizbisec(a,x);
   if ((sinal(w)!=sinal(e))&&(found!=1)) return raizbisec(x,b);
   if (((sinal(w)==sinal(e))&&(sinal(q)==sinal(e)))&&(found!=1))
   {
      y=raizbisec(a,x);
      if (found!=1) y=raizbisec(x,b);
      return y;
   }
   return x;
}
///////////////////////////////////////////////////////////////////////
double pos_falsa(double a,double b)
{
   double x,y,q,w,e,k;
   if (a>b) { y=a; a=b; b=y; }
   if (fabs(b-a) < prec) {found=1;return (a+b)/2;}
   q=ff(a);
   w=ff(b);
   if (fabs(q) < prec) {found=1;return a;}
   if (fabs(w) < prec) {found=1;return b;}
   while(!0)
   {
      if(w==q) x=HUGE_VAL; else x=((a*w)-(b*q))/(w-q);
      e=ff(x);
      plotfunc(x/scale,10,1);
      if ((cmpr(x,a)==0)&&(fabs(e)<prec)) { found=1; return x; }
      if (cmpr(x,a)==0) { found=0; return x; }
      if (fabs(e)<prec) { found=1; return x; }
      if ((q*e) > 0) {a=x; q=ff(a);} else {b=x; w=ff(b);}
      if (fabs(b-a) < prec) {found=1;return (a+b)/2;}
   }
}
///////////////////////////////////////////////////////////////////////
double pos_falsa_mod(double a,double b)
{
   double x,y,q,w,e,k,x0;
   if (a>b) { y=a; a=b; b=y; }
   q=ff(a);
   w=ff(b);
   x0=a;
   if (fabs(b-a) < prec) {found=1;return a;}
   if (fabs(q) < prec) {found=1;return a;}
   if (fabs(w) < prec) {found=1;return b;}
   while(!0)
   {
      if(w==q) x=HUGE_VAL; else x=((a*w)-(b*q))/(w-q);
      e=ff(x);
      plotfunc(x/scale,10,1);
      if (fabs(b-a)<prec) { found=1; return x; }
      if (fabs(e)<prec) { found=1; return x; }
      if (cmpr(x,a)==0) { found=0; return x; }
      if ((q*e) > 0) {a=x; q=ff(a);if((q*ff(x0))>0){w=w/2;}}
      else {b=x; w=ff(b); if((w*ff(x0))>0){q=q/2;}}
      x0=x;
   }
}
///////////////////////////////////////////////////////////////////////
double secante(double gx,double gy,double a,double b)
{
   double x,y,q,w,e,k;
   if (a>b) { y=a; a=b; b=y; }
   q=ff(a);
   w=ff(b);
   if (fabs(q) < prec) {found=1;return a;}
   if (fabs(w) < prec) {found=1;return b;}
   if (fabs(b-a) < prec) {found=1;return b;}
   while(!0)
   {
      q=ff(a); w=ff(b);
      if(w==q) x=HUGE_VAL; else x=b-(w/(w-q))*(b-a);
      e=ff(x);
      if ((x>gx)&&(x<gy)) plotfunc(x/scale,10,1);
      if (fabs(x-b)<prec) { found=1; return x; }
      if (fabs(e)<prec) { found=1; return x; }
      if (cmpr(x,a)==0) { found=0; return x; }
      a=b;
      b=x;
   }
}
///////////////////////////////////////////////////////////////////////
double mil(double a,double olho,double nariz)
{
   double x,y,q;
   int k=0;
   q=ff(a);
   if (fabs(q)<prec) {found=1;return a;}
   while (!0)
   {
      x=fi(a);
      y=ff(x);
      if ((olho<x)&&(x<nariz)) plotfunc(x/scale,10,1);
      if ((fabs(y)<prec)||(fabs(x-a)<prec)) {found=1;return x;}
      a=x;
      k++;
      if (k>ca) {found=0;return x;}
   }
}
///////////////////////////////////////////////////////////////////////
double newton(double a,double olho,double nariz)
{
   double x,y,q,w;
   int k=0;
   q=ff(a);
   if (fabs(q)<prec) {found=1;return a;}
   while (!0)
   {
      w=fi(a);
      q=ff(a);
      x=a-(q/w);
      y=ff(x);
      if ((olho<x)&&(x<nariz)) plotfunc(x/scale,10,1);
      if ((fabs(y)<prec)||(fabs(x-a)<prec)) {found=1;return x;}
      a=x;
      k++;
      if (k>ca) {found=0;return x;}
   }
}
///////////////////////////////////////////////////////////////////////
unsigned decplaces(char *val)
{
   unsigned i,j,k,l;
   j=0;
   k=0;
   l=0;
   i=strlen(val);
   while(1)
   {
      if (j==i) break;
      if ((j!=i)&&(k==1)) l++;
      if (val[j]=='.') k=1;
      j++;
   }
   return l;
}
///////////////////////////////////////////////////////////////////////
void drawfunc(double inicio,double final)
{
   unsigned j,offst;
   double x,f,tmp,scale,y;
   double startval=-158.0,endval=158.0;
   unsigned char floor=12,checker=0;
   if (inicio>final)
   {
      y=inicio;
      inicio=final;
      final=y;
   }
   if ((sinal(inicio)==-1) && (sinal(final)<=0))
   {
      startval=-158.0;
      endval=0.0;
   }
   if ((sinal(inicio)>=0) && (sinal(final)==1))
   {
      startval=0.0;
      endval=158.0;
   }
   for(j=0;j<=200;j++) precalcy1[j]=320*j;
   asm{
      mov ah,0
      mov al,0x13
      int 0x10
      mov ax,0xa000
      xor di,di
      mov es,ax
      mov cx,0xfa00
      mov al,0
      mov ah,al
      cld
      rep stosw
   }
   for(xx=-360.0;xx<=0.0;xx=xx+1.0)
   {
      if (((long)((xx+160.0)/30.0)%2)==0) checker=0; else checker=1;
      for(yy=-400.0;yy<=100.0;yy=yy+1.0)
      {
         if (((long)yy%40)==0) checker=!checker;
         calc3d(xx/1.5,-90.0,yy-90.0);
         if (checker==0) floor=12;
         if (checker==1) floor=11;
         putpixel(sx,sy,floor);
         putpixel(320-sx,sy,floor);
      }
   }
   for(j=0;j<=299;j++)
   {
      putpixel(158,j,26);sombra(158,j);sombra(158,-j);
      putpixel(159,j,28);sombra(159,j);sombra(159,-j);
      putpixel(160,j,15);sombra(160,j);sombra(160,-j);
      putpixel(161,j,28);sombra(161,j);sombra(161,-j);
      putpixel(162,j,26);sombra(162,j);sombra(162,-j);
   }
   for(j=0;j<=319;j++)
   {
      putpixel(j,98,26);sombra(j,98);
      putpixel(j,99,28);sombra(j,99);
      putpixel(j,100,15);sombra(j,100);
      putpixel(j,101,28);sombra(j,101);
      putpixel(j,102,26);sombra(j,102);
   }
   for(x=startval;x<=endval;x=x+0.01) plotfunc(x,14,0);
   getch();
}
///////////////////////////////////////////////////////////////////////
void wasfound(char achou,double a)
{
   unsigned x,y;
   char outc[40],aux;
   for(x=0;x<=40;x++) outc[x]=32;
   for(x=0;x<=320;x++)
      for(y=0;y<=20;y++)
      {
         putpixel(x,y,8);
         sombra(x,y);
      }
   if (achou==1) sprintf(outc,"Achei!"); else sprintf(outc,"N„o achei...");
   for(x=0;x<=39;x++)
   {
      gotoxy(x+1,1);
      aux=outc[x];
      asm{
         mov al,aux
         mov ah,9
         mov bl,3
         mov bh,8
         mov cx,1
         int 0x10
      }
   }
   gotoxy(1,2); sprintf(outc,"f(%.14f)=%.14f",a,ff(a));
   for(x=0;x<=39;x++)
   {
      gotoxy(x+1,2);
      aux=outc[x];
      asm{
         mov al,aux
         mov ah,9
         mov bl,3
         mov bh,8
         mov cx,1
         int 0x10
      }
   }
}
///////////////////////////////////////////////////////////////////////
void main(void)
{
   double a,inicio,final,apinic,apfin;
   int compr;
   int i,ii;
   char chosen;
   char prc[120];
   func=(char *)malloc(120);
   funcao=(char *)malloc(120);
   fiter=(char *)malloc(120);
   auxptr=(int *)malloc(sizeof(***var));
   var[0][0]=auxptr;
   inicio=0.0;final=0.0;apinic=0.0;apfin=0.0;
   clrscr();
   cprintf("Escolha o m‚todo:\n\r\n\r");
   cprintf("1 - Bisec‡„o\n\r");
   cprintf("2 - Posi‡„o Falsa\n\r");
   cprintf("3 - Posi‡„o Falsa Modificada\n\r");
   cprintf("4 - Iterativo Linear\n\r");
   cprintf("5 - Newton - Raphson\n\r");
   cprintf("6 - Secante\n\r");
   cprintf("0 - Sair\n\r\n\r");
   cprintf("Escolha: "); chosen=getche();
   if(chosen=='0'){free(funcao);free(func);free(fiter);free(auxptr);exit(0);}
   clrscr();
   for(i=0;i<120;i++) funcao[i]=32;
   tela_inicio(0);
   strcpy(func,"");
   compr = strlen(funcao);
   for (i=0; i<compr; i++) funcao[i] = toupper(funcao[i]);
   strcpy(func,funcao);
   for(i=0;i<120-compr;i++) strcat(func," ");
   i=0;ii=0;
   while(i<=compr+2)
   {
       if (funcao[ii]=='X')
       {
          func[i]='A';
          func[i+1]='1';
          i=i+2;
       }
       else
       {
          func[i]=funcao[ii];
          i=i+1;
       }
       ii=ii+1;
   }
   func[i]=0;
   free(funcao);
   if ((chosen=='4')||(chosen=='5'))
   {
      cprintf("\n\rItera‡„o:");
      funcao=(char *)malloc(120);
      for(i=0;i<120;i++) funcao[i]=32;
      tela_inicio(4);
      strcpy(fiter,"");
      compr = strlen(funcao);
      for (i=0; i<compr; i++) funcao[i] = toupper(funcao[i]);
      strcpy(fiter,funcao);
      for(i=0;i<120-compr;i++) strcat(fiter," ");
      i=0;ii=0;
      while(i<=compr+2)
      {
         if (funcao[ii]=='X')
         {
            fiter[i]='A';
            fiter[i+1]='1';
            i=i+2;
         }
         else
         {
            fiter[i]=funcao[ii];
            i=i+1;
         }
         ii=ii+1;
      }
      fiter[i]=0;
      free(funcao);
      cprintf("\n\r\n\rEntre com o n£mero de itera‡”es: ");
      scanf("%d",&ca);  fflush(stdin);
      cprintf("\n\r\n\rEntre com o intervalo inicial: ");
      scanf("%lf",&inicio);fflush(stdin);
      cprintf("\n\rEntre com o intervalo final: ");
      scanf("%lf",&final); fflush(stdin);
      cprintf("\n\r\n\rEntre com a aproxima‡„o inicial: ");
      scanf("%lf",&apinic);fflush(stdin);
      cprintf("\n\rEntre com a precis„o: ");
      scanf("%lf",&prec);  fflush(stdin);
      prec=fabs(prec);
      sprintf(prc,"%.14lg",prec);
      pritm=decplaces(prc);
      if (inicio==final)
      {
         if (inicio==0)
         {
            inicio=1;
            final=-1;
         }
         else inicio= -final;
      }
      if (inicio > 159.0) inicio = 159.0;if (inicio < -159.0) inicio = -159.0;
      if (final > 159.0) final = 159.0;if (final < -159.0) final = -159.0;
      if (fabs(inicio)>=fabs(final)) scale=fabs(inicio/158.0);
      else scale=fabs(final/158.0);

      drawfunc(inicio,final);
      if (chosen=='4') a=mil(apinic,inicio,final);
      if (chosen=='5') a=newton(apinic,inicio,final);
   }
   if (chosen<'4')
   {
      cprintf("\n\r\n\rEntre com o valor inicial: ");
      scanf("%lf",&inicio);fflush(stdin);
      cprintf("\n\rEntre com o valor final: ");
      scanf("%lf",&final); fflush(stdin);
      cprintf("\n\rEntre com a precis„o: ");
      scanf("%lf",&prec);  fflush(stdin);
      prec=fabs(prec);
      sprintf(prc,"%.14lg",prec);
      pritm=decplaces(prc);
      if (inicio==final)
      {
         if (inicio==0)
         {
            inicio=1;
            final=-1;
         }
         else inicio= -final;
      }
      if (inicio > 159.0) inicio = 159.0;if (inicio < -159.0) inicio = -159.0;
      if (final > 159.0) final = 159.0;if (final < -159.0) final = -159.0;
      if (fabs(inicio)>=fabs(final)) scale=fabs(inicio/158.0);
      else scale=fabs(final/158.0);
      drawfunc(inicio,final);
      if (chosen=='1') a=raizbisec(inicio,final);
      if (chosen=='2') a=pos_falsa(inicio,final);
      if (chosen=='3') a=pos_falsa_mod(inicio,final);
   }
   if (chosen=='6')
   {
      cprintf("\n\r\n\rEntre com o intervalo inicial: ");
      scanf("%lf",&inicio);fflush(stdin);
      cprintf("\n\rEntre com o intervalo final: ");
      scanf("%lf",&final); fflush(stdin);
      cprintf("\n\r\n\rEntre com a aproxima‡„o inicial: ");
      scanf("%lf",&apinic);fflush(stdin);
      cprintf("\n\rEntre com a aproxima‡„o final: ");
      scanf("%lf",&apfin); fflush(stdin);
      cprintf("\n\rEntre com a precis„o: ");
      scanf("%lf",&prec);  fflush(stdin);
      prec=fabs(prec);
      sprintf(prc,"%.14lg",prec);
      pritm=decplaces(prc);
      if (inicio==final)
      {
         if (inicio==0)
         {
            inicio=1;
            final=-1;
         }
         else inicio= -final;
      }
      if (inicio > 159.0) inicio = 159.0;if (inicio < -159.0) inicio = -159.0;
      if (final > 159.0) final = 159.0;if (final < -159.0) final = -159.0;
      if (fabs(inicio)>=fabs(final)) scale=fabs(inicio/158.0);
      else scale=fabs(final/158.0);
      drawfunc(inicio,final);
      if (chosen=='6') a=secante(inicio,final,apinic,apfin);
   }
   wasfound(found,a);
   getch();
   asm{
      mov ah,0
      mov al,0x3
      int 0x10
   }
   free(func);
   free(fiter);
   free(auxptr);
}

/*
ANALISE.C

   A fun‡„o de X pode conter ate' no m ximo 60 caracteres.
   Opera‡”es:
      <expr> + <expr>    Soma
      <expr> - <expr>    Subtra‡„o
      <expr> * <expr>    Multiplica‡„o
      <expr> / <expr>    Divis„o
      <expr> ^ <expr>    Exponencia‡„o
      <expr> : <expr>    Resto da divis„o inteira
      (<expr>)           Parea‡„o
      ABS(<expr>)        M¢dulo
      ACOS(<expr>)       Arco-cosseno
      ARD(<expr>)        Parte inteira (arredonda)
      ASEN(<expr>)       Arco-seno
      ATAN(<expr>)       Arco-tangente
      COS(<expr>)        Cosseno
      COSH(<expr>)       Cosseno hiperb¢lico
      EXP(<expr>)        e(2.71828182845904523536) elevado a <expr>
      LOG10(<expr>)      Logar¡tmo base 10
      LOG(<expr>)        Logar¡tmo base e
      POT10(<expr>)      10 elevado a <expr>
      QUAD(<expr>)       <expr> elevado a 2
      RAIZQ(<expr>)      Raiz quadrada de <expr>
      SEN(<expr>)        Seno
      SENH(<expr>)       Seno hiperb¢lico
      TAN(<expr>)        Tangente
      TANH(<expr>)       Tangente hiperb¢lica
      TRUNC(<expr>)      Parte inteira (trunca)
*/
