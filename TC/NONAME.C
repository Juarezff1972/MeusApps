#include "stdio.h";
#include "conio.h";
main()
{
char senha[12];
clrscr();
printf("Digite senha");
scanf("%s \n",senha);
if (senha=="12") printf("senha correta");
else printf("senha incorreta\n");
}


