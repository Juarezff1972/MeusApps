#include <conio.h>
#include <stdio.h>
#include <process.h>

main()
{
int c,a;

a=0;
c=0;
prossegue:
a=kbhit();
if (a != 0) c=getch();
printf("%c",c);
if (c==27) exit(0);
goto prossegue;

}