



/* *************************************************************
   this file contains all the routines for the zbuffer class
************************************************************* */

#include <string.h>
#include "zbuffer.h"

extern int INTERLACE;
extern int X2;

#define MAXFILTER 7

static float  default_filter[MAXFILTER*MAXFILTER] = {
         0.001, 0.004, 0.008, 0.010, 0.008, 0.004, 0.001,
         0.004, 0.012, 0.025, 0.029, 0.025, 0.012, 0.002,
         0.008, 0.025, 0.049, 0.058, 0.049, 0.025, 0.008,
         0.010, 0.029, 0.058, 0.068, 0.058, 0.029, 0.010,
         0.008, 0.025, 0.049, 0.058, 0.049, 0.025, 0.008,
         0.004, 0.012, 0.025, 0.029, 0.025, 0.012, 0.002,
         0.001, 0.004, 0.008, 0.010, 0.008, 0.004, 0.001,
      };

#ifdef DOS
void init_buff(unsigned int *dst, unsigned int count) {

   double dptr;

   count *= 12;

   _asm {
      fldz
      mov eax, 0
      mov esi, dst
      mov ebx, back
      mov edi, count

loop1:
      fst qword ptr [esi+edi-8]
      mov [esi+edi-12], ebx
      mov [esi+edi-16], eax
      mov [esi+edi-20], eax
      mov [esi+edi-24], ebx
      fst qword ptr [esi+edi-32]
      mov [esi+edi-36], ebx
      mov [esi+edi-40], eax
      mov [esi+edi-44], eax
      mov [esi+edi-48], ebx

      sub edi, 48
      jnz loop1

      fstp dptr
   }

}

#else

void init_buff(unsigned int *dst, unsigned int count) {

   unsigned int i = 0;
   unsigned int j = *(unsigned int *)&back;

   count *= 3;

   do {
      dst[count-1] = i;
      dst[count-2] = i;
      dst[count-3] = j;
      dst[count-4] = i;
      dst[count-5] = i;
      dst[count-6] = j;
      dst[count-7] = i;
      dst[count-8] = i;
      dst[count-9] = j;
      dst[count-10] = i;
      dst[count-11] = i;
      dst[count-12] = j;
      count-=12;
   } while (count);

}

#endif


/* *************************************************************
   this function reads in an anti-aliasing filter
************************************************************* */
int zbuffer::read_filter(char *fname, mapf *filter) {

   FILE   *infile;
   int    i, j;

   if (!(infile = fopen(fname, "r"))) {
      printf("Invalid Filter... using default\n");
      return 0;
   }

   fscanf(infile, "%d", &j);

   if (!j || !(j & 0x01)) {
      printf("Invalid Filter... using default\n");
      return 0;
   }

   filter->init_map(j, j);

   j *= j;

   for (i=0; i<j; i++)
      fscanf(infile, "%f", &filter->data[i]);

   fclose(infile);
   return 1;
}


/* *************************************************************
************************************************************* */
void zbuffer::save_rgb(char *fname, int mapno) {

   if (!compress_flag)
      compress();

   if (X2) {
      maxx += maxx;
      maxy += maxy;
   }

   head.xsize = (short)maxx;
   head.ysize = (short)maxy;
   head.zsize = 3;

#ifdef DOS
   sprintf(filename, "%4s%04d.rgb", fname, mapno);
#else
   sprintf(filename, "%s.%05d.rgb", fname, mapno);
#endif


   write_rle(filename, 1);

   if (X2) {
      maxx = maxx>>1;
      maxy = maxy>>1;
   }
}


