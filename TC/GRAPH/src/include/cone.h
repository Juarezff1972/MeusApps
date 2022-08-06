



#if !defined (__CONE_H)
#define __CONE_H

#include "geo.h"


/* *************************************************************
************************************************************* */
class cone : public geo {

   protected:
                                        // base
      void transform(eye *parm);
                                        // geo
      void pg(eye *parm, light *lmain, light *spot, zbuffer *zbuff);
      void lt(spotlight *spot);
      void geo2poly(texpolygon **ptob, shaderlisttype *slist);
      float calc_area();
                                        // cone
      float length;
      float radius[2];

   public:
                                        // superclass
      object_type query_whatami() { return CONE; }
      int parse(FILE *infile, char *token);
      void preprocess(void *data) { geo::preprocess(data); }

                                        // pc
      int read_data(FILE *infile) {
         fscanf(infile, "%f %f %f", &length, &radius[0], &radius[1]);
         length *= 0.5;

         return 1;
      }

                                        // cone
      cone() {
         length = radius[0] = radius[1] = 1;
      }

};

#endif
