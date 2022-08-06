



#include "neutron.h"


/* ********************************************************
   This function reads in the data for this child
******************************************************** */
int neutron::parse(FILE *infile, char *token) {

   switch (token[0]) {

      case 'f':
         if (!strcmp(token, "file")) {
            get_token(infile, obname);
            return 1;
         }

         break;

      case 'i':
         if (!strcmp(token, "inviso")) {
            get_token(infile, token);
            ir = atoi(token);
            get_token(infile, token);
            ig = atoi(token);
            get_token(infile, token);
            ib = atoi(token);

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
void neutron::whereami(int frameno, molecule **mptr, int *obcount, int wflag,
			quark *actors, quark *parent, vector4f *mx) {

   vector4f r[4];      // local xform mx
   linktype *ptr;              // pointer
   bpneutron *bptr;

   calc_initmx(mx, r);

   for (ptr=edge; ptr!=(linktype *)NULL; ptr=ptr->next)  // apply xform to
      if (ptr->link != parent)                           //    children
         ptr->link->whereami(frameno, mptr, obcount, wflag, actors, this, r);

   copymx4x4(xmx, localmx);
   matmatmulto(r, xmx);

   if (active && wflag) {
      (*obcount)++;

      bptr = new bpneutron;

      bptr->frameno = frameno;

      bptr->ob   = this;
      copymx4x4(bptr->mx, xmx);

      center[0] = xmx[0][3];
      center[1] = xmx[1][3];
      center[2] = xmx[2][3];

      bptr->next = *mptr;
      *mptr = bptr;
   }

   else {
      center[0] = xmx[0][3];
      center[1] = xmx[1][3];
      center[2] = xmx[2][3];
   }

}

