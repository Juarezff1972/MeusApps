



#include <stdio.h>
#include "tiffio.h"
#include "texture.h"


int tiff::read_data(char *fname, rgb *image) {

   TIFF *tif;
   unsigned int y, x;
   int i;
   unsigned char *j, *k;
   int l, m;

   if (!(tif = TIFFOpen(fname, "r"))) {
      printf("Could not open %s for input as TIFF file\n", fname);
      return 0;
   }

   TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &y);
   TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &x);

   if (x*3 != TIFFScanlineSize(tif)) {
      printf("%s is not a 24 bit TIFF file\n", fname);
      return 0;
   }

   image->head.xsize = (unsigned short)x;
   image->head.ysize = (unsigned short)y;
   image->head.zsize = 3;
   image->init_map(x, y);

   for (m=0, i=y-1; m<y; m++, i--) {
      TIFFReadScanline(tif, image->pdata[i], m);

      for (l=0, j=(unsigned char *)image->pdata[i] + 3*(x-1), k=(unsigned char *)(&image->pdata[i][x-1]);
           l<x; l++, j-=3, k-=4) {

         k[3] = j[0];
         k[2] = j[1];
         k[1] = j[2];
         k[0] = 0;
      }

   }

   TIFFClose(tif);

   return 1;
}

