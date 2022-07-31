/************************************************************
*  This is a small example on the BIOS Keyboard buffer area *
*  This source looks at the LOW DOS memory area and checks  *
*    for a key entered via if the head ptr and tail ptr are *
*    different.                                             *
*                                                           *
*  Forever Young Software                                   *
*  by Benjamin David Lunt                                   *
*  Copyright 1984-1998                                      *
*  All rights reserved                                      *
*  24 May 1998                                              *
************************************************************/

#include<conio.h>
#include<dos.h>
#include<stdio.h>

void main() {

  unsigned int headptr;         // pointer to head of buffer
  unsigned int tailptr;         // pointer to tail of buffer
	unsigned int key;

  /*assign what ptr is pointing at to key and print it to the screen*/

	while(1) {
    // the BIOS holds the head pointer at 0040:001Ah &
    //          holds the tail pointer at 0040:001Ch
    headptr = *(unsigned int _far *) 0x0040001A;
		tailptr = *(unsigned int _far *) 0x0040001C;
    // If the two pointers are different, then there is a key
    //  in this buffer ready for extraction
    // Buffer is in the 0040h segment with an offset of HEADPTR (above)
    if (headptr != tailptr) {
			key = *(unsigned int _far *) (0x00400000 + headptr);
			getch();
			break;
		}
	}
	printf("\n     Head: %xh"
				 "\n     Tail: %xh"
				 "\n Scancode: %xh"
				 "\n      Key: %c", headptr, tailptr, key, (char) key);
	exit(0);
}
