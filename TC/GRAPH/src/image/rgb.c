

#include <string.h>
#include <stdio.h>
#include "texture.h"

#define MAGIC   474
#define MAXCOUNT 0x80

#define   NORMAL    0
#define   DITHERED  1
#define   SCREEN    2
#define   COLORMAP  3

/* ***********************************************************************
        This procedure will write out an rgb file in Verbatim format
*********************************************************************** */
int rgb::write_ver(char *fname, int flip) {

   FILE *outfile;
   unsigned char *buffer;
   int i, j, x, y;
   unsigned char *base;

   if (data == (unsigned int *)NULL) {
      printf("No texture to write\n");
      return 0;
   }

   if (!(outfile = fopen(fname,"wb"))) {
      printf("Could not open %s for output as RGB file\n", fname);
      return 0;
   }

   if (head.xsize > 404)
      buffer = new unsigned char[head.xsize];
   else
      buffer = new unsigned char[404];

   putushort(outfile, MAGIC);    // MAGIC
   putuchar(outfile, VERBATIM);  // STORAGE is VERBATIM
   putuchar(outfile, 1);         // BPC is 1 (# of bytes per pixel component/channel (1 or 2))
   putushort(outfile, 3);        // DIMENSION is 3 - color, 2 - BW, 1 - line
   putushort(outfile, head.xsize);    // width on xaxis
   putushort(outfile, head.ysize);    // length on yaxis
   putushort(outfile, head.zsize);    // # of channels ( 1 - BW, 3 - color, 4 RGBA) zsize
   putuint(outfile, 0);         // PIXMIN is 0
   putuint(outfile, 255);       // PIXMAX is 255

   buffer[0] = 0;

   fwrite(buffer,4,1,outfile);   // dummy 4 bytes

   fwrite((void *)head.name,80,1,outfile);  // IMAGENAME
   putuint(outfile,NORMAL);     // COLORMAP is 0 - NORMAL

   fwrite(buffer,404,1,outfile);   // dummy 404 bytes

#ifdef DOS
   j = maxx*8;

   for (i=0; i<head.zsize; i++) {
      base = (unsigned char *)data + (flip ? (i==3 ? 3 : 2-i) : 3 - i) + maxx*(maxy-1)*4;

      for (y=0; y<head.ysize; y++, base-=j) {
         for (x=0; x<head.xsize; x++, base+=4)
            buffer[x] = *base;

         fwrite(buffer,head.xsize, 1, outfile);
      }

   }

#else
   for (i=0; i<head.zsize; i++) {
      base = (unsigned char *)data + (flip ? (i==3 ? 3 : 2-i) : 3 - i);

      for (y=0; y<head.ysize; y++) {
         for (x=0; x<head.xsize; x++, base+=4)
            buffer[x] = *base;

         fwrite(buffer,head.xsize, 1, outfile);
      }

   }
#endif

   strcpy(filename, fname);

   fclose(outfile);
   delete [] buffer;
   return 1;
}


