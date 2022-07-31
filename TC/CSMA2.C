//   Curso: Ciencia da Computacao - 2§ano
//   Alunos: 1 - Aderson Cleber Pifer
//           3 - Anderson Ferreira Martins
//           9 - Gustavo Henrique Busnardo
//           12- Joseane Spessatto
//           13- Juarez Fioravanti Filho
//////////////////////////////////////////////////////////////////////////////
#include <dos.h>
#include <stdio.h>
#include <io.h>
#include <conio.h>
#include <bios.h>
//////////////////////////////////////////////////////////////////////////////
#define buffer_TX    0x3f8
#define buffer_RX    0x3f8
#define line_status  0x3fd
#define    BYTE unsigned char
#define    transmissao 0
#define    ERROR unsigned
#define    MAXIMO 53
#define    ACK    0x06
#define    NAK    0x15
///////////////////////////////////////////////////////////////////////////////
union     REGS r;
int       fim=0,I=0,j=0,z=0,tam=0,CONTADOR=0,INDICE=0,Nbytes=0,status,erro=1;
unsigned  block=0,indicebit=16,resto=0,block1=0;
BYTE      buf[1];
int       Frame[MAXIMO];
char      final=0,over[1];
int       destino=0,origem=0;
FILE      *file2,*arq2;
////////////////////////////////////////////////////////////////////////////////
void interrupt Fisica ();
void interrupt (*salva) ();
void Enlace ();
/////////////////////////////////////////////////////////////////////////////////
void Janela (int x0,int y0,int x1,int y1,int fundo)
{
   window (1,1,80,25);
   window (x0,y0,x1,y1);
   textattr(fundo);
   clrscr ();
}
//////////////////////////////////////////////////////////////////////////////
void Inicializacao ()
{
   j=0;
   I=0;
   indicebit=16;
   over[0]=0;
   tam=0;
   z=0;
   block=0;
   resto=0;
   buf[0]=0;
}
//////////////////////////////////////////////////////////////////////////////
char Status_Registro ()
{
   char status=0;
   status=inportb(line_status);
   if (status & 1)  return (1);
   else return (0);
}
////////////////////////////////////////////////////////////////////////////////
int    Ler_Arquivo (char nome[20],int Nbyte)
{
   int  count=0,hand;
   unsigned  quant,quant1;
   if  ((file2=(FILE *) fopen(nome,"rb"))==0)
   {
	   printf ("%s arquivo nao encontrado\n ",nome);
	   exit (1);
   }
   quant1 = MAXIMO - 10;
   quant =  MAXIMO - 10;
   hand = fileno (file2);
   rewind (file2);
   fseek (file2,(quant * Nbyte),SEEK_SET);
   flushall ();
   while  (count<quant1)
   {
	   _read (hand,buf,1);
      if ((buf[0]==0x1a)||(buf[0]==0))
      {
	      fclose (file2);
	      final = 26;
	      Frame[3] = 0x02;
	      break;
	   }
	   else
      {
		   Frame [count + 7] = buf [0];
		   count++;
	   }
   }
   fclose (file2);
   return (count);
}
/////////////////////////////////////////////////////////////////////////////////
void   Polinomio (int doisbytes)
{
   unsigned  quant,bit;
   if (final==26)  quant=5;
      else  quant=4;
   if ((resto==0)&&(block<0x8000))
   {
      resto = doisbytes;
	   doisbytes = 0;
   }
   do
   {
      if ((tam==0)&&(((CONTADOR+quant)*8)>indicebit))
      {
	      block1=(unsigned)Frame[j+3];
         j++;
         indicebit+=8;
         tam=8;
       }
       else
       {
	       if (tam!=0)
          {
		       over[0]=(resto>>15);
		       bit=(block1<<z);
		       bit=(bit&0x00ff)>>7;
             resto=(resto<<1)|(bit);
             tam--;
             z++;
		       if (tam == 0)  z = 0;
	        }
	        else
           {
              over[0]=(resto>>15);
              resto=resto<<1;
	           indicebit++;
		        I++;
	         }
       }
   }
   while ((over[0]==0)&&(I!=32)) ;
   if (over[0]==1)
   {
       resto=resto^0x1021;
       over[0]=over[0]^1;
       block=resto;
   }
}
////////////////////////////////////////////////////////////////////////////////
void    Envia_Frame (char nome[20],int nbyte)
{
   int   b=0,quant,rand,pos;
   int  porta[1];
   unsigned   aux1=0,aux2=0;
   for (b=0;b<MAXIMO;b++)
   {
      Frame[b]=0x00;
   }
   Frame[0]=0xAA;
   Frame[1]=0xAA;
   Frame[2]=0x80;
   Frame[3]=0x01;
   Frame[4]=destino;
   Frame[5]=origem;
   CONTADOR=Ler_Arquivo (nome,nbyte);
   Frame[6]=CONTADOR;
   aux1=(unsigned)Frame[3];
   aux2=(unsigned)Frame[4];
   block=(aux1<<8)|(aux2);
   do
   {
	   Polinomio (block);
	}
   while (I!=32);
   Janela(1,5,80,25,0x70);
   printf ("** resto = %x **\n",resto);
	Frame[MAXIMO-3]=((resto&0xff00)>>8);
	Frame[MAXIMO-2]=(resto&0x00ff);
	Frame[MAXIMO-1]=0x81;
   Janela(1,5,80,25,0x70);
	for (b=0;b<MAXIMO;b++)
   {
	   printf  ("%c",Frame[b]);
	   outportb(buffer_TX, Frame [b]);
	   delay (5);
	   porta[0]=inportb(buffer_RX);
	   if (porta[0]!=Frame[b])
		{
		   outportb(buffer_TX,origem);
         Janela(1,1,80,3,0x70);
		   printf ("* * * * * C O L I S A O * * * * *");
		   porta[0]=inportb(buffer_RX);
		   delay (11*origem);
		   outportb(buffer_TX,origem);
		   b=0;
		   Janela(1,5,80,25,0x70);
		   porta[0]=inportb(buffer_RX);
		   if (porta[0]!=(char)origem)
		   Fisica ();
		   outportb(buffer_TX,Frame[b]);
		   delay (30);
		 }
	}

}
///////////////////////////////////////////////////////////////////////////////
void Envia_Arquivo ()
{
   char nome[20];
   int porta[1];
   BYTE  aux1,aux2;
   int  g=0,
	erro=1;
   printf ("\n");
   flushall ();
   printf (" NOME==> ");
   gets (nome);
   while ((erro!=0)||(Frame[3]!=0x02))
   {
      Inicializacao ();
      Envia_Frame (nome,Nbytes);
      do
      {
         if (Status_Registro ())
         {
	         porta[0]=inportb(buffer_RX);
   	      break;
	      }
      }
      while (1);
      if (porta[0]==0x06)
      {
	      Nbytes++;
         Janela(1,1,80,3,0x70);
	      printf ("\n********   ACK   ********\n");
	      erro=0;
      }
      else
      {
	      Janela(1,1,80,3,0x70);
	      printf ("\n********   NAK   ********\n");
	      erro=1;
      }
   }
}
//////////////////////////////////////////////////////////////////////////////
void Enlace ()
{
    int  nome[2];
    BYTE aux1,aux2;
    unsigned b;
    Inicializacao ();
    CONTADOR=Frame[6];
    if (CONTADOR<43)  CONTADOR++;
    if ((Frame[4]==origem)||(Frame[4]==0))
	     if(Frame[7] == 'M')
	     {
           Janela(1,5,80,25,0x70);
	        printf ("ORIGEM %i \n\n", Frame[5]);
	        for (b=8;b<CONTADOR+8;b++) printf("%c",Frame[b]);
	        erro=0;
	        delay(2000);
	     }
	     else
        {
	        aux1=(unsigned)Frame[3];
	        aux2=(unsigned)Frame[4];
	        block=(aux1<<8)|(aux2);
	        do
           {
		        Polinomio (block);
	        }
           while (I!=32);
		     nome[0]=((resto&0xff00)>>8);
		     nome[1]=(resto&0x00ff);
	        if ((nome[0]==Frame[MAXIMO-3])&&(nome[1]==Frame[MAXIMO-2]))
		     {
		        outportb(buffer_TX, ACK);
		        erro=0;
		        printf ("\n********   ACK   ********");
		        for(b=0;b<MAXIMO;b++)
              {
		           if((Frame[b]!=0xAA)&&(Frame[b]!=0x80)&&
			           (Frame[b]!=0x01)&&(Frame[b]!=0x81)&&
			           (Frame[b]!=0x00)&&(Frame[b]!=0x02))
			        {
			           printf("%c",Frame[b]);
			           fprintf(arq2,"%c", Frame[b]);
			        }
		        }
		        inportb(buffer_RX);
	        }
	        else
           {
		        outportb(buffer_TX,NAK);
		        printf ("\n********   NAK   ********");
		        erro=1;
		        for(b=0;b<MAXIMO;b++)
              {
		           if((Frame[b]!=0xAA)&&(Frame[b]!=0x80) &&
			           (Frame[b]!=0x01)&&(Frame[b]!=0x81) &&
			           (Frame[b]!=0x00)&&(Frame[b]!=0x02))
			        {
			           printf("%c",Frame[b]);
			           fprintf(arq2,"%c", Frame[b]);
			        }
		        }
	        inportb(buffer_RX);
        }
    }
}
//////////////////////////////////////////////////////////////////////////////
void   Envia_Mensagem ()
{
   char Bloco[MAXIMO];
   int aux;
   char porta[1];
   unsigned b;
   for (b=0;b<MAXIMO;b++)
   {
      Bloco[b]=0x00;
      Frame[b]=0x00;
   }
   Janela(1,1,80,3,0x70);
   printf ("MENSAGEM ==> ");
   gets (Bloco);
   Frame[0]=0xAA;
   Frame[1]=0xAA;
   Frame[2]=0x80;
   Frame[3]=0x02;
   Frame[4]=destino;
   Frame[5]=origem;
   aux=strlen (Bloco);
   Frame[6]=aux;
   Frame[7]='M';
   for(b=8;b<aux+9;b++) Frame[b]=Bloco[b-8];
   Frame[MAXIMO-1]=0x81;
   for (b=0;b<MAXIMO;b++) Bloco[b]=Frame[b];
   Janela(1,5,80,25,0x70);
	for (b=0;b<MAXIMO;b++)
   {
	   printf ("%c",Bloco[b]);
	   outportb(buffer_TX,Bloco[b]);
	   delay(5);
	   porta[0]=inportb(buffer_RX);
	   if (porta[0]!=Bloco[b])
	   {
		   outportb(buffer_TX,origem);
         Janela(1,1,80,3,0x70);
		   printf ("* * * * * C O L I S A O * * * * *");
		   porta[0]=inportb(buffer_RX);
		   delay(11*origem);
		   outportb(buffer_TX,origem);
		   b=0;
		   Janela(1,5,80,25,0x70);
		   porta[0]=inportb(buffer_RX);
		   if (porta[0]!=(char)origem) Fisica ();
		   outportb(buffer_TX,Bloco[b]);
		   delay (30);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////
void Transmite ()
{
   char ch;
   int ter=0;
   flushall ();
   Janela(1,1,80,3,0x70);
   printf ("DESTINO ==> ");
   scanf ("%i",&destino);
   printf("< A > TRANSMITE ARQUIVO      < C > TRANSMITE MENSAGEM  < F > FIM");
   flushall ();
   ch=getch ();
   if (toupper(ch)=='A') Envia_Arquivo ();
   if (toupper(ch)=='C') Envia_Mensagem ();
	Janela(1,1,80,3,0x70);
	Janela(1,5,80,25,0x70);
}
//////////////////////////////////////////////////////////////////////////////
void interrupt Fisica ()
{
   int porta[1],i,saida=1;
   int Buffer[MAXIMO];
   while (saida)
   {
	   if (Status_Registro())
	   {
	      porta[0]=inportb(buffer_RX);
	      if (porta[0]==0x80) INDICE=2;
         putch(porta[0]);
	      Buffer[INDICE]=porta[0];
	      if (Buffer[MAXIMO-1]==0x81)
         {
		      memcpy(Frame,Buffer,MAXIMO*2);
		      Enlace ();
		      INDICE=0;
		      for(i=0;i<MAXIMO;i++) Buffer[i]=0x00;
		      if ((Frame[3]==0x02)&&(erro==0))
            {
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
//////////////////////////////////////////////////////////////////////////////
void main ()
{
   char ch,status;
   int ter=1;
   Janela(1,1,80,25,0x00);
   Janela(1,1,80,3,0x70);
   printf ("ORIGEM ==> ");
   scanf ("%i",&origem);
   flushall ();
   outportb(0x21,176);
   outportb(0x3f9,3);
   status=inportb(0x3fa);
   r.x.ax=251;
   r.x.dx=0;
   int86(0x14,&r,&r);
   status=inportb(buffer_RX);
   Janela(1,5,80,25,0x70);
   do
   {
      if (Status_Registro ())
      {
         arq2=(FILE *)fopen ("tes.txt","wb");
	      Fisica ();
	   }
      if (kbhit ())
	   {
	      ch=getch ();
	      if (toupper(ch)=='F') ter=0;
	         else Transmite ();
	   }
   }
   while(ter);
}
