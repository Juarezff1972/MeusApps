#include <string.h>
#include <conio.h>
#include <ctype.h>

//char ecuaciones[]="((((4*5)+2)*3)-7)";
//char ecuaciones[]="((1*(2+3))/(5-(3*4)))";
//char ecuaciones[]="a*b*c+(4+5/(2*3)+8-(3+4*5)-(5/d+3)+((2*g)+5))";
char ecuaciones[]="(2*3)+(4*5)";
//char ecuaciones[]="a+b*c-d/e";
//char ecuaciones[]="4*5+2-5*3";
char index[80],bolha[80];
char size,i,aux;
char tempstr[80];
char openpars;
///////////////////////////////////////////////////////////////////
char Min(char ind[80])
{
   char aux,g,pos;
   pos=0;
   aux=99;
   for (g=0;g<=strlen(ind)-1;g++)
   {
      if ((ind[g]<aux)&&(ind[g]!=-1))
      {
         aux=ind[g];
         pos=g;
      }
   }
   return pos;
}
///////////////////////////////////////////////////////////////////
char Max(char ind[80])
{
   char aux,g,pos;
   pos=0;
   aux=0;
   for (g=0;g<=strlen(ind)-1;g++)
   {
      if ((ind[g]>aux)&&(ind[g]!=-1))
      {
         aux=ind[g];
         pos=g;
      }
   }
   return pos;
}
///////////////////////////////////////////////////////////////////
void substr(char str1[80],char tam,char inic)
{
   char q;
   for (q=0;q<=tam;q++) tempstr[q]=str1[q+inic];
   tempstr[q]=0;
}
///////////////////////////////////////////////////////////////////
void Tree(char substrg[80],char stpoint)
{
   char pos,substrg1[80],substrg2[80];
   int i,aux;
   for (i=0;i<=80;i++)
   {
      substrg1[i]=-1;
      substrg2[i]=-1;
      bolha[i]=substrg[i];
   }
   pos=Min(substrg);
   substr(substrg,pos-1,0);
   for (i=0;i<=strlen(tempstr);i++)
   {
      substrg1[i]=tempstr[i];
   }
   aux=strlen(substrg);
   substr(substrg,aux-pos+1,pos+1);
   for (i=0;i<=strlen(tempstr);i++)
   {
      substrg2[i]=tempstr[i];
   }
   if (strchr("+-/*",ecuaciones[stpoint+pos])!=0)
   {
      openpars++;
      textcolor(openpars);
      putch(32);
      putch(40);
   }
   openpars=3;
   textcolor(openpars);
   if (isalnum(toupper(ecuaciones[stpoint+pos]))!=0) putch(32);
   putch(ecuaciones[stpoint+pos]);
   if ((strlen(substrg1)<1) && (strlen(substrg2)<1))
   {
      if (strchr("+-/*",ecuaciones[stpoint+pos])!=0)
      {
         textcolor(openpars);
         openpars--;
         putch(41);
      }
      return;
   }
   if (strlen(substrg1)>=1) Tree(substrg1,stpoint);
   if (strlen(substrg2)>=1) Tree(substrg2,stpoint+pos+1);
   textcolor(openpars);
   openpars--;
   putch(41);
}
///////////////////////////////////////////////////////////////////
void InicPos(void)
{
   char y;
   for (y=0;y<=size-1;y++)
   {
      if (ecuaciones[y]=='(') i=i+10;
      if (ecuaciones[y]==')') i=i-10;
      if ((ecuaciones[y]=='*')||(ecuaciones[y]=='/')) index[y]=i+2;
      if ((ecuaciones[y]=='+')||(ecuaciones[y]=='-')) index[y]=i+3;
      if (isalnum(toupper(ecuaciones[y]))!=0) index[y]=i+1;
   }
   index[size]=0;
}
///////////////////////////////////////////////////////////////////
void main(void)
{
   clrscr();
   openpars=0;
   for (i=0;i<=80;i++) index[i]=-1;
   size=strlen(ecuaciones);
   i=0;
   aux=0;
   putch(13);putch(10);
   InicPos();
   Tree(index,0);
}