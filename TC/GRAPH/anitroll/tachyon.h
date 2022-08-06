


#if !defined (__TACHYON_H)

#define __TACHYON_H

#include "anitroll.h"


/* *************************************************************
   This is particle class
************************************************************* */
class tachyon : public quark {

   friend class bptachyon;

   protected:
      vector3f color;
      float  intensity, decay, death;

   public:
      int parse(FILE *infile, char *token);
      void whereami(int frameno, molecule **mptr, int *obcount, int wflag, quark *actors,
			quark *parent = (quark *)NULL, vector4f *mx = (vector4f *)NULL);

      tachyon() {
         decay = 1;
         intensity = 1;
         death = 0;
      }

      whatamitype whatami() { return TACHYON; }
};


#endif
