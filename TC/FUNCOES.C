#include    "dos.h"
#include    "stdio.h"

#define  UCHAR  unsigned char



/* FUNCAO 128 EXECUTA LINHA DE COMANDO */

int ExecLinhaComando (endereco)
int far endereco;
{
  union  REGS regs;
  struct SREGS sregs;

  regs.h.ah = -1;
  regs.h.al = 128;
  regs.x.dx = FP_OFF (endereco);
  sregs.ds  = FP_SEG (endereco);
  intdosx (&regs, &regs, &sregs);
  return ((int)regs.h.al);
};


/* FUNCAO 150 (96H) - RETORNA INFORMACOES DE UM DISCO */

UCHAR RetorInfDisc (disco, canal,subcanal, reg_CL)
UCHAR * canal, * subcanal;
UCHAR * reg_CL;
int  disco;
{
  UCHAR conf;
  union REGS regs;

  regs.h.ah = -1;
  regs.h.al = 150;
  regs.h.dl = disco;
  intdos (&regs,&regs);
  conf = regs.h.ch;
  if (((conf>>7) & 0x1) == 1)
    printf ("Unidade nao configurada :\n");
  if (((conf>>6) & 0x1) == 1)
    printf ("Unidade Bloqueada:\n");
  if (((conf>>5) & 0x1) == 1)
    printf ("Acesso Remoto Bloqueado :\n");
  if (((conf>>4) & 0x1) == 1)
	 {
      printf ("Unidade Formato MS-DOS");
    }
    else printf ("Unidade Formato NET16-MB");
  if (((conf>>3) & 0x1) == 1)
    printf ("Unidade Removivel");

   if (regs.h.cl == 0)
    {
      printf ("UNIDADE LOCAL :\n");
      * canal = regs.h.bh;
      * subcanal = regs.h.bl;
      return ((UCHAR)regs.x.cx);
	 }
    else
      {
	if (regs.h.cl == (0x80||0x81||0x82||0x83||0x84))    /* CASO 8xH */
	  {
	    printf (" UNIDADE REMOTA :");
	    * canal = (UCHAR)regs.h.bh;
	    * subcanal = (UCHAR)regs.h.bl;
	    return ((UCHAR)regs.h.cl);
	  }
      }
}




/* FUNCAO 129 (81H) - BLOQUEIA RECURSO VIRTUAL */

int BRI (endereco)
int endereco;
{
  union REGS regs;
  struct SREGS sregs;

  regs.h.ah = -1;
  regs.h.al = 129;
  regs.x.dx = FP_OFF (endereco);
  sregs.ds  = FP_SEG (endereco);
  intdosx (&regs, &regs, &sregs);
  return ((UCHAR)regs.h.al);
}



/* FUNCAO 163 (0A3H) -SELECIONA IMPRESSORA */

int  SI (impressora)
{
  union REGS regs;
  regs.h.ah = -1;
  regs.h.al = 163;
  regs.h.dl = impressora;
  intdos (&regs, &regs);
  return ((int)regs.h.al);
}


/* FUNCAO 182 (0B6H) - ENVIA CARACTER PARA A IMPRESSORA */
void ECPI (caracter)
char  caracter;
{
  union REGS regs;
  regs.h.ah = -1;
  regs.h.al = 182;
  regs.h.dl = caracter;
  intdos (&regs, &regs);
}

/* FUNCAO 183 (0B7H) - OBTEM ESTADO DA IMPRESSORA */
int OEI ()
{
  union REGS regs;
  regs.h.ah = -1;
  regs.h.al = 183;
  intdos (&regs, &regs);
  return ((int)regs.h.al);
}

/* FUNCAO 180 (0B4H) - ABRE IMPRESSORA */
int AI ()
{
  union REGS regs;
  regs.h.ah = -1;
  regs.h.al = 180;
  intdos (&regs, &regs);
  return ((int)regs.h.al);
}



/* FUNCAO 181 (0B5H) - FECHA IMPRESSORA */
void  FI ()
{
  union REGS regs;
  regs.h.ah = -1;
  regs.h.al = 181;
  intdos (&regs, &regs);
}



