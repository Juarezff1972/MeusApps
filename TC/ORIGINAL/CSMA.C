#include "dos.h"
#include "stdio.h"
#include "io.h"
#include "conio.h"
#include "bios.h"

#define buffer_TX    0x3f8
#define buffer_RX    0x3f8
#define line_status  0x3fd

#define    UCHAR unsigned char
#define    transmissao 0
#define    ERROR unsigned
#define    MAXIMO 53
#define    ACK    0x06
#define    NAK    0x15


union REGS r;

int         fim = 0,     /* indica o fim da simulacao de erro     */
            I   = 0,     /* grau do polinomio                     */
	    j   = 0,     /* contador                              */
	    z   = 0,     /* contador                              */
	    tam = 0,     /* indica o tamanho do block1            */
	    CONTADOR = 0,/* contem o numero de caracteres lidos   */
	    INDICE = 0,  /* indice o vetor de recepcao            */
            Nbytes = 0,  /* quantidade de frames                  */
	    status;
int    erro = 1;       /* indica se houve erro na transmissao */
unsigned    block     = 0, /* e o dividendo                       */
            i         = 0, /* contador                            */
	    indicebit = 16,/* incrementado ate chegar ao total    */
			   /* de bits do frame transmitido        */
	    resto     = 0, /* resto da divisao                    */
	    block1    = 0; /* contem o proximo byte a ser         */
			   /* dividido                            */
UCHAR        buf [1];       /* recebe o caracter lido              */
int         Frame [MAXIMO];    /* contem o frame a ser transmitido    */
char        final = 0;     /* indica o final do arquivo           */
char        over [1];      /* bit mais significativo do dividendo */
/*char        ACK [6] ;       contem o codigo ACK ja codificado   */
/*char        NAK [6] ;       contem o codigo NAK ja codificado   */
int        destino=0;       /*endereco no remoto*/
int        origem=0;        /*endereco no origem*/
FILE * file2;         /* para leitura */
FILE * arq2;          /* para gravasao*/


void interrupt Fisica ();
void interrupt (*salva) ();
void Enlace ();

void Janela (x0,y0,x1,y1,fundo)
int x0,y0,x1,y1,fundo;
{
   window (1,1,80,25);
   window (x0,y0,x1,y1);
   textattr(fundo);
   clrscr ();
}

/****************************************************************************/
/* inicializa os vetores ACK e NAK                                          */
/****************************************************************************/
/*void Init_Vetor ()
{
    ACK [0] = 0x02;
    ACK [1] = 0x06;
    ACK [2] = 0x03;
    ACK [3] = 0x17;
    ACK [4] = 0x68;
    ACK [5] = 0x4d;
    NAK [0] = 0x02;
    NAK [1] = 0x15;
    NAK [2] = 0x03;
    NAK [3] = 0x17;
    NAK [4] = 0x72;
    NAK [5] = 0x7e;
} */

void inicializacao ()
{
       j = 0;
       I = 0;
       indicebit = 16;
       over[0] = 0;
       tam = 0;
       z = 0;
       block = 0;
       resto = 0;
       buf[0] = 0;
}

Status_Registro ()
{
 char status=0;
 status = inportb(line_status);
 if (status & 1)
    return (1);
    else return (0);
 }


/****************************************************************************/
/*  le os frames a serem transmitidos                                       */
/****************************************************************************/
int    Ler_Arquivo (nome,Nbyte)
char     nome [20];
int      Nbyte;

{
    int         count = 0, /* numero de caracteres lidos         */
		hand;      /* handle                             */
    unsigned    quant,     /* posiciona o ponteiro no arquivo    */
		quant1;    /* numero de caracteres a serem lidos */

    if  ((file2  = (FILE *)fopen (nome,"r + b")) == 0){
	printf ("%s arquivo nao encontrado\n ",nome);
	exit (1);
    }
       quant1 = MAXIMO - 10;
       quant =  MAXIMO - 10;
    hand = fileno (file2);
    rewind (file2);
    fseek (file2,(quant * Nbyte),SEEK_SET);
    flushall ();
    while  (count < quant1){
	  _read (hand,buf,1);
          if ((buf [0] == 0x1a) || (buf[0] == 0)){
	     fclose (file2);
	     final = 26;
	     Frame[3] = 0x02;
	     break;
	    }
	  else{
		  Frame [count + 7] = buf [0];
		  count++;

	  }
    }
    fclose (file2);
    return (count);
}

