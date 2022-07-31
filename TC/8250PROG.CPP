#include <CONIO.H>
#include <STDIO.H>
#include <STRING.H>
#include <DOS.H>
#include <STDLIB.H>
/***************************************************************************/
typedef struct
{
	unsigned char HiByte;
	unsigned char LoByte;
	char BaudRate[11];
}SetBaud;

unsigned char DivisorLatchLo;    /* Valor usado em Porta   */
unsigned char DivisorLatchHi;    /* Valor usado em Porta+1 */
unsigned char Value;             /* Valor usado em Porta+3 */

unsigned int com[4];             /* Vari veis auxiliares */
unsigned int Porta;

const DivisorLatchAccess=0x80;   /* Constantes usadas para definir */
const SetBreakOn=0x40;           /* a vari vel Value               */
const StickParity=0x20;
const EvenParity=0x10;
const ParityEnable=0x08;
const StopBits=0x04;

unsigned char i,y,x;              /* Vari veis auxiliares */
unsigned int aux;

SetBaud L[19];
/***************************************************************************/
void Sombra(int x1,int y1,int x2,int y2,unsigned int sombraatrib)
{
	 int           x,y;
	 unsigned char z,t;
	 y2++;
	for(x=x1+2;x<=x2;x++)
	{
		gotoxy(x,y2);
		asm {
			push ax
			mov ah,8
			int 0x10
			mov z,ah
			mov t,al
			pop ax
		};
		if ((z==sombraatrib)||(z==0)) z=0; else z=sombraatrib;
		if (t==219) z=0;
		asm {
			push ax
			push bx
			push cx
			mov cx,1
			xor bh,bh
			mov bl,z
			mov al,t
			mov ah,9
			int 0x10
			pop cx
			pop bx
			pop ax
		};
	};
	for(x=x2+1;x<=x2+2;x++)
		for(y=y1+1;y<=y2;y++)
		{
			gotoxy(x,y);
			asm {
				push ax
				mov ah,8
				int 0x10
				mov z,ah
				mov t,al
				pop ax
			};
			if ((z==sombraatrib)||(z==0)) z=0; else z=sombraatrib;
			if (t==219) z=0;
			asm {
				push ax
				push bx
				push cx
				mov cx,1
				xor bh,bh
				mov bl,z
				mov al,t
				mov ah,9
				int 0x10
				pop cx
				pop bx
				pop ax
			};
		};
};
/***************************************************************************/
void Cursor(unsigned int y)     /* Procedimento para "sumir" com o cursor */
{
	struct REGPACK regs;
	regs.r_ax=0x0100;
	if (y==1) regs.r_cx=0x1900;
	if (y!=1) regs.r_cx=0x607;
	intr(0x10,&regs);
};
/***************************************************************************/
void Janela(int x1,int y1,int x2,int y2)    /* Procedimento para desenhar */
{                                           /* uma janela na tela         */
	int aux;
	Sombra(x1,y1,x2,y2,8);
	window(x1,y1,x2,y2);
	clrscr();
	window(1,1,80,25);
	gotoxy(x1,y1); cprintf("É");
	gotoxy(x2,y2); cprintf("¼");
	gotoxy(x2,y1); cprintf("»");
	gotoxy(x1,y2); cprintf("È");
	for(aux=x1+1;aux<x2;aux++)
	{
		gotoxy(aux,y1); cprintf("Í");
		gotoxy(aux,y2); cprintf("Í");
	}
	for(aux=y1+1;aux<y2;aux++)
	{
		gotoxy(x1,aux); cprintf("º");
		gotoxy(x2,aux); cprintf("º");
	}
};
/***************************************************************************/
unsigned char Selecao()                     /* Fun‡„o para definir a */
{                                           /* programa‡„o da UART   */
	unsigned char c,FiveBits,choose[7][20];
	int k;
	FiveBits=0;
	c='°';
	for (k=0;k<=1999;k++) poke(0xb800,2*k,c+(16*BLUE+LIGHTGRAY)*256);
	c=32;
	textcolor(BLACK);
	textbackground(LIGHTGRAY);
	gotoxy(1,1);	cprintf("%80c",c);
	gotoxy(1,25);  cprintf("%79c",c);
	poke(0xb800,3998,32+(16*LIGHTGRAY+LIGHTGRAY)*256);
	gotoxy(31,1);  cprintf("Programador 8250");
	gotoxy(28,25);  cprintf("Juarez Fioravanti Filho");
	textcolor(LIGHTCYAN);
	textbackground(BLACK);
	Janela(28,9,50,14);
	gotoxy(30,9); cprintf(" Selecione a porta ");
	for(x=1;x<=4;x++)
	{
		gotoxy(30,9+x); cprintf(" COM%d:     %3X     ",x,com[x]);
	}
	x=1;
	k=0;
	while ((k!=13)&&(k!=27))
	{
		textcolor(YELLOW);
		textbackground(RED);
		gotoxy(30,9+x); cprintf(" COM%d:     %3X     ",x,com[x]);
		k=getch();
		textcolor(LIGHTCYAN);
		textbackground(BLACK);
		gotoxy(30,9+x); cprintf(" COM%d:     %3X     ",x,com[x]);
		if (k==27) { clrscr(); Cursor(0); exit(1); };
		if (!k) k=getch();
		if (k==0x48) {x=x-1; if (x==0) x=4;};
		if (k==0x50) {x=x+1; if (x==5) x=1;};
		if (k==13) Porta=com[x];
		if (Porta==0) k=1;
	};
	aux=x-1;
	strcpy(choose[1]," Par     ");
	strcpy(choose[2]," Impar   ");
	strcpy(choose[3]," Espa‡o  ");
	strcpy(choose[4]," Marca   ");
	strcpy(choose[5]," Nenhuma ");
	textcolor(LIGHTCYAN);
	textbackground(BLACK);
	Janela(27,9,51,15);
	gotoxy(30,9); cprintf(" Escolha a paridade ");
	for(x=1;x<=5;x++)
	{
		gotoxy(29,9+x); cprintf(" Paridade:  %s",choose[x]);
	}
	x=1;
	k=0;
	while ((k!=13)&&(k!=27))
	{
		textcolor(YELLOW);
		textbackground(RED);
		gotoxy(29,9+x); cprintf(" Paridade:  %s",choose[x]);
		k=getch();
		textcolor(LIGHTCYAN);
		textbackground(BLACK);
		gotoxy(29,9+x); cprintf(" Paridade:  %s",choose[x]);
		if (k==27) { clrscr(); Cursor(0); exit(2);};
		if (!k) k=getch();
		if (k==0x48) {x=x-1; if (x==0) x=5;};
		if (k==0x50) {x=x+1; if (x==6) x=1;};
		if ((k==13)&&(x==1)) Value=EvenParity+ParityEnable;
		if ((k==13)&&(x==2)) Value=ParityEnable;
		if ((k==13)&&(x==3)) Value=EvenParity+ParityEnable+StickParity;
		if ((k==13)&&(x==4)) Value=ParityEnable+StickParity;
		if ((k==13)&&(x==5)) Value=0;
	};
	textbackground(BLUE);
	textcolor(LIGHTGRAY);
	c='°';
	for (y=9;y<=16;y++)
	for (x=27;x<=53;x++)
	{
		gotoxy(x,y); cprintf("%c",c);
	};
	strcpy(choose[1]," 5 ");
	strcpy(choose[2]," 6 ");
	strcpy(choose[3]," 7 ");
	strcpy(choose[4]," 8 ");
	textcolor(LIGHTCYAN);
	textbackground(BLACK);
	Janela(27,9,51,14);
	gotoxy(30,9); cprintf(" Tamanho da palavra ");
	for(x=1;x<=4;x++)
	{
		gotoxy(29,9+x); cprintf(" N£mero de bits:  %s",choose[x]);
	}
	x=1;
	k=0;
	while ((k!=13)&&(k!=27))
	{
		textcolor(YELLOW);
		textbackground(RED);
		gotoxy(29,9+x); cprintf(" N£mero de bits:  %s",choose[x]);
		k=getch();
		textcolor(LIGHTCYAN);
		textbackground(BLACK);
		gotoxy(29,9+x); cprintf(" N£mero de bits:  %s",choose[x]);
		if (k==27) { clrscr(); Cursor(0); exit(3); };
		if (!k) k=getch();
		if (k==0x48) {x=x-1; if (x==0) x=4;};
		if (k==0x50) {x=x+1; if (x==5) x=1;};
		if ((k==13)&&(x==1)) FiveBits=1;
		if ((k==13)&&(x==2)) Value=Value+1;
		if ((k==13)&&(x==3)) Value=Value+2;
		if ((Value!=0)&&(x==4)) k=1;
		if ((k==13)&&(x==4)) Value=Value+3;
	};
	textbackground(BLUE);
	textcolor(LIGHTGRAY);
	c='°';
	for (y=9;y<=15;y++)
	for (x=27;x<=53;x++)
	{
		gotoxy(x,y); cprintf("%c",c);
	};
	strcpy(choose[1],"1   ");
	strcpy(choose[2],"1.5 ");
	strcpy(choose[3],"2   ");
	textcolor(LIGHTCYAN);
	textbackground(BLACK);
	Janela(27,9,51,12);
	gotoxy(32,9); cprintf(" Bits de parada ");
	for(x=1;x<=2;x++)
	{
		gotoxy(29,9+x);
		if (((FiveBits==1)&&(x==2))||(x==1))
			cprintf(" N£mero de bits: %s",choose[x]);
		if ((FiveBits==0)&&(x==2))
			cprintf(" N£mero de bits: %s",choose[x+1]);
	}
	x=1;
	k=0;
	while ((k!=13)&&(k!=27))
	{
		textcolor(YELLOW);
		textbackground(RED);
		gotoxy(29,9+x);
		if (((FiveBits==1)&&(x==2))||(x==1))
			cprintf(" N£mero de bits: %s",choose[x]);
		if ((FiveBits==0)&&(x==2))
			cprintf(" N£mero de bits: %s",choose[x+1]);
		k=getch();
		textcolor(LIGHTCYAN);
		textbackground(BLACK);
		gotoxy(29,9+x);
		if (((FiveBits==1)&&(x==2))||(x==1))
			cprintf(" N£mero de bits: %s",choose[x]);
		if ((FiveBits==0)&&(x==2))
			cprintf(" N£mero de bits: %s",choose[x+1]);
		if (k==27) { clrscr(); Cursor(0); exit(4); };
		if (!k) k=getch();
		if (k==0x48) {x=x-1; if (x==0) x=2;};
		if (k==0x50) {x=x+1; if (x==3) x=1;};
		if ((k==13)&&(x==2)) Value=Value+StopBits;
		if ((k==13)&&(x==1)) Value=Value+0;
	};
	Value=Value+DivisorLatchAccess;
	textbackground(BLUE);
	textcolor(LIGHTGRAY);
	c='°';
	for (y=9;y<=13;y++)
	for (x=27;x<=53;x++)
	{
		gotoxy(x,y); cprintf("%c",c);
	};
	textcolor(LIGHTCYAN);
	textbackground(BLACK);
	Janela(27,4,51,23);
	gotoxy(29,4); cprintf(" Taxa de transmiss„o ");
	for(x=1;x<=18;x++)
	{
		gotoxy(29,4+x);
		cprintf(" %s          ",L[x].BaudRate);
	}
	x=1;
	k=0;
	while ((k!=13)&&(k!=27))
	{
		textcolor(YELLOW);
		textbackground(RED);
		gotoxy(29,4+x);
		cprintf(" %s          ",L[x].BaudRate);
		k=getch();
		textcolor(LIGHTCYAN);
		textbackground(BLACK);
		gotoxy(29,4+x);
		cprintf(" %s          ",L[x].BaudRate);
		if (k==27) { clrscr(); Cursor(0); exit(5); };
		if (!k) k=getch();
		if (k==0x48) {x=x-1; if (x==0) x=18;};
		if (k==0x50) {x=x+1; if (x==19) x=1;};
	};
	return(x);
};
/***************************************************************************/
void main()
{
	Porta=0x3f8;	directvideo=1;	Cursor(1); aux=0;
	for(x=1;x<=4;x++) com[x]=peek(0x40,(x-1)*2);
	L[ 1].HiByte=0x09; L[ 1].LoByte=0x00; strcpy(L[ 1].BaudRate,"      50  ");
	L[ 2].HiByte=0x06; L[ 2].LoByte=0x00; strcpy(L[ 2].BaudRate,"      75  ");
	L[ 3].HiByte=0x04; L[ 3].LoByte=0x17; strcpy(L[ 3].BaudRate,"     110  ");
	L[ 4].HiByte=0x03; L[ 4].LoByte=0x59; strcpy(L[ 4].BaudRate,"     134.5");
	L[ 5].HiByte=0x03; L[ 5].LoByte=0x00; strcpy(L[ 5].BaudRate,"     150  ");
	L[ 6].HiByte=0x01; L[ 6].LoByte=0x80; strcpy(L[ 6].BaudRate,"     300  ");
	L[ 7].HiByte=0x00; L[ 7].LoByte=0xC0; strcpy(L[ 7].BaudRate,"     600  ");
	L[ 8].HiByte=0x00; L[ 8].LoByte=0x60; strcpy(L[ 8].BaudRate,"   1 200  ");
	L[ 9].HiByte=0x00; L[ 9].LoByte=0x40; strcpy(L[ 9].BaudRate,"   1 800  ");
	L[10].HiByte=0x00; L[10].LoByte=0x3A; strcpy(L[10].BaudRate,"   1 986  ");
	L[11].HiByte=0x00; L[11].LoByte=0x30; strcpy(L[11].BaudRate,"   2 400  ");
	L[12].HiByte=0x00; L[12].LoByte=0x20; strcpy(L[12].BaudRate,"   3 600  ");
	L[13].HiByte=0x00; L[13].LoByte=0x18; strcpy(L[13].BaudRate,"   4 800  ");
	L[14].HiByte=0x00; L[14].LoByte=0x10; strcpy(L[14].BaudRate,"   7 200  ");
	L[15].HiByte=0x00; L[15].LoByte=0x0C; strcpy(L[15].BaudRate,"   9 600  ");
	L[16].HiByte=0x00; L[16].LoByte=0x08; strcpy(L[16].BaudRate,"  14 400  ");
	L[17].HiByte=0x00; L[17].LoByte=0x06; strcpy(L[17].BaudRate,"  19 200  ");
	L[18].HiByte=0x00; L[18].LoByte=0x01; strcpy(L[18].BaudRate," 115 200  ");
	i=Selecao();
	DivisorLatchLo=L[i].LoByte;    /* Ver tabela abaixo para */
	DivisorLatchHi=L[i].HiByte;    /* a defini‡„o dos bytes. */
	asm{
		mov dx,aux
		mov ax,0x00e7
		int 0x14
		mov dx,aux
		mov ax,0x013f
		int 0x14
	};
	outp(Porta+3,Value);
	outp(Porta,DivisorLatchLo);
	outp(Porta+1,DivisorLatchHi);
	outp(Porta+3,Value-128);
	outp(Porta,0x41);
	Cursor(0);
	clrscr();
}

/*
	DivisorLatchHi  |   DivisorLatchLo  |  Taxa em bauds
	----------------+-------------------+-----------------
	0x09            |   0x00            |  50
	0x06            |   0x00            |  75
	0x04            |   0x17            |  110 (erro de 0.026%)
	0x03            |   0x59            |  134.5 (erro de 0.058%)
	0x03            |   0x00            |  150
	0x01            |   0x80            |  300
	0x00            |   0xC0            |  600
	0x00            |   0x60            |  1200
	0x00            |   0x40            |  1800
	0x00            |   0x3A            |  2000 (erro de 0.69%)
	0x00            |   0x30            |  2400
	0x00            |   0x20            |  3600
	0x00            |   0x18            |  4800
	0x00            |   0x10            |  7200
	0x00            |   0x0C            |  9600
	0x00            |   0x08            |  14400
	0x00            |   0x06            |  19200
	0x00            |   0x01            |  115200

*/