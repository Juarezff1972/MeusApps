#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>


#include "vesa.h"

static void quadcpy8(unsigned int *dst, unsigned int *src, int wordcount) {

   _asm {
      mov eax, wordcount
      mov edx, 0
      shl eax, 2
      mov edi, dst
      mov esi, src
      lea ecx, [eax + eax*2]

loop1:
      mov eax, [esi+edx+8]
      mov ebx, [esi+edx+20]
      mov [edi], eax
      mov [edi+4], ebx
      mov eax, [esi+edx+32]
      mov ebx, [esi+edx+44]
      mov [edi+8], eax
      mov [edi+12], ebx
      mov eax, [esi+edx+56]
      mov ebx, [esi+edx+68]
      mov [edi+16], eax
      mov [edi+20], ebx
      mov eax, [esi+edx+80]
      mov ebx, [esi+edx+92]
      mov [edi+24], eax
      mov [edi+28], ebx

      add edx, 96
      add edi, 32
      cmp edx, ecx
      jne loop1
   }

}


static void quadcpy8_24(unsigned int *dst, unsigned int *src, int wordcount) {

   _asm {
      mov eax, wordcount
      mov edx, 0
      shl eax, 2
      mov edi, dst
      mov esi, src
      lea ecx, [eax + eax*2]

loop1:
      mov eax, [esi+edx+8]
      mov ebx, [esi+edx+32]
      mov [edi], ax
      mov [edi+6], bx
      shr eax, 8
      shr ebx, 8
      mov [edi+2], ah
      mov [edi+8], bh

      mov eax, [esi+edx+20]
      mov ebx, [esi+edx+44]
      mov [edi+3], al
      mov [edi+9], bl
      shr eax, 8
      shr ebx, 8
      mov [edi+4], ax
      mov [edi+10], bx

      add edx, 48
      add edi, 12
      cmp edx, ecx
      jne loop1
   }

}


// ***********************************************************************
void vesa_api::dump_vesa_head() {

   printf("Magic = %c%c%c%c\n", vesa_head.magic[0], vesa_head.magic[1], vesa_head.magic[2], vesa_head.magic[3]);
   printf("Version %d.%d\n", vesa_head.major, vesa_head.minor);
   printf("Variable bit size ? %d\n", vesa_head.Capabilities);
   printf("Memory on card = %fM\n", vesa_head.memblocks/16.0);
   printf("Card Info: %s\n", vesa_head.oem);
}


int vesa_api::get_mode() {

   real_mode.reg.EAX=0x4f03;
   real_mode.int10();

   return real_mode.reg.EBX & 0x0000ffff;
}


int vesa_api::get_mode(int mode, ModeInfoBlock *x) {

   union REGS regs;
   struct SREGS sregs;
   int selector;
   int segment;
   char far *str;
   char *ttr;

   memset(&sregs, 0, sizeof(sregs_type));
   real_mode.new_dos_mem(256, &segment, &selector, &regs, &sregs);

   // Set up real-mode call structure
   real_mode.reg.EAX=0x4f01;
   real_mode.reg.ECX=mode;
   real_mode.reg.DS=0;
   real_mode.reg.ES=segment;

   real_mode.int10x(&sregs);

   str = (char far *)MK_FP(selector,0);
   ttr = (char *)x;
   for (int i=0; i<256; i++)
      ttr[i] = str[i];

   real_mode.delete_dos_mem(selector, &regs, &sregs);

   x->WinFunc = dos2prot(x->WinFunc);

   return real_mode.reg.EAX == 0x4f;
}


void vesa_api::set_mode(int mode) {

   get_mode(mode, &current_mode);

   real_mode.reg.EAX=0x4f02;
   real_mode.reg.EBX=mode;
   real_mode.int10();
}


