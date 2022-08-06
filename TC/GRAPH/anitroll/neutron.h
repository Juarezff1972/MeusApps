


#include "anitroll.h"

#if !defined (__NEUTRON_H)

#define __NEUTRON_H


/* *************************************************************
   This is the base class of 3D object children of an atom
   POLYGON, SPATCH, SPHERE, CONE, CYLINDER
************************************************************* */
class neutron : public quark {

   friend class bpneutron;

   public:
      char   obname[MAXSTRLEN];                 // object filename
      int    ir, ig, ib;

      neutron() {
         obname[0] = 0;
      }

      int    parse(FILE *infile, char *token);
      void   whereami(int frameno, molecule **mptr, int *obcount, int wflag, quark *actors,
			quark *parent = (quark *)NULL, vector4f *mx = (vector4f *)NULL);
      whatamitype whatami() { return NEUTRON; }
};


#endif
