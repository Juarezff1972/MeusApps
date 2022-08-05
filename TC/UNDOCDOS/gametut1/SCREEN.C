#include "screen.h"

// the screen initialize routine
int scrninit()
{
	_asm {
		mov  ax,0013h
		int  10h
	}
	return(0);  // for now (no error)
}

void scrnnrml()
{
	_asm {
		mov  ax,0003h
		int  10h
	}
}

void display_item(int item_num, int x, int y)
{
	unsigned int  postn, temp;

	y *= 10;
	x *= 10;
	postn = y * 320 + x;
	temp = item_num * 100;

	_asm {
		push ds
		push es
		push si
		push di
		mov  ax,0A000h
		mov  es,ax
		mov  si,offset itembitmap
		add  si,temp
		mov  di,postn
		mov  ax,seg itembitmap
		mov  ds,ax
		mov  cx,10
loop1:
		push cx
		mov  cx,05
		rep  movsw
		add  di,310
		pop  cx
		loop loop1
		pop  di
		pop  si
		pop  es
		pop  ds
	}
}

