

#include <string.h>
#include "texture.h"

/* Define internal ILBM types */
#define ILBM_NORMAL     0
#define ILBM_EHB        1
#define ILBM_HAM        2
#define ILBM_HAM8       3
#define ILBM_24BIT      4


typedef unsigned char ilbm_colortype[3];

int ilbm::read_data(char *fname, rgb *image) {

   FILE *infile;
   int i, j, k;
   char buffer[MAXSTRLEN];
   unsigned char *cbuffer;
   char bmhdflag = 0, camgflag = 0;

   unsigned int depth, mask, compression;
   unsigned int viewmode;

   int bytesize, chunksize;

   ilbm_colortype *cmap = (ilbm_colortype *)NULL;
   ilbm_colortype *icptr;
   int colorcount, memcount;

   char format;

   unsigned int lineskip;
   unsigned char bitmask;

   unsigned char curr[3];
   unsigned char *pic;
   unsigned char *ucptr, *ucptr2, *ucptr3;
   unsigned int ultemp;
   int col, colbit;

   if (!(infile = fopen(fname,"r"))) {
      printf("Could not open %s for input as ilbm file\n", fname);
      return 0;
   }

   for (i=0; i<4; i++)
      buffer[i] = (char)getuchar(infile);
   buffer[4] = 0;

   if (strcmp(buffer, "FORM")) {
      printf("%s is not in Lightwave format... Aborting...\n", fname);
      fclose(infile);
      return 0;
   }

   bytesize = getuint(infile);

   for (i=0; i<4; i++)
      buffer[i] = (char)getuchar(infile);
   buffer[4] = 0;
   bytesize -= 4;

   if (strcmp(buffer, "ILBM")) {
      printf("%s is not a ILBM chunk... Aborting...\n", fname);
      fclose(infile);
      return 0;
   }

   while (bytesize > 0) {

      buffer[0] = (char)getuchar(infile);
      buffer[1] = (char)getuchar(infile);
      buffer[2] = (char)getuchar(infile);
      buffer[3] = (char)getuchar(infile);
      buffer[4] = 0;

      chunksize = getuint(infile);
      bytesize -= 8 + chunksize;

      if (bytesize < 0) {
         printf("ERROR - Reading past bytesize... Aborting...\n");
         fclose(infile);
         if (cmap != (ilbm_colortype *)NULL)
            delete [] cmap;
         return 0;
      }

      if (!strcmp(buffer, "BMHD")) {
         printf("BMHD...\n");

         bmhdflag = 1;

         image->head.xsize = getushort(infile);
         image->head.ysize = getushort(infile);
         image->head.zsize = 3;
         image->init_map(image->head.xsize, image->head.ysize);

         getuint(infile);               // dunno what this is...

         depth = getuchar(infile);
         mask = getuchar(infile);
         compression = getuchar(infile);

         getuchar(infile);              // dunno what this is...

         getushort(infile);             // transcol
         chunksize -= 14;

         if (chunksize) {
            printf("Warning... ignoring %d bytes in \"BMHD\"\n", chunksize);
            fread(buffer, 1, chunksize, infile);
         }

      }

      else if (!strcmp(buffer, "CMAP")) {
         printf("CMAP...\n");

         memcount = colorcount = chunksize / 3;
         if (cmap != (ilbm_colortype *)NULL)
            delete [] cmap;

         cmap = new ilbm_colortype[colorcount];

         for (i=0; i<colorcount; i++) {
            cmap[i][0] = (char)getuchar(infile);              // r
            cmap[i][1] = (char)getuchar(infile);              // g
            cmap[i][2] = (char)getuchar(infile);              // b
         }

      }

      else if (!strcmp(buffer, "CAMG")) {
         printf("CAMG...\n");
         camgflag = 1;
         viewmode = getuint(infile);
      }

      else if (!strcmp(buffer, "BODY")) {
         printf("BODY...\n");

         if (cmap == (ilbm_colortype *)NULL) {                          // BMHD found?
            printf("BODY detected before CMAP... Abort input %s\n", fname);
            fclose(infile);

            return 0;
         }

         if (!bmhdflag) {                               // BMHD found?
            printf("BODY detected before BMHD... Abort input %s\n", fname);
            fclose(infile);

            delete [] cmap;
            return 0;
         }

         if (bytesize > 0)
            printf("WARNING: %d unread bytes...\n", bytesize);

         if (compression == 1) {                // rle compressed
            printf("decompressing RLE...\n");
            i = (((image->head.xsize + 15)>>4)<<1)*image->head.ysize*depth; // uncompressed size
            cbuffer = new unsigned char[i];

            if (decomprle(infile, cbuffer, i) == -1) {
               printf("Error - ILBM-RLE chunk corrupt...\n");
               fclose(infile);

               delete [] cmap;
               delete [] cbuffer;

               return 0;
            }

         }

         else {
            printf("no compression...\n");
            cbuffer = new unsigned char[chunksize];
            for (i=0; i<chunksize; i++)
               cbuffer[i] = (char)getuchar(infile);
         }

         printf("input complete...\n");

         /* the following determines the type of the ILBM file.
           it's either NORMAL, EHB, HAM, HAM8 or 24BIT */

         format = ILBM_NORMAL;                        /* assume normal ILBM */

         if (depth == 24) {
            printf("24 bit...\n");
            format = ILBM_24BIT;
         }

         if (camgflag)
            if (depth == 8) {
               if (viewmode & 0x800) {
                  printf("HAM8...\n");
                  format = ILBM_HAM8;
               }

            }

            else
               if (depth > 5) {
                  if (viewmode & 0x80) {
                     printf("EHB...\n");
                     format = ILBM_EHB;
                  }

                  else
                     if (viewmode & 0x800) {
                        printf("HAM...\n");
                        format = ILBM_HAM;
                     }

               }

         if (format != ILBM_24BIT || format != ILBM_HAM8) {
            switch(format) {
               case ILBM_NORMAL:
                  printf("NORMAL...\n");
                  if (colorcount < (i = (1<<depth))) {
                     printf("CMAP depth < format... Aborting...\n");
                     fclose(infile);

                     delete [] cmap;
                     delete [] cbuffer;
                     return 0;
                  }

                  colorcount =  i;

                  break;

               case ILBM_EHB:
                  if (colorcount < 32) {
                     printf("CMAP depth < format... Aborting...\n");
                     fclose(infile);

                     delete [] cmap;
                     delete [] cbuffer;
                     return 0;
                  }

                  colorcount = 32;
                  break;

               case ILBM_HAM:
                  if (colorcount < 16) {
                     printf("CMAP depth < format... Aborting...\n");
                     fclose(infile);

                     delete [] cmap;
                     delete [] cbuffer;
                     return 0;
                  }

                  colorcount = 16;
                  break;
            }

            for (i=0; i<colorcount; i++) {
               cmap[i][0] = (cmap[i][0]>>4)*17;
               cmap[i][1] = (cmap[i][1]>>4)*17;
               cmap[i][2] = (cmap[i][2]>>4)*17;
            }

         }

         ucptr = cbuffer;
         lineskip = ((image->head.xsize + 15) >> 4) << 1; // # of bytes/line
         ultemp = lineskip * depth;

         if (format != ILBM_NORMAL && format != ILBM_EHB) {

            for (i=0; i<image->head.ysize; i++) {
               pic = (unsigned char *)image->pdata[image->head.ysize-(1+i)];
               bitmask = 0x80;
               ucptr2 = ucptr;

               // at start of each line, init RGB values to background
               curr[0] = cmap[0][0];
               curr[1] = cmap[0][1];
               curr[2] = cmap[0][2];

               for (j=0; j<image->head.xsize; j++) {
                  col = 0;
                  colbit = 1;
                  ucptr3 = ucptr2;

                  for (k=0; k<depth; k++) {
                     if (*ucptr3 & bitmask)
                        col += colbit;

                     ucptr3 += lineskip;
                     colbit <<= 1;
                  }

                  if (format==ILBM_HAM)
                     switch (col & 0x30) {
                        case 0x00:
                           k = col & 0x0f;
                           curr[0] = cmap[k][0];
                           curr[1] = cmap[k][1];
                           curr[2] = cmap[k][2];
                           break;

                        case 0x10:
                           curr[2] = (col & 0x0f) * 17;
                           break;

                        case 0x20:
                           curr[0] = (col & 0x0f) * 17;
                           break;

                        case 0x30:
                           curr[1] = (col & 0x0f) * 17;
                     }

                  else if (format == ILBM_HAM8)
                     switch(col & 0xc0) {
                        case 0x00:
                           k = col & 0x3f;
                           curr[0] = cmap[k][0];
                           curr[1] = cmap[k][1];
                           curr[2] = cmap[k][2];
                           break;

                        case 0x40:
                           curr[2] = (curr[2] & 3) | ((col & 0x3f) << 2);
                           break;

                        case 0x80:
                           curr[0] = (curr[0] & 3) | ((col & 0x3f) << 2);
                           break;

                        case 0xc0:

                           // next line - XV BUG??? (green based on red???)
                           // curr[1] = (curr[0] & 3) | ((col & 0x3f) << 2);
                           curr[1] = (curr[1] & 3) | ((col & 0x3f) << 2);
                     }

                  else {
                     curr[0] = col & 0xff;
                     curr[1] = (col >> 8) & 0xff;
                     curr[2] = (col >> 16) & 0xff;
                  }

                  *pic++;
                  *pic++ = curr[2];
                  *pic++ = curr[1];
                  *pic++ = curr[0];

                  bitmask = bitmask >> 1;
                  if (!bitmask) {
                     bitmask = 0x80;
                     ucptr2++;
                  }

               }

               ucptr += ultemp;
            }

         }  /* HAM, HAM8, or 24BIT */

         else {
            printf("8-bit ILBM...\n");

            if (format == ILBM_EHB) {           // double cmap for EHB mode
               if (colorcount + 32 > memcount) {
                  icptr = cmap;
                  memcount = colorcount + 32;
                  cmap = new ilbm_colortype[memcount];
                  memcpy(cmap, icptr, colorcount*3);

                  delete [] icptr;
               }

               for (i=0; i<32; i++) {
                  cmap[i+colorcount][0] = cmap[i][0]>>1;
                  cmap[i+colorcount][1] = cmap[i][1]>>1;
                  cmap[i+colorcount][2] = cmap[i][2]>>1;
               }

            }

            for (i=0; i<image->head.ysize; i++) {
               pic = (unsigned char *)image->pdata[image->head.ysize-(1+i)];
               bitmask = 0x80;                      /* left most bit (mask) */
               ucptr2 = ucptr;                 /* work ptr to source */

               for (j=0; j<image->head.xsize; j++) {
                  col = 0;
                  colbit = 1;
                  ucptr3 = ucptr2;              /* ptr to byte in 1st pln */

                  for (k=0; k<depth; k++) {
                     if (*ucptr3 & bitmask)          /* if bit set in this pln */
                        col = col + colbit;           /* add bit to chunky byte */
                     ucptr3 += lineskip;           /* go to next line */
                     colbit <<= 1;                   /* shift color bit */
                  }

//                  *pic++ = col;                     /* write to chunky buffer */
                  *pic++;
                  *pic++ = cmap[col][2];
                  *pic++ = cmap[col][1];
                  *pic++ = cmap[col][0];

                  bitmask = bitmask >> 1;             /* shift mask to next bit */
                  if (!bitmask) {                /* if mask is zero */
                     bitmask = 0x80;                  /* reset mask */
                     ucptr2++;                     /* mv ptr to next byte */
                  }

               }  /* for j ... */

               ucptr += ultemp;
            }  /* for i ... */

         }  /* if NORMAL or EHB */

         break;
      }

      else { // skip - unknown...
         printf("Unknown chunk %s... skipping chunk...\n", buffer);

         for (i=0; i<chunksize; i++)
            getuchar(infile);
      }

   }

   if (cmap != (ilbm_colortype *)NULL)
      delete [] cmap;

   if (cbuffer != (unsigned char *)NULL)
      delete [] cbuffer;

   return 1;
}





/**************************************************************************
  void decomprle(source, destination, source length, buffer size)

  Decompress run-length encoded data from source to destination. Terminates
  when source is decoded completely or destination buffer is full.

  The decruncher is as optimized as I could make it, without risking
  safety in case of corrupt BODY chunks.
***************************************************************************/


/*******************************************/
int ilbm::decomprle(FILE *infile, unsigned char *dest, int size) {

   int bytecount = 0, count = 0;
   unsigned char len, temp;
   int index = 0;
   int i;

   while (count < size) {

      /* read control byte */
      len = (char)getuchar(infile);
      bytecount++;

      if (len == 0x80)
         return bytecount;      // done ????

      else if (!(len & 0x80)) {
         len++;
         bytecount += len;
         count += len;

         if (count > size)
            break;

         for (i=index+len; index<i; index++)
            dest[index] = (char)getuchar(infile);
      }

      else {
         len = 0x81 - (len & 0x7f);
         bytecount++;
         count += len;

         if (count > size)
            break;

         temp = (char)getuchar(infile);

         for (i=index+len; index<i; index++)
            dest[index] = temp;
      }

   }

   if (count > size)
      return -1;

   return bytecount;
}

