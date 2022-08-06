


#if !defined (__BUCKEY_H)

#define __BUCKEY_H

#include "anitroll.h"


/* *************************************************************
   This is the class of 3D object children of a SPHERE
************************************************************* */
class buckey : public neutron {

   public:
      vector3f axis;

      buckey() {
         axis[0] = axis[1] = axis[2] = 1.0;
      }

      int parse(FILE *infile, char *token);
      whatamitype whatami() { return BUCKEY; }
};


#endif
