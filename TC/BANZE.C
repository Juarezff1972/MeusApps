#include <stdio.h>
main ()
{
int *x;
int y;
char z;
loop:
y +=(*x);
z =y;
x ++;
printf("%d\n",y);
printf("%p\n",x);
printf("%c\n",z);
goto loop;
}
