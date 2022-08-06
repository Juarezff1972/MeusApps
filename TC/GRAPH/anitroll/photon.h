

#if !defined (__PHOTON_H)

#define __PHOTON_H

#include "anitroll.h"
#include "light.h"


/* *************************************************************
   This is the lighting class
************************************************************* */
class photon : public quark {

   friend class bpphoton;

   protected:
      object_type mode;		// light mode
      lightclass group;
      char simpleflag;

      vector3f  Ia, Ip;		// light intensities
      vector3f  vpn, vup;
      float     distance2plane;
      float     width, length;
      float     fatt[3];

   public:

      photon() {
         mode = FAR;
         simpleflag = 0;
         group = GLOBAL_LIGHT;
         Ia[0] = Ia[1] = Ia[2] = 0.3;
         Ip[0] = Ip[1] = Ip[2] = 0.7;
         vpn[0] = vpn[1] = 0; vpn[2] = 1;
         vup[0] = vup[2] = 0; vup[1] = 1;
         distance2plane = length = width = 1;
         fatt[0] = fatt[1] = fatt[2] = 0.0;
         active = 0;
      }

      int  parse(FILE *infile, char *token);
      void whereami(int frameno, molecule **mptr, int *obcount, int wflag, quark *actors,
			quark *parent = (quark *)NULL, vector4f *mx = (vector4f *)NULL);
      whatamitype whatami() { return PHOTON; }
};

#endif

