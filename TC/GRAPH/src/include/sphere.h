



#if !defined (__SPHERE_H)
#define __SPHERE_H

#include "geo.h"

/* *************************************************************
************************************************************* */
class sphere : public geo {

   friend class radiant;

   protected:
						// pc
      void transform(eye *parm);
						// geo
      float calc_area();
      void pg(eye *parm, light *lmain, light *spot, zbuffer *zbuff);
      void lt(spotlight *spot);
      void geo2poly(texpolygon **ptob, shaderlisttype *slist);

						// sphere
      float wscale[3];
      float normalcalc[4][4];

   public:
						// superclass
      object_type query_whatami() { return SPHERE; }
      int parse(FILE *infile, char *token);
      void preprocess(void *data) { geo::preprocess(data); }

						// pc
      int read_data(FILE *infile) { 
         fscanf(infile, "%f %f %f", &wscale[0], &wscale[1], &wscale[2]);
         return 1;
      }

						// sphere
      sphere() {
         wscale[0] = wscale[1] = wscale[2] = 1.0;
      }

};

#include "radiant.h"
#endif
