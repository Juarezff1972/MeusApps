//#include "bdscio.h"	/* System, h'ware constants	*/
#include <conio.h>
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>

#define MDATA peek(0x40,(com-1)*2)
#define ABORTAR 0x1e
#define ACK	0x06	/* Ascii ACK for handshaking	*/
#define NAK	0x15
#define EOT	0x04	/* End of transmission		*/
#define ETX	0x03	/* Abort Transmission		*/
#define SECSIZ 4096

FILE *rArq;
FILE *tArq;
char chflag;		/* checksumming enabled flag	*/
char cflag;		/* text-collection enabled flag */
char pflag;		/* pausing flag 		*/
char spflag;		/* stripping parity bit flag	*/
char lflag;		/* list device enabled flag	*/
char nflag;		/* recognizing nulls flag	*/
char fflag;		/* true if changing CR-LF's into
			    just CR when transmitting	*/
char lastc;		/* last char xmitted		*/
char dodflag;		/* true if displaying outging
			   data				*/
char didflag;		/* true if displaying incoming
			   data				*/
char hdflag;		/* true if effectively working
			   in half-duplex		*/
char abortf;		/* true when file I/O aborted	*/
char rbuf[SECSIZ]; 	/* file I/O buffer for incoming
			   data file			*/
char tbuf[SECSIZ]; 	/* sector buffer for file being
			   transmitted			*/
char rname[20]; 	/* name of receiving file	*/
char tname[20]; 	/* name of transmitting file	*/
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
			   after itself			*/
union REGS w;
int com;
////////////////////////////////////////////////////////////////////////////
/*
	Routine to return true if input is present on
	the modem:
*/
int Miready()
{
   w.x.dx=com;   // porta serial
   w.h.ah=3;   // 3 = Funcao de status
   int86(0x14,&w,&w);
   if(w.h.al>=0x80) return 1;
   else return 0;
}

////////////////////////////////////////////////////////////////////////////
/*
	Routine to return true if modem is ready to output
	a byte:
*/

