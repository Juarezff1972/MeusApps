

#include <string.h>

#include "dpmi.h"

    // DPMI call 100h allocates DOS memory

dpmi::dpmi() {

   memset(&reg, 0, sizeof(rminfo));
}


void dpmi::new_dos_mem(int x, int *segment, int *selector, regs_type *regs, sregs_type *sregs) {

   regs->w.bx = x >> 4;
   if (x & 0xf)
      regs->w.bx++;

   regs->w.ax=0x0100;
   int386x( 0x31, regs, regs, sregs);
   *segment=regs->w.ax;
   *selector=regs->w.dx;
}

void dpmi::delete_dos_mem(int selector, regs_type *regs, sregs_type *sregs) {

    regs->w.ax=0x0100;
    regs->w.dx=selector;
    int386x( 0x31, regs, regs, sregs);
}


void dpmi::int10x(sregs_type *sregs) {

   regs_type regs;

   // Use DMPI call 300h to issue the DOS interrupt
   regs.w.ax = 0x0300;
   regs.w.bx = 0x0010;
   regs.h.bh = 0;
   regs.w.cx = 0;

   sregs->es = FP_SEG(&reg);
   regs.x.edi = FP_OFF(&reg);

   int386x( 0x31, &regs, &regs, sregs );
}


void dpmi::int10() {

   regs_type regs;

   // Use DMPI call 300h to issue the DOS interrupt
   regs.w.ax = 0x0300;
   regs.w.bx = 0x0010;
   regs.h.bh = 0;
   regs.w.cx = 0;

   regs.x.edi = FP_OFF(&reg);

   int386( 0x31, &regs, &regs );
}

// memory map physical device
unsigned char *dpmi::int10_800(void *device, unsigned int buffer_size) {

   regs_type regs;

   regs.w.ax = 0x0800;
   regs.x.ecx = (int)device;
   regs.x.edi = buffer_size;
   regs.x.ebx = ((int)device)>>16;
   regs.x.esi = buffer_size>>16;

   int386( 0x31, &regs, &regs );

   return (unsigned char *)((regs.x.ebx<<16) + regs.w.cx);
}