/* ***********************************************************************
        This procedure will write out an rgb file in RLE format
*********************************************************************** */
int rgb::write_rle(char *fname, int flip) {

   FILE *outfile;
   unsigned int i, j, k, l;
   unsigned char *buffer;
   unsigned char count;
   unsigned char ctype;
   unsigned int qtr;
   unsigned int *starttab, *lengthtab;
   int          tablem;
   unsigned char *ptr;

   if (data == (unsigned int *)NULL) {
      printf("No texture to write\n");
      return 0;
   }

   if (!(outfile = fopen(fname,"wb"))) {
      printf("Could not open %s for output as RLERGB file\n", fname);
      return 0;
   }

   putushort(outfile, MAGIC);    // MAGIC
   putuchar(outfile, RLE);       // STORAGE is RLE
   putuchar(outfile, 1);         // BPC is 1 (# of bytes per pixel component/channel (1 or 2))
   putushort(outfile, 3);        // DIMENSION is 3 - color, 2 - BW, 1 - line
   putushort(outfile, head.xsize);    // width on xaxis
   putushort(outfile, head.ysize);    // length on yaxis
   putushort(outfile, head.zsize);    // # of channels ( 1 - BW, 3 - color, 4 RGBA) zsize
   putuint(outfile, 0);         // PIXMIN is 0
   putuint(outfile, 255);       // PIXMAX is 255

   tablem = head.ysize*head.zsize;
   j = tablem<<3;

   if (404 > j)
      buffer = new unsigned char[404];
   else
      buffer = new unsigned char[j];

   fwrite(buffer,4,1,outfile);   // dummy 4 bytes

   fwrite((void *)head.name,80,1,outfile); // IMAGENAME
   putuint(outfile,NORMAL);    // COLORMAP is 0 - NORMAL

   fwrite(buffer,404,1,outfile);// dummy 404 bytes

   starttab  = new unsigned int[tablem];
   lengthtab = new unsigned int[tablem];
   starttab[0] = 512+j;
   lengthtab[0] = 0;

   fwrite(buffer,(unsigned int)j,1,outfile);  // blank tables

#ifdef DOS
   l = maxx*8;
#endif

   qtr = 0;
   for (i=0; i<head.zsize; i++) {

#ifdef DOS
      ptr = (unsigned char *)data + (flip ? ( (i==3) ? 3 : 2-i) : 3 - i) + maxx*(maxy-1)*4;
      for (j=0; j<head.ysize; j++, ptr-=l) {
#else
      ptr = (unsigned char *)data + (flip ? i : 3 - i);
      for (j=0; j<head.ysize; j++) {
#endif

         count = 1;
         buffer[0] = *ptr;
         ptr+=4;

         for (k=1; k<head.xsize; k++, ptr+=4) {
            if (count == 1) {
               if (buffer[0] == *ptr)
                  ctype = 0;
               else {
                  ctype = 1;
                  buffer[1] = *ptr;
               }

               count = 2;
            }

            else
               if (ctype)
                  if (buffer[count-1] == *ptr) {
                     count--;
                     lengthtab[qtr] += count+1;
                     ctype = count | MAXCOUNT;
                     fwrite(&ctype, 1, 1, outfile);
                     fwrite(buffer, count, 1, outfile);
                     buffer[0] = *ptr;
                     count = 2;
                     ctype = 0;
                  }

                  else {
                     if (count < MAXCOUNT-1) {
                        buffer[count] = *ptr;
                        count++;
                     }

                     else {
                        lengthtab[qtr] += count+1;
                        ctype = count | MAXCOUNT;
                        fwrite(&ctype, 1, 1, outfile);
                        fwrite(buffer, count, 1, outfile);
                        buffer[0] = *ptr;
                        count = 1;
                     }

                  }

               else
                  if (buffer[0] == *ptr)
                     if (count < MAXCOUNT-1)
                        count++;
                     else {
                        lengthtab[qtr] += 2;
                        fwrite(&count, 1, 1, outfile);
                        fwrite(buffer, 1, 1, outfile);
                        count = 1;
                     }

                  else {
                     lengthtab[qtr] += 2;
                     fwrite(&count, 1, 1, outfile);
                     fwrite(buffer, 1, 1, outfile);
                     buffer[0] = *ptr;
                     count = 1;
                  }

         }

         if (ctype) {
            lengthtab[qtr] += count+1;
            ctype = MAXCOUNT | count;
            fwrite(&ctype, 1, 1, outfile);
            fwrite(buffer, count, 1, outfile);
         }

         else {
            lengthtab[qtr] += 2;
            fwrite(&count, 1, 1, outfile);
            fwrite(buffer, 1, 1, outfile);
         }

         count = 0;
         fwrite(&count, 1, 1, outfile);
         lengthtab[qtr]++;

         if (qtr+1 < tablem) {
            starttab[qtr+1] = starttab[qtr] + lengthtab[qtr];
            qtr++;
            lengthtab[qtr] = 0;
         }

      }

   }

   fseek(outfile,512,SEEK_SET);

   for (i=0; i<tablem; i++)
      putuint(outfile, starttab[i]);

   for (i=0; i<tablem; i++)
      putuint(outfile, lengthtab[i]);

   strcpy(filename, fname);

   delete [] buffer;
   delete [] starttab;
   delete [] lengthtab;

   fclose(outfile);

   return 1;
}


/* ***********************************************************************
        This procedure will read in the header of an rgb file
*********************************************************************** */
int rgb::read_header(FILE *infile) {

   if (getushort(infile) != MAGIC)
      return 0;

   head.storage = (storagetype)getuchar(infile);
   head.bpc = getuchar(infile);
   head.dimension = getushort(infile);
   head.xsize = getushort(infile);
   head.ysize = getushort(infile);
   head.zsize = getushort(infile);

   fread((void *)head.name,1,12,infile);
   fread((void *)head.name,1,80,infile);

   head.colormap = getuint(infile);
   return 1;
}


/* ***********************************************************************
        This procedure will read in an rgb file
*********************************************************************** */
int rgb::read_data(char *fname, int flip) {

   FILE     *infile;
   unsigned int *starttab, *lengthtab;
   int i, j;
   int ptr;
   unsigned char  *charbuffer;
   unsigned short *shortbuffer;
   unsigned char  *base, *buffer, *map, *limit;
   unsigned char  ccount;

   infile = fopen(fname,"rb");

   if (!infile) {
      printf("Could not open %s for input as RGB file\n", fname);
      return 0;
   }

   if (!read_header(infile)) {
      printf("%s is not in RGB format\n", fname);
      fclose(infile);
      return 0;
   }

   fseek(infile,512,SEEK_SET);

   init_map(head.xsize, head.ysize);

   buffer = new unsigned char[head.xsize];

   if (head.storage == VERBATIM)                        // read in verbatim
      for (ptr=0; ptr<head.zsize; ptr++) {

#ifdef DOS
         base = (unsigned char *)data + (flip ? (ptr==3 ? 3 : 2-ptr) : 3 - ptr);
#else
         base = (unsigned char *)data + (flip ? ptr : 3 - ptr);
#endif

         for (i=0; i<head.ysize; i++) {
            fread(buffer,1,head.xsize,infile);
            for (j=0; j<head.xsize; j++, base+=4)
               *base = buffer[j];
         }

      }

   else {                                               // read in rle
      j = head.ysize*head.zsize;

      starttab  = new unsigned int[j];
      lengthtab = new unsigned int[j];

      for (i=0; i<j; i++)
         starttab[i] = getuint(infile);

      for (i=0; i<j; i++)
         lengthtab[i] = getuint(infile);

      if (head.bpc == 1)
         charbuffer = new unsigned char[MAXCOUNT];
      else
         shortbuffer = new unsigned short[MAXCOUNT];

      ptr = 0;
      for (i=0; i<head.zsize; i++) {
#ifdef DOS
         base = (unsigned char *)data + (flip ? 2 - i : 3 - i);
#else
         base = (unsigned char *)data + (flip ? i : 3 - i);
#endif

         for (j=0; j<head.ysize; j++, ptr++) {

            fseek(infile,starttab[ptr],SEEK_SET);

            if (head.bpc == 1)
               while (lengthtab[ptr]) {
                  fread(&ccount,1,1,infile);
                  lengthtab[ptr]--;

                  if (!ccount)
                     break;

                  if (ccount & 0x80) {
                     ccount = ccount & 0x7f;
                     fread(buffer,ccount,1,infile);
                     lengthtab[ptr] -= ccount;
                     for (map=buffer, limit = map + ccount; map < limit; map++, base+=4)
                        *base = *map;
                  }

                  else {
                     fread(buffer,1,1,infile);
                     lengthtab[ptr]--;
                     for (limit = base + (ccount<<2); base<limit; base+=4)
                        *base = *buffer;
                  }

               }

            else
               while (lengthtab[ptr]) {
                  getuchar(infile);
                  ccount = (unsigned char)getuchar(infile);
                  lengthtab[ptr]-=2;

                  if (!ccount)
                     break;

                  if (ccount & 0x80) {
                     ccount = ccount & 0x7f;
                     lengthtab[ptr]-=ccount<<1;
                     while(ccount--) {
                        *base = (unsigned char)getuchar(infile);
                        getuchar(infile);
                        base += 4;
                     }

                  }

                  else {
                     buffer[0] = (unsigned char)getuchar(infile);
                     getuchar(infile);
                     lengthtab[ptr]-=2;
                     while(ccount--) {
                        *base = *buffer;
                        base += 4;
                     }

                  }

               }

         }

      }

      delete [] starttab;
      delete [] lengthtab;

      if (head.bpc == 1)
         delete [] charbuffer;
      else
         delete [] shortbuffer;
   }

   strcpy(filename, fname);

   map = (unsigned char *)data;

   j = (head.xsize*head.ysize)<<2;
   if (head.zsize == 1)
      if (flip)
         for (i=0; i<j; i+=4)
#ifdef DOS
            map[i] = map[i+1] = map[2];
#else
            map[i+1] = map[i+2] = map[i];
#endif
      else
         for (i=1; i<j; i+=4)
            map[i] = map[i+1] = map[i+2];

/*
   for (i=((flip==1) ? 3:0); i<j; i+=4)
      map[i] = 0;
*/

   delete [] buffer;
   fclose(infile);
   return 1;
}


/* ***********************************************************************
*********************************************************************** */
int rgb::scan_header(sfile *file) {

   if (file->scan_ushort() != MAGIC)
      return 0;

   head.storage = (storagetype)file->scan_uchar();
   head.bpc = file->scan_uchar();
   head.dimension = file->scan_ushort();
   head.xsize = file->scan_ushort();
   head.ysize = file->scan_ushort();
   head.zsize = file->scan_ushort();

   file->skip(12);
   file->sread((char *)head.name, 80);

   head.colormap = file->scan_uint();
   return 1;
}


/* ***********************************************************************
*********************************************************************** */
int rgb::scan_data(sfile *file, int flip) {

   unsigned int *starttab, *lengthtab, *pstarttab, *plengthtab;
   int i, j;
   int ptr;
   unsigned char  *charbuffer;
   unsigned short *shortbuffer;
   unsigned char  *base, *buffer, *map, *limit;
   unsigned char  b;
   unsigned char  ccount;

   if (!scan_header(file)) {
      printf("%s is not in RGB format\n", file->filename);
      return 0;
   }

   file->sseek(512);

   init_map(head.xsize, head.ysize);
   buffer = new unsigned char[head.xsize];

   if (head.storage == VERBATIM)                        // read in verbatim
      for (ptr=0; ptr<head.zsize; ptr++) {

#ifdef DOS
         base = (unsigned char *)data + (flip ? (ptr==3 ? 3 : 2-ptr) : 3 - ptr);
#else
         base = (unsigned char *)data + (flip ? ptr : 3 - ptr);
#endif

         for (i=0; i<head.ysize; i++) {
            file->sread((char *)buffer, head.xsize);
            for (j=0; j<head.xsize; j++, base+=4)
               *base = buffer[j];
         }

      }

   else {                                               // read in rle
      j = head.ysize*head.zsize;

      pstarttab = starttab  = new unsigned int[j];
      plengthtab = lengthtab = new unsigned int[j];

      for (i=0; i<j; i++)
         starttab[i] = file->scan_uint();

      for (i=0; i<j; i++)
         lengthtab[i] = file->scan_uint();

      if (head.bpc == 1)
         charbuffer = new unsigned char[MAXCOUNT];
      else
         shortbuffer = new unsigned short[MAXCOUNT];

      ptr = 0;
      for (i=0; i<head.zsize; i++) {

#ifdef DOS
         base = (unsigned char *)data + (flip ? 2 - i : 3 - i);
#else
         base = (unsigned char *)data + (flip ? i : 3 - i);
#endif

         for (j=0; j<head.ysize; j++, pstarttab++, plengthtab++) {

            file->sseek(*pstarttab);

            if (head.bpc == 1)
               while (*plengthtab) {
                  ccount = file->scan_uchar();
                  *plengthtab -= 1;

                  if (!ccount)
                     break;

                  if (ccount & 0x80) {
                     ccount &= 0x7f;
                     file->sread((char *)buffer, ccount);
                     *plengthtab -= ccount;
                     for (map=buffer, limit = map + ccount; map < limit; map++, base+=4)
                        *base = *map;
                  }

                  else {
                     b = file->scan_uchar();
                     *plengthtab -= 1;
                     for (limit = base + (ccount<<2); base<limit; base+=4)
                        *base = b;
                  }

               }

            else
               while (*plengthtab) {
                  file->skip(1);
                  ccount = file->scan_uchar();
                  *plengthtab -= 2;

                  if (!ccount)
                     break;

                  if (ccount & 0x80) {
                     ccount &= 0x7f;
                     *plengthtab -= ccount<<1;

                     while (ccount--) {
                        *base = file->scan_uchar();
                        file->skip(1);
                        base += 4;
                     }

                  }

                  else {
                     b = file->scan_uchar();
                     file->skip(1);
                     *plengthtab -= 2;

                     while (ccount--) {
                        *base = b;
                        base += 4;
                     }

                  }

               }

         }

      }

      delete [] starttab;
      delete [] lengthtab;

      if (head.bpc == 1)
         delete [] charbuffer;
      else
         delete [] shortbuffer;
   }

   strcpy(filename, file->filename);

   map = (unsigned char *)data;

   j = (head.xsize*head.ysize)<<2;
   if (head.zsize == 1)
      if (flip)
         for (i=0; i<j; i+=4)
#ifdef DOS
            map[i] = map[i+1] = map[2];
#else
            map[i+1] = map[i+2] = map[i];
#endif
      else
         for (i=1; i<j; i+=4)
            map[i] = map[i+1] = map[i+2];

/*
   for (i=((flip==1) ? 3:0); i<j; i+=4)
      map[i] = 0;
*/

   delete [] buffer;

   return 1;
}


/* ***********************************************************************
*********************************************************************** */
int rgb::read_gif(char *filename) {

   int ret;
   gif *gifimage;

   gifimage = new gif;

   ret = gifimage->read_data(filename, this);

   delete gifimage;

   return ret;
}


/* ***********************************************************************
*********************************************************************** */
int rgb::read_ilbm(char *filename) {

   int ret;
   ilbm *ilbmimage;

   ilbmimage = new ilbm;

   ret = ilbmimage->read_data(filename, this);

   delete ilbmimage;

   return ret;
}


/* ***********************************************************************
*********************************************************************** */
int rgb::read_jpg(char *filename) {

   jpeg jpegimage;

   return jpegimage.read_data(filename, this);
}


/* ***********************************************************************
*********************************************************************** */
int rgb::read_tiff(char *filename) {

   tiff tiffimage;

   return tiffimage.read_data(filename, this);
}


/* ***********************************************************************
*********************************************************************** */
int rgb::read_ppm(char *filename) {

   ppm *ppmimage;
   int ret;

   ppmimage = new ppm;

   ret = ppmimage->read_data(filename, this);

   delete ppmimage;

   return ret;
}


/* ***********************************************************************
*********************************************************************** */
int rgb::write_ppm(char *filename, int flip) {

   ppm *ppmimage;
   int ret;

   ppmimage = new ppm;

   ret = ppmimage->write_data(filename, this, flip);

   delete ppmimage;

   return ret;
}


/* ***********************************************************************
        This procedure will return a texel in the image
*********************************************************************** */
void rgb::query_coord(float x[][2]) {

   x[1][0] = x[0][0] = 0.0;
   x[3][1] = x[0][1] = 0.0;
   x[2][1] = x[1][1] = (float)(head.ysize-1);
   x[3][0] = x[2][0] = (float)(head.xsize-1);
}

