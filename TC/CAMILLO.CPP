#define PORT 0

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "codborda.h"

void sport(int port, char c);
rport(int port);
void recebe(void);
void recebe_arquivo(void);
void recebe_mensagem();
void port_init(int port,unsigned char code);
char wait();
void menu();
void main();
void envia_mensagem();
void envia_arquivo();

char opcao;
char *tipo;
char *rec;

void port_init(int port, unsigned char code)
{
   union REGS r;
   r.x.dx=port; /*port serial*/
   r.h.ah=0;  /*funcao de iniciacao do port*/
   r.h.al=code; /*codigo de iniciacao*/
   int86(0x14, &r, &r);
}

void menu()
{
  clrscr();
  borda1(7,2,50,14,LIGHTBLUE,BLACK);
  gotoxy(10,4);
  printf("Enviar [M]ensagem ");
  gotoxy(10,6);
  printf("Enviar [A]rquivo ");
  gotoxy(10,8);
  printf("Aguardar [T]ransmissao");
  gotoxy(10,10);
  printf("[S]air");
  gotoxy(10,12);
  printf("Opcao: ");
  opcao = toupper(getche());
  switch(opcao){
      case 'M': envia_mensagem();
		break;
      case 'A': envia_arquivo();
		break;
      case 'T': break;
      case 'S': exit(0);
		break;
      default:{
	   gotoxy(10,13);
	   printf("Selecione opcao valida!!!!!");
	   delay(500);
	   menu();
	   }
  }
}

void main()
{
 char q;
 port_init(PORT,231);
 while(opcao != 'S'){
    clrscr();
    borda2(10,10,70,20,LIGHTBLUE,BLACK);
    gotoxy(15,25);
    printf("Aguardando transmissao...");
    gotoxy(20,19);
    printf("Pressione qualquer tecla para outra opcao!");
    q=wait();
    if(q=='.') recebe();
    else menu();
 }
}

void envia_mensagem()
{
  int cont,i;
  char *mensagem;
  clrscr();
  borda2(5,5,75,12,LIGHTBLUE,BLACK);
  gotoxy(7,7);
  printf("Mensagem: ");
  gets(mensagem);
  tipo="0";
  strcat(mensagem,tipo);
  strcat(mensagem,"è");
  cont=strlen(mensagem);
  for(i=0;i<3;i++) sport(PORT,'.');
  for(i=0;i<cont;i++)  sport(PORT,mensagem[i]);
  while(!kbhit);
  return;
}
/* Envia  um arquivo especifico */
void envia_arquivo()
 {
  int t,g;
  FILE *fp;
  char ch;
  char *fname;
   clrscr();
   borda2(5,5,75,12,LIGHTBLUE,BLACK);
   gotoxy(7,7);
   printf("Nome do arquivo a transmitir: ");
   gets(fname);
   if (!(fp=fopen(fname,"rb")))
    {
     gotoxy(12,10);
     printf("O arquivo nao pode ser aberto!");
     menu();
    }
  tipo="1";
  strcat(fname,tipo);
  strcat(fname,"è");
  g = strlen(fname);
  strcat(fname,'\0');
  for(t=0;t<5;t++) sport(PORT,'.');
  for(t=0;t<g;t++) sport(PORT,fname[t]);

  for(t=0; t < 9; t++) sport(PORT,'.');

  do{
     ch=getc(fp);
     sport(PORT, ch);
   }while(!feof(fp));
   sport(PORT,'è');
   fclose(fp);
}


void recebe(void)
{
  char y;
  int i=0;
  y = rport(PORT);
  while(y == '.') y = rport(PORT);
  while(y != 'è')
   {
     rec[i]=y;
     y = rport(PORT);
     i++;
   }
  rec[i]='\0';
  if (rec[i-1] == '0') recebe_mensagem();
  if (rec[i-1] == '1') recebe_arquivo();
  rec[i-1]='\0';
}


/*Recebendo um arquivo*/
void recebe_arquivo()
{
  FILE *fp;
  char ch;
  char *fname;
  fname = rec;
  remove(fname);
  if(!(fp=fopen(fname, "wb"))){
     clrscr();
     borda2(8,22,72,24,LIGHTBLUE,BLACK);
     printf("O arquivo nao pode ser aberto.Pressione qualquer tecla...");
     while(!kbhit);
     exit(1);
  }
  else {
    ch = rport(PORT);
    while(ch == '.') ch = rport(PORT);
    while(ch != 'è') {
      putc(ch,fp);
      ch = rport(PORT);
    }
  }
  fclose(fp);
}

void recebe_mensagem()
{
 clrscr();
 borda2(5,5,75,12,LIGHTBLUE,BLACK);
 gotoxy(31,6);
 printf("Mensagem recebida.");
 gotoxy(7,7);
 printf("%s",rec);
 gotoxy(10,11);
 printf("Pressione qualquer tecla para retornar ...");
 while(!kbhit);
 return;
}


/*Espera por uma resposta*/
char wait()
{
  char c;
  do
   c = rport(PORT);
  while((c != '.') || (!kbhit));
  if (c == '.') return c;
  else return(0);
}

/*Envia um caracter para a saida do port serial*/
void sport(int port,char c)
{
  union REGS r;
  r.x.dx=port; /*porta serial*/
  r.h.al=c; /*caractere a ser enviado*/
  r.h.ah=1; /*funcao do envio de caracter*/
  int86(0x14, &r, &r);
  if(r.h.ah & 128)  {  /*verifica bit 7 */
     printf("detectado erro no port serial\n");
     exit(1);
  }
}

/*Le um caractere do port*/
rport(int port)
{
  union REGS r;
   r.x.dx=port; /*port serial*/
   r.h.ah=2; /*funcao de leitura de um caractere*/
   int86(0x14, &r, &r);
   if(r.h.ah & 128)
      printf("detectado erro de leitura no port serial\n");
   return r.h.al;
}


