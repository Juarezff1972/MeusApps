
#if !defined (__ZBUFFER_H)
#define __ZBUFFER_H

#include "matrix.h"
#include "alex.h"
#include "texture.h"

extern float front;
extern float back;


typedef struct {
   float zdata;
   vector4uc ambient;
   vector4uc pixel;
} zexel;


/* *************************************************************
   this is the definition of the zbuffer class
************************************************************* */
class zbuffer : public rgb {

   protected:
      int read_filter(char *fname, mapf *filter);

   public:
      zexel *zbuff;
      int   compress_flag;

      zbuffer() {
          zbuff = NULL;
      }

      int      initbuff(unsigned int x, unsigned int y);
      int      initbuff(unsigned int *d, puint *pd, unsigned int x, unsigned int y);
      void     save_rgb(char *fname, int mapno);
      void     antialias(char *fname);
      void     antialias_edge(char *fname);
      void     gammabuff(unsigned char *pgamma);
      void     compress();

#ifndef DOS
      void     render(ALEXimage mcanvas);
      void     render(mapul *mcanvas);
#endif
};


#ifdef SGI

#define set_data(zbuff, index, z, acolor, dcolor, a0, a1, a2, d0, d1, d2)       \
   a0 = BYTE_LUT((int)acolor[0]);             \
   a1 = BYTE_LUT((int)acolor[1]);             \
   a2 = BYTE_LUT((int)acolor[2]);             \
   d0 = BYTE_LUT((int)dcolor[0]+a0);          \
   d1 = BYTE_LUT((int)dcolor[1]+a1);          \
   d2 = BYTE_LUT((int)dcolor[2]+a2);          \
   (zbuff)[index].zdata = z;            \
   (zbuff)[index].ambient[3] = BYTE_LUT(a0);  \
   (zbuff)[index].ambient[2] = BYTE_LUT(a1);  \
   (zbuff)[index].ambient[1] = BYTE_LUT(a2);  \
   (zbuff)[index].pixel[3] = BYTE_LUT(d0);  \
   (zbuff)[index].pixel[2] = BYTE_LUT(d1);  \
   (zbuff)[index].pixel[1] = BYTE_LUT(d2)


#define set_datatr(zbuff, index, z, acolor, dcolor, a0, a1, a2, d0, d1, d2)     \
   a0 = BYTE_LUT((int)acolor[0]);           \
   a1 = BYTE_LUT((int)acolor[1]);           \
   a2 = BYTE_LUT((int)acolor[2]);           \
   d0 = BYTE_LUT((int)dcolor[0]+a0);        \
   d1 = BYTE_LUT((int)dcolor[1]+a1);        \
   d2 = BYTE_LUT((int)dcolor[2]+a2);        \
   if (a0 > (zbuff)[index].ambient[3]) \
      (zbuff)[index].ambient[3] = a0;  \
   if (a1 > (zbuff)[index].ambient[2]) \
      (zbuff)[index].ambient[2] = a1;  \
   if (a2 > (zbuff)[index].ambient[1]) \
      (zbuff)[index].ambient[1] = a2;  \
   if (d0 > (zbuff)[index].pixel[3])   \
      (zbuff)[index].pixel[3] = d0;    \
   if (d1 > (zbuff)[index].pixel[2])   \
      (zbuff)[index].pixel[2] = d1;    \
   if (d2 > (zbuff)[index].pixel[1])   \
      (zbuff)[index].pixel[1] = d2

#endif

#ifdef LINUX

#define set_data(zbuff, index, z, acolor, dcolor, a0, a1, a2, d0, d1, d2)       \
   F2I(a0, acolor[0]);          \
   F2I(a1, acolor[1]);          \
   F2I(a2, acolor[2]);          \
   F2I(d0, dcolor[0]);          \
   F2I(d1, dcolor[1]);          \
   F2I(d2, dcolor[2]);          \
   WAIT();                      \
   (zbuff)[index].zdata = z;           \
   (zbuff)[index].ambient[0] = BYTE_LUT(a0);  \
   (zbuff)[index].ambient[1] = BYTE_LUT(a1);  \
   (zbuff)[index].ambient[2] = BYTE_LUT(a2);  \
   (zbuff)[index].pixel[0] = BYTE_LUT(a0+d0);  \
   (zbuff)[index].pixel[1] = BYTE_LUT(a1+d1);  \
   (zbuff)[index].pixel[2] = BYTE_LUT(a2+d2)

