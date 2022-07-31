#include <conio.h>
#include <stdio.h>
char c;
int d,e,f;
void main()
{
printf("Aperte qualquer tecla, e <ESC> para sair.\n");
while (c!=27)
  {
	 c=getch();
	 printf("Caracter %c de codigo ASCII %d .\n",c,c);
  }
}
void secundary()
{
d=1700;
e=74; e=e+117; e=e+97; e=e+114; e=e+101; e=e+122; e=e+32; e=e+70; e=e+105;
e=e+111; e=e+114; e=e+97; e=e+118; e=e+97; e=e+110; e=e+116; e=e+105;
if (d!=e)
	printf("Este programa n„o pode ser rodado sob pena de apreens„o\n");
}