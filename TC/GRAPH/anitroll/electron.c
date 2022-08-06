



#include "electron.h"


/* ********************************************************
   this function reads in camera data
******************************************************** */
int electron::parse(FILE *infile, char *token) {

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

   return quark::parse(infile, token);
}


/* ********************************************************
   this function calculates the camera position
   and puts data in a list
******************************************************** */
void electron::whereami(int frameno, molecule **mptr, int *obcount, int wflag,
			quark *actors, quark *parent, vector4f *mx) {

   linktype *ptr;                       // pointer
   bpelectron *bptr;
   vector4f r[4];

   calc_initmx(mx, r);

   for (ptr=edge; ptr!=(linktype *)NULL; ptr=ptr->next)  // apply xform to
      if (ptr->link != parent)                           //    children
         ptr->link->whereami(frameno, mptr, obcount, wflag, actors, this, r);

   copymx4x4(xmx, localmx)
   matmatmulto(r, xmx);

   if (active && wflag) {

      (*obcount)++;

      bptr = new bpelectron;

      bptr->ob = this;
      bptr->frameno = frameno;

      center[0] = xmx[0][3];
      center[1] = xmx[1][3];
      center[2] = xmx[2][3];

      copyarray3(bptr->location, center);
      copyarray3(bptr->dop, coa);
      subarray3(bptr->dop, origin);
      bptr->dop[3] = bptr->location[3] = 1;

      matvecmulto(xmx, bptr->dop);
      copyarray3(bptr->vup, vup);
      matvecmultv(xmx, bptr->vup);

      bptr->next = *mptr;
      *mptr = bptr;
   }

   else {
      center[0] = xmx[0][3];
      center[1] = xmx[1][3];
      center[2] = xmx[2][3];
   }

}
