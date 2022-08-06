


#if !defined (__TUBE_H)

#define __TUBE_H

#include "anitroll.h"


/* *************************************************************
   This is the class of 3D object children of a SPHERE
************************************************************* */
class tube : public neutron {

   public:
      float  length, radius;

      tube() {
         length = radius = 1;
      }

      int parse(FILE *infile, char *token);
      whatamitype whatami() { return TUBE; }
};


#endif
