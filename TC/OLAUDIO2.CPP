#include <string.h>
#include <conio.h>
/////////////////////////////////////////////////////////////////////////
char string[80];
char next;
char cursor;
/////////////////////////////////////////////////////////////////////////
char getchar(void);
char expr(void);
char factor(void);
char term(void);
/////////////////////////////////////////////////////////////////////////
char getchar(void)
{
   char chr;
   chr=string[cursor];
   cursor++;
   return chr;
}
/////////////////////////////////////////////////////////////////////////
char expr(void)
{
   if (!term()) return 0;
   if (next=='+')
   {
      next=getchar();
      if (next=='#') return 0;
      if (!expr()) return 0; else return 1;
   } else return 1;
}
/////////////////////////////////////////////////////////////////////////
char factor(void)
{
   if (next=='#') return 0;
   if (next=='(')
   {
      next=getchar();
      if (next=='#') return 0;
      if (!expr()) return 0;
      if (next!=')') return 0; else {next=getchar(); return 1;}
   }
   if (next!='i') return 0; else {next=getchar(); return 1;}
}
/////////////////////////////////////////////////////////////////////////
char term(void)
{
   if (!factor()) return 0;
   if (next=='*')
   {
      next=getchar();
      if (next=='#') return 0;
      if (!term()) return 0; else return 1;
   } else return 1;
}
/////////////////////////////////////////////////////////////////////////
void main(void)
{
   char *input;
   char i;
   strcpy(input,"(i+i)*i#");
   while (strlen(input)>0)
   {
      for (i=0;i<=strlen(input);i++) string[i]=input[i];
      cursor=0;
      next=getchar();
      if (expr())
      {
         if (next=='#') putch('V'); else putch('I');
      } else putch('I');
   strcpy(input,"");
   }
}
