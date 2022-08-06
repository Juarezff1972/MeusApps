



#include "chain.h"


/* *************************************************************
************************************************************* */
int chain::parse(FILE *infile, char *token) {


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

      case 'e':
         if (!strcmp(token, "end_point")) {
            get_token(infile, token);
            endpt[0] = atof(token);
            get_token(infile, token);
            endpt[1] = atof(token);
            get_token(infile, token);
            endpt[2] = atof(token);

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
                     strcpy(obtype, "GOURAUD");

                     get_token(infile, token);
                     color[0][0] = atof(token);
                     get_token(infile, token);
                     color[0][1] = atof(token);
                     get_token(infile, token);
                     color[0][2] = atof(token);
                     get_token(infile, token);
                     color[1][0] = atof(token);
                     get_token(infile, token);
                     color[1][1] = atof(token);
                     get_token(infile, token);
                     color[1][2] = atof(token);

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
void chain::whereami(int frameno, molecule **mptr, int *obcount, int wflag,
			quark *actors, quark *parent, vector4f *mx) {

   linktype *ptr;			// pointer
   bpchain *bptr;
   vector4f r[4];			// local xform mx
   vector4f temp;

   calc_initmx(mx, r);

   for (ptr=edge; ptr!=(linktype *)NULL; ptr=ptr->next)  // apply xform to
      if (ptr->link != parent)                           //    children
         ptr->link->whereami(frameno, mptr, obcount, wflag, actors, this, r);

   if (intensity > death) {
      copymx4x4(xmx, localmx)
      matmatmulto(r, xmx);

      if (active && wflag) {
         (*obcount)++;

         bptr = new bpchain;

         bptr->ob = this;
         bptr->frameno = frameno;

         bptr->location[0] = xmx[0][3];
         bptr->location[1] = xmx[1][3];
         bptr->location[2] = xmx[2][3];

         copyarray4(bptr->endpt, endpt);
         subarray3(bptr->endpt, origin);
         matvecmulto(xmx, bptr->endpt);

         copyarray3(center, bptr->location);
         addarray3(center, bptr->endpt);
         smultarray3(center, 0.5);

         bptr->intensity = intensity;

         bptr->next = *mptr;
         *mptr = bptr;
      }

      else {

         center[0] = xmx[0][3];
         center[1] = xmx[1][3];
         center[2] = xmx[2][3];

         copyarray4(temp, endpt);
         subarray3(temp, origin);
         matvecmulto(xmx, temp);
         addarray3(center, temp);
         smultarray3(center, 0.5);
      }

      intensity *= decay;
   }

}

