


#if !defined (__FUNNEL_H)

#define __FUNNEL_H

#include "anitroll.h"


/* *************************************************************
   This is the class of 3D object children of a SPHERE
************************************************************* */
class funnel : public neutron {

   public:
      float length;
      float radius[2];

      int parse(FILE *infile, char *token);
      whatamitype whatami() { return FUNNEL; }
};


#endif