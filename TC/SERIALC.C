// Programa para copiar arquivos via RS-232
// Ciˆncia da Computa‡„o - 2§ ano
// Por:   Gustavo Henrique Busnardo  N§ 9
//        Aderson Cleber Piffer      N§ 1
//        Anderson F. Martins        N§ 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <io.h>
#include <conio.h>

#define BUFF_SIZE 32768
#define EndOF(a) filelength(fileno(a))
//////////////////////////////////////////////////////////////////////////
void Receptor(void);
void Transmissor(void);
void Transmite(int bytes);
void InicializaPorta(void);
int Recebe(void);
//////////////////////////////////////////////////////////////////////////
char string_bytes[6];
char nome_arq[20];
//char buffer[BUFF_SIZE];
union REGS w;
int flag=0;
int com;
FILE *recep_arq;
FILE *trans_arq;
char chflag;		/* checksumming enabled flag	*/
char cflag;		/* text-collection enabled flag */
char pflag;		/* pausing flag 		*/
char spflag;		/* stripping parity bit flag	*/
char lastc;		/* last char xmitted		*/
char abortf;		/* true when file I/O aborted	*/
char rbuf[BUFF_SIZE]; 	/* file I/O buffer for incoming
			   data file			*/
char tbuf[BUFF_SIZE]; 	/* sector buffer for file being
			   transmitted			*/
int rfd, tfd;		/* file descriptors		*/

char *cptr;		/* pointer to free space in buf */
unsigned bfree;		/* number of bytes free in buf	*/
int bcount;		/* counts bytes in current block
			   when checksumming		*/
int scount;		/* Number of sectors
				 sent/received		*/
int checksum;		/* the checksum value itself	*/
char timoutf;		/* true if time-out happens
			   while waiting for modem data	*/
char *i;		/* odd-job char pointer 	*/

int dod_sav, did_sav;	/* scratch variables		*/

unsigned bufspace;	/* # of bytes available for text
			   collection buffer in ram	*/

char *buf;		/* text collection pointer; will
			   point to the location just
			   after itself */

//////////////////////////////////////////////////////////////////////////
void main(int argc,char *argv[])
{
  if(argc!=4) {
    printf("Use o formato: SERIALC [R || T] <n§ da serial> <Nome-do-arquivo>\n");
    printf("Onde: R - Programa funciona como receptor.\n");
    printf("      T - Programa funciona como transmissor.\n");
    exit(1);
  }

  if(strcmp(toupper(argv[1]),"R")!=0 && strcmp(toupper(argv[1]),"T")!=0) {
    printf("Funcao invalida. Digite SERIALC para help.\n");
    exit(1);
  }
  if(atoi(argv[2])<1 || atoi(argv[2])>4) {
    printf("Numero de porta serial invalido. Validos: 1 a 4\n");
  }
  com=atoi(argv[2])-1;
  strcpy(nome_arq,argv[3]);
  if(strcmp(toupper(argv[1]),"R")==0)
     Receptor();
  if(strcmp(toupper(argv[1]),"T")==0)
     Transmissor();
  printf("\nFim do programa.\n");*/
  com=0; // com2:
  InicializaPorta();
  while(!kbhit()) {
    //w.x.dx=com;
    //w.h.ah=2;
    //int86(0x14,&w,&w);
    //if(w.h.al=='i') break;
//    flag=w.h.al;
    flag=inportb(0x3f8);
    printf("%d - %c\n",flag,flag);
  }
}
//////////////////////////////////////////////////////////////////////////
void InicializaPorta()
{
   w.x.dx=com;   // porta serial
   w.h.ah=0;   // 0 = Funcao de CONFIGURACAO
   w.h.al=231; // 9600 bauds, s/paridade, 2 stop bits, 8 bits de dados
   int86(0x14,&w,&w);
}
//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////
void Receptor(void)
{
  if((recep_arq=fopen(nome_arq,"wb"))==NULL) {
    printf("Arquivo nao pode ser aberto.\n");
    exit(1);
   }
  InicializaPorta();
  while(1) {
    w.x.dx=com;
    w.h.ah=2;
    int86(0x14,&w,&w);
    if(w.h.al=='i') break;
  }
  while(1) {
    if(Recebe()) break;
   }
  fclose(recep_arq);
  return;
}
//////////////////////////////////////////////////////////////////////////
int Recebe(void)
{
  unsigned i;
  unsigned bytes_a_receber=0;
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
    w.x.dx=com;
    w.h.ah=1;
    w.h.al='o';
    int86(0x14,&w,&w);
    i++;
   }
  buffer[i]=0;
  printf("Escrevendo bloco em disco : %u bytes.\n",bytes_a_receber);
  fwrite(buffer,bytes_a_receber,1,recep_arq);
  w.x.dx=com;
  w.h.ah=1;
  w.h.al='c';
  int86(0x14,&w,&w);
  if(bytes_a_receber<BUFF_SIZE) return 1;
  else return 0;
}
//////////////////////////////////////////////////////////////////////////
void Outmod(char c)
{
	if(Kbabort()) return;
	outp(MDATA,c);
}
//////////////////////////////////////////////////////////////////////////
void Xmite()
{
	int incheck;
	int n;
	char c;
	if(pflag) return 0;
	c=tbuf[bcount++];
	checksum+=c;
   outmod(c);
	lastc=c;
	if(dodflag) display(c);
	if(bcount!=SECSIZ) return 0;
	bcount=0;
	if(!chflag) return !read1();
	incheck=(getmod()<<8)+getmod();
	if(incheck!=checksum) {
		for (n=0;n<20000;n++); 	// let line settle down
		printf("\nError. Resending sector %d...\n",scount+1);
		outmod(NAK);
	}
	else if(read1()) {
		if(!dodflag) printf("Good sector <%d>\n",++scount);
		outmod(ACK);
	}
          else { outmod(EOT); return 1; }

	checksum=0;
	if(getmod()!=0xFD) {
			printf("\nPhase error; aborting...");
			abortf=1;
			}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
void Transmite(int bytes_lidos)
{
  unsigned i;
  InicializaPorta();
  itoa(bytes_lidos,string_bytes,10);
  if(flag==0) {
    w.x.dx=com;
    w.h.ah=1;
    w.h.al='i';
    int86(0x14,&w,&w);
    flag=1;
  }
  printf("Transmitindo tamanho do bloco : %u bytes\n",bytes_lidos);
  i=0;
  while(i<=6) {
  Xmite();
   if(w.h.ah>=0x80) {
	   printf("\nErro na transmissao do byte.\n");
	   exit(1);
   }
   printf("Transmitindo bloco...\n");
   i=0;
   while(i<=bytes_lidos) {
   w.x.dx=com;
   w.h.ah=1;
   w.h.al=buffer[i];
   int86(0x14,&w,&w);
   if(w.h.ah>=0x80) {
	   printf("\nErro na transmissao do byte.\n");
   	exit(1);
   }
   while(1) {
      w.x.dx=com;
      w.h.ah=2;
      int86(0x14,&w,&w);
      if(w.h.al=='o') break;
     }
    i++;
   }
  printf("Fim da transmissao do bloco.\n");
  while(1) {
    w.x.dx=com;
    w.h.ah=2;
    int86(0x14,&w,&w);
    if(w.h.al=='c')
      break;
  }
}
//////////////////////////////////////////////////////////////////////////
int Kbabort()
{
	if((kbhit()&&getch()==ABORTAR) {
		abortf=1;
		return 1;
	}
	return 0;
}