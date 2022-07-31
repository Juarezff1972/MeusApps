#include "dos.h"
#include "conio.h"
#include "string.h"

/* letras maiusculas A-Z */
     /* codigo */  /* decimal */

#define  A 		65
#define  B 		66
#define  C 		67
#define  D 		68
#define  E 		69
#define  F 		70
#define  G 		71
#define  H 		72
#define  I 		73
#define  J 		74
#define  K 		75
#define  L 		76
#define  M 		77
#define  N 		78
#define  O 		79
#define  P 		80
#define  Q		81
#define  R 		82
#define  S 		83
#define  T 		84
#define  U 		85
#define  V 		86
#define  W 		87
#define  X 		88
#define  Y 		89
#define  Z 		90

/* teclas de funcoes */
     /* codigo */  /* decimal */

#define  F1  		59
#define  F2    		60
#define  F3    		61
#define  F4    		62
#define  F5    		63
#define  F6    		64
#define  F7    		65
#define  F8    		66
#define  F9    		67
#define  F10    	68

/* composicao alt_numeros 1-10 */
     /* codigo */  /* decimal */

#define  ALT_1         120
#define  ALT_2         121
#define  ALT_3         122
#define  ALT_4         123
#define  ALT_5         124
#define  ALT_6         125
#define  ALT_7         126
#define  ALT_8         127
#define  ALT_9         128
#define  ALT_10        129

/* composicao alt_letras/maiusculas */
     /* codigo */  /* decimal */

#define  ALT_Q  	16
#define  ALT_W  	17
#define  ALT_E  	18
#define  ALT_R  	19
#define  ALT_T  	20
#define  ALT_Y 		21
#define  ALT_U  	22
#define  ALT_I  	23
#define  ALT_O  	24
#define  ALT_P  	25
#define  ALT_A  	30
#define  ALT_S  	31
#define  ALT_D  	32
#define  ALT_F  	33
#define  ALT_G  	34
#define  ALT_H  	35
#define  ALT_J  	36
#define  ALT_K  	37
#define  ALT_L  	38
#define  ALT_Z  	44
#define  ALT_X  	45
#define  ALT_C  	46
#define  ALT_V  	47
#define  ALT_B  	48
#define  ALT_N  	49
#define  ALT_M  	50

/* composicao alt_teclas/funcoes F1-F10 */
     /* codigo */  /* decimal */

#define  ALT_F1        104
#define  ALT_F2        105
#define  ALT_F3        106
#define  ALT_F4        107
#define  ALT_F5        108
#define  ALT_F6        109
#define  ALT_F7        110
#define  ALT_F8        111
#define  ALT_F9        112
#define  ALT_F10       113

/* composicao shift_teclas/funcoes F1-F10
     /* codigo */  /* decimal */

#define  SHIFT_F1 	84
#define  SHIFT_F2 	85
#define  SHIFT_F3 	86
#define  SHIFT_F4 	87
#define  SHIFT_F5 	88
#define  SHIFT_F6 	89
#define  SHIFT_F7 	90
#define  SHIFT_F8 	91
#define  SHIFT_F9 	92
#define  SHIFT_F10 	93

/* composicao ctrl_teclas/funcoes F1-F10
     /* codigo */  /* decimal */

#define  CTRL_F1   	94
#define  CTRL_F2   	95
#define  CTRL_F3   	96
#define  CTRL_F4  	97
#define  CTRL_F5   	98
#define  CTRL_F6   	99
#define  CTRL_F7       100
#define  CTRL_F8       101
#define  CTRL_F9       102
#define  CTRL_F10      103

/* teclas especiais */
     /* codigo */     /* decimal */

#define  PgUp   	   73
#define  PgDn   	   81
#define  END    	   79
#define  INS   		   82
#define  DEL   		   83
#define  SHIFT_TAB 	   15
#define  CTRL_PrtSc       114 /* comeca e termina impressao */
#define  CTRL_LEFT_ARROW  115
#define  CTRL_RIGHT_ARROW 116
#define  CTRL_END         117
#define  CTRL_PgDn        118
#define  CTRL_HOME        119
#define  CTRL_PgUp        132

