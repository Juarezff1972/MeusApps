


#if !defined (__ELECTRON_H)

#define __ELECTRON_H

#include "anitroll.h"

/* *************************************************************
   This is the camera class
************************************************************* */
class electron : public quark {

   friend class bpelectron;

   protected:
      vector3f coa, vup;
      int    power;                     // on/off flag

   public:
           electron() {
              coa[0] = coa[1] = 0; coa[2] = -1;
              vup[0] = vup[2] = 0; vup[1] = 1;
              active = 0;
           }

      int  parse(FILE *infile, char *token);
      void whereami(int frameno, molecule **mptr, int *obcount, int wflag, quark *actors,
			quark *parent = (quark *)NULL, vector4f *mx = (vector4f *)NULL);
      whatamitype whatami() { return ELECTRON; }
};


#endif
