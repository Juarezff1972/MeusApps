#include <conio.h>
#include <stdio.h>

typedef char *string;
////////////////////////////////////////////////////////////////////////
int compr(const string s)
{
   int count=0;
   while(s[count]!=0) count++;
   return(count);
}
////////////////////////////////////////////////////////////////////////
string strinv(string st)
{
   int siz,i;
   char aux;
   char *aux2;
   siz=compr(st)-1;
   for (i=0;i<=siz;i++) aux2[i]=st[i];
   for (i=0;i<=(siz/2);i++)
   {
      aux=aux2[i];
      aux2[i]=aux2[siz-i];
      aux2[siz-i]=aux;
   }
   return(aux2);
}
////////////////////////////////////////////////////////////////////////
string substr(string s,int pos,int tam)
{
   string aux=" ";
   int i;
   pos--;tam--;
   for (i=0;i<=tam;i++)
   {
      if ((i+pos)>(compr(s)-1)) break;
      aux[i]=s[i+pos];
   }
   aux[i]=0;
   return(aux);
}
////////////////////////////////////////////////////////////////////////
void main(void)
{
   string s;
   int pos=1,tam=1;
   cprintf("\n\rEntre com uma palavra ou frase: ");   gets(s);
   cprintf("\n\rTamanho:  %d",compr(s));
   cprintf("\n\rInvers∆o: %s\n\r",strinv(s));
   cprintf("\n\rEntre com a posiá∆o:");   cscanf("%d",&pos);
   cprintf("\n\rEntre com o tamanho:");   cscanf("%d",&tam);
   cprintf("\n\rResultado: %s",substr(s,pos,tam));
   cprintf("\n\r\n\r");
}

