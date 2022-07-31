#include "stdio.h";
#include "conio.h";
main()
{
char senha[2];
clrscr();
printf("Digite senha: ");
scanf("%s",senha);
if (senha == "12") printf("\n senha correta\n");
else printf("\n senha incorreta\n");
}
