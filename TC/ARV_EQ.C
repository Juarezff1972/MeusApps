#include <string.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <conio.h>
#include <stdarg.h>
#include <stdlib.h>

#define VERDADE 1
#define FALSO 0
#define MAIS 0
#define MENOS 1
#define VEZES 2
#define DIVIDIR 3
#define EXP 4
#define MODULO 5
#define APAREN 6
#define FPAREN 7
#define NUM 8
#define CELL 9
#define FUNC 10
#define EOLN 11
#define MAU 12
#define TEXTO 0
#define VALOR 1
#define FORMULA 2
#define MAXFUNCNAMELEN 5
#define PARSERSTACKSIZE 20
#define MSGSTACKERROR "\nERRO: estouro de pilha.\n"
#define MAXINPUT 79
#define MAXCOLS 100   /* Maximo e' 702 */
#define MAXROWS 100
///////////////////////////////////////////////////////////////////////
struct TOKENREC
{
   char state;
   union
   {
      double value;
      struct
      {
         int row, col;
      } c;
   char funcname[MAXFUNCNAMELEN + 1];
   } x;
};
///////////////////////////////
struct CELLREC
{
   char attrib;
   union
   {
      char text[MAXINPUT + 1];
      double value;
      struct
      {
         double fvalue;
         char formula[MAXINPUT + 1];
      } f;
   } v;
};
///////////////////////////////
static struct TOKENREC stack[PARSERSTACKSIZE], curtoken;
int stacktop, tokentype, error;
char *input, isformula;
typedef struct CELLREC *CELLPTR;
CELLPTR cell[MAXCOLS][MAXROWS];
///////////////////////////////////////////////////////////////////////
int rowwidth(int row)
{
   return((row == 0) ? 1 : (int)log10(row + 1) + 1);
}
///////////////////////////////////////////////////////////////////////
int formulastart(char **input, int *col, int *row)
/* Retorna VERDADE se o string e' o inicio de uma formula, sen„o FALSO.
   Tambem retorna a coluna e linha da formula.
*/
{
   int len, maxlen = rowwidth(MAXROWS);
   char *start, numstring[10];
   if (!isalpha(**input)) return(FALSO);
   *col = *((*input)++) - 'A';
   if (isalpha(**input))
   {
      *col *= 26;
      *col += *((*input)++) - 'A' + 26;
   }
   if (*col >= MAXCOLS) return(FALSO);
   start = *input;
   for (len = 0; len < maxlen; len++)
   {
      if (!isdigit(*((*input)++)))
      {
         (*input)--;
         break;
      }
   }
   if (len == 0) return(FALSO);
   strncpy(numstring, start, len);
   numstring[len] = 0;
   *row = atoi(numstring) - 1;
   if ((*row >= MAXROWS) || (*row == -1)) return(FALSO);
   return(VERDADE);
}
///////////////////////////////////////////////////////////////////////
int matherr(struct exception *e)
{
   e->retval = HUGE_VAL;
   return(1);
}
///////////////////////////////////////////////////////////////////////
int isfunc(char *s)
/* Retorna VERDADE se o string e' uma fun‡„o legal, sen„o FALSO. */
{
   int len = strlen(s);
   if (strncmp(s, input, len) == 0)
   {
      strncpy(curtoken.x.funcname, input, len);
      curtoken.x.funcname[len] = 0;
      input += len;
      return(VERDADE);
   }
   return(FALSO);
}
///////////////////////////////////////////////////////////////////////
int nexttoken(void)
/* Pega o proximo token da corrente de entrada */
{
   char *start, numstring[80];
   int decimal, len, numlen;
   while (*input == ' ') input++;
   if (*input == 0) return(EOLN);
   if (strchr("0123456789.", *input))
   {
      start = input;
      len = 0;
      decimal = FALSO;
      while ((isdigit(*input)) || ((*input == '.') && (!decimal)))
      {
         if (*input == '.') decimal = VERDADE;
         input++;
         len++;
      }
      if ((len == 1) && (start[0] == '.')) return(MAU);
      if (*input == 'E')
      {
         input++;
         len++;
         if (strchr("+-", *input) != NULL)
         {
            input++;
            len++;
         }
         numlen = 0;
         while ((isdigit(*input)) && (++numlen <= 3))
         {
            input++;
            len++;
         }
      }
      strncpy(numstring, start, len);
      numstring[len] = 0;
      curtoken.x.value = atof(numstring);
      if (errno == ERANGE) return(MAU);
      return(NUM);
   }
   else if (isalpha(*input))
        {
        if (isfunc("ABS")   || isfunc("ACOS")  || isfunc("ASIN") ||
            isfunc("ATAN")  || isfunc("COSH")  || isfunc("COS")  ||
            isfunc("EXP")   || isfunc("LOG10") || isfunc("LOG")  ||
            isfunc("POW10") || isfunc("ROUND") || isfunc("SINH") ||
            isfunc("SIN")   || isfunc("SQRT")  || isfunc("SQR")  ||
            isfunc("TANH")  || isfunc("TAN")   || isfunc("TRUNC"))
            return(FUNC);
        if (formulastart(&input, &curtoken.x.c.col, &curtoken.x.c.row))
        {
           isformula = VERDADE;
           return(CELL);
        }
        else return(MAU);
   }
   else switch(*(input++))
        {
           case '+' : return(MAIS);
           case '-' : return(MENOS);
           case '*' : return(VEZES);
           case '/' : return(DIVIDIR);
           case '^' : return(EXP);
           case ':' : return(MODULO);
           case '(' : return(APAREN);
           case ')' : return(FPAREN);
           default  : return(MAU);
        }
}
///////////////////////////////////////////////////////////////////////
void push(struct TOKENREC *token)
/* Coloca o novo token na pilha */
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
/* Tira o token do topo da pilha */
{
   return(stack[stacktop--]);
}
///////////////////////////////////////////////////////////////////////
int gotostate(int production)
/* Acha o novo estado baseado na ultima produ‡„o e o estado-topo. */
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
double cellvalue(int col, int row)
/* Acha o valor de uma celula */
{
   if (cell[col][row] == NULL) return(0);
   if (cell[col][row]->attrib == TEXTO) return(HUGE_VAL);
   if (cell[col][row]->attrib == FORMULA) return(cell[col][row]->v.f.fvalue);
   return(cell[col][row]->v.value);
}
///////////////////////////////////////////////////////////////////////
void shift(int state)
/* Desloca um token na pilha */
{
   curtoken.state = state;
   push(&curtoken);
   tokentype = nexttoken();
}
///////////////////////////////////////////////////////////////////////
void reduce(int reduction)
/* Completa uma redu‡„o */
{
   struct TOKENREC token1, token2;
   int counter;
   switch (reduction)
   {
      case 1 :
         token1 = pop();
         pop();
         token2 = pop();
         curtoken.x.value = token1.x.value + token2.x.value;
      break;
      case 2 :
         token1 = pop();
         pop();
         token2 = pop();
         curtoken.x.value = token2.x.value - token1.x.value;
      break;
      case 4 :
         token1 = pop();
         pop();
         token2 = pop();
         curtoken.x.value = token1.x.value * token2.x.value;
      break;
      case 5 :
         token1 = pop();
         pop();
         token2 = pop();
         if (token1.x.value == 0) curtoken.x.value = HUGE_VAL;
         else curtoken.x.value = token2.x.value / token1.x.value;
      break;
      case 7 :
         token1 = pop();
         pop();
         token2 = pop();
         curtoken.x.value = pow(token2.x.value, token1.x.value);
      break;
      case 9 :
         token1 = pop();
         pop();
         curtoken.x.value = -token1.x.value;
      break;
      case 11 :
         token1 = pop();
         pop();
         token2 = pop();
         curtoken.x.value = 0;
         if (token1.x.c.row == token2.x.c.row)
         {
            if (token1.x.c.col < token2.x.c.col) error = VERDADE;
            else
            {
               for (counter=token2.x.c.col;counter<=token1.x.c.col;counter++)
                  curtoken.x.value += cellvalue(counter, token1.x.c.row);
            }
         }
         else if (token1.x.c.col == token2.x.c.col)
              {
                 if (token1.x.c.row < token2.x.c.row) error = VERDADE;
                 else
                 {
                    for (counter=token2.x.c.row;counter<=token1.x.c.row;counter++)
                       curtoken.x.value += cellvalue(token1.x.c.col, counter);
                 }
              }
              else error = VERDADE;
      break;
      case 13 :
         curtoken = pop();
         curtoken.x.value = cellvalue(curtoken.x.c.col, curtoken.x.c.row);
      break;
      case 14 :
         pop();
         curtoken = pop();
         pop();
      break;
      case 16 :
         pop();
         curtoken = pop();
         pop();
         token1 = pop();
         if (strcmp(token1.x.funcname, "ABS") == 0)
            curtoken.x.value = fabs(curtoken.x.value);
         else
         if (strcmp(token1.x.funcname, "ACOS") == 0)
            curtoken.x.value = acos(curtoken.x.value);
         else
         if (strcmp(token1.x.funcname, "ASIN") == 0)
            curtoken.x.value = asin(curtoken.x.value);
         else
         if (strcmp(token1.x.funcname, "ATAN") == 0)
            curtoken.x.value = atan(curtoken.x.value);
         else
         if (strcmp(token1.x.funcname, "COSH") == 0)
            curtoken.x.value = cosh(curtoken.x.value);
         else
         if (strcmp(token1.x.funcname, "COS") == 0)
            curtoken.x.value = cos(curtoken.x.value);
         else
         if (strcmp(token1.x.funcname, "EXP") == 0)
            curtoken.x.value = exp(curtoken.x.value);
         else
         if (strcmp(token1.x.funcname, "LOG10") == 0)
            curtoken.x.value = log10(curtoken.x.value);
         else
         if (strcmp(token1.x.funcname, "LOG") == 0)
            curtoken.x.value = log(curtoken.x.value);
         else
         if (strcmp(token1.x.funcname, "ROUND") == 0)
            curtoken.x.value = (int)(curtoken.x.value + 0.5);
         else
         if (strcmp(token1.x.funcname, "POW10") == 0)
            curtoken.x.value = pow10(curtoken.x.value);
         else
         if (strcmp(token1.x.funcname, "SINH") == 0)
            curtoken.x.value = sinh(curtoken.x.value);
         else
         if (strcmp(token1.x.funcname, "SIN") == 0)
            curtoken.x.value = sin(curtoken.x.value);
         else
         if (strcmp(token1.x.funcname, "SQRT") == 0)
            curtoken.x.value = sqrt(curtoken.x.value);
         else
         if (strcmp(token1.x.funcname, "SQR") == 0)
            curtoken.x.value *= curtoken.x.value;
         else
         if (strcmp(token1.x.funcname, "TANH") == 0)
            curtoken.x.value = tanh(curtoken.x.value);
         else
         if (strcmp(token1.x.funcname, "TAN") == 0)
            curtoken.x.value = tan(curtoken.x.value);
         else
         if (strcmp(token1.x.funcname, "TRUNC") == 0)
            curtoken.x.value = (int)curtoken.x.value;
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
double parse(char *s, int *att)
/* Parse sobre o string s - retorna o valor do string analisado, e coloca
   o atributo em att: TEXTO = 0, CONSTANTE = 1, FORMULA = 2.
*/
{
   struct TOKENREC firsttoken;
   char accepted = FALSO;
   char copy[80];
   error = FALSO;
   isformula = FALSO;
   input = copy;
   strupr(strcpy(copy, s));
   stacktop = -1;
   firsttoken.state = 0;
   firsttoken.x.value = 0;
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
            else if (tokentype == CELL) shift(7);
                 else if (tokentype == FUNC) shift(11);
                      else if (tokentype == MENOS) shift(5);
                           else if (tokentype == APAREN) shift(9);
                                else error = VERDADE;
         break;
         case 1 :
            if (tokentype == EOLN) accepted = VERDADE;
            else if (tokentype == MAIS) shift(12);
                 else if (tokentype == MENOS) shift(13);
                      else error = VERDADE;
         break;
         case 2 :
            if (tokentype == VEZES) shift(14);
            else if (tokentype == DIVIDIR) shift(15);
                 else reduce(3);
         break;
         case 3 :
            reduce(6);
         break;
         case 4 :
            if (tokentype == EXP) shift(16);
            else reduce(8);
         break;
         case 5 :
            if (tokentype == NUM) shift(10);
            else if (tokentype == CELL) shift(7);
                 else if (tokentype == FUNC) shift(11);
                      else if (tokentype == APAREN) shift(9);
                           else error = VERDADE;
         break;
         case 6 :
            reduce(10);
         break;
         case 7 :
            if (tokentype == MODULO) shift(18);
            else reduce(13);
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
            if (tokentype == CELL) shift(26);
            else error = VERDADE;
         break;
         case 19 :
            if (tokentype == MAIS) shift(12);
            else if (tokentype == MENOS) shift(13);
                 else if (tokentype == FPAREN) shift(27);
                      else error = VERDADE;
         break;
         case 21 :
            if (tokentype == VEZES) shift(14);
            else if (tokentype == DIVIDIR) shift(15);
                 else reduce(1);
         break;
         case 22 :
            if (tokentype == VEZES) shift(14);
            else if (tokentype == DIVIDIR) shift(15);
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
            if (tokentype == MAIS) shift(12);
            else if (tokentype == MENOS) shift(13);
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
   if (error)
   {
      *att = TEXTO;
      return(0);
   }
   if (isformula) *att = FORMULA;
   else *att = VALOR;
   strcpy(s, copy);
   return(stack[stacktop].x.value);
}
///////////////////////////////////////////////////////////////////////
void main(void)
{
   double a;
   int *attr;
   clrscr();
   a=parse("2*3+4*5",attr);
   printf("\n\nValor: %f\n\nAtributo: %d\n\n",a,attr);
}