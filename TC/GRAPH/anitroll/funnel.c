



#include "funnel.h"


/* ********************************************************
   This function reads in the data for this child
******************************************************** */
int funnel::parse(FILE *infile, char *token) {

   switch (token[0]) {

      case 'b':
         if (!strcmp(token, "bottom_radius")) {
            get_token(infile, token);
            radius[0] = atof(token);
            return 1;
         }

         break;

      case 'l':
         if (!strcmp(token, "length")) {
            get_token(infile, token);
            length = atof(token);
            return 1;
         }

         break;

      case 't':
         if (!strcmp(token, "top_radius")) {
            get_token(infile, token);
            radius[0] = atof(token);
            return 1;
         }

         break;

      default:
         break;
   }

   return neutron::parse(infile, token);
}