/* FUNCAO 151 (97H) - RETORNA INFORMACOES DE UMA IMPRESSORA */
unsigned char  RII (impressora, canal, subcanal)
int impressora;
unsigned * canal;
unsigned * subcanal;
{
  UCHAR conf;
  union REGS regs;
  regs.h.ah = -1;
  regs.h.al = 151;
  regs.h.dl = impressora;
  intdos (&regs, &regs);
  conf = regs.h.ch;
  if (((conf>>7) & 0x1) == 0x1)
    {
      printf ("IMPRESSORA NAO CONFIGURADA\n");
      return (0);
    }
  if (((conf>>5) & 0x1) == 0x1)
    {
      printf ("ACESSO REMOTO BLOQUEADO\n:");
      return (0);
    }
  if (regs.h.cl == 0)
    {
      printf ("IMPRESSORA LOCAL\n");
		* canal = regs.h.bh;
      * subcanal = regs.h.bl;
      return ((unsigned char)regs.h.cl);
    }
}







/* FUNCAO 141 (8DH) - AGUARDA POR TEMPO */
void APT (tempo)
int tempo;
{
  union REGS regs;
  regs.h.ah = -1;
  regs.h.al = 141;
  regs.x.dx = tempo;
  intdos (&regs, &regs);
}







/* FUNCAO 204 (CCH) - TRANSMITIR MENSAGEM */
UCHAR   TR (recepcao, mensagem)
UCHAR   * recepcao, * mensagem;
{
  union REGS regs;
  struct SREGS sregs;

  regs.h.ah = -1;
  regs.h.al = 204;
  regs.x.dx = FP_OFF (recepcao);
  sregs.ds  = FP_SEG (recepcao);
  regs.x.bx = FP_OFF (mensagem);
  sregs.es  = FP_SEG (mensagem);
  intdosx (&regs, &regs, &sregs);
  return ((UCHAR)regs.h.al);
}


/* FUNCAO 200 (C8H) - ABRIR CANAL DE RECEPCAO DE MENSAGENS */
UCHAR ACRM (endereco, codigo)
UCHAR endereco, * codigo;
{
  union  REGS regs;
  struct SREGS sregs;

  regs.h.ah = -1;
  regs.h.al = 200;
  regs.x.dx = FP_OFF (endereco);
  sregs.ds  = FP_SEG (endereco);
  intdosx (&regs, &regs, &sregs);
  * codigo = regs.h.al;
  return ((UCHAR)regs.x.cx);
}


/* FUNCAO 175 (0AF) - BLOQUEIA UNIDADE DE DISCO */
UCHAR BUD (disco)
int disco;
{
  union REGS regs;

  regs.h.ah = -1;
  regs.h.al = 175;
  regs.h.dl = disco;
  intdos (&regs, &regs);
  return ((UCHAR)regs.h.al);
}






void main ()
{
  int canal, subcanal;
  unsigned char  cana, retorno, subcana, reg, codigo;

/*
  printf ("retorno = %d",(int) ExecLinhaComando(0024300));

  retorno = BRI(&canal);
  printf ("canal = %d",canal);


  printf ("canal = %u   subcanal = %u  registrador CL = %u\n",canal, subcanal,
	   reg);


  retorno = SI (-1);
  printf("canal = %d, subcanal = %d",canal,subcanal);

  FI ();
  printf ("ABRE IMPRESSORA (== 0) : %d\n",AI ());
  retorno = OEI ();
  printf ("RETORNO = %d",retorno);
  if (retorno != 0)
	 ECPI('B');      /* Envia Caracter para a Impressora */
 /*	 else printf ("IMPRESSORA OCUPADA !");

  ECPI ('R');
  APT (10);
  ECPI ('O');

  RII (0, &cana, &subcana);
  printf ("CANAL = %x   SUBCANAL = %x\n", cana, subcana);


  */
/*******************FUNCOES DE COMUNICACAO ENTRE PROCESSOS********************/


 /*
  printf ("RETORNO CX = %x", ACRM (&cana, &codigo));
  printf ("CODIGO DE RETORNO\n = %u", codigo);

  printf ("RETORNO DE TR = %u",TR (&cana, &subcana));


  retorno = (UCHAR) RetorInfDisc (0, &cana, &subcana, &reg);

*/

  retorno  = BUD (1);
  printf ("RETORNO (BLOQUEIA DRIVE) = %u",retorno);

}







