#if !defined __LINE_H
#define __LINE_H

#include "base.h"

class shadline;

/* *************************************************************
************************************************************* */
class line : public pc {

   friend class shadline;

   protected:
						// pc
      void transform(eye *parm);
      int  bound_box(eye *parm);
      int clip(eye *parm, int maxx, int maxy);
						// line
      vector3f wwi;
      vector4f wwai[2];
      vector3f color[2], wcolor[2];


   public:
						// superclass
      object_type query_whatami() { return LINE; }
      int parse(FILE *infile, char *token);
      void preprocess(void *data) { pc::preprocess(data); }
						// pc
      void datacopy();
      int read_data(FILE *infile);

      int beamscan(spotlight *spot);
      int scan(camera *cparm, light *lmain);

      void beamrender(spotlight *spot);
      void render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff);
      void prender();
				 		// line
      line() {
         copyarray3(wwi, center);
         color[0][0] = color[0][1] = color[0][2] =
         color[1][0] = color[1][1] = color[1][2] = 1.0;
      }

};

#include "shadline.h"

#endif