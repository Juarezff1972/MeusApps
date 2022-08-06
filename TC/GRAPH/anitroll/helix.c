



#include "helix.h"


/* ********************************************************
   This function reads in the data for this child
******************************************************** */
int helix::parse(FILE *infile, char *token) {

   switch (token[0]) {
      case 'b':
         if (!strcmp(token, "back")) {
            get_token(infile, token);
            back = atof(token);
            return 1;
         }

         break;

      case 'd':
         if (!strcmp(token, "dither")) {
            dither = 1;
            return 1;
         }

         break;

      case 'h':
         if (!strcmp(token, "h")) {
            get_token(infile, token);
            h = atof(token);
            return 1;
         }

         break;

      case 'l':
         if (!strcmp(token, "lacunarity")) {
            get_token(infile, token);
            lac = atof(token);
            return 1;
         }

         break;

      case 'o':
         if (!strcmp(token, "octaves")) {
            get_token(infile, token);
            octaves = atof(token);
            return 1;
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
void helix::whereami(int frameno, molecule **mptr, int *obcount, int wflag,
			quark *actors, quark *parent, vector4f *mx) {

   linktype *ptr;                               // pointer
   bphelix *bptr;
   vector4f r[4];

   calc_initmx(mx, r);

   for (ptr=edge; ptr!=(linktype *)NULL; ptr=ptr->next)  // apply xform to
      if (ptr->link != parent)                           //    children
         ptr->link->whereami(frameno, mptr, obcount, wflag, actors, this, r);

   copymx4x4(xmx, localmx);
   matmatmulto(r, xmx);

   if (active && wflag) {
      (*obcount)++;

      bptr = new bphelix;

      bptr->ob   = this;
      bptr->frameno = frameno;

      copymx4x4(bptr->mx, xmx);

      bptr->next = *mptr;
      *mptr = bptr;
   }

   center[0] = xmx[0][3];
   center[1] = xmx[1][3];
   center[2] = xmx[2][3];
}

