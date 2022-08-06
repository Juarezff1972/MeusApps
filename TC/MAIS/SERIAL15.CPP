// Programa para copiar arquivos via RS-232
// Ciˆncia da Computa‡„o - 2§ ano
// Por:   Aderson Cleber Piffer      N§ 1
//        Anderson F. Martins        N§ 3
//        Gustavo Henrique Busnardo  N§ 9
//        Juarez Fioravanti Filho    N§ 13
//        Joseane Spessatto          N§ 12

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <io.h>
#include <conio.h>
#include "crc_ccit.h"

#define BUFF_SIZE 5000
#define EndOF(a) filelength(fileno(a))

void Receptor(void);
void Transmissor(void);
void Transmite(int bytes);
void Recebe(void);
unsigned int Checksum(register unsigned char *s, register int n);

char string_bytes[15];
char nome_arq[20];
char buffer[BUFF_SIZE];
char ack;
union REGS w;
int flag=0;
unsigned com;
int num_com;
int Beethoven;
void ConfiguraPorta(void);
FILE *recep_arq;
FILE *trans_arq;

void main(int argc,char *argv[])
{
  if(argc!=4) {
    printf("Use o formato: SERIALC [R || T] <n§ da serial> <Nome-do-arquivo>\n");
    printf("Onde: R - Programa funciona como receptor.\n");
    printf("      T - Programa funciona como transmissor.\n");
    exit(1);
  }
  if(strcmp(argv[1],"R")!=0 && strcmp(argv[1],"T")!=0 &&
	strcmp(argv[1],"r")!=0 && strcmp(argv[1],"t")!=0) {
    printf("Funcao invalida. Digite SERIALC para help.\n");
    exit(1);
  }
  if(atoi(argv[2])<1 || atoi(argv[2])>4) {
    printf("Numero de porta serial invalido. Validos: 1 a 4\n");
  }
  num_com=atoi(argv[2])-1;
  if(num_com==0) com=0x3f8;
  if(num_com==1) com=0x2f8;
  strcpy(nome_arq,argv[3]);
  if(strcmp(argv[1],"r")==0 || strcmp(argv[1],"R")==0)
    Receptor();
  if(strcmp(argv[1],"t")==0 || strcmp(argv[1],"T")==0)
    Transmissor();
  printf("\nFim do programa.\n");
}

void Transmissor(void)
{
  unsigned bytes_lidos=0;
  if((trans_arq=fopen(nome_arq,"rb"))==NULL) {
    printf("Arquivo nao encontrado.\n");
    exit(1);
   }
// Este while le BUFF_SIZE unidades de 1 byte e grava no array buffer.
  while(1) {
    printf("Lendo arquivo : \n");
    bytes_lidos=fread(buffer,1,BUFF_SIZE,trans_arq);
    Transmite(bytes_lidos);
    if(ftell(trans_arq)>=EndOF(trans_arq))
	 break;
  }
  fclose(trans_arq);
  printf("Final da transmissao.\n");
}

void Receptor(void)
{
  if((recep_arq=fopen(nome_arq,"wb"))==NULL) {
    printf("Arquivo nao pode ser aberto.\n");
    exit(1);
   }
  ConfiguraPorta();
  while(1) {
    printf("\n Aguardando inicio da transmissao...\n");
    if(inp(com)=='i') break;
  }
  while(1) {
    printf("\n Estou no while do Andrey (aquele do Recebe()???)\n");
    Recebe();
    printf("\n Musica=%d \n",Beethoven);
    if(Beethoven==1) break;
   }
  fclose(recep_arq);
  return;
}

