


#if !defined (__CHAIN_H)

#define __CHAIN_H

#include "anitroll.h"


/* *************************************************************
************************************************************* */
class chain : public quark {

   friend class bpchain;

   protected:

      vector4f endpt;
      vector3f color[2];

      float  intensity, decay, death;

   public:
      chain() {
         endpt[0] = endpt[1] = endpt[2] = 0; endpt[3] = 1;
         decay = 1;
         intensity = 1;
         death = 0;
      }

      int  parse(FILE *infile, char *token);
      void whereami(int frameno, molecule **mptr, int *obcount, int wflag, quark *actors,
			quark *parent = (quark *)NULL, vector4f *mx = (vector4f *)NULL);

      whatamitype whatami() { return CHAIN; }
};


#endif
