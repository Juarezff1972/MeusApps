



#include "tachyon.h"


/* *************************************************************
************************************************************* */
int tachyon::parse(FILE *infile, char *token) {


   switch (token[0]) {

      case 'd':
         if (!strcmp(token, "death")) {
            get_token(infile, token);
            death = atof(token);
            return 1;
         }

         if (!strcmp(token, "decay")) {
            get_token(infile, token);
            decay = 1.0 - atof(token);
            return 1;
         }

         break;

      case 'i':

         if (!strcmp(token, "intensity")) {
            get_token(infile, token);
            intensity = atof(token);
            return 1;
         }

         break;

      case 's':
         if (!strcmp(token, "shade")) {
            get_token(infile, token);
            lower_case(token);

            switch (token[0]) {
               case 'w':
                  if (!strcmp(token, "wfbw")) {
                     strcpy(obtype, "WFBW");
                     return 1;
                  }

                  return 0;

               case 'g':
                  if (!strcmp(token, "gouraud")) {
                     strcpy(obtype, "ALEX");

                     get_token(infile, token);
                     color[0] = atof(token);
                     get_token(infile, token);
                     color[1] = atof(token);
                     get_token(infile, token);
                     color[2] = atof(token);

                     return 1;
                  }

                  return 0;

               default:
                  break;
            }

            return 0;
         }

         break;

      default:
         break;
   }

   return quark::parse(infile, token);
}


/* *************************************************************
   place object based on local and parent xforms
************************************************************* */
void tachyon::whereami(int frameno, molecule **mptr, int *obcount, int wflag,
			quark *actors, quark *parent, vector4f *mx) {

   linktype *ptr;			// pointer
   bptachyon *bptr;
   vector4f r[4];		// local xform mx

   calc_initmx(mx, r);

   for (ptr=edge; ptr!=(linktype *)NULL; ptr=ptr->next)  // apply xform to
      if (ptr->link != parent)                           //    children
         ptr->link->whereami(frameno, mptr, obcount, wflag, actors, this, r);

   if (intensity > death) {
      copymx4x4(xmx, localmx);
      matmatmulto(r, xmx);

      center[0] = xmx[0][3];
      center[1] = xmx[1][3];
      center[2] = xmx[2][3];

      if (active && wflag) {
         (*obcount)++;

         bptr = new bptachyon;

         bptr->ob = this;
         bptr->frameno = frameno;

         copyarray4(bptr->location, center);
         bptr->intensity = intensity;

         bptr->next = *mptr;
          *mptr = bptr;
      }

      intensity *= decay;
   }

}

