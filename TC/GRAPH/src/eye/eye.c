



/* *************************************************************
   this file contains all functions to do transforms
************************************************************* */

#include <stdlib.h>
#include <string.h>
#include "eye.h"

/* *************************************************************
   This procedure calculates the Nper matrix
************************************************************* */
void eye::nper() {

   float r[4][4];

   init_mx(transform);

   transform[0][3] = -location[0];
   transform[1][3] = -location[1];
   transform[2][3] = -location[2];

   normalize3(vpn);

   copyarray3(r[2], vpn);
   r[2][3] = 0;

   xproduct(r[0], vup, r[2]);
   r[0][3] = 0;

   normalize3(r[0]);

   xproduct(r[1], r[2], r[0]);
   r[1][3] = 0;

   normalize3(r[1]);

   r[3][0] = r[3][1] = r[3][2] = 0;
   r[3][3] = 1;

   matmatmulto(r, transform);

   inversemxrt(transform, Tinverse);            // inverse transformation
}


/* *************************************************************
************************************************************* */
int eye::parse(FILE *infile, char *token) {

   switch(token[0]) {

      case 'l':
         if (!strcmp(token, "location")) {
            get_token(infile, token);
            location[0] = atof(token);
            get_token(infile, token);
            location[1] = atof(token);
            get_token(infile, token);
            location[2] = atof(token);

            return 1;
         }

         break;

      case 'u':
         if (!strcmp(token, "up")) {
            get_token(infile, token);
            vup[0] = atof(token);
            get_token(infile, token);
            vup[1] = atof(token);
            get_token(infile, token);
            vup[2] = atof(token);

            return 1;
         }

         break;

      default:
         break;
   }

   return superclass::parse(infile, token);
}

