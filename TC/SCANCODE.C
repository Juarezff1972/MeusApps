#include <bios.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

int x;

main()
{
clrscr();
x=bioskey(0);
printf("O numero e' : %u ",x);
}