// ***********************************************************************
int vesa1_2::read_vesa_head() {

   regs_type regs;
   sregs_type sregs;
   int selector;
   int segment;
   char far *str;
   char *ttr;
   int i;

   memset(&sregs, 0, sizeof(sregs_type));
   real_mode.new_dos_mem(256, &segment, &selector, &regs, &sregs);

   // Set up real-mode call structure
   real_mode.reg.EAX=0x4f00;
   real_mode.reg.DS=0;
   real_mode.reg.ES=segment;

   real_mode.int10x(&sregs);                     // make dos int 10h call

   str = (char far *)MK_FP(selector,0);

   ttr = (char *)&vesa_head;
   for (i=0; i<256; i++)
      ttr[i] = str[i];

   real_mode.delete_dos_mem(selector, &regs, &sregs);

   vesa_head.oem = (char *)dos2prot(vesa_head.oem);
   vesa_head.videomode = (short *)dos2prot(vesa_head.videomode);

   return real_mode.reg.EAX == 0x4f;
}


int vesa1_2::set_mode(int length, int height, int bpp) {

   short           *p;
   ModeInfoBlock   data;

   for (p = vesa_head.videomode; *p != -1; p++)
      if (get_mode(*p, &data) && data.maxx == length && data.maxy == height && data.bpp == bpp) {
         current_mode = data;

         real_mode.reg.EAX=0x4f02;
         real_mode.reg.EBX=*p;

         real_mode.int10();

         if ((current_mode.WinAAttrib & 0x01) && (current_mode.WinAAttrib & 0x04)) {
            screen = (unsigned char *)(((unsigned int)current_mode.WinASeg)<<4);
            write_bank = 0;
         }

         else {
            screen = (unsigned char *)(((unsigned int)current_mode.WinBSeg)<<4);
            write_bank = 1;
         }

         current_bank = -1;

         bank_size = current_mode.WinSize*1024;
         for (win_size = 0; (64>>win_size) != current_mode.WinGranularity; win_size++);
         win_remainder = current_mode.maxx*current_mode.maxy*(current_mode.bpp>>3);
         win_count = win_remainder>>16;
         win_remainder -= (win_remainder/bank_size)*bank_size;

         for (bank_bit = 10; bank_size > (1<<bank_bit); bank_bit++);

         postmask = ((1<<bank_bit) - 1);
         premask = ~postmask;

         return 1;
      }

   return 0;
}


/* *******************************************************************
   Set new read/write bank. We must set both Window A and Window B, as
   many VBE's have these set as separately available read and write
   windows. We also use a simple (but very effective) optimisation of
   checking if the requested bank is currently active.
******************************************************************* */
void vesa1_2::setBank(int bank) {

    current_bank = bank;                 /* Save current bank number         */

    bank = bank<<win_size;

/*
    set_ax_bx(bank, write_bank);
    ((void (*)(void))current_mode.WinFunc)();
*/

   real_mode.reg.EAX=0x4f05;
   real_mode.reg.EBX=write_bank;
   real_mode.reg.EDX=bank;
   real_mode.int10();
}


void vesa1_2::vesa_bitblt(unsigned char *buffer) {

   unsigned int i, j;

   j = win_count;

   if (current_bank)
      setBank(0);

   memcpy(screen, buffer, bank_size);

   i = 1;
   buffer += bank_size;

   do {
      setBank(i);
      memcpy(screen, buffer, bank_size);

      i++;
      buffer += bank_size;
   } while (i<j);

   if (win_remainder) {
      setBank(i);
      memcpy(screen, buffer, win_remainder);
   }

}


void vesa1_2::vesa_bitblt3(unsigned char *buffer) {

   unsigned int i, j;
   int bs = bank_size*3;

   j = win_count;

   if (current_bank)
      setBank(0);

   quadcpy8((unsigned int *)screen, (unsigned int *)buffer, bank_size>>2);

   i = 1;
   buffer += bs;

   do {
      setBank(i);
      quadcpy8((unsigned int *)screen, (unsigned int *)buffer, bank_size>>2);

      i++;
      buffer += bs;
   } while (i<j);

   if (win_remainder) {
      setBank(i);
      quadcpy8((unsigned int *)screen, (unsigned int *)buffer, win_remainder>>2);
   }

}