void Moready()
{
   w.x.dx=com;
   w.h.ah=3;
   int86(0x14,&w,&w);
   if(w.h.ax==0x00) return 1;
   else return 0;
}
////////////////////////////////////////////////////////////////////////////
int Ask(char *s) // funcao para perguntas aleatorias
{
	char c;
	while (1)
	{
		printf("%s ",s);
		printf("(y/n)? ");
		c=toupper(getchar());
		if(c=='Y')
		{
			printf("es\n");
			return 1;
		}
		else if(c=='N')
		{
			printf("o\n");
			return 0;
		}
		else putcharc('\n');
	}
}
////////////////////////////////////////////////////////////////////////////
void Reset(void)
{
	timoutf=chflag=cflag=0;
	scount=0;
	spflag=1;
	dodflag=!hdflag;
	didflag=1;
}
////////////////////////////////////////////////////////////////////////////
void Init(void)
{
   w.x.dx=com;   // porta serial
   w.h.ah=0;   // 0 = Funcao de CONFIGURACAO
   w.h.al=231; // 9600 bauds, s/paridade, 2 stop bits, 8 bits de dados
   int86(0x14,&w,&w);
	printf("\n\nAnswer `y' if either your modem is set to half-duplex,\n");
	printf("or you expect an echo from the system on the");
	printf(" other end\n");
	printf("of the line; else answer `n':\n");
	hdflag = ask("Do you expect an echo");
	reset();
	printf("OK; you're on line...\n\n");
}
////////////////////////////////////////////////////////////////////////////
int Kbabort()
{
	if((kbhit()&&getch()==ABORTAR) {
		abortf=1;
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
void Outmod(char c)
{
	while(!Moready())
	if(Kbabort()) return;
	outp(MDATA,c);
}
////////////////////////////////////////////////////////////////////////////
void Tabort(void)
{
	if(chflag) while((bcount++)!=133) outmod(ETX);
	printf("\nTransmission of %s aborted.\n",tname);
 	fclose(tfd);
	Reset();
}
////////////////////////////////////////////////////////////////////////////
unsigned char Getmod(void)
{
	unsigned char c;
	unsigned n;
	if(timoutf) return;
	for(n=20000;!miready()&&n;n--)
	   if(Kbabort()) return 1;
	if(!n) {
	   timoutf=1;
	   return 1;
	 }
	c=inp(MDATA);
	return c;
}
////////////////////////////////////////////////////////////////////////////
void Prcoms(void)
{
	printf("\nBDS Telnet commands are:\n");
	printf("Double SPECIAL: send SPECIAL\n");
	printf("o: Open output file, start collection\n");
	printf("p: Pause (suspend collection or transmission)\n");
	printf("r: Resume after pausing\n");
	printf("d: Dump (append) text buffer to output file\n");
	printf("c: Close output file (after dumping buffer)\n");
	printf("v: View contents of text buffer\n");
	printf("k: Kill (erase) contents of text buffer\n");
	printf("t: Transmit a file to modem\n");
	printf("a: Abort transfer of file\n");
	printf("n: accept or ignore Nulls\n");
	printf("7: select policy regarding Parity bits\n");
	printf("f: select whether to transmit CR-LF as just CR\n");
	printf("h: set Half/full duplex mode\n");
	printf("l: control CP/M List device\n");
	printf("z: clear console terminal screen\n");
	printf("s: display Status of Telnet\n");
	printf("q: dump & close output file (if open) and Quit to CP/M");
}
////////////////////////////////////////////////////////////////////////////
void Dostat(void)
{
	   putchar('\n');
	   if(rArq) {
		printf("Output file = %s\n",rname);
		printf("Text buffer has %u bytes free",free);
		printf("\nText collection: ");
		  if(cflag) if(pflag) printf("on, but pausing\n");
			     else printf("on\n");
		  else printf("off\n");
	    }
	   else printf("No output file\n");

	   if(tArq) {
		printf("Transmitting: %s ",tname);
		if(pflag) printf("(but pausing)");
		putchar('\n');
	    }
	   else printf("Not transmitting any file\n");
	   printf("Incoming nulls are being %s\n",
		 nflag ?"collected" : "ignored");
	   printf("Parity bits are being %s\n",
		 spflag ?"stripped" : "preserved");
	   printf("Half-duplex mode: %s",
		hdflag ? "on" : "off");
}
////////////////////////////////////////////////////////////////////////////
void Special(void)
{
	   char c;
	   int n;
      printf("\nSpecial: ");
	   if((c=getchar())!='\n') printf("  ");
	   switch(toupper(c)) {
		   case '\n'   : return;
		   case ABORTAR: Outmod(ABORTAR);
			              printf("Special char sent\n");
			              break;
    		case '7'    : spflag=ask("Strip parity");
			              break;
   		case 'P'    : if(pflag) printf("Already pausing");
			              else if(!(tArq||rArq))
				           printf("Not transmitting or receiving");
			              else {
				              pflag=1;
				              dod_sav=dodflag;
				              did_sav=didflag;
				              dodflag=!hdflag;
				              didflag=1;
				              printf("Ok, pausing from %s", tflag ?
                            "transmission" : "collection");
			                }
			              goto lf;
		   case 'R'    : if(!pflag) printf("Not pausing");
			              else {
				              pflag=0;
				              dodflag=dod_sav;
				              didflag=did_sav;
				              printf("%s now enabled again.", tflag ?
				                "transmission" : "collection");
			                }
			              break;
		case 'Q'       : if(tflag) Tabort();
			              if(rflag) rclose();
			              exit();
		case 'S'      :  Dostat();
			              break;
		default       :  prcoms();
	}
}
////////////////////////////////////////////////////////////////////////////
void Xmit()
{
	int incheck;
	int n;
	char c;
	if(pflag||!moready()) return 0;
	c=tbuf[bcount++];
	checksum+=c;
   outmod(c);
	lastc = c;
	if (dodflag) display(c);
	if (bcount != SECSIZ) return 0;
	bcount = 0;
	if (!chflag) return !read1();
	incheck = (getmod() << 8) + getmod();
	if (incheck != checksum) {
		for (n=0; n<20000; n++); 	/* let line settle down */
		printf("\nError. Resending sector %d...\n",scount+1);
		outmod(NAK);
	}
	else if (read1()) {
		if (!dodflag) printf("Good sector <%d>\n",++scount);
		outmod(ACK);
	}
          else { outmod(EOT); return 1; }

	checksum = 0;
	if (getmod() != 0xFD) {
			printf("\nPhase error; aborting...");
			abortf = 1;
			}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
/*
	Read a sector of the transmission file:
*/
read1()
{
	int i;
	i = read(tfd, tbuf, 1);
	if ( i == ERROR) {
		printf("\nRead error from %s; Aborting.\n",
			tname);
		tabort();
	 }
	return i;
}
////////////////////////////////////////////////////////////////////////////
void main(void)
{
	char c, c2;
	int n;
	init();
  loop:	if (abortf) {
		if (rflag) rclose();
		if (tflag) tabort();
		abortf = 0;
	}
	if (tflag && xmit()) {
			printf("\nTransmission complete.\n");
			close(tfd);
			reset();
		    }
	if (abortf) goto loop;
	if (miready()) {
	  c = c2 = getmod();
	  if (spflag) c &= 0x7f;
	  if (tflag && (c == ETX)) {
		printf("Reciever has aborted;\n");
		abortf = 1;
		goto loop;
		}
	  if (didflag && (c || nflag) && (c != EOF))
			display(c);
	  if (cflag && !pflag) {
	    if (c || nflag)
	      if (!free) printf("**BUFFER FULL**\007\007");
	      else { *cptr++ = c; bfree--; }
	    if (chflag) {
		checksum += c2;
		bcount++;
		if (bcount == SECSIZ) {
		  bcount = 0;
		  outmod(checksum >> 8);
		  outmod(checksum);
		  checksum = 0;
		  c = getmod();
		  if (c == EOT) {
			rdump(0); rclose();
			printf("\n%s recieved OK\n",rname);
		   }
		  else if (c == ACK) {
			if (cptr > buf+1000) rdump(0);
			if (!didflag) printf("Good sector <%d>\n",++scount);
			outmod(0xFD);
		   }
		  else  {
		    cptr -= SECSIZ;
		    bfree += SECSIZ;
		    printf("\nChecksum error. Retrying <%d>\n",scount+1);
		    outmod(0xFD);
		    timoutf = 0;
		  }

		}
	      }
	  }
	}
	if (kbready()) {
	  c = getch();
	  if (c != SPECIAL) {
	    if (pflag || (!tflag && !(rflag && chflag))) {
		outmod(c);
		if (dodflag) display(c);
	      }
	   }
	  else special();
	 }
	goto loop;
}
