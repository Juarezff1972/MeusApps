#include <string.h>
#include <process.h>
#include <stdio.h>
/////////////////////////////////////////////////////////////////////////
#define soma 0
#define subt 1
#define mult 2
#define divi 3
#define apar 4
#define fpar 5
#define vari 6
#define fini 7
/////////////////////////////////////////////////////////////////////////
typedef struct { char tipo, valor; } tokntype;
/////////////////////////////////////////////////////////////////////////
char entrada[70];
tokntype pilha[100];
char topopilha=0;
tokntype tkn;
char prod[][11]={"E#"  , // S  0
                 "Te"  , // E  1
                 ""    , // e  2
                 "+Tae", // e  3
                 "-Tse", // e  4
                 "Ft"  , // T  5
                 ""    , // t  6
                 "*Fmt", // t  7
                 "/Fdt", // t  8
                 "i"   , // F  9
                 "(E)"}; // F  10
char produ[70];
tokntype token;
tokntype op,op1,op2;
/////////////////////////////////////
char func_m[5][8]=
   { // +  -  *  /  (  )  i  #
/*E */{-1,-1,-1,-1, 1,-1, 1,-1},
/*E'*/{ 3, 4,-1,-1,-1, 2,-1, 2},
/*T */{-1,-1,-1,-1, 5,-1, 5,-1},
/*T'*/{ 6, 6, 7, 8,-1, 6,-1, 6},
/*F */{-1,-1,-1,-1,10,-1, 9,-1}
   };
/////////////////////////////////////////////////////////////////////////
void push(tokntype token)
{
   pilha[topopilha]=token;
   topopilha++;
}
/////////////////////////////////////////////////////////////////////////
tokntype pop(void)
{
   topopilha--;
   return pilha[topopilha];
}
/////////////////////////////////////////////////////////////////////////
tokntype proxtoken(char *prog)
{
   tokntype aux;
   if (prog[0]=='i')
   {
      aux.valor='i';
      aux.tipo=vari;
   }
   if (prog[0]=='+')
   {
      aux.valor='+';
      aux.tipo=soma;
   }
   if (prog[0]=='-')
   {
      aux.valor='-';
      aux.tipo=subt;
   }
   if (prog[0]=='*')
   {
      aux.valor='*';
      aux.tipo=mult;
   }
   if (prog[0]=='/')
   {
      aux.valor='/';
      aux.tipo=divi;
   }
   if (prog[0]=='(')
   {
      aux.valor='(';
      aux.tipo=apar;
   }
   if (prog[0]==')')
   {
      aux.valor=')';
      aux.tipo=fpar;
   }
   *prog++;
   strcpy(entrada,prog);
   return aux;
}
/////////////////////////////////////////////////////////////////////////
char deriva(char *prodc)
{
   char *aux,aux2,aux3;
   printf("%s   <%d>\n",produ,topopilha);
   strcpy(aux,produ);
   aux2=0;
   if (aux[0]=='E') aux2=0;
   if (aux[0]=='e') aux2=1;
   if (aux[0]=='T') aux2=2;
   if (aux[0]=='t') aux2=3;
   if (aux[0]=='F') aux2=4;
   if (aux[0]=='a')
   {
      op1=pop();
      op2=pop();
      /*@add*/
      op=op1; //Apenas para evitar erros neste .CPP, simula @add
      push(op);
      *prodc++;
      if ((aux3>=0) && (aux3<=10)) strcpy(aux,prod[aux3]); else strcpy(aux,"");
      strcat(aux,prodc);
      strcpy(produ,aux);
      return 0;
   }
   if (aux[0]=='s')
   {
      op1=pop();
      op2=pop();
      /*@sub*/
      op=op1; //Apenas para evitar erros neste .CPP, simula @sub
      push(op);
      *prodc++;
      if ((aux3>=0) && (aux3<=10)) strcpy(aux,prod[aux3]); else strcpy(aux,"");
      strcat(aux,prodc);
      strcpy(produ,aux);
      return 0;
   }
   if (aux[0]=='m')
   {
      op1=pop();
      op2=pop();
      /*@mul*/
      op=op1; //Apenas para evitar erros neste .CPP, simula @mul
      push(op);
      *prodc++;
      if ((aux3>=0) && (aux3<=10)) strcpy(aux,prod[aux3]); else strcpy(aux,"");
      strcat(aux,prodc);
      strcpy(produ,aux);
      return 0;
   }
   if (aux[0]=='d')
   {
      op1=pop();
      op2=pop();
      /*@div*/
      op=op1; //Apenas para evitar erros neste .CPP, simula @div
      push(op);
      *prodc++;
      if ((aux3>=0) && (aux3<=10)) strcpy(aux,prod[aux3]); else strcpy(aux,"");
      strcat(aux,prodc);
      strcpy(produ,aux);
      return 0;
   }
   if (aux[0]=='#')
   {
      op1=pop();
      /*@print*/
      exit(0);
   }
   aux3=func_m[aux2][token.tipo];
   if (aux3==-1) return -1; // ERRO
   if ((aux3>0) && (aux3<11))
   {
      if (aux3==9) push(token);
      *prodc++;
      strcpy(aux,prod[aux3]);
      strcat(aux,prodc);
      strcpy(produ,aux);
      return aux3;
   }

   return -1;
}
/////////////////////////////////////////////////////////////////////////
void main(void)
{
   char *tmp;
   strcpy(entrada,"i+i*(i-i+i*i-i/i)#");
   token=proxtoken(entrada);
   strcpy(produ,prod[0]);
   while (strlen(entrada)>0)
   {
      if (deriva(produ)==-1) exit(768);
      if (token.valor==produ[0])
      {
         strcpy(tmp,produ);
         *tmp++;
         strcpy(produ,tmp);
         if (strlen(entrada)>1) token=proxtoken(entrada);
      }
   }
}