void vesa1_2::vesa_lineblt3(unsigned char *buffer, int y) {

   unsigned int i, j;
   int bank1, bank2;
   unsigned int start, end;
   int bs = bank_size*3;

   i = current_mode.maxx<<2;
   start = y*i;
   end   = (y+1)*i;

   bank1 = (start & premask)>>bank_bit;
   bank2 = (end   & premask)>>bank_bit;

   if (bank1 != current_bank)
      setBank(bank1);

   j = start & postmask;

   if (bank1 == bank2) {
      quadcpy8((unsigned int *)(screen+j), (unsigned int *)buffer, i>>2);
      return;
   }

   i = bank_size-j;
   quadcpy8((unsigned int *)(screen+j), (unsigned int *)buffer, i>>2);
   buffer += i*3;
   bank1++;

   while (bank1 < bank2) {
      setBank(bank1);
      quadcpy8((unsigned int *)screen, (unsigned int *)buffer, bank_size>>2);
      buffer += bs;
      bank1++;
   }

   start += i + bank_size*(bank2-bank1);

   if ((i=end-start)) {
      setBank(bank1);
      quadcpy8((unsigned int *)screen, (unsigned int *)buffer, i>>2);
   }

}

void vesa1_2::vesa_hline32(unsigned int y, unsigned int start, unsigned int end, unsigned char color) {

   unsigned int bank1, bank2;
   unsigned int i, j;

   y *= current_mode.maxx;
   start = (y+start)<<2;
   end   = (y+end)<<2;

   bank1 = (start & premask)>>bank_bit;
   bank2 = (end   & premask)>>bank_bit;

   if (bank1 != current_bank)
      setBank(bank1);

   if (bank1 == bank2) {
      memset(screen + (start & postmask), color, end-start);
      return;
   }

   j = start & postmask;
   memset(screen + j, color, i = bank_size-j);
   start += i;
   bank1++;

   while (bank1 < bank2) {
      setBank(bank1);
      memset(screen, color, bank_size);
      start += bank_size;
      bank1++;
   }

   if ((i=end-start)) {
      setBank(bank1);
      memset(screen, color, i);
   }

}


void vesa1_2::vesa_vline32(unsigned int start, unsigned int end, unsigned int x, unsigned char color) {

   unsigned int bank, inc;
   unsigned char *ptr;

   start = (start*current_mode.maxx + x)<<2;
   end   = (end  *current_mode.maxx + x)<<2;
   inc   = current_mode.maxx<<2;

   for (; start < end; start += inc) {

      bank = (start & premask)>>bank_bit;

      if (bank != current_bank)
         setBank(bank);

      ptr = screen + (start & postmask);

      *ptr     = color;
      *(ptr+1) = color;
      *(ptr+2) = color;
      *(ptr+3) = color;
   }

}


void vesa1_2::vesa_put_pixel32(unsigned int y, unsigned int x, unsigned char color) {

   unsigned int bank;
   unsigned char *ptr;

   x = (y*current_mode.maxx + x)<<2;

   bank = (x & premask)>>bank_bit;

   if (bank != current_bank)
      setBank(bank);

   ptr = screen + (x & postmask);

   *ptr = color;
   *(ptr+1) = color;
   *(ptr+2) = color;
   *(ptr+3) = color;
}




// ***********************************************************************

