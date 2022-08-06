



/* *************************************************************
   this file contains all functions to do transforms
************************************************************* */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "camera.h"


/* *************************************************************
************************************************************* */
int camera::parse(FILE *infile, char *token) {

   switch (token[0]) {

      case 'c':
         if (!strcmp(token, "coa")) {
            get_token(infile, token);
            coa[0] = atof(token);
            get_token(infile, token);
            coa[1] = atof(token);
            get_token(infile, token);
            coa[2] = atof(token);

            return 1;
         }

         break;

      default:
         break;
   }

   return eye::parse(infile, token);
}


/* *************************************************************
************************************************************* */
void camera::preprocess(void *data) {

   int i;

   eye::preprocess(data);

   for (i=0; i<3; i++)
      vpn[i] = location[i] - coa[i];

   imscale = 1.0/mscale;
   nper();
}


/* *************************************************************
************************************************************* */
camera::camera(int x, int y) : eye() {

   location[0] = 0;
   location[1] = 0;
   location[2] = 4;

   coa[0] = coa[1] = coa[2] = 0;

   vup[0] = 0;
   vup[1] = 1;
   vup[2] = 0;

   vrc[0] =  -(vrc[1] = (x/((float)(y + y))));
   vrc[2] = -0.5;
   vrc[3] =  0.5; 
 
   mscale = y;
}


/* *************************************************************
************************************************************* */
void camera::read_def(FILE *infile, unsigned int *x, unsigned int *y) {

   float distance2plane;               // distance to view plane
   float wwidth;                       // width of viewplane
   float temp[2];
   float ffactor;

   fscanf(infile, "%f %f %f", &location[0], &location[1], &location[2]);
   fscanf(infile, "%f %f %f", &coa[0], &coa[1], &coa[2]);
   fscanf(infile, "%f %f %f", &vup[0], &vup[1], &vup[2]);
   fscanf(infile, "%f", &distance2plane);
   fscanf(infile, "%f", &wwidth);

   /* calculate viewing parameters */
   ffactor = *x/(float)(*y);

   wwidth /= distance2plane;
   temp[1] = 0.5*wwidth;
   temp[0] = temp[1]*ffactor;	// temp*4/3

   vrc[0] = -temp[0];
   vrc[1] =  temp[0];
   vrc[2] = -temp[1];
   vrc[3] =  temp[1];

   mscale = *y/wwidth;

   *y = (int)(wwidth * mscale);
   *x = (int)(*y*ffactor);

   preprocess(NULL);
}


void camera::write_lst(FILE *outfile, int frameno) {

   fprintf(outfile, "%d\nCAMERA\n", frameno);
   fprintf(outfile, "%f %f %f\n", location[0], location[1], location[2]);
   fprintf(outfile, "%f %f %f\n", coa[0], coa[1], coa[2]);
   fprintf(outfile, "%f %f %f\n", vup[0], vup[1], vup[2]);
}


/* *************************************************************
  This procedure reads and calculates the viewing parameters
************************************************************* */
void camera::read_data(FILE *infile) {

   fscanf(infile, "%f %f %f", &location[0], &location[1], &location[2]);
   fscanf(infile, "%f %f %f", &coa[0], &coa[1], &coa[2]);
   fscanf(infile, "%f %f %f", &vup[0], &vup[1], &vup[2]);

   /* calculate viewing parameters */

   preprocess(NULL);
}


/* *************************************************************
************************************************************* */
void camera::dump() {

   int i;

   printf("%f %f %d\n", mscale, imscale, frame);
   printf("\n %f %f %f\n", location[0], location[1], location[2]);
   printf("%f %f %f\n", coa[0], coa[1], coa[2]);
   printf("%f %f %f\n", vup[0], vup[1], vup[2]);
   printf("%f %f %f\n", vpn[0], vpn[1], vpn[2]);
   printf("%f %f %f %f\n", vrc[0], vrc[1], vrc[2], vrc[3]);
   print_mx(i, transform);
   print_mx(i, Tinverse);
   printf("\n");
}

