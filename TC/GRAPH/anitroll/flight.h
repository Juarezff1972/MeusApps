


#include "wwdigo.h"

#if !defined (__FLIGHT_H)

#define __FLIGHT_H


/* *************************************************************
************************************************************* */
class flight : public wwdigo {

   public:
      int fraction;

      flight() {
         fraction = 0;
         upflag = 1;
      }

      void   whereami(int frameno, molecule **mptr, int *obcount, int wflag, quark *actors,
			quark *parent = (quark *)NULL, vector4f *mx = (vector4f *)NULL);
      whatamitype whatami() { return FLIGHT; }
};


#endif