int vesa2_0::read_vesa_head() {

   regs_type regs;
   sregs_type sregs;
   int selector;
   int segment;
   char *dosbuff;
   char *ttr;
   int i;

   memset(&sregs, 0, sizeof(sregs_type));

   real_mode.new_dos_mem(512, &segment, &selector, &regs, &sregs);

   dosbuff = (char *)((segment & 0xffff)<<4);

   dosbuff[0] = 'V';
   dosbuff[1] = 'B';
   dosbuff[2] = 'E';
   dosbuff[3] = '2';

   // Set up real-mode call structure
   real_mode.reg.EAX=0x4f00;
   real_mode.reg.ES  = segment;
   real_mode.reg.EDI = 0;

   real_mode.int10x(&sregs);                     // make dos int 10h call

   ttr = (char *)&vesa_head;

   for (i=0; i<512; i++)
      ttr[i] = dosbuff[i];

   real_mode.delete_dos_mem(selector, &regs, &sregs);

   vesa_head.oem = (char *)dos2prot(vesa_head.oem);
   vesa_head.videomode = (short *)dos2prot(vesa_head.videomode);
   vesa_head.OemVendorNamePtr  = (char *)dos2prot(vesa_head.OemVendorNamePtr );
   vesa_head.OemProductNamePtr = (char *)dos2prot(vesa_head.OemProductNamePtr);
   vesa_head.OemProductRevPtr  = (char *)dos2prot(vesa_head.OemProductRevPtr );

   return real_mode.reg.EAX == 0x4f;
}


int vesa2_0::set_mode(int length, int height, int bpp) {

   short           *p;
   ModeInfoBlock   data;
   short           mode;

   for (p = vesa_head.videomode; *p != -1; p++)
      if (get_mode(*p, &data) && data.maxx == length && data.maxy == height && data.bpp == bpp && data.PhysBasePtr) {
         current_mode = data;
         screen = real_mode.int10_800(data.PhysBasePtr, length*height*(bpp>>3));
         mode = (*p) | 0x4000;

         real_mode.reg.EAX=0x4f02;
         real_mode.reg.EBX=mode;

         real_mode.int10();

         real_mode.reg.EAX=0x4f0a;
         real_mode.reg.EBX=0;

         real_mode.int10();

         win_reset = (unsigned char *)((real_mode.reg.ES << 4) + (real_mode.reg.EDI & 0xFFFF));
         win_reset += *(((short *)win_reset) + 1);

         return 1;
      }

   return 0;
}


void vesa2_0::vesa_reset() {

   unsigned char *func = win_reset;

   _asm {
      pushad

      mov eax, 0x4f07
      mov ebx, 0x8000
      mov ecx, 0
      mov edx, 0
      call [func]

      popad
   }

}


void vesa2_0::vesa_clear_screen() {

   unsigned char *buffer = screen;
   unsigned int i = current_mode.maxx*current_mode.maxy*current_mode.bpp>>3;

//   vesa_reset();

   _asm {
      pushad
      mov eax, buffer
      mov ebx, i
      mov ecx, 0

loop1:
      mov [eax+ebx-4], ecx
      mov [eax+ebx-8], ecx
      mov [eax+ebx-12], ecx
      mov [eax+ebx-16], ecx
      mov [eax+ebx-20], ecx
      mov [eax+ebx-24], ecx
      mov [eax+ebx-28], ecx
      mov [eax+ebx-32], ecx
      sub ebx, 32;
      jnz loop1;
      popad
   }
}


void vesa2_0::vesa_bitblt(unsigned char *buffer) {

   unsigned int bsize = current_mode.maxx*current_mode.maxy;
   unsigned char *sptr = screen;

   if (current_mode.bpp == 32) {
      _asm {
         pushad
         mov ecx, bsize
         mov edi, sptr
         mov esi, buffer
         rep movsd
         popad
      }

   }

   else {
      _asm {
         pushad
         mov ecx, 0
         mov edi, sptr
         mov esi, buffer
         mov edx, bsize
         mov ebp, 0

loop2:
         mov eax, [esi + ecx*4]
         mov ebx, [esi + ecx*4 + 8]
         mov [edi + ebp], ax
         mov [edi + ebp + 6], bx
         shr eax, 8
         shr ebx, 8
         mov [edi + ebp + 2], ah
         mov [edi + ebp + 8], bh

         mov eax, [esi + ecx*4 + 4]
         mov ebx, [esi + ecx*4 + 12]
         mov [edi + ebp + 3], al
         mov [edi + ebp + 9], bl
         shr eax, 8
         shr ebx, 8
         mov [edi + ebp + 4], ax
         mov [edi + ebp + 10], bx

         add ebp, 3
         inc ecx
         cmp ecx, edx
         jne loop2

         popad
      }

   }

}


