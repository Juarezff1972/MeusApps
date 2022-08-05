#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

////////////////////////////////////////////////////////////////////////
char *pegatoken(char *string,int *ii)
{
   char *aux;
   int i=0;
   if ((string[*ii]=='+') || (string[*ii]=='-') || (string[*ii]=='*')
    || (string[*ii]=='/') || (string[*ii]=='^') || (string[*ii]=='(')
    || (string[*ii]==')'))
    {
       aux[0]=string[*ii];
       aux[1]=0;
       *ii=*ii+1;
       return aux;
    }
   while ((string[*ii]>='0') && (string[*ii]<='9'))
   {
      aux[i]=string[*ii];
      *ii=*ii+1;
      i++;
   }
   aux[i]=0;
   return aux;
}
////////////////////////////////////////////////////////////////////////
void tipotoken(char *token,int *f,int *g)
{
   if ((*token=='+')||(*token=='-')) {*f=1; *g=2; return;}
   if ((*token=='*')||(*token=='/')) {*f=3; *g=4; return;}
   if (*token=='^') {*f=6; *g=5; return;}
   if (*token=='(') {*f=9; *g=0; return;}
   if (*token==')') {*f=0; *g=-1;return;}
   *f=7; *g=8;
}
////////////////////////////////////////////////////////////////////////
void main(void)
{
   char *tmp,*posfix,*val;
   int f,g,f1,g1,k=1;
   int i,topo=0;
   char *pilha[8192];
   *pilha=(char *)malloc(16384);
   i=0;

   val="(123+456)*(789)";

   strcpy(pilha[topo],"(");
   posfix[0]=0;
   strcpy(posfix,0);
   strcat(val,")\x0");
   while (strlen(val)>i)
   {
      if (k==1)
      {
         tmp=pegatoken(val,&i);
         tipotoken(tmp,&f,&g);
      }
      tipotoken(pilha[topo],&f1,&g1);
      if (f>g1)
      {
         topo++;
         strcpy(pilha[topo],tmp);
         k=1;
      }
      if (g1>f)
      {
         strcat(posfix,pilha[topo]);
         strcat(posfix," ");
         strcat(posfix,0);
         topo--;
         k=0;
      }
      if (f==g1)
      {
         topo--;
         k=1;
      }
   }
   free(pilha);
   clrscr();
   printf("%s",posfix);
}
