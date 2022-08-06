#if !defined __POLYGON_H
#define __POLYGON_H


#include "datatype.h"
#include "texobj.h"

#define BUCKETSIZE      256

/* *************************************************************
        prenum = parm->imscale*normal[0]
        prediv = v[0]*normal[0] + v[1]*normal[1] - normal[2];
************************************************************* */
#define engine_dx(normal, prenum, prediv, t)    \
   prediv += prenum;                    \
   t = (abs(prediv) > CORRECT) ? normal[3]/prediv : back;

/* *************************************************************
        FAR: prenum = -parm->imscale*normal[0]/normal[2]
             prediv = previous "t"
************************************************************* */
#define fengine_dx(prenum, prediv, t)      \
   prediv += prenum;                    \
   t = prediv;


#define COMMON                  \
        int i;                  \
        edgetype *ptr;          \
        edgetype *aet;          \
        int scany, scanxy;      \
        float scanfy;           \
        int index;              \
        int start, end;

#define SHADECOMMON             \
        edgetype   *qtr;        \
        texcolortype texcolor;  \
        float      surface[4];  \
        vector3f   rcolor, acolor; \
        int        a0, a1, a2, d0, d1, d2;      \
        surface[3] = 1

#define SHADE3D                 \
        shaderparamtype sparam; \
        sparam.setup(world, iworld, frame, parm, this, shaderlist->scale)


typedef struct {

   vector3f constcolor;
   float    area;
   vector3f ambient;
} faceinfo;


class shadow;


/* *************************************************************
   object to render poligonal data.
************************************************************* */
class polygon : public pc {

   friend class shadow;

   protected:
                                                        // pc
      int bound_box(eye *parm);
      int clip(eye *parm, int maxx, int maxy);          // use polyclet instead
      void transform(eye *parm);

   public:
                                                        // polygon
      polytype   *dob;                          // pointer to data

      texpolygon *tob;

      faceinfo *facedata;

      polytype *ob;                             // pointer to composite data
      edgetype **et;

      char bflag, tflag, bbflag;                // backface culling, transform flag, bounding box flag

      int   clip_greater(edgetype *ptr, int i, float  plane, float *normal, eye *parm);
      int   clip_lesser(edgetype *ptr, int i, float  plane, float *normal, eye *parm);
      void  post_clip(edgetype *ptr);

      void  fillbucket(edgetype **head, edgetype *node);
      void  sortbucket(edgetype **head, edgetype *node);

      int   prepare_edge(int polynum, edgetype *ptr, eye *parm, light *lmain);
      void  add_edge_table(eye *parm, light *lmain);
      void  add_edge_table(edgetype *ptr, int i, eye *parm, light *lmain);

      edgetype *edgelist(int i);
      int    polyclet(eye *parm, light *lmain, int maxx, int maxy);

      void   removeside();

      void   constcolor(eye *parm, light *lmain);
      float  engine(float *normal, float *middle, eye *parm, float *prenum, float *prediv);

      void polylt(spotlight *parm, int winx, int winy);
      void polyltr(spotlight *parm, int winx, int winy);

      void polywfbw(zbuffer *zbuff);

      void polybw(zbuffer *zbuff, int winx, int winy);
      void polybwr(eye *parm, zbuffer *zbuff, int winx, int winy);

      void polyct(zbuffer *zbuff, int winx, int winy);
      void polyct2(zbuffer *zbuff, int winx, int winy);

#ifdef DOS
      void polyct(int i, zbuffer *zbuff);
      void polyct2(int i, int flagb, zbuffer *zbuff);
      void polyct2db(int i, zbuffer *zbuff);
      void polyct2d(int i, zbuffer *zbuff);
#endif

