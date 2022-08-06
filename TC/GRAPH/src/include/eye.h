

#if !defined __EYE_H
#define __EYE_H

#include "matrix.h"
#include "pstring.h"
#include "superclass.h"

/* *************************************************************
************************************************************* */


class eye : public superclass {                            // camera data

   public:
					// superclass
      int parse(FILE *infile, char *token);
      void  preprocess(void *data) { superclass::preprocess(data); }

					// eye
      vector3f location;  

      vector3f vup;
      vector3f vpn;

      vector4f vrc;

      float mscale, imscale;           // used to size the viewport

      vector4f transform[4];
      vector4f Tinverse[4];

      eye() {
         location[0] = location[1] = location[2] = 0;

         vup[0] = vup[2] = 0;
         vup[1] = 1;

         vpn[0] = vpn[1] = 0;
         vpn[2] = -1;

         vrc[0] = -(vrc[1] = 4.0/3.0);
         vrc[2] = -1;
         vrc[3] = 1;

         imscale = mscale = 1;
      }

      void  nper();
      float query_scale() { return mscale; }

      virtual float zscale(float n, float d) = 0;
      virtual void  map2screen(float *v) = 0;
      virtual void  screen2map(float *v) = 0;
};

#endif
