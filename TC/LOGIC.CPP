/*
   Ciencia da Computacao - 2§ano
   Alunos: Anderson Ferreira Martins
           Juarez Fioravanti Filho
*/
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <alloc.h>
#include <process.h>
///////////////////////////////
#define VERDADE 1
#define FALSO 0
#define AND 0
#define NOT 1
#define OR 2
#define BICOND 4
#define APAREN 6
#define FPAREN 7
#define NUM 8
#define EOLN 11
#define MAU 12
#define ERRO 0
#define VALOR 1
#define PARSERSTACKSIZE 20
#define MSGSTACKERROR "\nERRO: estouro de pilha.\n"
///////////////////////////////////////////////////////////////////////
struct TOKENREC
{
   char state;
   int value;
};
static struct TOKENREC stack[PARSERSTACKSIZE], curtoken;
int stacktop, tokentype, error;
char *input;
char *funcao,*funct;
///////////////////////////////////////////////////////////////////////
int nexttoken(void)
{
   char *start, numstring[8];
   int len, numlen;
   while (*input == ' ') input++;
   if (*input == 0) return(EOLN);
   if (strchr("01", *input))
   {
      start = input;
      len = 0;
      while (isdigit(*input))
      {
         input++;
         len++;
      }
      strncpy(numstring, start, len);
      numstring[len] = 0;
      curtoken.value = atoi(numstring);
      if (errno == ERANGE) return(MAU);
      return(NUM);
   }
   else switch(*(input++))
        {
           case '&' : return(AND);
           case '!' : return(NOT);
           case '|' : return(OR);
           case '=' : return(BICOND);
           case '(' : return(APAREN);
           case ')' : return(FPAREN);
           default  : return(MAU);
        }
}
///////////////////////////////////////////////////////////////////////
void push(struct TOKENREC *token)
{
   if (stacktop == PARSERSTACKSIZE - 1)
   {
      printf(MSGSTACKERROR);
      error = VERDADE;
   }
   else stack[++stacktop] = *token;
}
///////////////////////////////////////////////////////////////////////
struct TOKENREC pop(void)
{
   return(stack[stacktop--]);
}
///////////////////////////////////////////////////////////////////////
int gotostate(int production)
{
   int state = stack[stacktop].state;
   if (production <= 3)
   {
      switch(state)
      {
         case 0 : return(1);
         case 9 : return(19);
         case 20 : return(28);
      }
   }
   else if (production <= 6)
        {
           switch(state)
           {
              case 0 :
              case 9 :
              case 20 : return(2);
              case 12 : return(21);
              case 13 : return(22);
           }
        }
        else if (production <= 8)
             {
                switch(state)
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
             else if (production <= 10)
                  {
                     switch(state)
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
                  else if (production <= 12)
                       {
                          switch(state)
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
                          switch(state)
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
void shift(int state)
{
   curtoken.state = state;
   push(&curtoken);
   tokentype = nexttoken();
}
///////////////////////////////////////////////////////////////////////
void reduce(int reduction)
{
   struct TOKENREC token1, token2;
   int counter;
   switch (reduction)
   {
      case 1 :
           token1 = pop();
           pop();
           token2 = pop();
           curtoken.value = token1.value | token2.value;
      break;
      case 4 :
           token1 = pop();
           pop();
           token2 = pop();
           curtoken.value = token1.value & token2.value;
      break;
      case 2 :
      case 5 :
      case 11 :
      case 13 :
      case 16 :
      break;
      case 7 :
           token1 = pop();
           pop();
           token2 = pop();
           curtoken.value = (!token2.value & !token1.value) |
                            (token2.value & token1.value);
      break;
      case 9 :
           token1 = pop();
           pop();
           curtoken.value = !token1.value;
      break;
      case 14 :
           pop();
           curtoken = pop();
           pop();
      break;
      case 3 :
      case 6 :
      case 8 :
      case 10 :
      case 12 :
      case 15 :
           curtoken = pop();
      break;
   }
   curtoken.state = gotostate(reduction);
   push(&curtoken);
}
///////////////////////////////////////////////////////////////////////
int parse(char *s)
{
   struct TOKENREC firsttoken;
   char accepted = FALSO;
   char copy[80];
   error = FALSO;
   input = copy;
   strupr(strcpy(copy, s));
   stacktop = -1;
   firsttoken.state = 0;
   firsttoken.value = 0;
   push(&firsttoken);
   tokentype = nexttoken();
   do
   {
      switch (stack[stacktop].state)
      {
         case  0 :
         case  9 :
         case 12 :
         case 13 :
         case 14 :
         case 15 :
         case 16 :
         case 20 :
            if (tokentype == NUM) shift(10);
            else if (tokentype == NOT) shift(5);
                 else if (tokentype == APAREN) shift(9);
                      else error = VERDADE;
         break;
         case 1 :
            if (tokentype == EOLN) accepted = VERDADE;
            else if (tokentype == OR) shift(12);
                 else error = VERDADE;
         break;
         case 2 :
            if (tokentype == AND) shift(14);
            else reduce(3);
         break;
         case 3 :
            reduce(6);
         break;
         case 4 :
            if (tokentype == BICOND) shift(16);
            else reduce(8);
         break;
         case 5 :
            if (tokentype == NUM) shift(10);
            else if (tokentype == APAREN) shift(9);
                 else error = VERDADE;
         break;
         case 6 :
            reduce(10);
         break;
         case 7 :
            reduce(13);
         break;
         case 8 :
            reduce(12);
         break;
         case 10 :
            reduce(15);
         break;
         case 11 :
            if (tokentype == APAREN) shift(20);
            else error = VERDADE;
         break;
         case 17 :
            reduce(9);
         break;
         case 18 :
            error = VERDADE;
         break;
         case 19 :
            if (tokentype == OR) shift(12);
            else if (tokentype == FPAREN) shift(27);
                 else error = VERDADE;
         break;
         case 21 :
            if (tokentype == AND) shift(14);
            else reduce(1);
         break;
         case 22 :
            if (tokentype == AND) shift(14);
            else reduce(2);
         break;
         case 23 :
            reduce(4);
         break;
         case 24 :
            reduce(5);
         break;
         case 25 :
            reduce(7);
         break;
         case 26 :
            reduce(11);
         break;
         case 27 :
            reduce(14);
         break;
         case 28 :
            if (tokentype == OR) shift(12);
            else if (tokentype == FPAREN) shift(29);
                 else error = VERDADE;
         break;
         case 29 :
            reduce(16);
         break;
         case 30 :
            error = VERDADE;
         break;
      }
   }
   while ((!accepted) && (!error));
   if (error) {printf("\nErro na express„o.\n");return(0);}
   strcpy(s, copy);
   return(stack[stacktop].value);
}
///////////////////////////////////////////////////////////////////////
void main(void)
{
   int a;
   int compr;
   int i;
   funct=(char *)malloc(60);
   funcao=(char *)malloc(60);
   printf("\nt : verdade  f : falsidade");
   printf("\n! : nega‡„o  | : disjun‡„o  & : conjun‡„o  = : bicondicional");
   printf("\nEntre com a express„o: ");
   scanf("%s",funcao);
   strcpy(funct,"");
   compr = strlen(funcao);
   funcao=strupr(funcao);
   strcpy(funct,funcao);
   for(i=0;i<60-compr;i++) strcat(funct," ");
   i=0;
   while(i<=compr+1)
   {
     if (funcao[i]=='T') funct[i]='1';
     else if (funcao[i]=='F') funct[i]='0';
          else funct[i]=funcao[i];
     i++;
   }
   free(funcao);
   a=parse(funct);
   if (a==1) printf("\n\nValor: T\n");
   if (a==0) printf("\n\nValor: F\n");
   free(funct);
}