/*****************************************************************************/
/* codificacao                                                               */
/*****************************************************************************/
void     Polinomio (doisbytes)
int      doisbytes;
{
    unsigned    quant,/* numero de codigos de transmissao              */
		bit;  /* bit do block1 a ser acrescentado ao dividendo */

       if (final == 26)
	   quant = 5;
       else
	   quant = 4;
    if ((resto == 0) && (block < 0x8000)){
        resto = doisbytes;
	doisbytes = 0;
    }
    do{
      if ((tam == 0) && ((
      (CONTADOR + quant) * 8) > indicebit)) {
	block1 = (unsigned)Frame[j+3];
        j++;
        indicebit += 8;
        tam = 8;
      }
      else{
	     if (tam != 0) {
		over [0] = (resto >> 15);
		bit = (block1 << z);
		bit = (bit & 0x00ff) >> 7;
                resto = (resto << 1) | (bit);
                tam--;
                z++;
		if (tam == 0)
		   z = 0;
	     }
	     else{
                    over [0] = (resto >> 15);
                    resto = resto << 1;
	            indicebit++;
		    I++;
	     }
      }
   } while ((over [0] == 0) && (I != 32)) ;
   if (over [0] == 1){
       resto = resto ^ 0x1021;
       over[0] = over[0] ^ 1;
       block = resto;
   }
}

/****************************************************************************/
/* preenche o vetor frame com os caracteres lidos                           */
/* transmite e verifica colisao                                             */
/****************************************************************************/
void    Envia_Frame (nome,nbyte)
char    nome [20];
int     nbyte;
{
    int   b = 0, /* contador                         */
	  quant, /* numero de codigos de transmissao */
	  rand,  /* handle                           */
	  pos;   /* posicao do ponteiro no arquivo   */
    int  porta [1]; /* recebe valor da serial           */
    unsigned   aux1 = 0,/* primeiro byte do dividendo */
	       aux2 = 0;/* segundo  byte do dividendo */

       for (b = 0; b < MAXIMO;b++){
       Frame [b] = 0x00;
       }

       Frame [0] = 0xAA;  /*preambulo*/
       Frame [1] = 0xAA;  /*preambulo*/
       Frame [2] = 0x80;  /*stat bit*/
       Frame [3] = 0x01;  /*prioridade*/
       Frame [4] = destino;    /*endereco destino*/
       Frame [5] = origem;    /*endereco origem */
       CONTADOR = Ler_Arquivo (nome,nbyte);
       Frame [6] = CONTADOR;
       aux1 = (unsigned)Frame[3];
       aux2 = (unsigned)Frame[4];
       block = (aux1 << 8)  |  (aux2);
       do{
	  Polinomio (block);
	 }while (I != 32);
       Janela(1,5,80,25,0x70);
       printf ("** resto = %x **\n",resto);
	 Frame[MAXIMO-3] = ((resto & 0xff00) >> 8);
	 Frame[MAXIMO-2] = (resto & 0x00ff);
	 Frame[MAXIMO-1] = 0x81;
            Janela(1,5,80,25,0x70);
	  for (b = 0; b < MAXIMO; b++){
	      printf  ("%c",Frame[b]);
	      outportb(buffer_TX, Frame [b]);
	      delay (5);
	      porta[0]= inportb(buffer_RX);
	      if (porta[0] != Frame [b])
		 {
		   outportb(buffer_TX,origem);
                   Janela(1,1,80,3,0x70);
		   printf ("* * * * * C O L I S A O * * * * *");
		   porta[0] = inportb(buffer_RX);
		   delay (11*origem);
		   outportb(buffer_TX,origem);
		   b=0;
		   Janela(1,5,80,25,0x70);
		   porta[0] = inportb(buffer_RX);
		   if (porta[0] != (char)origem)
		      Fisica ();
		    outportb(buffer_TX,Frame[b]);
		    delay (30);
		 }
	  }

}