#define  SET_CIMA          72
#define  SET_BAIXO         80
#define  SET_ESQ           75
#define  SET_DIR           77

#define  ESC               27
#define ENTER 13

/************************************************************/

/* funcao desliga_cursor */

void desliga_cursor(void)
{
   struct REGPACK reg;
   reg.r_ax = 0x168;
   reg.r_cx = 6800;
   intr(0x10, &reg);
}

/* funcao liga_cursor */

void liga_cursor(void)
{
  struct REGPACK reg;
  reg.r_ax = 0x168;
  reg.r_cx = 0x607;
  intr(0x10, &reg);
}

/***********************************************************/

void centraliza_texto(char str[20], int col_in, int col_fim, int linha)
{
   int tam_str, tam_cols;
   tam_str = strlen(str);
   tam_cols = col_fim - col_in;
   gotoxy(col_in + ((tam_cols/2) - (tam_str/2)), linha);
   cprintf(" %s ", str);
}

void borda1(int ix, int iy, int fx, int fy, int cor_text, int cor_fund)
{
  register  i;
  textcolor(cor_text);
  textbackground(cor_fund);
  gotoxy(ix, iy);
  for(i=1; i<=fx-ix; i++)
  cprintf("\xCD");
  gotoxy(ix, fy);
  for(i=1; i<=fx-ix; i++)
  cprintf("\xC4");
  for(i=1; i<=fy-iy; i++)
  {
     gotoxy(ix, iy+i); cprintf("\xB3");
     gotoxy(fx, iy+i); cprintf("\xB3");
  }

  gotoxy(ix, iy); cprintf("\xD5");
  gotoxy(ix, fy); cprintf("\xC0");
  gotoxy(fx, iy); cprintf("\xB8");
  gotoxy(fx, fy); cprintf("\xD9");
}

void borda2(int ix, int iy, int fx, int fy, int cor_text, int cor_fund)
{
  register i;
  textcolor(cor_text);
  textbackground(cor_fund);
  gotoxy(ix, iy);
  for(i=1; i<=fx-ix; i++)
  cprintf("\xCD");
  gotoxy(ix, fy);
  for(i=1; i<=fx-ix; i++)
  cprintf("\xCD");
  for(i=1; i<=fy-iy; i++)
  {
     gotoxy(ix, iy+i); cprintf("\xBA");
     gotoxy(fx, iy+i); cprintf("\xBA");
  }

  gotoxy(ix, iy); cprintf("\xC9");
  gotoxy(ix, fy); cprintf("\xC8");
  gotoxy(fx, iy); cprintf("\xBB");
  gotoxy(fx, fy); cprintf("\xBC");
}

void borda3(int ix, int iy, int fx, int fy, int cor_text, int cor_fund)
{
  register i;
  textcolor(cor_text);
  textbackground(cor_fund);
  gotoxy(ix, iy);
  for(i=1; i<=fx-ix; i++)
  cprintf("\xC4");
  gotoxy(ix, fy);
  for(i=1; i<=fx-ix; i++)
  cprintf("\xC4");
  for(i=1; i<=fy-iy; i++)
  {
     gotoxy(ix, iy+i); cprintf("\xB3");
     gotoxy(fx, iy+i); cprintf("\xB3");
  }

  gotoxy(ix, iy); cprintf("\xDA");
  gotoxy(ix, fy); cprintf("\xC0");
  gotoxy(fx, iy); cprintf("\xBF");
  gotoxy(fx, fy); cprintf("\xD9");
}

void tam_cursor(int inicio, int fim)
{
   union REGS regs;
   regs.h.ch = inicio;
   regs.h.ah = fim;
   regs.h.ah = 0x01;
   int86(0x10, &regs, &regs);
}
   