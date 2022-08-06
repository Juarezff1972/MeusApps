
#include <stdio.h>
#include "texture.h"
#include "jpeglib.h"


int jpeg::read_data(char *fname, rgb *image) {

   /* This struct contains the JPEG decompression parameters and pointers to
    * working space (which is allocated as needed by the JPEG library).
    */
   struct jpeg_decompress_struct cinfo;

   FILE * infile;               /* source file */
   struct jpeg_error_mgr jerr;
   int    i;
   unsigned char *j, *k;
   int l;

   if ((infile = fopen(fname, "rb")) == NULL) {
      printf("Could not open %s for input as JPEG file\n", fname);
      return 0;
   }

   /* Step 1: allocate and initialize JPEG decompression object */
   cinfo.err = jpeg_std_error(&jerr);   // We set up the normal JPEG error routines
   jpeg_create_decompress(&cinfo);      // Now we can initialize the JPEG decompression object.

   /* Step 2: specify data source (eg, a file) */
   jpeg_stdio_src(&cinfo, infile);

   if (!jpeg_read_header(&cinfo)) {
      printf("%s is not in JPEG format\n", fname);
      fclose(infile);
      return 0;
   }

   jpeg_start_decompress(&cinfo);

   image->head.xsize = (unsigned short)cinfo.output_width;
   image->head.ysize = (unsigned short)cinfo.output_height;
   image->head.zsize = 3;
   image->init_map(cinfo.output_width, cinfo.output_height);

   /* Step 6: while (scan lines remain to be read) */
   /*           jpeg_read_scanlines(...); */

   if (cinfo.out_color_space == JCS_GRAYSCALE) {
      for (i=cinfo.output_height-1; i > -1; i--) {
         jpeg_read_scanlines(&cinfo, (unsigned char **)(&image->pdata[i]), 1);

         j=(unsigned char *)image->pdata[i] + (cinfo.output_width-1);
         k=(unsigned char *)(&image->pdata[i][cinfo.output_width-1]);
         for (l=0; l < cinfo.output_width; l++, j--, k-=4) {
            k[3] = k[2] = k[1] = *j;
            k[0] = 0;
         }

      }
   }

   else {
      for (i=cinfo.output_height-1; i > -1; i--) {
         jpeg_read_scanlines(&cinfo, (unsigned char **)(&image->pdata[i]), 1);

         j=(unsigned char *)image->pdata[i] + 3*(cinfo.output_width-1);
         k=(unsigned char *)(&image->pdata[i][cinfo.output_width-1]);
         for (l=0; l < cinfo.output_width; l++, j-=3, k-=4) {
            k[3] = j[0];
            k[2] = j[1];
            k[1] = j[2];
            k[0] = 0;
         }

      }

   }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  fclose(infile);
  return 1;
}

