



#if !defined __VESA_H
#define __VESA_H

#include "dpmi.h"

#pragma pack(__push,1);

// SVGA info block

typedef struct {
   char    magic[4];           // 'VESA' 4 byte signature
   char    minor, major;       // VBE version number
   char    *oem;               // Pointer to OEM string
   long    Capabilities;       // Capabilities of video card
   short   *videomode;         // Pointer to supported modes
   short   memblocks;          // Number of 64kb memory blocks

   short  OemSoftwareRev;       // vesa 2.0 specific
   char  *OemVendorNamePtr;
   char  *OemProductNamePtr;
   char  *OemProductRevPtr;
   char   reserved[222];
   char   oem_data[256];
} VgaInfoBlock;

// SVGA mode info block

typedef struct {
   short   ModeAttrib;          // Mode attributes
   char    WinAAttrib;          // Window A attributes
   char    WinBAttrib;          // Window B attributes
   short   WinGranularity;      // Window granularity in kbytes (its the size of a packet, where "x" packets make up a 64kb bank
   unsigned short WinSize;      // Window size in k
   unsigned short WinASeg;      // Window A segment
   unsigned short WinBSeg;      // Window B segment
   void    *WinFunc;            // Pointer to window function
   short   bps;                 // Bytes per scanline

   short   maxx;                // Horizontal resolution
   short   maxy;                // Vertical resolution
   char    XCharSize;           // Character cell width
   char    YCharSize;           // Character cell height
   char    planecount;          // Number of memory planes
   char    bpp;                 // Bits per pixel
   char    bankcount;           // Number of CGA style banks
   char    MemoryModel;         // Memory model type
   char    BankSize;            // Size of CGA style banks
   char    imagepagecount;      // Number of images pages
   char    pad1;                // Reserved

   char    RedMaskSize;         // Size of direct color red mask
   char    RedFieldPosition;    // Bit posn of lsb of red mask
   char    GreenMaskSize;       // Size of direct color green mask
   char    GreenFieldPosition;  // Bit posn of lsb of green mask
   char    BlueMaskSize;        // Size of direct color blue mask
   char    BlueFieldPosition;   // Bit posn of lsb of blue mask
   char    RsvdMaskSize;        // Size of direct color res mask
   char    RsvdFieldPosition;   // Bit posn of lsb of res mask
   char    DirectColorModeInfo; // Direct color mode attributes

                                // vbe 2.0 specific
   char  *PhysBasePtr;          // 32 bit flat mem mode ptr to screen buffer
   char  *OffScreenMemOffset;   // ptr to start of off screen memory
   short  OffScreenMemSize;     // screen memory in 1k units
   char   pad[206];             // Pad to 256 byte block size
} ModeInfoBlock;


class vesa_api {

   public:
      unsigned char     *screen;
      int               start_mode;
      VgaInfoBlock      vesa_head;
      ModeInfoBlock     current_mode;
      dpmi              real_mode;

      void dump_vesa_head();

      int get_mode();
      int get_mode(int mode, ModeInfoBlock *x);

      void set_mode(int mode);

      virtual int read_vesa_head() = 0;

      virtual int set_mode(int length, int height, int bpp) = 0;

      virtual void vesa_bitblt(unsigned char *buffer) = 0;
      virtual void vesa_bitblt3(unsigned char *buffer) = 0;
      virtual void vesa_lineblt3(unsigned char *buffer, int y) = 0;
      virtual void vesa_hline32(unsigned int y, unsigned int start, unsigned int end, unsigned char color) = 0;
      virtual void vesa_vline32(unsigned int start, unsigned int end, unsigned int x, unsigned char color) = 0;
      virtual void vesa_put_pixel32(unsigned int y, unsigned int x, unsigned char color) = 0;
};


class vesa1_2 : public vesa_api {

   public:
      int               write_bank;
      int               current_bank;
      int               bank_size, bank_bit;
      int               win_size, win_remainder, win_count;
      int               premask, postmask;

      int read_vesa_head();

      int set_mode(int length, int height, int bpp);

      void vesa_bitblt(unsigned char *buffer);
      void vesa_bitblt3(unsigned char *buffer);
      void vesa_lineblt3(unsigned char *buffer, int y);
      void vesa_hline32(unsigned int y, unsigned int start, unsigned int end, unsigned char color);
      void vesa_vline32(unsigned int start, unsigned int end, unsigned int x, unsigned char color);
      void vesa_put_pixel32(unsigned int y, unsigned int x, unsigned char color);

      void setBank(int bank);
};


class vesa2_0 : public vesa_api {

   public:
      unsigned char *win_reset;

      int read_vesa_head();

      int  set_mode(int length, int height, int bpp);

      void vesa_bitblt(unsigned char *buffer);
      void vesa_bitblt3(unsigned char *buffer);
      void vesa_bitbltx2(unsigned char *buffer);
      void vesa_lineblt3(unsigned char *buffer, int y);
      void vesa_hline32(unsigned int y, unsigned int start, unsigned int end, unsigned char color);
      void vesa_vline32(unsigned int start, unsigned int end, unsigned int x, unsigned char color);
      void vesa_put_pixel32(unsigned int y, unsigned int x, unsigned char color);

      void vesa_clear_screen();
      void vesa_reset();
};


#pragma pack(__pop);

#endif


