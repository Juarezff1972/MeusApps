



#if !defined __SPATCH_H
#define __SPATCH_H

#define SPATPOLY 1
#define SPATREND 2

#define SPDELTA 5

#define vdownrow(x, maxcol) ( (x)+((maxcol)+2) )
#define vuprow(x, maxcol) ( (x)-((maxcol)+2) )
#define vtwo2one(row, col, maxcol) ( ((row)+1)*((maxcol)+2) + ((col)+1) )
#define pdownrow(x, maxcol) ( (x)+((maxcol)-1) )
#define ptwo2one(row, col, maxcol) ( (row)*((maxcol)-1) + (col) )


#include "polygon.h"

/* *************************************************************
************************************************************* */
class spatch : public make_poly {

   protected:
					// pc
      void transform(eye *parm);
      int  bound_box(eye *parm);
      int  clip(eye *parm, int maxx, int maxy);
					// spatch
      spatchtype   *ob;                 // pointer to composite patch data
      spatchtype   *dob;                // pointer to patch data
      texpolygon   *tob;

      camera		*cptr;
      light		*lptr;

      polygon		polyob;
      shaderlisttype	slist;
      spatchtype	ptr;
      texpolygon	ttr;

      void convert(int i, int j, spatchtype *sob, texpolygon *stob, shaderlisttype *ssob);
      void spline2poly(spatchtype *sob, texpolygon *stob, int xflag);

   public:
					// superclass
      object_type query_whatami() { return SPATCH; }
      int parse(FILE *infile, char *token);
      void preprocess(void *data);
					// pc
      void begin_scan() {
         ob   = new spatchtype;
         tpob = (texpolygon *)NULL;
         shadptr = (pc *)NULL;
         slist.shade = (shadertype *)NULL;
      }

      void end_scan() {

         if (ob != (spatchtype *)NULL)
            delete ob;

         if (tpob != (texpolygon *)NULL)
            delete tpob;

         if (slist.shade != (shadertype *)NULL) {
            delete [] slist.shade;
            slist.shade = (shadertype *)NULL;
         }

         polyob.end_scan();
      }

      void datacopy();

      int read_data(char dataname[], datatype **dptr);
      int read_tex(char *filename, char *dfilename, texbase **tptr, texpolygon **tpptr);
      int read_3dtex(char *filename, char *dfilename, shader *sptr, shaderlisttype **slptr);

      int beamscan(spotlight *spot);
      int scan(camera *cparm, light *lmain);

      void beamrender(spotlight *lmain);
      void prender();
      void render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff);
};

#endif
