/* FONT.C
   This is a port to C from the Assember routine FONT.ASM.
   I did not compile this to make sure but it should run OK.
   If this doesn't run on your system, please let me know and I
   will try to fix it for portability.

   Please report any errors you might find to blunt@zekes.com.
   Source from http://www.zekes.com/~blunt/index.htm.

   Forever Young Software
   Benjamin David Lunt
   (C)opyright 1997
   All rights reserved
   07/29/97

   I use Microsoft C Compiler.  If you use Borland C then don't use
   the *mptr pointer.  Just use OurFont in place of mptr in the FP_SEG
   and FP_OFF functions.

*/


#include <dos.h>

unsigned char OurFont[28] = {
	    0x00,          /* 00000000b */
            0x7F,          /* 01111111b */
            0x63,          /* 01100011b */
            0x63,          /* 01100011b */
            0x63,          /* 01100011b */
            0x7F,          /* 01111111b */
            0x63,          /* 01100011b */
            0x63,          /* 01100011b */
            0x63,          /* 01100011b */
            0x63,          /* 01100011b */
            0x63,          /* 01100011b */
            0x63,          /* 01100011b */
            0x00,          /* 00000000b */
            0x00,          /* 00000000b */

            0x00,          /* 00000000b */
            0x7E,          /* 01111110b */
            0x66,          /* 01100110b */
            0x66,          /* 01100110b */
            0x66,          /* 01100110b */
            0x7F,          /* 01111111b */
            0x63,          /* 01100011b */
            0x63,          /* 01100011b */
            0x63,          /* 01100011b */
            0x63,          /* 01100011b */
            0x63,          /* 01100011b */
            0x7F,          /* 01111111b */
            0x00,          /* 00000000b */
	    0x00           /* 00000000b */
};

void main(void)
{
  union REGS pregs;
  struct SREGS sregs;
  void far *mptr = OurFont;

  pregs.x.ax = 0x1100;
  pregs.x.dx = 0x41;       /* 65  'A' */
  pregs.x.cx = 0x02;       /* two char's to change */
  pregs.h.bh = 0x0E;
  pregs.x.bp = FP_OFF(mptr);
  sregs.es = FP_SEG(mptr);
  int86x(0x10, &pregs, &pregs, &sregs);
}