void vesa2_0::vesa_bitblt3(unsigned char *buffer) {

   if (current_mode.bpp == 32)
      quadcpy8((unsigned int *)screen, (unsigned int *)buffer, current_mode.maxx*current_mode.maxy);
   else
      quadcpy8_24((unsigned int *)screen, (unsigned int *)buffer, current_mode.maxx*current_mode.maxy);
}


void vesa2_0::vesa_bitbltx2(unsigned char *buffer) {

   int rowsize = current_mode.maxx>>1;
   int buffinc = rowsize*12;
   unsigned char *buffend = buffer + (current_mode.maxy>>1)*buffinc;
   int destinc = rowsize*8;
   unsigned char *dest = screen;

   for (; buffer!=buffend; buffer += buffinc, dest += destinc+destinc) {

      _asm {

         pushad
         mov edx, buffer
         mov ecx, dest
         add edx, buffinc

         mov edi, destinc
         sub esi, esi

         lea ebp, [ecx+ edi]

loop2:
            mov eax, [edx+esi-4]
            mov ebx, [edx+esi-16]
            mov [ecx+edi - 4], eax
            mov [ecx+edi - 8], eax
            mov [ecx+edi - 12], ebx
            mov [ecx+edi - 16], ebx
            mov [ebp+edi - 4], eax
            mov [ebp+edi - 8], eax
            mov [ebp+edi - 12], ebx
            mov [ebp+edi - 16], ebx
            mov eax, [edx+esi-28]
            mov ebx, [edx+esi-40]
            mov [ecx+edi - 20], eax
            mov [ecx+edi - 24], eax
            mov [ecx+edi - 28], ebx
            mov [ecx+edi - 32], ebx
            mov [ebp+edi - 20], eax
            mov [ebp+edi - 24], eax
            mov [ebp+edi - 28], ebx
            mov [ebp+edi - 32], ebx
            sub esi, 48
            sub edi, 32
            jnz loop2

         popad
      }

   }

}


void vesa2_0::vesa_lineblt3(unsigned char *buffer, int y) {

   unsigned int start;

   start = y * current_mode.maxx*current_mode.bpp>>3;

   if (current_mode.bpp == 32)
      quadcpy8((unsigned int *)(screen+start), (unsigned int *)buffer, current_mode.maxx);
   else
      quadcpy8_24((unsigned int *)(screen+start), (unsigned int *)buffer, current_mode.maxx);
}

void vesa2_0::vesa_hline32(unsigned int y, unsigned int start, unsigned int end, unsigned char color) {

   unsigned char *buffer;
   unsigned int bypp = current_mode.bpp>>3;

   buffer = screen + ((y*current_mode.maxx + start)*bypp);
   memset(buffer, color, (end-start)*bypp);
}


void vesa2_0::vesa_vline32(unsigned int start, unsigned int end, unsigned int x, unsigned char color) {

   unsigned int inc;
   unsigned char *ptr;
   unsigned int bypp = current_mode.bpp>>3;

   start = (start*current_mode.maxx + x)*bypp;
   end   = (end  *current_mode.maxx + x)*bypp;
   inc   = current_mode.maxx*bypp;
   ptr   = screen;

   if (bypp == 4)
      for (; start < end; start += inc) {
         *(ptr+start)   = color;
         *(ptr+start+1) = color;
         *(ptr+start+2) = color;
         *(ptr+start+3) = color;
      }

   else
      for (; start < end; start += inc) {
         *(ptr+start)   = color;
         *(ptr+start+1) = color;
         *(ptr+start+2) = color;
      }

}


void vesa2_0::vesa_put_pixel32(unsigned int y, unsigned int x, unsigned char color) {

   unsigned char *ptr;

   ptr = screen + ((y*current_mode.maxx + x)*current_mode.bpp>>3);

   *ptr = color;
   *(ptr+1) = color;
   *(ptr+2) = color;

   if (current_mode.bpp == 32)
      *(ptr+3) = color;
}


