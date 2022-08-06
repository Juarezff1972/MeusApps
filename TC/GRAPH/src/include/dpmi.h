

#if !defined __DPMI_H
#define __DPMI_H

#include <dos.h>

#pragma pack(__push,1);


typedef struct SREGS sregs_type;
typedef union REGS regs_type;

typedef struct {
    long EDI;
    long ESI;
    long EBP;
    long reserved_by_system;
    long EBX;
    long EDX;
    long ECX;
    long EAX;
    short flags;
    short ES,DS,FS,GS,IP,CS,SP,SS;
} rminfo;


class dpmi {

   public:
      rminfo reg;

      dpmi();
      void new_dos_mem(int x, int *segment, int *selector, regs_type *regs, sregs_type *sregs);
      void delete_dos_mem(int selector, regs_type *regs, sregs_type *sregs);
      void int10x(sregs_type *sregs);
      void int10();
      unsigned char *int10_800(void *device, unsigned int buffer_size);
};

#pragma pack(__pop);

#define dos2prot(x) ( (void *)((((unsigned int)(x) & 0xffff0000)>>12) + ((unsigned int)(x) & 0x0ffff)))

#endif