void Envia_Arquivo ()
{
    char nome [20];
    int porta[1];
    UCHAR  aux1,aux2;  /* salva o valor de final              */
    int    g      = 0, /* contador                            */
	   erro = 1;       /* indica se houve erro na transmissao */

    printf ("\n");
     flushall ();
     printf ("NOME ==> ");
     gets (nome);
    while ((erro != 0) || (Frame[3] != 0x02)){
      inicializacao ();
      Envia_Frame (nome,Nbytes);
      do {
      if (Status_Registro ())
       {
	porta [0] = inportb(buffer_RX);
	break;
	}
      } while (1);
      if (porta[0] == 0x06){
	 Nbytes++;
         Janela(1,1,80,3,0x70);
	 printf ("\n********   ACK   ********\n");
	 erro = 0;
      }
      else{
	 Janela(1,1,80,3,0x70);
	 printf ("\n********   NAK   ********\n");
	 erro = 1;
      }
    }
}

void Enlace ()
{
    int  nome [2];
    UCHAR  aux1,aux2;       /* salva o valor de final              */

    unsigned b;
    inicializacao ();
     CONTADOR = Frame[6];
     if (CONTADOR < 43)
       CONTADOR++;
     if ((Frame[4] == origem) || (Frame [4] == 0))
	if(Frame[7] == 'M')
	  {
             Janela(1,5,80,25,0x70);
	     printf ("ORIGEM %i \n\n", Frame[5]);
	    for (b = 8; b < CONTADOR+8; b++)
	       printf("%c",Frame[b]);
	    erro = 0;
	    delay (2000);
	   }
	 else{
	     aux1 = (unsigned)Frame[3];
	     aux2 = (unsigned)Frame[4];
	     block = (aux1 << 8)  |  (aux2);
	     do{
		Polinomio (block);
	       }while (I != 32);
		nome [0] = ((resto & 0xff00) >> 8);
		nome [1] = (resto & 0x00ff);
	       if ((nome [0] == Frame[MAXIMO-3]) &&
		   (nome [1] == Frame [MAXIMO-2]))
		  {
		    outportb(buffer_TX, ACK);
		    erro = 0;
		    printf ("\n********   ACK   ********");
		    for (b = 0; b < MAXIMO; b++){
		      if((Frame [b] != 0xAA) && (Frame [b] != 0x80) &&
			 (Frame [b] != 0x01) && (Frame [b] != 0x81) &&
			 (Frame [b] != 0x00) && (Frame [b] != 0x02))
			 {
			  printf("%c",Frame[b]);
			  fprintf(arq2,"%c", Frame[b]);
			  }
		    }
		inportb(buffer_RX);
	       }
	       else{
		   outportb(buffer_TX,NAK);
		   printf ("\n********   NAK   ********");
		   erro = 1;
		   for (b = 0; b < MAXIMO; b++){
		      if((Frame [b] != 0xAA) && (Frame [b] != 0x80) &&
			 (Frame [b] != 0x01) && (Frame [b] != 0x81) &&
			 (Frame [b] != 0x00) && (Frame [b] != 0x02))
			 {
			  printf("%c",Frame[b]);
			  fprintf(arq2,"%c", Frame[b]);
			  }
		    }
	    inportb(buffer_RX);
     }
    }

}


