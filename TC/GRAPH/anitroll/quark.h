


#include "global.h"
#include "anitroll.h"

#if !defined (__QUARK_H)

#define __QUARK_H

typedef enum {
   BNONE   = 0,
   BPOINT  = 1,
   BSPHERE = 2,
   BPLANE  = 3,
   BRECT   = 4
} btype;

typedef enum {
   QUARK, ATOM, ELECTRON, PHOTON, NEUTRON, TACHYON, ION, HELIX, BUCKEY, TUBE,
   FUNNEL, BOID, BOIDBAIT, FLIGHT, CHAIN, SHELL, WWDIGO, LOCK
} whatamitype;


/* *************************************************************
   This is the basic class from which all classes are inherited
************************************************************* */
class quark {

   protected:
      float Q[4][4];
      int firstflag;

             int  remove_link(quark *x);
             int  query_texture(char *instring);
             void calc_initmx(vector4f *in, vector4f *out);

      void   calc_localquat(float  mx[][4]);
      void   preparequat(float  mx[][4]);
      void   calc_localsp(float  mx[][4]);
      void   preparesp(float  mx[][4]);

   public:
      int    qstatus;                           // flag for transition
      vector4f *qstart, *qend;                  // control angles
      int    qkey;                              // ptr to current knots
      float  qframe;                            // local frame #
      float  qframesperspline;                  // # of frames/spline
      int    qcount;                            // # of angles
      int    qauto;                             // flag for repetition or not
      float  qcurrent[4], qdelta[4];            // calculated for next frame
      float  qold[4];                           // old pos
      float  qcatchup;                          // # of frames to
      int    qcounter;                          //    get ready for new

      int    kstatus;                           // flag for transition
      vector4f *kknot;				// knots
      int    kkey;                              // ptr to current knots
      float  kframe;                            // local frame #
      float  kframesperspline;                  // # of frames/spline
      int    kcount;                            // # of knots
      int    kauto;                             // flag for repetition or not
      float  kcurrent[4], kdelta[4];            // calculated for next frame
      float  kold[4];                           // old pos
      float  kcatchup;                          // # of frames to
      int    kcounter;                          //    get ready for new

      char     name[MAXSTRLEN];                 // name of object
      vector4f initxform[4];			// initial world
      vector4f localmx[4];			// local only
      vector3f origin;				// rotation home
      vector4f location;
      char     obtype[MAXSTRLEN];               // coloring info
      char     colorname[MAXSTRLEN];		// ilm name
      char     texname[MAXSTRLEN];		// texture name
      unsigned int mcinfo;
      char     shadow;
      float    scale, iscale;
      int      active;

      float    bsphere;                         // for bounding sphere
      btype    bflag;				// flag for what type of bound

      linktype *edge;                           // ptr to children
			// state data
      vector4f center;				// current location
      vector4f xmx[4];				// current/old transformation mx
      vector4f bdata[8];			// vertex for bounding box
			// old state data
      vector4f old_xmx[4];
      vector4f oldcenter;			// last location
      vector4f bold_data[8];
      vector4f bold_normal[6];

                    quark();
                    ~quark();
              quark *find(quark *parent, char *part);
              quark *find(quark **parent, char *part);
              int   split(quark *ob);
              void  bond(quark *ob);

              quark *read_quark(FILE *infile, quark *parent);
              int    read_bound(FILE *infile);

      virtual whatamitype whatami() { return QUARK; }

      virtual int  parse(FILE *infile, char *token);
      virtual void preprocess();
      virtual void update(quark *parent);
      virtual void whereami(int frameno, molecule **mptr, int *obcount, int wflag, quark *actors,
			quark *parent = (quark *)NULL, vector4f *mx = (vector4f *)NULL);
      virtual void new_action(FILE *infile, float  timefactor, char *buffer = (char *)NULL);
};

#endif