



#include "tube.h"


/* ********************************************************
   This function reads in the data for this child
******************************************************** */
int tube::parse(FILE *infile, char *token) {

   switch (token[0]) {

      case 'l':
         if (!strcmp(token, "length")) {
            get_token(infile, token);
            length = atof(token);
            return 1;
         }

         break;

      case 'r':
         if (!strcmp(token, "radius")) {
            get_token(infile, token);
            radius = atof(token);
            return 1;
         }

         break;

      default:
         break;
   }

   return neutron::parse(infile, token);
}

