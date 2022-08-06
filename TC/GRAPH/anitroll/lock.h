


#include "wwdigo.h"

#if !defined (__LOCK_H)

#define __LOCK_H


/* *************************************************************
************************************************************* */
class lock : public wwdigo {

   public:
      quark *target;
      vector4f offset;
      char actor_name[MAXSTRLEN], piece[MAXSTRLEN];
      vector4f old_imx[4];

      lock() {
         init_mx(old_imx);
         target = (quark *)NULL;
         offset[0] = offset[1] = offset[2] = 0;
         offset[3] = 1;
         actor_name[0] = piece[0] = 0;
      }

      void whereami(int frameno, molecule **mptr, int *obcount, int wflag, quark *actors,
			quark *parent = (quark *)NULL, vector4f *mx = (vector4f *)NULL);
      void new_action(FILE *infile, float timefactor, char *buffer = (char *)NULL);
      whatamitype whatami() { return LOCK; }
};


#endif