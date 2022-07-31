#include <dos.h>
#include <dir.h>
#include <conio.h>
#include <stdio.h>
#include <process.h>
#include <io.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "crc_ccit.h"

/***************************************************************************/
unsigned int PORTA;
unsigned int com[4];
unsigned char Valor,Status;
/***************************************************************************/
unsigned int Checksum (register unsigned char *s, register int n)
{
	register unsigned int crc=0;
   while(n>0) {
		crc=crc_table[(crc>>8^*s)]^(crc<<8);
		s++;
		n--;
	};
	return crc;
};
/***************************************************************************/
void Enviar(unsigned int Porta,unsigned int Valor)
{
	unsigned int c;
	c=com[Porta];
	delay(8);
	outp(c,Valor);
};
/***************************************************************************/
void Ler(unsigned int Porta)
{
	asm{
		push ax
		push dx
		mov ax,0x200
		mov dx,Porta
		int 0x14
		mov Status,ah
		mov Valor,al
		pop dx
		pop ax
	};
};
/***************************************************************************/
int LerCRC(void)
{
   char stam[5];
   char Count;
   while(!(kbhit()))
	{
		Enviar(PORTA,'#');
		Ler(PORTA);
		if (Valor=='$')
		{
			Ler(PORTA);
			if (Valor=='@') break;
			stam[Count]=Valor;
			Count++;
		};
		if ((Count==5)||(Valor=='@')) break;
	};
   return atoi(stam);
}
/***************************************************************************/
void Receber(unsigned int Porta)
{
	FILE *fp;
	unsigned long Count;
   char bloco[512];
	char *NomeDeArquivo;
	char *stam;
	long tam;
   unsigned int CRCnew,Sphere=0;
	NomeDeArquivo="            ";
	stam="                                 ";
	clrscr();
	Count=0;
	Ler(Porta);
	gotoxy(1,1); printf("Esperando receber\n");
	while(!(kbhit()))
	{
		Enviar(Porta,'.');
		Ler(Porta);
		if (Valor=='$')
		{
			Ler(Porta);
			if (Valor=='%') break;
			NomeDeArquivo[Count]=Valor;
			gotoxy(1,3); printf("Recebendo %s",NomeDeArquivo);
			Count++;
		};
		if ((Count==14)||(Valor=='%')) break;
	};
	printf("\n");
	Count=0;
	while(!(kbhit()))
	{
		Enviar(Porta,'#');
		Ler(Porta);
		if (Valor=='$')
		{
			Ler(Porta);
			if (Valor=='@') break;
			stam[Count]=Valor;
         gotoxy(1,5);
         printf("Tamanho: %s",stam);
			Count++;
		};
		if ((Count==33)||(Valor=='@')) break;
	};
	tam=atol(stam);
	if (!(fp=fopen(NomeDeArquivo,"wb")))
	{
		printf(" \n\nO arquivo n„o pode ser aberto\n");
		exit(2);
	};
	Count=0;
	while(!(kbhit()))
	{
		Enviar(Porta,'.');
		Ler(Porta);
		if (Valor=='$')
		{
			Ler(Porta);
			putc(Valor,fp);
//         bloco[Sphere]=Valor;
//         Sphere++;
	  		Count++;
 /*        if (((Sphere % 512)==0)&&(Sphere>0))
         {
            CRCnew=LerCRC();
            Sphere=0;
            if (CRCnew==Checksum(bloco,512))
               Enviar(Porta,'8'); else
               {
                  Enviar(Porta,'+');
                  fseek(fp,ftell(fp)-512,1);
               }
            CRCnew=LerCRC();
         }*/
         if((Count % 100)==0)
         {
             gotoxy(1,7);
             printf("Recebendo %d bytes.\n",Count);
         }
			if (Count>=tam) break;
		};
	};
	fclose(fp);
}
/***************************************************************************/
void MandarNomeDeArquivo(char nome[13])
{
	unsigned char ball,a;
	ball=0;
	gotoxy(1,1); printf("Enviando: ");
	while (strlen(nome)<13) strcat(nome," ");
	for (a=0;a>=0;a++)
	{
		Ler(PORTA);
		if (Valor=='.')
		{
			Enviar(PORTA,'$');
			Enviar(PORTA,nome[ball]);
			printf("%c",nome[ball]);
			ball++;
		};
		if (ball==13) break;
		if (kbhit()) break;
	};
	Enviar(PORTA,'%');
	printf("\n");
};
/***************************************************************************/
void MandarTamanhoDeArquivo(long tam)
{
	char stam[33];
	unsigned char a,Count;
	Count=0;
	ltoa(tam,stam,10);
	while (strlen(stam)<33) strcat(stam," ");
	gotoxy(1,3); printf("Tamanho: ");
	for (a=0;a>=0;a++)
	{
		Ler(PORTA);
		if (Valor=='#')
		{
			Enviar(PORTA,'$');
			Enviar(PORTA,stam[Count]);
			printf("%c",stam[Count]);
			Count++;
		};
		if (Count==33) break;
		if (kbhit()) break;
	};
	Enviar(PORTA,'@');
};
/***************************************************************************/
void MandarCRC(long tam)
{
	char stam[5];
	unsigned char a,Count;
	Count=0;
	ltoa(tam,stam,10);
	while (strlen(stam)<5) strcat(stam," ");
	for (a=0;a>=0;a++)
	{
		Ler(PORTA);
		if (Valor=='#')
		{
			Enviar(PORTA,'$');
			Enviar(PORTA,stam[Count]);
			printf("%c",stam[Count]);
			Count++;
		};
		if (Count==5) break;
		if (kbhit()) break;
	};
	Enviar(PORTA,'@');
};
/***************************************************************************/
void InicializaSerial(unsigned int Porta)
{
	asm{
		mov dx,Porta
		mov ax,0x00e7
		int 0x14
		mov dx,Porta
		mov ax,0x013f
		int 0x14
	};
	outp(com[Porta]+3,0x83);
	outp(com[Porta],1);
	outp(com[Porta]+1,0);
	outp(com[Porta]+3,3);
	outp(com[Porta],0x00);
};
/***************************************************************************/
void main(int argc,char *argv[])
{
	struct REGPACK regs;
	struct ffblk s;
	int c,x,achar,a;
	long ball,Sphere=0;
   char bloco[512];
	FILE *fp;
	for(x=0;x<=3;x++) com[x]=peek(0x40,(x)*2);
	InicializaSerial(0);
	if (argc<2)
	{
		printf("\n\nUSO: TCOPY t <nome-de-arquivo>\n");
		printf("ou         TCOPY r\n\n");
		exit(1);
	};
	if (tolower(*argv[1])=='r')
	{
		Receber(0);
		exit(0);
	};
	if (tolower(*argv[1])!='t')
	{
		printf("\n\nUSO: TCOPY t <nome-de-arquivo>\n");
		printf("ou         TCOPY r\n\n");
		exit(1);
	};
	achar=findfirst(argv[2],&s,0);
	if (achar)
	{
		printf("Arquivo n„o encontrado\n");
		exit(3);
	};
	clrscr();
	while(strlen(s.ff_name)<=13) strcat(s.ff_name," ");
	MandarNomeDeArquivo(s.ff_name);
	MandarTamanhoDeArquivo(s.ff_fsize);
	if (!(fp=fopen(argv[2],"rb")))
	{
		printf("O arquivo n„o pode ser aberto\n");
		exit(2);
	};
	c=1;
	ball=0;
	for (a=0;a>=0;a++)
	{
		Ler(PORTA);
		if (Valor=='.')
		{
			Enviar(PORTA,'$');
			c=getc(fp);
			Enviar(PORTA,c);
//         bloco[Sphere]=c;
//         Sphere++;
			if ((ball%512)==0)
         {
            delay(15);
        /*    if (ball>0)
            {
               MandarCRC(Checksum(bloco,512));
               Ler(PORTA);
               if (Valor=='+') fseek(fp,ftell(fp)-512,1);
               if (Valor=='8') gotoxy(1,4);
               Sphere=0;
            }*/
         }
			ball++;
         if((ball % 100)==0)
         {
             gotoxy(1,5);
             printf("Enviando %d bytes.\n",ball);
         }
			if (ball>=s.ff_fsize) break;
		};
		if (feof(fp)) break;
		if (kbhit())  break;
	};
	fclose(fp);
}

