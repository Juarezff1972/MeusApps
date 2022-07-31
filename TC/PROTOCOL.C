#include "crc_ccit.h"
#include <conio.h>
#include <string.h>
/////////////////////////////////////////////////////////////////
/* codigo da funcao checksum */
unsigned int checksum (register unsigned char *s, register int n)
{
	register unsigned int crc=0;

	while (n > 0) {
		crc = crc_table[(crc >> 8 ^ *s)] ^ (crc << 8);
		s++;
		n--;
	};
	return crc;
};
/////////////////////////////////////////////////////////////////
void main(void)
{
/* chamada da funcao checksum para calculo do crc */
   register unsigned char *pao;
   register int queijo;
   unsigned int vinho;
   pao="Gustavo Henrique Busnardo";
   queijo=strlen(pao);
   vinho=checksum(pao,(int) queijo);
   cprintf("\n\r%u\n\r",vinho);
   getch();
}
