#if !defined __PARTICLE_H
#define __PARTICLE_H

#include "base.h"

class shadowpt;

/* *************************************************************
************************************************************* */
class particle : public pc {

   friend class shadowpt;

   protected:
						// pc
      void transform(eye *parm);
      int  bound_box(eye *parm);
      int  clip(eye *parm, int maxx, int maxy);
						// particle
      vector3f color;
      vector4f wwai;
      vector3f wcolor;

   public:
						// superclass
      object_type query_whatami() { return PARTICLE; }
      int parse(FILE *infile, char *token);
      void preprocess(void *data) { pc::preprocess(data); }
						// pc
      void datacopy();
      int read_data(FILE *infile);

      int scan(camera *cparm, light *lmain);
      int beamscan(spotlight *spot);

      void render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff);
      void prender();
      void beamrender(spotlight *spot);
						// particle
      particle() { color[0] = color[1] = color[2] = 1.0; }
};

#include "shadowpt.h"

#endif