/* **************************************************
   this function anti-aliases the zbuffer
************************************************** */
void zbuffer::antialias(char *fname) {

   mapf      filter;
   int       defflag = 0;
   int       current;                           // center of filter
   int       h, i, j, k, l, m, n, o;                  // looping variables
   float     temp[3];                           // current pixel value
   int       cmaxx, cmaxy;

   unsigned char *tbuff;
   unsigned char **ptbuff;
   unsigned char *ctemp;

   unsigned int index, end;
   vector4uc *cdata = (vector4uc *)data;

   if (!compress_flag)
      compress();

   end = maxy*maxx;

   if (strlen(fname))                       // read filter from file
      defflag = read_filter(fname, &filter);

   if (!defflag) {
      filter.init_map(MAXFILTER, MAXFILTER);

      for (index=0, end = MAXFILTER*MAXFILTER; index<end; index++)
         filter.data[index] = default_filter[index];
   }

   k = filter.maxy*(h=maxx<<2);
   tbuff = new unsigned char[k];
   ptbuff = new puchar[filter.maxy];

   for (i=j=0; i<filter.maxy; i++, j+=h)
      ptbuff[i] = &tbuff[j];

   memcpy(ptbuff[1], data, j-h);

   current = (filter.maxx-1) >> 1;
   cmaxx = maxx - current;
   cmaxy = maxy - current;

   for (i=current*maxx, o=cmaxy*maxx, n=(filter.maxy-1)*maxx; i<o; i += maxx, n+=maxx) {

      ctemp = ptbuff[0];
      for (k=0, l=filter.maxy-1; k<l; k++)
         ptbuff[k] = ptbuff[k+1];
      ptbuff[k] = ctemp;
      memcpy(ctemp, &data[n], h);

      for (j=current, m=0; j<cmaxx; j++, m+=4) {
         temp[0] = temp[1] = temp[2] = 0;

#ifdef SGI
         for (k=0; k<filter.maxy; k++)
            for (l=0, index=m; l<filter.maxx; l++, index+=4) {
               temp[0] += filter.pdata[k][l] * ptbuff[k][index+1];
               temp[1] += filter.pdata[k][l] * ptbuff[k][index+2];
               temp[2] += filter.pdata[k][l] * ptbuff[k][index+3];
            }

         cdata[i+j][1] = pbyte_lut[(int)temp[0]];
         cdata[i+j][2] = pbyte_lut[(int)temp[1]];
         cdata[i+j][3] = pbyte_lut[(int)temp[2]];
#else

         for (k=0; k<filter.maxy; k++)
            for (l=0, index=m; l<filter.maxx; l++, index+=4) {
               temp[0] += filter.pdata[k][l] * ptbuff[k][index+0];
               temp[1] += filter.pdata[k][l] * ptbuff[k][index+1];
               temp[2] += filter.pdata[k][l] * ptbuff[k][index+2];
            }

         cdata[i+j][0] = pbyte_lut[(int)temp[0]];
         cdata[i+j][1] = pbyte_lut[(int)temp[1]];
         cdata[i+j][2] = pbyte_lut[(int)temp[2]];
#endif
      }

   }

   delete [] ptbuff;
   delete [] tbuff;
}