      void polyctr(eye *parm, zbuffer *zbuff, int winx, int winy);
      void polyctr2(eye *parm, zbuffer *zbuff, int winx, int winy);
      void polyct3(eye *parm, zbuffer *zbuff, int winx, int winy);
      void polyctr3(eye *parm, zbuffer *zbuff, int winx, int winy);
      void polyctt(zbuffer *zbuff, int winx, int winy);
      void polyctt2(zbuffer *zbuff, int winx, int winy);
      void polycttr(eye *parm, zbuffer *zbuff, int winx, int winy);
      void polycttr2(eye *parm, zbuffer *zbuff, int winx, int winy);
      void polyctt3(eye *parm, zbuffer *zbuff, int winx, int winy);
      void polycttr3(eye *parm, zbuffer *zbuff, int winx, int winy);
      void polyctb(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy);
      void polyctb2(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy);
      void polyctbr(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy);
      void polyctbr2(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy);
      void polyctb3(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy);
      void polyctbr3(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy);

      void polygd(zbuffer *zbuff, int winx, int winy);
      void polygd2(zbuffer *zbuff, int winx, int winy);
      void polygdr(eye *parm, zbuffer *zbuff, int winx, int winy);
      void polygdr2(eye *parm, zbuffer *zbuff, int winx, int winy);
      void polygd3(eye *parm, zbuffer *zbuff, int winx, int winy);
      void polygdr3(eye *parm, zbuffer *zbuff, int winx, int winy);
      void polygdt(zbuffer *zbuff, int winx, int winy);
      void polygdt2(zbuffer *zbuff, int winx, int winy);
      void polygdtr(eye *parm, zbuffer *zbuff, int winx, int winy);
      void polygdtr2(eye *parm, zbuffer *zbuff, int winx, int winy);
      void polygdt3(eye *parm, zbuffer *zbuff, int winx, int winy);
      void polygdtr3(eye *parm, zbuffer *zbuff, int winx, int winy);
      void polygdb(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy);
      void polygdb2(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy);
      void polygdbr(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy);
      void polygdbr2(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy);
      void polygdb3(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy);
      void polygdbr3(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy);

      void polyal(eye *parm, light *main, zbuffer *zbuff, int winx, int winy);
      void polyal2(eye *parm, light *main, zbuffer *zbuff, int winx, int winy);
      void polyalr(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polyalr2(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polyal3(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polyalr3(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polyalt(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polyalt2(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polyaltr(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polyaltr2(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polyalt3(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polyaltr3(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polyalb(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy);
      void polyalb2(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy);
      void polyalbr(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy);
      void polyalbr2(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy);
      void polyalb3(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy);
      void polyalbr3(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy);

      void polypg(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polypg2(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polypgr(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polypgr2(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polypg3(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polypgr3(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polypgt(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polypgt2(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polypgtr(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polypgtr2(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polypgt3(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polypgtr3(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy);
      void polypgb(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy);
      void polypgb2(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy);
      void polypgbr(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy);
      void polypgbr2(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy);
      void polypgb3(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy);
      void polypgbr3(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy);

   public:
                                                                // superclass
      object_type query_whatami() { return POLYGON; }
      int parse(FILE *infile, char *token);
      void preprocess(void *data);
                                                                // pc
      void begin_scan();
      void end_scan();
      void datacopy();

      int read_tex(char *filename, char *dfilename, texbase **tptr, texpolygon **tpptr);
      int read_3dtex(char *filename, char *dfilename, shader *sptr, shaderlisttype **slptr);

      int read_data(char filename[], datatype **dptr);

      int  beamscan(spotlight *spot);
      int  scan(camera *cparm, light *lmain);

      void beamrender(spotlight *spot);
      void render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff);
      void prender();
                                                                // polygon
      unsigned int inviso;

      polygon() {
         facedata = (faceinfo *)NULL;
         tflag = bflag = bbflag = 1;
      }

      ~polygon() {
         if (facedata)
            delete [] facedata;
      }

      void setup_patch(polytype *sob, colortype sctype, unsigned int scinfo, int sid, int ssflag,
                       float  ssplane[], float  box[][4], shadetype *c);
      void setup_texture(texpolygon *stob);
      void setup_texture3d(shaderlisttype *stob, float sworld[][4], float siworld[][4]);
};


class make_poly : public pc {

   protected:
                                // make_poly
      polytype     *pob;
      texpolygon   *tpob;

      void setup_transfer(int vcount, int pcount);

   public:
      int parse(FILE *infile, char *token) { return pc::parse(infile, token); }
      void preprocess(void *data) { pc::preprocess(data); }
};





#include "shadow.h"


#endif
