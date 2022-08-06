



#if !defined (__CYLINDER_H)
#define __CYLINDER_H

#include "geo.h"


/* *************************************************************
************************************************************* */
class cylinder : public geo {

   protected:
								// pc
      void transform(eye *parm);
								// geo
      void pg(eye *parm, light *lmain, light *spot, zbuffer *zbuff);
      void lt(spotlight *spot);
      void geo2poly(texpolygon **ptob, shaderlisttype *slist);
      float calc_area();
								// cylinder
      float length, hlength, radius;

   public:
								// superclass
      object_type query_whatami() { return CYLINDER; }
      int parse(FILE *infile, char *token);
      void preprocess(void *data) { geo::preprocess(data); }
								// pc
      int read_data(FILE *infile) { 
         fscanf(infile, "%f %f", &length, &radius);
         hlength = length * 0.5;
         return 1;
      }
								// cylinder
      cylinder() {
         hlength = radius = 1;
         length = 2;
      }

};

#endif
