


#if !defined (__ION_H)

#define __ION_H

#include "anitroll.h"

/* *************************************************************
   This is radiant class
************************************************************* */
class ion : public quark {

   friend class bpion;

   protected:
      float  radius, innerrad;

   public:

      ion() {
         radius = 2;
         innerrad = 1;
      }

      int parse(FILE *infile, char *token);
      void whereami(int frameno, molecule **mptr, int *obcount, int wflag, quark *actors,
			quark *parent = (quark *)NULL, vector4f *mx = (vector4f *)NULL);
      whatamitype whatami() { return ION; }
};




#endif