/***************************************************************************/
/* preenche o vetor bloco com os caracteres lidos                           */
/****************************************************************************/
void     Envia_Mensagem ()
{
  char Bloco [MAXIMO];
  int       aux;
  char porta [1];
  unsigned   b;       /* contador                   */
  for (b=0; b < MAXIMO; b++){
     Bloco[b] = 0x00;
     Frame[b] = 0x00;}
      Janela(1,1,80,3,0x70);
      printf ("MENSAGEM ==> ");
      gets (Bloco);
      Frame [0] = 0xAA;  /*preambulo*/
      Frame [1] = 0xAA;  /*preambulo*/
      Frame [2] = 0x80;  /*stat bit*/
      Frame [3] = 0x02;  /*prioridade*/
      Frame [4] = destino;    /*endereco destino*/
      Frame [5] = origem;    /*endereco origem */
      aux = strlen (Bloco);
      Frame [6] = aux;
      Frame [7] = 'M';
      for(b=8; b < aux+9; b++)
	 Frame[b] = Bloco[b-8];
      Frame [MAXIMO-1] = 0x81;  /*stop bit*/
      for (b=0; b< MAXIMO; b++)
	Bloco[b] = Frame[b];
           Janela(1,5,80,25,0x70);
	  for (b = 0; b < MAXIMO; b++){
	      printf ("%c",Bloco[b]);
	      outportb(buffer_TX, Bloco[b]);
	      delay(5);
	       porta[0] = inportb(buffer_RX);
	       if (porta[0]  != Bloco[b])
		 {
		   outportb(buffer_TX,origem);
                   Janela(1,1,80,3,0x70);
		   printf ("* * * * * C O L I S A O * * * * *");
		   porta[0] = inportb(buffer_RX);
		   delay (11*origem);
		   outportb(buffer_TX,origem);
		   b=0;
		   Janela(1,5,80,25,0x70);
		   porta[0] = inportb(buffer_RX);
		   if (porta[0] != (char)origem)
		      Fisica ();
		    outportb(buffer_TX,Bloco[b]);
		    delay (30);
		 }
	  }
};


void Transmite ()
{
   char ch;
   int ter = 0;
   flushall ();
   Janela(1,1,80,3,0x70);
   printf ("DESTINO ==> ");
   scanf ("%i", &destino);
  printf("< A > TRANSMITE ARQUIVO      < C > TRANSMITE MENSAGEM  < F > FIM");
  flushall ();
   ch = getch ();
   if (toupper(ch)== 'A')
	Envia_Arquivo ();
   if (toupper(ch) == 'C')
	Envia_Mensagem ();
	Janela(1,1,80,3,0x70);
	Janela(1,5,80,25,0x70);
}

void interrupt Fisica ()
{
   int porta[1],i,saida=1;
   int  Buffer [MAXIMO];
  while (saida){
	if (Status_Registro())
	   {
	      porta[0] = inportb(buffer_RX);
	      if (porta[0] == 0x80)
		 INDICE = 2;
	      Buffer [INDICE] = porta[0];
	       if (Buffer[MAXIMO -1] == 0x81){
		 memcpy(Frame,Buffer,MAXIMO*2);
		  Enlace ();
		  INDICE = 0;
		  for(i=0; i < MAXIMO; i++)
		    Buffer [i] = 0x00;
		  if ((Frame [3] == 0x02) && (erro == 0)){
		   fclose (arq2);
		   getchar ();
		   Janela(1,1,80,3,0x70);
		   Janela(1,5,80,25,0x70);
		   saida=0;
		   }
		 }
	   else INDICE++;
       }
     }
}
main ()
{
   char ch;
   char status;
   int ter = 1;
   Janela(1,1,80,25,0x00);
   Janela(1,1,80,3,0x70);
   printf ("ORIGEM ==> ");
   scanf ("%i", &origem);
   flushall ();

   outportb (0x21,176);
   outportb(0x3f9,3);
   status = inportb (0x3fa);
   r.x.ax = 251;
   r.x.dx = 0;
   int86(0x14,&r,&r);
   /*outportb (0x3fb,0x80);
   outportb (0x3f8,0x06);
   outportb (0x3f9,0x00);

   outportb (0x3fb,0x80);
   outportb (0x3f9,0x01);
   outportb (0x3fc,0x0b);*/

   status = inportb(buffer_RX);
   /*salva = getvect (0x0c);
   setvect(0x0c,Fisica);*/

   Janela(1,5,80,25,0x70);
   do{
      if (Status_Registro ()){
      arq2 = (FILE *)fopen ("tes.txt","w+b");
	Fisica ();
	  }
      if (kbhit ())
	{
	 ch = getch ();
	 if (toupper(ch) == 'F')
	    ter = 0;
	 else
	   Transmite ();
	}
   }while(ter);
/*setvect (0x0c,salva);*/
}
