

#include <string.h>
#include "shading.h"

/* *************************************************************
   This file contains all the global data vars for my programs
************************************************************* */


shadetype::shadetype() {

   ka[0] = ka[1] = ka[2] = 1.0;
   kp[0] = kp[1] = kp[2] = 0.7;
   ks[0] = ks[1] = ks[2] = 0.3;
   specn = 1;
   lum[0] = lum[1] = lum[2] = 0.0;
}


void shadetype::read_data(FILE *infile) {

   char junk[MAXSTRLEN];

   fscanf(infile, "%s %f %f %f", junk, &ka[0], &ka[1], &ka[2]);
   fscanf(infile, "%s %f %f %f", junk, &kp[0], &kp[1], &kp[2]);
   fscanf(infile, "%s %f %f %f", junk, &ks[0], &ks[1], &ks[2]);
   fscanf(infile, "%s %f", junk, &specn);
   fscanf(infile, "%s %f %f %f", junk, &lum[0], &lum[1], &lum[2]);
   smultarray3(ka, 255);
   smultarray3(kp, 255);
   smultarray3(ks, 255);
   smultarray3(lum, 255);
}


void shadetype::write_data(FILE *outfile) {

   fprintf(outfile, "KA %f %f %f\n", ka[0]/255, ka[1]/255, ka[2]/255);
   fprintf(outfile, "KP %f %f %f\n", kp[0]/255, kp[1]/255, kp[2]/255);
   fprintf(outfile, "KS %f %f %f\n", ks[0]/255, ks[1]/255, ks[2]/255);
   fprintf(outfile, "SPECN %f\n", specn);
   fprintf(outfile, "LUM %f %f %f\n\n", lum[0]/255, lum[1]/255, lum[2]/255);
}

/* ***************************************************************
*************************************************************** */

illtype illtype::operator = (shadetype *x) {

   color = x;
   ambient[0] = ambient[1] = ambient[2] = 0;

   return *this;
}


texcolortype texcolortype::operator = (illtype &x) {

   color = *x.color;
   copyarray3(ambient, x.ambient);

   return *this;
}


texcolortype texcolortype::operator = (shadetype &x) {

   color = x;
   ambient[0] = ambient[1] = ambient[2] = 0;

   return *this;
}


int shadelist::read_data(char *filename) {

   FILE *infile;                        // file pointer
   char junk[MAXSTRLEN];
   int i;

   strcpy(shadename, filename);

   if (!(infile = fopen(filename, "r"))) {
      printf("Couldn't open %s\n", filename);
      return 0;
   }

   base.read_data(infile);

   fscanf(infile, "%s %d", junk, &count);

   if (list != (shadetype *)NULL)
      delete [] list;

   list = new shadetype[count];

   for (i=0; i<count; i++) {
      fscanf(infile, "%s %d", junk, &list[i].index);
      list[i].index--;
      list[i].read_data(infile);
   }

   fclose(infile);
   return 1;
}


int shadelist::write_data(char *filename) {

   FILE *outfile;                        // file pointer
   int i;

   if (!(outfile = fopen(filename, "w"))) {
      printf("Couldn't open %s for output\n", filename);
      return 0;
   }

   base.write_data(outfile);

   fprintf(outfile, "no_sides %d\n\n", count);

   for (i=0; i<count; i++) {
      fprintf(outfile, "# %d\n", i+1);
      list[i].write_data(outfile);
   }

   fclose(outfile);
   return 1;
}

