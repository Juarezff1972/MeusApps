// Trabalho de Parser LL(1)
// Ciencia da Computacao - 3§ ano
// Prof. Claudio de Oliveira
// Alunos: Gustavo Henrique Busnardo - Juarez Fioravanti Filho
//////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//////////////////////////////////////////////////////////////////////////
char M[13][8]=
   { // +  -  *  /  (  )  i  #
/*E */{-1,-1,-1,-1, 1,-1, 1,-1},
/*E'*/{ 3, 4,-1,-1,-1, 2,-1, 2},
/*T */{-1,-1,-1,-1, 5,-1, 5,-1},
/*T'*/{ 6, 6, 7, 8,-1, 6,-1, 6},
/*F */{-1,-1,-1,-1,10,-1, 9,-1},
/*+ */{11,-1,-1,-1,-1,-1,-1,-1},
/*- */{-1,11,-1,-1,-1,-1,-1,-1},
/** */{-1,-1,11,-1,-1,-1,-1,-1},
/*/ */{-1,-1,-1,11,-1,-1,-1,-1},
/*( */{-1,-1,-1,-1,11,-1,-1,-1},
/*) */{-1,-1,-1,-1,-1,11,-1,-1},
/*i */{-1,-1,-1,-1,-1,-1,11,-1},
/*# */{-1,-1,-1,-1,-1,-1,-1,12},
   };
//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////
void main(void)
{
   char *T,*start,*sent,*aux;
   char p,t,u,parse,prd,i;

   strcpy(T,"i+i*i");
   strcat(T,"#");
   p=1;
   strcpy(start,"S");
   strcpy(sent,start);
   strcat(sent,"#");

   parse=0;
   while (1)
   {
      t=sent[0];
      u=T[p];
      if (M[t,u]==prd)
      {
         strcpy(aux,prod[prd]);
         strcat(aux,sent[1]);
         strcpy(sent,aux);
         strcpy(aux,parse);
         strcat(aux," ");
         strcat(aux,itoa(i));
         strcpy(parse,aux);
      }
      else
      {
         if (M[t,u]==11)
         {
            sent=sent[1];
            p++;
         }
         else
         {
            if (M[t,u]==12)
            {
               printf("\nSucesso no parser\n");
            } else printf("\nInsucesso no parser\n");
         }
      }
   }
}
