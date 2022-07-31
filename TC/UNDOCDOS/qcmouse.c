/*
  QCMOUSE.C  You may use this code as you would like as long as you don't
  make any money doing so.  This package is donated to public domain.
  If any damages come to your machine due to this package I am not reliable
  Enough of the legal stuff, you can do what you want with this.  If you
  make any money from this, please give me some credit, donations, or what
  ever.  This is so that you can learn the mouse functions if you don't
  know them by now.  If you distribute this, please leave me some credit.
    This was quickly thrown together, so if you find any errors, please
     let me know.
 
    QCMOUSE.C
    Forever Young Software
    Benjamin David Lunt
    All rights reserved
    QuickC 2.5
    10/28/97
*/
 
#include <graph.h>

void main(void)
{
  unsigned int  horz, vert, lang, temp;
  unsigned char vermaj, vermin, buttns;

  _setvideomode(_TEXTC80);

  // turn cursor off
	_asm {
    mov  ah,01h
    mov  ch,20h
    int  10h
	}

  _asm {
    xor  ax,ax
    int  33h
    mov  temp,00h
    jz   short ResetD
    mov  temp,0FFFFh
ResetD:
  }

  if (temp) {
    printf("Error while reseting mouse\n");
    exit(-1);
  }

  printf("Mouse Language Used:  ");
  _asm {
    mov  ax,23h
    int  33h
    mov  lang,bx
  }
  switch(lang) {
    case 0: printf("English\n");
      break;
    case 1: printf("French\n");
      break;
    case 2: printf("Dutch\n");
      break;
    case 3: printf("German\n");
      break;
    case 4: printf("Swedish\n");
      break;
    case 5: printf("Finnish\n");
      break;
    case 6: printf("Spanish\n");
      break;
    case 7: printf("Portugues\n");
      break;
    case 8: printf("Italian\n");
      break;
  }

  _asm {
    mov  ax,24h
    int  33h
    mov  vermaj,bh
    mov  vermin,bl
  }
  printf("Mouse driver version: ");
  printf("%u.%u\n", vermaj, vermin);

  printf("Setting Cursor Position to 50, 12\n");
  _asm {
    mov  cx,50 * 8
    mov  dx,12 * 8
    mov  ax,04h
    int  33h
  }

  printf("Setting boundries at 30,5 and 70,20\n");
  _settextposition(6, 31);  printf("*");
  _settextposition(6, 71);  printf("*");
  _settextposition(21, 31); printf("*");
  _settextposition(21, 71); printf("*");

  // set horzontal coords.
  _asm {
    mov  cx,30 * 8
    mov  dx,70 * 8
    mov  ax,07h
    int  33h
  }
  // set vertical coords.
  _asm {
    mov  cx,5 * 8
    mov  dx,20 * 8
    mov  ax,08h
    int  33h
  }

  // Mouse on
  _asm {
    mov  ax,01h
    int  33h
  }

  do {
    // get mouse status (buttons and pos)
    _asm {
      mov  ax,03h
      int  33h
      mov  buttns,bl
      mov  horz,cx
      mov  vert,dx
    }
    _settextposition(10, 7);
    printf("        Horz:  %u   ", horz>>3 );
    _settextposition(11, 7);
    printf("        Vert:  %u   ", vert>>3 );
    _settextposition(12, 7);
    printf(" Left Button:  ");
    if (buttns & 1) printf("Pressed"); else printf("       ");
    _settextposition(13, 7);
    printf("Right Button:  ");
    if (buttns & 2) printf("Pressed"); else printf("       ");
  } while (!kbhit());

  // Mouse off
  _asm {
    mov  ax,02h
    int  33h
  }

  // turn cursor on
	_asm {
    mov  ah,01h
    mov  cx,0607h
    int  10h
	}

  exit (0);
}

