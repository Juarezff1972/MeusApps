



#if !defined __SHADLINE_H
#define __SHADLINE_H


class shadline;

#include "line.h"

/* *************************************************************
************************************************************* */
class shadline : public line {

   public:
					// superclass
      int parse(FILE *infile, char *token) { return line::parse(infile, token); }
      void preprocess(void *data) { line::preprocess(data); }
					// pc
      void render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff);
					// shadline
      int  datacopy2(line *pob, light *lmain);
      int  point2plane(float *psplane, light *lmain);
};

#endif
