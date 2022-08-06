


#include "anitroll.h"

#if !defined (__WWDIGO_H)

#define __WWDIGO_H

#define CATCHUP 30
#define ICATCHUP 0.03333333333

/* *************************************************************
************************************************************* */
class wwdigo : public quark {

   public:
      vector3f old_vx, old_vy, old_vz;
      char upflag;
      int catchup;
      float icatchup;
      int fraction, turn_rate;
      vector4f angle[4];

      vector4f old_local_center;

      wwdigo() {
         old_vx[0] = 1; old_vx[1] = old_vx[2] = 0;
         old_vy[1] = 1; old_vy[0] = old_vy[2] = 0;
         old_vz[2] = 1; old_vz[0] = old_vz[1] = 0;
         catchup = CATCHUP;
         icatchup = ICATCHUP;
         upflag = 0;
      }

      void calc_orientation(float *new_dir);

      whatamitype whatami() { return WWDIGO; }
      int  parse(FILE *infile, char *token);
      void preprocess();
      void new_action(FILE *infile, float timefactor, char *buffer = (char *)NULL);
};


#endif
