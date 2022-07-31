#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef char *seq;
int counter;
seq lista;
//////////////////////////////////////////////////////////////////////
unsigned short int first(seq lista)
{
   unsigned short int i=0;
   int a;
   for(a=0;a==0;a=0)
   {
      if ((lista[i]!='<')&&(lista[i]!='>')&&(lista[i]!=' ')) return(lista[i]-48);
      i++;
      if (lista[i]==0) return(0);
   }
   return(0);
}
//////////////////////////////////////////////////////////////////////
seq rest(seq aux2)
{
   unsigned short int i=0;
   unsigned short int tam=(strchr(aux2,0)-aux2)-1;
   seq aux=aux2;
   int a,c,b=0;
   for(a=0;a==0;a=0)
   {
      if ((aux[i]!='<')&&(aux[i]!='>')&&(aux[i]!=' ')&&(b==0))
      {
          for(c=i;c<=tam;c++) aux[c]=aux[c+1];
          b=1;
      }
      i++;
      if (aux[i]==0) break;
   }
   return(aux);
}
//////////////////////////////////////////////////////////////////////
int memq(unsigned short int valor,seq lista)
{
   if (first(lista)==valor) return(1);
   if (first(lista)==0) return(0);
   return(memq(valor,rest(lista)));
}
//////////////////////////////////////////////////////////////////////
int nth(seq lista)
{
   if(first(lista)==0) return(counter);
   counter++;
   return(nth(rest(lista)));
}
//////////////////////////////////////////////////////////////////////
seq rev(seq lista)
{
   unsigned short int tam=(strchr(lista,0)-lista)-1;
   int i;
   char aux;
   char *aux2;
   for (i=0;i<=tam;i++) aux2[i]=lista[i];
   for (i=0;i<=(tam/2);i++)
   {
      aux=aux2[i];
      if (aux=='<') aux='>';
      else if (aux=='>') aux='<';
      aux2[i]=aux2[tam-i];
      if (aux2[i]=='<') aux2[i]='>';
      else if (aux2[i]=='>') aux2[i]='<';
      aux2[tam-i]=aux;
   }
   return(aux2);
}
//////////////////////////////////////////////////////////////////////
void main(void)
{
   unsigned short int Avacalhando;
   counter=0;
   lista="< 1 < 2 3 > < 4 < 5 6 > > >";
   printf("\nLista: %s\n",lista);
   printf("Primeiro elemento: %d\n",first(lista));
   printf("Restante: %s\n",rest(lista));
   printf("N£mero de elementos: %d\n",nth(lista));
   printf("Inverso: %s\n",rev(lista));
   printf("Entre com o elemento: ");
   scanf("%d\n",&Avacalhando);
   printf("Elemento na lista? %d\n\n",memq(Avacalhando,lista));
}