/* **************************************************
   this function anti-aliases the zbuffer
************************************************** */
void zbuffer::antialias_edge(char *fname) {

   mapf      filter;
   int       defflag = 0;
   int       current;                           // center of filter
   int       h, i, j, k, l, m, n, o, p;                  // looping variables
   float     temp;                           // current pixel value
   int       cmaxx, cmaxy;

   unsigned char *tbuff;
   unsigned char **ptbuff;
   unsigned char *ctemp;

   unsigned int index, end;
   vector4uc *cdata = (vector4uc *)data;

   if (!compress_flag)
      compress();

   end = maxy*maxx;

   if (strlen(fname))                       // read filter from file
      defflag = read_filter(fname, &filter);

   if (!defflag) {
      filter.init_map(MAXFILTER, MAXFILTER);

      for (index=0, end = MAXFILTER*MAXFILTER; index<end; index++)
         filter.data[index] = default_filter[index];
   }

   k = filter.maxy*(h=maxx<<2);
   tbuff = new unsigned char[k];
   ptbuff = new puchar[filter.maxy];

   for (i=j=0; i<filter.maxy; i++, j+=h)
      ptbuff[i] = &tbuff[j];

   memcpy(ptbuff[1], data, j-h);

   current = (filter.maxx-1) >> 1;
   cmaxx = maxx - current;
   cmaxy = maxy - current;

   for (i=current*maxx, o=cmaxy*maxx, n=(filter.maxy-1)*maxx; i<o; i += maxx, n+=maxx) {

      ctemp = ptbuff[0];
      for (k=0, l=filter.maxy-1; k<l; k++)
         ptbuff[k] = ptbuff[k+1];
      ptbuff[k] = ctemp;
      memcpy(ctemp, &data[n], h);

      for (j=current, m=0; j<cmaxx; j++, m+=4) {

#ifdef SGI
         for (p = 1; p<4; p++) {
#else
         for (p = 0; p<3; p++) {
#endif
            temp = -ptbuff[current-1][m+p+4] - ptbuff[current+1][m+p+4]
                   -ptbuff[current][m+p] + 4*ptbuff[current][m+p+4] - ptbuff[current][m+p+8];
            if (temp > 0.0) {
               temp = 0.0;

               for (k=0; k<filter.maxy; k++)
                  for (l=0, index=m; l<filter.maxx; l++, index+=4)
                     temp += filter.pdata[k][l] * ptbuff[k][index+p];

               cdata[i+j][p] = pbyte_lut[(int)temp];
            }

         }

      }

   }

   delete [] ptbuff;
   delete [] tbuff;
}


/* **************************************************
   this function initializes the buffer
************************************************** */
void zbuffer::gammabuff(unsigned char *pgamma) {

   vector4uc *cdata = (vector4uc *)data;
   int end = maxx*maxy;
   int index;

   if (!compress_flag)
      compress();

   for (index=0; index < end; index++) {
#ifdef SGI
      cdata[index][1] = pgamma[cdata[index][1]];
      cdata[index][2] = pgamma[cdata[index][2]];
      cdata[index][3] = pgamma[cdata[index][3]];
#else
      cdata[index][0] = pgamma[cdata[index][0]];
      cdata[index][1] = pgamma[cdata[index][1]];
      cdata[index][2] = pgamma[cdata[index][2]];
#endif
   }

}


/* **************************************************
   this function initializes the buffer
************************************************** */
int zbuffer::initbuff(unsigned int x, unsigned int y) {

   unsigned int i;

   zexel *pzbuff;

   if (maxy == y)
      maxy = y*3;

   init_map(x, y*3);

   i  = x*y;

   maxx = x;
   maxy = y;

   zbuff = (zexel *)data;

   if (INTERLACE) {
#ifdef DOS
      pzbuff = zbuff+x;
#else
      pzbuff = zbuff;
#endif
      x+=x;
   }

   else
      pzbuff = zbuff;

   while (i) {
      i -= x;
      init_buff((unsigned int *)(pzbuff+i), maxx);
   }

   compress_flag = 0;

   return 1;
}


/* **************************************************
   this function initializes the buffer
************************************************** */
int zbuffer::initbuff(unsigned int *d, puint *pd, unsigned int x, unsigned int y) {

   unsigned int i;

   zexel *pzbuff;

   if (data != (unsigned int *)NULL) {
      delete [] data;
      delete [] pdata;
   }

   data = d;
   pdata = pd;

   i = x*y;

   maxx = x;
   maxy = y;

   zbuff = (zexel *)data;

   if (INTERLACE) {
#ifdef DOS
      pzbuff = zbuff+x;
#else
      pzbuff = zbuff;
#endif
      x+=x;
   }

   else
      pzbuff = zbuff;

   while (i) {
      i -= x;
      init_buff((unsigned int *)(pzbuff+i), maxx);
   }

   compress_flag = 0;

   return 1;
}

#ifndef DOS

/* *************************************************************
   this function puts an XImage on the screen
************************************************************* */
void zbuffer::render(ALEXimage mcanvas) {

   int  i, j;
   unsigned int *ptr;

   if (!compress_flag)
      compress();

   ptr  = ((unsigned int *)(mcanvas->data)) + mcanvas->width*mcanvas->height;

   j = maxx<<2;
   for (i=0, ptr-=maxx; i<maxy; i++, ptr-=maxx)
      memcpy(ptr, pdata[i], j);
}


/* *************************************************************
   this function puts an XImage on the screen
************************************************************* */
void zbuffer::render(mapul *mcanvas) {

   if (!compress_flag)
      compress();

   memcpy(mcanvas->data, data, maxx*maxy<<2);
}

#endif


/* *************************************************************
************************************************************* */
void zbuffer::compress() {

   int i, j;
   unsigned int *ptr, *qtr, *rtr;

   compress_flag = 1;

   if (INTERLACE) {

#ifdef DOS
      ptr = data + maxx;
      qtr = data + 3*maxx;
#else
      ptr = qtr = data;
#endif

      j = maxy>>1;

      do {
         i = maxx>>3;

         do {
            *(ptr)   = *(qtr+2);
            *(ptr+1) = *(qtr+5);
            *(ptr+2) = *(qtr+8);
            *(ptr+3) = *(qtr+11);
            *(ptr+4) = *(qtr+14);
            *(ptr+5) = *(qtr+17);
            *(ptr+6) = *(qtr+20);
            *(ptr+7) = *(qtr+23);
            ptr+=8;
            qtr+=24;
         } while (--i);

         memset(ptr, 0, maxx<<2);
         ptr += maxx;
         qtr += maxx*3;
      } while (--j);

      return;
   }

   if (X2) {

      rtr = ptr = data + maxx*maxy*4;
      qtr = data + maxx*maxy*3;

      j = maxy;

      do {
         i = maxx>>2;

         do {
            *(ptr-1) = *(qtr-1);
            *(ptr-2) = *(qtr-1);
            *(ptr-3) = *(qtr-4);
            *(ptr-4) = *(qtr-4);
            *(ptr-5) = *(qtr-7);
            *(ptr-6) = *(qtr-7);
            *(ptr-7) = *(qtr-10);
            *(ptr-8) = *(qtr-10);
            ptr-=8;
            qtr-=12;
         } while (--i);

         i = maxx>>2;

         do {
            *(ptr-1) = *(rtr-1);
            *(ptr-2) = *(rtr-1);
            *(ptr-3) = *(rtr-3);
            *(ptr-4) = *(rtr-3);
            *(ptr-5) = *(rtr-5);
            *(ptr-6) = *(rtr-5);
            *(ptr-7) = *(rtr-7);
            *(ptr-8) = *(rtr-7);
            ptr-=8;
            rtr-=8;
         } while (--i);

        rtr = ptr;
      } while (--j);

      return;
   }

   ptr = qtr = data;
   i=(maxx*maxy)>>3;

   do {
      *(ptr)   = *(qtr+2);
      *(ptr+1) = *(qtr+5);
      *(ptr+2) = *(qtr+8);
      *(ptr+3) = *(qtr+11);
      *(ptr+4) = *(qtr+14);
      *(ptr+5) = *(qtr+17);
      *(ptr+6) = *(qtr+20);
      *(ptr+7) = *(qtr+23);
      ptr+=8;
      qtr+=24;
   } while (--i);

}

float back = -1000000000.0;
float front = -1.0;

