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
char *funcao0,*funcao1,*funcao2,*funcao3,*funcao4,*func;
int *auxptr;
int chosen;
double g[6][6],a[6];
typedef struct REGVAR *VARPTR;
double xt[20], fx[20];
double eqm;
int numtabela;
VARPTR var[MAXCOLS][MAXLINHAS];

///////////////////////////////////////////////////////////////////////
int matherr(struct exception *e);
void tela_inicio(char what);
int formula_inicio(char **entrada, int *col, int *lin);
int isfunc(char *s);
int prox_token(void);
void push(struct REGTOKEN *token);
struct REGTOKEN pop(void);
double varvalor(int col, int lin);
void desloca(int estado);
void reduza(int reducao);
double calcula(char *s);
double gg(double x,unsigned char quack);
void minquad(void);
void Escalona(void);
void Mostra_Resultado(void);
void Cor(int Cor_Frente, int Cor_Fundo);
void Janela(char X_S, char Y_S, char X_I, char Y_I, char T, char Preenche);
void Mostra_Texto(int X, int Y, char *texto);
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
   gotoxy(1,7);
   cprintf("Entre com a fun‡„o g%d de vari vel ' x '(g0(x), g1(x), ...) :",what);
   gotoxy(1,9);  putch('[');
   gotoxy(64,9); putch(']');
   gotoxy(3,9);
   func[0]=32;
   func[i]=0;
   func[i+1]=0;
   while(1)
   {
      if (kbhit()) c=getch();
      if (c==13) break;
      if (c==27)
      {
         free(funcao0);
         free(funcao1);
         free(funcao2);
         free(funcao3);
         free(funcao4);
         free(func);
         free(auxptr);
         exit(0);
      }
      if (c>31)
      {
         if (func[i]==0)
         {
            func[i]=c;
            gotoxy(2+i,9);
            putch(func[i]);
            func[i+1]=0;
            i++;
         }
         else
         if (strlen(func)<61)
         {
            j=strlen(func);
            for(k=j;k>=i;k--)
            {
               func[k]=func[k-1];
               gotoxy(2+k,9);
               putch(func[k]);
            }
            func[j+1]=0;
            func[i]=c;
            gotoxy(2+i,9);
            putch(func[i]);
            i++;
         }
      }
      if (c==8)
      {
         if (func[i]==0)
         {
            i--;
            func[i]=0;
            gotoxy(2+i,9);
            putch(32);
         }
         else
         {
            i--;
            j=strlen(func);
            for(k=i;k<=j;k++)
            {
               func[k]=func[k+1];
               gotoxy(2+k,9);putch(func[k]);
            }
            func[j]=0;
         }
      }
      if (c==0) c=getch();
      if (c==77) {if(func[i]==0) i--; i++;}
      if (c==75) i--;
      if (c==79) i=strlen(func);
      if (c==83)
      for(k=i;k<=strlen(func);k++)
      {
         func[k]=func[k+1];
         gotoxy(2+k,9);putch(func[k]);
      }
      c=1;
      if(i<1)  i=1;
      if(i>60) i=60;
      gotoxy(2+i,9);
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
double gg(double x,unsigned char quack)
{
   double tmp;
   var[0][0]->v.valor=x;
   if (quack==0) tmp=calcula(funcao0);
   if (quack==1) tmp=calcula(funcao1);
   if (quack==2) tmp=calcula(funcao2);
   if (quack==3) tmp=calcula(funcao3);
   if (quack==4) tmp=calcula(funcao4);
   return tmp;
}
///////////////////////////////////////////////////////////////////////
void erroquad(void)
{
   int i,n;
   double erro[6];
   eqm=0;
   for(i=0;i<6;i++) erro[i]=0.0;
   for(n=0;n<=chosen;n++)
      for(i=0;i<numtabela;i++)
         erro[n]=erro[n]+(fabs(fx[i]-gg(xt[i],n)));
   for(n=0;n<=chosen;n++)
      eqm=eqm+erro[n];
   eqm=eqm/(chosen+1.0);
   eqm*=eqm;
}
///////////////////////////////////////////////////////////////////////
void minquad(void)
{
   int i,n,k;
   for(i=0;i<=chosen;i++)
      for(n=0;n<=chosen;n++)
         for(k=0;k<numtabela;k++)
            g[i][n]=g[i][n]+(gg(xt[k],i)*gg(xt[k],n));
   for(i=0;i<=chosen;i++)
      for(k=0;k<numtabela;k++)
         g[i][chosen+1]=g[i][chosen+1]+(gg(xt[k],i)*fx[k]);
   Escalona();
}
///////////////////////////////////////////////////////////////////////
void Escalona(void)
{
   int i,k,j;
   double c, sum[10];
   for(k=0;k<=chosen-1;k++)
   {
      i=k+1;
      while(i!=chosen+1)
      {
         c=(g[i][k]/g[k][k]);
         for(j=0;j<=chosen+1;j++) g[i][j]=(g[i][j]-(c*g[k][j]));
         i=i+1;
      }
   }
   for(i=0;i<=chosen;i++) sum[i]=0;
   for(j=0;j<=chosen+1;j++) a[j]=0;
   for(i=chosen+1;i>0;i--)
   {
      for(j=chosen+2;j>0;j--)
      {
         sum[i-1]=sum[i-1]+(g[i-1][j-1]*a[j-1]);
      }
      if (g[i-1][i-1]==0.0)
      {
         clrscr();
         printf("ERRO: Impossivel ajustar a curva com as fun‡oes especificadas.\n\n");
         free(funcao0);
         free(funcao1);
         free(funcao2);
         free(funcao3);
         free(funcao4);
         free(func);
         free(auxptr);
         exit(1);
      }
      a[i-1]=(g[i-1][chosen+1]-a[i-1])/g[i-1][i-1];
   }
}
///////////////////////////////////////////////////////////////////////
void Mostra_Funcao(char x,char y,char *funcao)
{
   unsigned tamanho,cont;
   tamanho=strlen(funcao);
   gotoxy(x,y);
   for(cont=0;cont<=tamanho;cont++)
   {
      if ((funcao[cont]!=65) && (funcao[cont+1]!=49)) putch(funcao[cont]);
      if ((funcao[cont]==65) && (funcao[cont+1]==49)) { putch(88); cont++; }
   }
}
///////////////////////////////////////////////////////////////////////
void Mostra_Resultado(void)
{
     clrscr();
     Mostra_Texto(2,1,"Funcoes");
     Mostra_Texto(2,3,"g0(x)=");
     Mostra_Funcao(8,3,funcao0);
     if(chosen>=1)
     {
       Mostra_Texto(2,4,"g1(x)=");
       Mostra_Funcao(8,4,funcao1);
     }
     if(chosen>=2)
     {
       Mostra_Texto(2,5,"g2(x)=");
       Mostra_Funcao(8,5,funcao2);
     }
     if(chosen>=3)
     {
       Mostra_Texto(2,6,"g3(x)=");
       Mostra_Funcao(8,6,funcao3);
     }
     if(chosen>=4)
     {
       Mostra_Texto(2,7,"g4(x)=");
       Mostra_Funcao(8,7,funcao4);
     }

     Cor(CYAN,YELLOW);
     Janela(3,9,48,17,'S',' ');
     Mostra_Texto(5,9," Resultado ");
     Mostra_Texto(5,11,"a0=");
     cprintf("%lf",a[0]);
     if(chosen>=1)
     {
       Mostra_Texto(5,12,"a1=");
       cprintf("%lf",a[1]);
     }
     if(chosen>=2)
     {
       Mostra_Texto(5,13,"a2=");
       cprintf("%lf",a[2]);
     }
     if(chosen>=3)
     {
       Mostra_Texto(5,14,"a3=");
       cprintf("%lf",a[3]);
     }
     if(chosen>=4)
     {
       Mostra_Texto(5,15,"a4=");
       cprintf("%lf",a[4]);
     }
     Mostra_Texto(5,16,"Erro=");
     cprintf("%lf",eqm);
}
///////////////////////////////////////////////////////////////////////
void main(void)
{
   int compr;
   int i,ii,iii;
   char prc[120];

   func=(char *)malloc(120);
   funcao0=(char *)malloc(120);
   funcao1=(char *)malloc(120);
   funcao2=(char *)malloc(120);
   funcao3=(char *)malloc(120);
   funcao4=(char *)malloc(120);
   auxptr=(int *)malloc(sizeof(***var));
   var[0][0]=auxptr;
   for(i=0;i<5;i++) for(ii=0;ii<5;ii++) g[i][ii]=0;

   Cor(WHITE,BLUE);
   clrscr();
   Mostra_Texto(3,3, "Entre com o numero de elementos da tabela x / f(x) (no maximo 20 elementos)");
   scanf("%d", &numtabela);
   for(i=0; i<numtabela; i++)
   {
      xt[i]=0;
      fx[i]=0;
   }
   clrscr();
   Mostra_Texto(3,3, "Digite os elementos x tabelados:\n\r");
   for(i=0;i<numtabela;i++)
   {
      cprintf("%d§ ",i);
      scanf("%lf",&xt[i]);
   }
   clrscr();
   Mostra_Texto(3,3, "Agora digite os elementos f(x) tabelados:\n\r");
   for(i=0;i<numtabela;i++)
   {
      cprintf("%d§ ",i);
      scanf("%lf",&fx[i]);
   }
   clrscr();
   Mostra_Texto(3,6, "Escolha o numero de funcoes g(x) (1 a 5 ou X para terminar)");
   scanf("%i",&chosen);
   chosen=chosen-1;
   if(chosen>5)
   {
      free(funcao0);
      free(funcao1);
      free(funcao2);
      free(funcao3);
      free(funcao4);
      free(func);
      free(auxptr);
      exit(0);
   }
   clrscr();
   for(iii=0;iii<=chosen;iii++)
   {
      for(i=0;i<120;i++) func[i]=32;
      clrscr();
      tela_inicio(iii);
      if (iii==0)
      {
         strcpy(funcao0,"");
         compr = strlen(func);
         for(i=0;i<compr;i++) func[i]=toupper(func[i]);
         strcpy(funcao0,func);
         for(i=0;i<120-compr;i++) strcat(funcao0," ");
         i=0;ii=0;
         while(i<=compr+2)
         {
            if (func[ii]=='X')
            {
               funcao0[i]='A';
               funcao0[i+1]='1';
               i=i+2;
            }
            else
            {
               funcao0[i]=func[ii];
               i=i+1;
            }
            ii=ii+1;
         }
         funcao0[i]=0;
      }
      if (iii==1)
      {
         strcpy(funcao1,"");
         compr = strlen(func);
         for(i=0;i<compr;i++) func[i]=toupper(func[i]);
         strcpy(funcao1,func);
         for(i=0;i<120-compr;i++) strcat(funcao1," ");
         i=0;ii=0;
         while(i<=compr+2)
         {
            if (func[ii]=='X')
            {
               funcao1[i]='A';
               funcao1[i+1]='1';
               i=i+2;
            }
            else
            {
               funcao1[i]=func[ii];
               i=i+1;
            }
            ii=ii+1;
         }
         funcao1[i]=0;
      }
      if (iii==2)
      {
         strcpy(funcao2,"");
         compr = strlen(func);
         for(i=0;i<compr;i++) func[i]=toupper(func[i]);
         strcpy(funcao2,func);
         for(i=0;i<120-compr;i++) strcat(funcao2," ");
         i=0;ii=0;
         while(i<=compr+2)
         {
            if (func[ii]=='X')
            {
               funcao2[i]='A';
               funcao2[i+1]='1';
               i=i+2;
            }
            else
            {
               funcao2[i]=func[ii];
               i=i+1;
            }
            ii=ii+1;
         }
         funcao2[i]=0;
      }
      if (iii==3)
      {
         strcpy(funcao3,"");
         compr = strlen(func);
         for(i=0;i<compr;i++) func[i]=toupper(func[i]);
         strcpy(funcao3,func);
         for(i=0;i<120-compr;i++) strcat(funcao3," ");
         i=0;ii=0;
         while(i<=compr+2)
         {
            if (func[ii]=='X')
            {
               funcao3[i]='A';
               funcao3[i+1]='1';
               i=i+2;
            }
            else
            {
               funcao3[i]=func[ii];
               i=i+1;
            }
            ii=ii+1;
         }
         funcao3[i]=0;
      }
      if (iii==4)
      {
         strcpy(funcao4,"");
         compr = strlen(func);
         for(i=0;i<compr;i++) func[i]=toupper(func[i]);
         strcpy(funcao4,func);
         for(i=0;i<120-compr;i++) strcat(funcao4," ");
         i=0;ii=0;
         while(i<=compr+2)
         {
            if (func[ii]=='X')
            {
               funcao4[i]='A';
               funcao4[i+1]='1';
               i=i+2;
            }
            else
            {
               funcao4[i]=func[ii];
               i=i+1;
            }
            ii=ii+1;
         }
         funcao4[i]=0;
      }
   }
   // Chamada
   minquad();
   Mostra_Resultado();

   free(funcao0);
   free(funcao1);
   free(funcao2);
   free(funcao3);
   free(funcao4);
   free(func);
   free(auxptr);
}

/*
AJUSTE.C

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