void Recebe(void)
{
  unsigned k;
  char aux[6];
  unsigned i,j,crc,crc2;
  unsigned bytes_a_receber=0;
  printf("Recebendo tamanho do bloco : ");
  for(i=0;i<=15;i++) string_bytes[i]=0;
  i=0;
  while(string_bytes[i-1]!='*') {
    string_bytes[i-1]=inp(com);
    i++;
    printf("\n Acabei de receber um byte do string_bytes!!!");
    printf("\n O string atual e: %s\n",string_bytes);
    printf("O i vale agora: %u\n",i);
    outp(com,'o');
   }
  for(i=0;i<=6;i++) aux[i]=0;
  i=0;
  while(string_bytes[i]!='#') {
     aux[i]=string_bytes[i];
     i++;
    }
  aux[i]=0;
  bytes_a_receber=atoi(aux);
  i++;
  j=0;
  while(string_bytes[i]!='*') {
    aux[j]=string_bytes[i];
    i++;
    j++;
   }
  aux[i]=0;
  crc=atoi(aux); //
  outp(com,'o');
inicio:
  for(i=0;i<BUFF_SIZE;i++) buffer[i]=0;
  printf(" %u bytes.\n",bytes_a_receber);
  printf("Recebendo bloco...\n");
  i=0;
  while(i<=bytes_a_receber) {
    buffer[i]=inp(com);
    i++;
    outp(com,'o');
   }
/*  for(k=0;k<=50;k++) {
    w.x.dx=com;
    w.h.ah=1;
    w.h.al='o';
    int86(0x14,&w,&w);
  }*/
  buffer[i]=0;
  crc2=Checksum(buffer,bytes_a_receber);
  printf("\n O crc no receptor e %u \n",crc2);
  if(crc==crc2) outp(com,'o');
  if(crc!=crc2) {
    outp(com,'m');
    goto inicio;
   }
  printf("Escrevendo bloco em disco : %u bytes.\n",bytes_a_receber);
  fwrite(buffer,bytes_a_receber,1,recep_arq);
  outp(com,'c');
  if(bytes_a_receber<BUFF_SIZE)
  {
     Beethoven=1;
  }
  else
  {
     Beethoven=0;
     goto SAIA_PO;
  }
SAIA_PO:
}

void ConfiguraPorta()
{
   asm{
		mov dx,num_com
		mov ax,0x00e7
		int 0x14
		mov dx,num_com
		mov ax,0x013f
		int 0x14
	  };
	outp(com+3,0x83);
	outp(com,1);
	outp(com+1,0);
	outp(com+3,3);
	outp(com,0x00);
}

void Transmite(int bytes_lidos)
{
  unsigned i,crc;
  char string_crc[6];
  ConfiguraPorta();
  itoa(bytes_lidos,string_bytes,10);
  if(flag==0) {
    outp(com,'i');
    flag=1;
  }
  crc=Checksum(buffer,bytes_lidos);
  ultoa(crc,string_crc,10);
  strcat(string_bytes,"#"); // separa tamanho do bloco do no. crc
  strcat(string_bytes,string_crc);
  strcat(string_bytes,"*"); // determina fim do string
  printf("Transmitindo tamanho do bloco : %u bytes\n",bytes_lidos);
  i=0;
  while(string_bytes[i]!='*') {
    outp(com,string_bytes[i]);
    while(1) if(inp(com)=='o') break;
    i++;
   }
   if(string_bytes[i]=='*') outp(com,'*');
   while(1) {
     printf("\nEsperando o 'o' p/ comecar a transm. bloco...\n");
     if(inp(com)=='o') break;
    }
recomeco:
   printf("\nTransmitindo bloco...\n");
   i=0;
   while(i<=bytes_lidos) {
     outp(com,buffer[i]);
     while(1) if(inp(com)=='o') break;
    }
   i++;
  while(1) {
    printf("\nEsperando para ver o que aconteceu com o bloco...\n");
    printf("(se deu o (ok) ou m (merda))\n");
    ack=inp(com);
    if(ack=='o') break;
    if(ack=='m') goto recomeco;
  }
  printf("Fim da transmissao do bloco.\n");
  while(1) {
    printf("\n Esperando o receptor gravar em disco e autorizar o");
    printf("proximo bloco (se houver, e claro)\n");
    if(inp(com)=='c')  break;
  }
  return;
}

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