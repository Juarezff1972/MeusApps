



#if !defined __SHADOW_H
#define __SHADOW_H


#include "polygon.h"

/* *************************************************************
   object to render poligonal data.
************************************************************* */
class shadow : public polygon {

   protected:
							// shadow
      void object2plane(light *lmain);
      void shadct(zbuffer *zbuff, int winx, int winy);
      void shadctr(eye *parm, zbuffer *zbuff, int winx, int winy);

   public:
							// superclass
      int parse(FILE *infile, char *token) { return polygon::parse(infile, token); }
      void preprocess(void *data) { polygon::preprocess(data); }
							// shadow
      int  datacopy2(polygon *pob, camera *cparm, light *lmain, int invert);
      void render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff);
};

#endif
