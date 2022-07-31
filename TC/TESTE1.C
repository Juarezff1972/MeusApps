/***********************************************************************/
/* Programa de transferencia de arquivos usando um software handshaking*/
/* Port inicializado para velocidade 9600,                             */
/* sem paridade,                                                       */
/* oito bits de dados,                                                 */
/* dois stop bits.                                                     */
/***********************************************************************/

#define PORT 0

#include <dos.h>
#include <stdio.h>

/*************************************************************************/
/*Verifica o status do port serial*/
check_stat(port)
int port; /*port e/s */
{
  union REGS r;
  r.x.dx=port; /*port serial*/
  r.h.ah=3; /*le status*/
  int86(0x14, &r, &r);
  return r.x.ax;
}
/*************************************************************************/
/*Retorna o tamanho, em bytes, de um arquivo */
unsigned int filesize(fp)
FILE *fp;
{
  unsigned long int i;
  i=0;
  do{
	 getc(fp);
	 i++;
	 }while(!feof(fp));
	 rewind(fp);
	 i--;
	 return i; /*nao conta o caracter EOF */
}
/*************************************************************************/
/*Envia um caracter para a saida do port serial*/
void sport(port,c)
int port; /*port e/s*/
char c;  /*caractere a ser enviado */
{
  union REGS r;
  r.x.dx=port; /*porta serial*/
  r.h.al=c; /*caractere a ser enviado*/
  r.h.ah=1; /*funcao do envio de caracter*/
  int86(0x14, &r, &r);
  if(r.h.ah & 0x80)  {  /*verifica bit 7 */
	  printf("detectado erro no port serial\n");
	  exit(1);
  }
}
/*************************************************************************/
/*Le um caractere do port*/
rport(port)
int port;  /*port e/s */
{
  union REGS r;
  char *c;
  /*espera por um caractere*/
  while(!(check_stat(PORT)&0x20))
	  if(kbhit())  { /*aborta se pressionar alguma tecla*/
		 getch();
		 exit(1);
	}
	r.x.dx=port; /*port serial*/
	r.h.ah=2; /*funcao de leitura de um caractere*/
	int86(0x14, &r, &r);
	if(r.h.ah != 0) return 0;
	*c=r.h.al;
	return 1;
}
/*************************************************************************/
/*Espera por uma resposta*/
void wait(port)
int port;
{
  if(rport(port)!='.')  {
	 printf("erro de comunicacao\n");
	 exit(1);
  }
}
/*************************************************************************/
/*Recebe o nome do arquivo*/
void get_file_name(f)
char *f;
{
  printf("Esperando pela recepcao...\n");
  while(rport(PORT)!='?');
  sport(PORT,'.'); /*avisa o recebimento*/
  while((*f==rport(PORT))){
	  if(*f!='?') {
	f++;
	sport(PORT,'.'); /*avisa o recebimento*/
	  }
  }
}
/*************************************************************************/
/*Recebendo um arquivo*/
void rec_file()
{
  FILE *fp;
  char ch;
  char fname[14];
  union{
	 char c[2];
	 unsigned int count;
  }cnt;

  get_file_name(fname);  /*obtem o nome do arquivo*/
  printf("recebendo oo arquivo %s\n", fname);
  remove(fname);
  if(!(fp=fopen(fname, "wb"))){
	  printf("o arquivo nao pode ser aberto\n");
	  exit(1);
  }
  /*obtem o tamanho do arquivo*/
  sport(PORT, '.');  /*acusa o recebimento*/
  cnt.c[0]=rport(PORT);
  sport(PORT, '.');  /*acusa o recebimento*/
  cnt.c[1]=rport(PORT);
  sport(PORT, '.');  /*acusa o recebimento*/

  for(;cnt.count;cnt.count--){
	 ch=rport(PORT);
	 putc(ch, fp);
	 if(ferror(fp)) {
		printf("erro de escrita no arquivo");
		exit(1);
		}
		sport(PORT, '.');  /*acusa o recebimento*/
  }
  fclose(fp);
}
/*************************************************************************/
/*Envia o nome do arquivo*/
void send_file_name(f)
char *f;
{
  printf("esperando pela transmissao... \n");
  do {
	 sport(PORT, '?');
  }while(!kbhit() && !(check_stat(PORT)&255));
  if(kbhit()){
	 getch();
	 exit(1);
  }
  wait(PORT); /*espera o receptor acusar o recebimento*/
  printf("enviando %s\n",f);

  /*agora, enviando o nome*/
  while(*f){
	 sport(PORT, *f++);
	 wait(PORT); /*espera o receptor acusar o recebimento*/
	 }
  sport(PORT, '\0');  /*terminador null*/
}
/*************************************************************************/
/* Envia  um arquivo especifico */
void send_file(fname)
char *fname;
 {
  FILE *fp;
  char ch;
  union {
	  char c[2];
	  unsigned int count;
	 } cnt;

	if (!(fp=fopen(fname,"rb")))
	 {
	  printf("O arquivo nao pode ser aberto/n");
	  exit(1);
	 }

  send_file_name(fname); /*envia o nome do arquivo*/

  wait(PORT); /*espera para o receptor acusar o recebimento*/
  /*descobrir o tamanho do arquivo*/
  cnt.count=filesize(fp);
  /*envia o tamanho*/
  sport(PORT, cnt.c[0]);
  wait(PORT);
  sport(PORT, cnt.c[1]);

  do{
	  ch=getc(fp);
	  if(ferror(fp)) {
	 printf("erro de leitura de arquivo");
	 break;
	 }
	  /*espera ate que o receptor esteja pronto*/
	  if(!feof(fp)){
	 wait(PORT);
	 sport(PORT, ch);
	 }
	}while(!feof(fp));
	wait(PORT); /*le o ultimo bloco do port*/
	fclose(fp);
}
/*************************************************************************/
/*iniciacao do port*/
void port_init()
{
	union REGS r;
	r.x.dx=0; /*port serial*/
	r.h.ah=0;  /*funcao de iniciacao do port*/
	r.h.al=231; /*codigo de iniciacao*/
	int86(0x14, &r, &r);
}
/*************************************************************************/
void main(argc,argv)
int argc;
char *argv[];
 {
  if(argc<2)
	{
	 printf("Uso: trans s nome do arquivo ou trans r\n");
	 exit(1);
	}

  printf("Programa de transferencia de arquivos em operacao.\n");
  printf("Para abortar pressione qualquer tecla.\n\n");

  port_init();  /* inicializacao da porta serial */

  if(tolower(*argv[1]) == 's')  send_file(argv[2]);
  else rec_file();
}
