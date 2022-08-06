


#if !defined (__HELIX_H)

#define __HELIX_H

#include "anitroll.h"

/* *************************************************************
   This is sbfast class
************************************************************* */
class helix : public quark {

   friend class bphelix;

   protected:
      char   disname[MAXSTRLEN];
      float  back, octaves, h, lac;
      int    dither;

   public:

      helix() {
         octaves = 100;
         h = 0.9;
         lac = 2.1;
         back = 1000;
         strcpy(disname, "default_alt");
//        scalesb = 157.241;
         dither = 0;
      }

      int  parse(FILE *infile, char *token);
      void whereami(int frameno, molecule **mptr, int *obcount, int wflag, quark *actors,
			quark *parent = (quark *)NULL, vector4f *mx = (vector4f *)NULL);
      whatamitype whatami() { return HELIX; }
};




#endif
