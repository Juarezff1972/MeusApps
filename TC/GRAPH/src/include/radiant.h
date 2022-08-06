



#if !defined __RADIANT_H
#define __RADIANT_H

#include "sphere.h"

/* *************************************************************
************************************************************* */
class radiant : public pc {

   protected:
                                                // pc
      int   bound_box(eye *parm);
      void  transform(eye *parm);
      int clip(eye *parm, int maxx, int maxy);

                                                // radiant
      float radius, inner_circle, wradius, winner_circle, wratio, tradius, sqrwradius, scale;
      float wcenter[4];
      int   row[2], col[2];

      sphere r;

      float radiance(float pt[]);

   public:
                                                // superclass
      object_type query_whatami() { return RADIANT; }
      int parse(FILE *infile, char *token);
      void preprocess(void *data);
                                                // pc
      void end_scan() { if (mctype == WFBW) r.end_scan(); }
      void datacopy();

      int read_data(FILE *infile);

      int scan(camera *cparm, light *lmain);
      int beamscan(spotlight *spot);

      void render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff);
      void prender();
      void beamrender(spotlight *spot);
                                                // radiant
      int   flagh;

      radiant() {
         inner_circle = 1;
         radius = 2;
         mcinfo = CITRANSPARENT;

         flagh = 0;
      }

};

typedef radiant * pradiant;

#include "sphere.h"

#endif
