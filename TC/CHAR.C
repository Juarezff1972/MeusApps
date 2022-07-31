#include <stdio.h>
main()
{
char c;
c = 1;
count:
printf("%d   ---   %c\n",c,c);
sum:
c = c + 1;
if (c == 0) goto fim;
if (c == 26) goto sum;
goto count;
fim:
printf("Programa terminado.");
}
