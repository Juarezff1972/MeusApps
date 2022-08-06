// Programa para copiar arquivos via RS-232
// Ciencia da Computacao - 2§ ano
//  Alunos:  Aderson Cleber Piffer      N§  1
//           Anderson F. Martins        N§  3
//           Gustavo Henrique Busnardo  N§  9
//           Joseane Spessatto          N§ 12
//           Juarez Fioravanti          N§ 13
///////////////////////////////////////////////////////////////////////
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <io.h>
///////////////////////////////////////////////////////////////////////
#define BUFF_SIZE 10000
#define EndOF(a) filelength(fileno(a))
///////////////////////////////////////////////////////////////////////
void Receptor(void);
void Transmissor(void);
void Transmite(int bytes);
int Recebe(void);
void Enviar(unsigned int Porta,unsigned int Valor);
///////////////////////////////////////////////////////////////////////
char string_bytes[6];
char nome_arq[20];
char buffer[BUFF_SIZE];
union REGS w;
int flag=0;
int com;
FILE *recep_arq;
///////////////////////////////////////////////////////////////////////
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
  com=atoi(argv[2])-1;
  strcpy(nome_arq,argv[3]);
  if(strcmp(argv[1],"r")==0 || strcmp(argv[1],"R")==0)
    Receptor();
  if(strcmp(argv[1],"t")==0 || strcmp(argv[1],"T")==0)
    Transmissor();
  printf("\nFim do programa.\n");
}
///////////////////////////////////////////////////////////////////////
void Enviar(unsigned int Porta,unsigned int Valor)
{
	unsigned int c;
	c=Porta;
	delay(8);
	outp(c,Valor);
};
///////////////////////////////////////////////////////////////////////
void Transmissor(void)
{
  unsigned bytes_lidos=0;
  FILE *trans_arq;
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
///////////////////////////////////////////////////////////////////////
void Receptor(void)
{
  if((recep_arq=fopen(nome_arq,"wb"))==NULL) {
    printf("Arquivo nao pode ser aberto.\n");
    exit(1);
   }
  w.x.dx=com;   // porta serial
  w.h.ah=0;   // 0 = Funcao de CONFIGURACAO
  w.h.al=231; // 9600 bauds, s/paridade, 2 stop bits, 8 bits de dados
  int86(0x14,&w,&w);
  while(!kbhit()) {
    w.x.dx=com;
    w.h.ah=2;
    int86(0x14,&w,&w);
    if(w.h.al=='i') {
/*      w.x.dx=com;
      w.h.ah=1;
      w.h.al='o';
      int86(0x14,&w,&w);*/
      Enviar(com,111);
     }
    else break;
  }
  while(!kbhit()) {
    if(Recebe()) break;
   }
  fclose(recep_arq);
  return;
}
///////////////////////////////////////////////////////////////////////
int Recebe(void)
{
  unsigned i;
  unsigned bytes_a_receber=0;
  w.x.dx=com;   // porta serial
  w.h.ah=0;   // 0 = Funcao de CONFIGURACAO
  w.h.al=231; // 9600 bauds, s/paridade, 2 stop bits, 8 bits de dados
  int86(0x14,&w,&w);
  printf("Recebendo tamanho do bloco : ");
  i=0;
  while(i<=6) {
    w.x.dx=com;
    w.h.ah=2;
    int86(0x14,&w,&w);
    string_bytes[i]=w.h.al;
    i++;
   }
  bytes_a_receber=atoi(string_bytes);
  printf(" %u bytes.\n",bytes_a_receber);
  printf("Recebendo bloco...\n");
  i=0;
  while(i<=bytes_a_receber) {
    w.x.dx=com;
    w.h.ah=2;
    int86(0x14,&w,&w);
    buffer[i]=w.h.al;
    i++;
   }
  buffer[i]=0;
  printf("Escrevendo bloco em disco : %u bytes.\n",bytes_a_receber);
  fwrite(buffer,bytes_a_receber,1,recep_arq);
  if(bytes_a_receber<BUFF_SIZE) return 1;
  else return 0;
}
///////////////////////////////////////////////////////////////////////
void Transmite(int bytes_lidos)
{
  unsigned i;
  w.x.dx=com;   // porta serial
  w.h.ah=0;   // 0 = Funcao de CONFIGURACAO
  w.h.al=231; // 9600 bauds, s/paridade, 2 stop bits, 8 bits de dados
  int86(0x14,&w,&w);
  itoa(bytes_lidos,string_bytes,10);
  if(flag==0) {
    while(!kbhit()) {
      inicio:
/*      w.x.dx=com;
      w.h.ah=1;
      w.h.al='i';
      int86(0x14,&w,&w);*/
      Enviar(com,105);
      w.x.dx=com;
      w.h.ah=2;
      int86(0x14,&w,&w);
      if(w.h.al=='o') break;
      else goto inicio;
/*    if(w.h.ah>=0x80) {
	   printf("\nErro na transmissao do i (byte de inicio de transm.)\n");
	   exit(1);
      }   */
    }
    flag=1;
  }
  printf("Transmitindo tamanho do bloco : %u bytes\n",bytes_lidos);
  i=0;
  while(i<=6) {
/*    w.x.dx=com;
    w.h.ah=1;
    w.h.al=string_bytes[i];
    int86(0x14,&w,&w);*/
    Enviar(com,string_bytes[i]);
/*    if(w.h.ah>=0x80) {
	 printf("\nErro na transmissao do tamanho do bloco.\n");
	 exit(1);
    }  */
    i++;
  }
  printf("Transmitindo bloco...\n");
  i=0;
  while(i<=bytes_lidos) {
/*    w.x.dx=com;
    w.h.ah=1;
    w.h.al=buffer[i];
    int86(0x14,&w,&w);*/
    Enviar(com,buffer[i]);
    /*  if(w.h.ah>=0x80) {
	printf("\nErro na transmissao do bloco.\n");
	exit(1);
       } */
  printf("Fim da transmissao do bloco.\n");
  }
}
