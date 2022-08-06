



#include "ion.h"


/* ********************************************************
******************************************************** */
int ion::parse(FILE *infile, char *token) {

   switch (token[0]) {
      case 'i':
         if (!strcmp(token, "inner_radius")) {
            get_token(infile, token);
            innerrad = atof(token);
            return 1;
         }

         break;

      case 'o':
         if (!strcmp(token, "outer_radius")) {
            get_token(infile, token);
            radius = atof(token);
            return 1;
         }

         break;

      default:
         break;
   }

   return quark::parse(infile, token);
}


/* ********************************************************
   this function calculates the camera position
   and puts data in a list
******************************************************** */
void ion::whereami(int frameno, molecule **mptr, int *obcount, int wflag,
			quark *actors, quark *parent, vector4f *mx) {

   linktype *ptr;                       // pointer
   bpion    *bptr;
   vector4f r[4];
   float    temp, temp2;

   calc_initmx(mx, r);

   for (ptr=edge; ptr!=(linktype *)NULL; ptr=ptr->next)  // apply xform to
      if (ptr->link != parent)                           //    children
         ptr->link->whereami(frameno, mptr, obcount, wflag, actors, this, r);

   copymx4x4(xmx, localmx);
   matmatmulto(r, xmx);

   center[0] = xmx[0][3];
   center[1] = xmx[1][3];
   center[2] = xmx[2][3];

   if (active && wflag) {
      temp = magnitude3(xmx[0]);

      temp2 = magnitude3(xmx[1]);
      if (temp2 < temp)
         temp = temp2;

      temp2 = magnitude3(xmx[2]);
      if (temp2 < temp)
         temp = temp2;

      (*obcount)++;
      bptr = new bpion;

      bptr->ob = this;
      bptr->frameno = frameno;

      copyarray4(bptr->location, center);
      bptr->inrad = innerrad*temp;
      bptr->outrad = radius*temp;

      bptr->next = *mptr;
      *mptr = bptr;
   }

}