#define set_datatr(zbuff, index, z, acolor, dcolor, a0, a1, a2, d0, d1, d2)     \
   F2I(a0, acolor[0]);          \
   F2I(a1, acolor[1]);          \
   F2I(a2, acolor[2]);          \
   F2I(d0, dcolor[0]);          \
   F2I(d1, dcolor[1]);          \
   F2I(d2, dcolor[2]);          \
   WAIT();                      \
   a0 = BYTE_LUT(a0);           \
   a1 = BYTE_LUT(a1);           \
   a2 = BYTE_LUT(a2);           \
   d0 = BYTE_LUT(d0+a0);           \
   d1 = BYTE_LUT(d1+a1);           \
   d2 = BYTE_LUT(d2+a2);           \
   if (a0 > (zbuff)[index].ambient[0]) \
      (zbuff)[index].ambient[0] = a0;  \
   if (a1 > (zbuff)[index].ambient[1]) \
      (zbuff)[index].ambient[1] = a1;  \
   if (a2 > (zbuff)[index].ambient[2]) \
      (zbuff)[index].ambient[2] = a2;  \
   if (d0 > (zbuff)[index].pixel[0]) \
      (zbuff)[index].pixel[0] = d0;  \
   if (d1 > (zbuff)[index].pixel[1]) \
      (zbuff)[index].pixel[1] = d1;  \
   if (d2 > (zbuff)[index].pixel[2]) \
      (zbuff)[index].pixel[2] = d2

#endif

#ifdef DOS

// can do this way - fst acolor, fist acolor, fadd dcolor, fistp dcolor
#define set_data(zbuff, index, z, acolor, dcolor, a0, a1, a2, d0, d1, d2)       \
   F2I(a0, acolor);             \
   F2I(a1, acolor+4);           \
   F2I(a2, acolor+8);           \
   F2I(d0, dcolor);             \
   F2I(d1, dcolor+4);           \
   F2I(d2, dcolor+8);           \
   WAIT();                      \
   (zbuff)[index].zdata = z;            \
   (zbuff)[index].ambient[2] = BYTE_LUT(a0);  \
   (zbuff)[index].ambient[1] = BYTE_LUT(a1);  \
   (zbuff)[index].ambient[0] = BYTE_LUT(a2);  \
   (zbuff)[index].pixel[2] = BYTE_LUT(a0+d0);  \
   (zbuff)[index].pixel[1] = BYTE_LUT(a1+d1);  \
   (zbuff)[index].pixel[0] = BYTE_LUT(a2+d2)

#define set_datatr(zbuff, index, z, acolor, dcolor, a0, a1, a2, d0, d1, d2)     \
   F2I(a0, acolor);             \
   F2I(a1, acolor+4);           \
   F2I(a2, acolor+8);           \
   F2I(d0, dcolor);             \
   F2I(d1, dcolor+4);           \
   F2I(d2, dcolor+8);           \
   WAIT();                      \
   a0 = BYTE_LUT(a0);           \
   a1 = BYTE_LUT(a1);           \
   a2 = BYTE_LUT(a2);           \
   d0 = BYTE_LUT(d0+a0);           \
   d1 = BYTE_LUT(d1+a1);           \
   d2 = BYTE_LUT(d2+a2);           \
   if (a0 > (zbuff)[index].ambient[2]) \
      (zbuff)[index].ambient[2] = a0;  \
   if (a1 > (zbuff)[index].ambient[1]) \
      (zbuff)[index].ambient[1] = a1;  \
   if (a2 > (zbuff)[index].ambient[0]) \
      (zbuff)[index].ambient[0] = a2;  \
   if (d0 > (zbuff)[index].pixel[2]) \
      (zbuff)[index].pixel[2] = d0;  \
   if (d1 > (zbuff)[index].pixel[1]) \
      (zbuff)[index].pixel[1] = d1;  \
   if (d2 > (zbuff)[index].pixel[0]) \
      (zbuff)[index].pixel[0] = d2
#endif

#endif
