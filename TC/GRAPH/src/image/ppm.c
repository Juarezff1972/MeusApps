

#include <string.h>
#include <stdlib.h>
#include "texture.h"


/* ***************************************************************
   P6 hor(x)(col)count ver(y)(row)count 255(1 byte) top to bottom
*************************************************************** */
int ppm::read_data(char *fname, rgb *image) {

   char buffer[MAXSTRLEN], magic[MAXSTRLEN];
   int  bsize;
   FILE *infile;
   int  i, j;
   int flag = 0;

   if (!(infile = fopen(fname,"r"))) {
      printf("Could not open %s for input as ppm file\n", fname);
      return 0;
   }

   while (flag < 4) {

      fscanf(infile, "%255s", buffer);

      if (buffer[0] == '#')
         while (getuchar(infile) != '\n');
      else {
         switch(flag) {

            case 0:
               strcpy(magic, buffer);
               break;

            case 1:
               xsize = atoi(buffer);
               break;

            case 2:
               ysize = atoi(buffer);
               break;

            case 3:
               bsize = atoi(buffer);
               break;
         }

         flag++;
      }

   }

   if (strcmp(magic, "P6")) {
      printf("%s is not in .ppm format\n", fname);
      fclose(infile);
      return 0;
   }

   if (bsize != 255) {
      printf("Byte byte!=255... Abort read ppm...\n");
      fclose(infile);
      return 0;
   }

   while (getuchar(infile) != '\n');

   image->head.xsize = xsize;
   image->head.ysize = ysize;
   image->head.zsize = 3;
   image->init_map(xsize, ysize);

   for (i=ysize-1; i>=0; i--)
      for (j=0; j < xsize; j++) {
         ((unsigned char *)(&image->pdata[i][j]))[3] = (unsigned char)getuchar(infile);
         ((unsigned char *)(&image->pdata[i][j]))[2] = (unsigned char)getuchar(infile);
         ((unsigned char *)(&image->pdata[i][j]))[1] = (unsigned char)getuchar(infile);
         ((unsigned char *)(&image->pdata[i][j]))[0] = 0;
      }

   fclose(infile);
   return 1;
}


int ppm::write_data(char *fname, rgb *image, int flip) {

   FILE *outfile;
   int  i, j;

   if (!(outfile = fopen(fname,"w"))) {
      printf("Could not open %s for output as ppm file\n", fname);
      return 0;
   }

   fprintf(outfile, "P6 %d %d 255\n", image->head.xsize, image->head.ysize);

   if (flip)
      for (i=image->head.ysize-1; i>=0; i--)
         for (j=0; j < image->head.xsize; j++) {
            fwrite(&((unsigned char *)(&image->pdata[i][j]))[0], 1, 1, outfile);
            fwrite(&((unsigned char *)(&image->pdata[i][j]))[1], 1, 1, outfile);
            fwrite(&((unsigned char *)(&image->pdata[i][j]))[2], 1, 1, outfile);
         }

   else
      for (i=image->head.ysize-1; i>=0; i--)
         for (j=0; j < image->head.xsize; j++) {
            fwrite(&((unsigned char *)(&image->pdata[i][j]))[3], 1, 1, outfile);
            fwrite(&((unsigned char *)(&image->pdata[i][j]))[2], 1, 1, outfile);
            fwrite(&((unsigned char *)(&image->pdata[i][j]))[1], 1, 1, outfile);
         }

   fclose(outfile);
   return 1;
}

