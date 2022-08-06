



#if !defined __SHADOWPT_H
#define __SHADOWPT_H


class shadowpt;

#include "particle.h"

/* *************************************************************
   object to render poligonal data.
************************************************************* */
class shadowpt : public particle {

   public:
						// superclass
      int parse(FILE *infile, char *token) { return particle::parse(infile, token); }
      void preprocess(void *data) { particle::preprocess(data); }
						// shadowpt
      int  datacopy2(particle *pob, light *lmain);
      int  point2plane(float psplane[], light *lmain);
      void render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff);
};

#endif
