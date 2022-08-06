



#if !defined (__GEO_H)
#define __GEO_H

#include "polygon.h"

#define SCANPOLY 1
#define SCANCAST 2


/* *************************************************************
************************************************************* */
class geo : public make_poly {

   protected:
							// pc
      int bound_box(eye *parm);
      int clip(eye *parm, int maxx, int maxy);
							// geo
      int row[2], col[2];
      float texcoord[4][2];
      texbase  *tob;
      vector4f wcenter;

      polygon polyob;
      shaderlisttype slist;

      unsigned int query_texel(float tx, float ty, float *c, float d);
      virtual void pg(eye *parm, light *lmain, light *spot, zbuffer *zbuff) = 0;
      virtual void lt(spotlight *spot) = 0;
      virtual void geo2poly(texpolygon **ptob, shaderlisttype *slist) = 0;
      virtual float calc_area() = 0;

   public:
							// superclass
      void preprocess(void *data);
 							// pc
      int read_tex(char *filename, char *dfilename, texbase **tptr, texmap **tpptr);
      int read_3dtex(char *filename, char *dfilename, shader *sptr, shaderlisttype **slptr);

      void begin_scan();
      void end_scan();
      void datacopy();

      int beamscan(spotlight *spot);
      int scan(camera *cparm, light *lmain);

      void beamrender(spotlight *spot);
      void render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff);
      void prender();
};

#endif
