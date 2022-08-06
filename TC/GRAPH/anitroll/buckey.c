



#include "buckey.h"


/* ********************************************************
   This function reads in the data for this child
******************************************************** */
int buckey::parse(FILE *infile, char *token) {


   switch(token[0]) {
      case 'a':
         if (!strcmp(token, "axis")) {
            get_token(infile, token);
            axis[0] = atof(token);
            get_token(infile, token);
            axis[1] = atof(token);
            get_token(infile, token);
            axis[2] = atof(token);
            return 1;
         }

         break;

      case 'r':
         if (!strcmp(token, "radius")) {
            get_token(infile, token);
            axis[0] = axis[1] = axis[2] = atof(token);
            return 1;
         }

         break;

      default:
         break;
   }

   return neutron::parse(infile, token);
}
