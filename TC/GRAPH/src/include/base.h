



#if !defined (__BASE_H)
#define __BASE_H

#include "camera.h"
#include "light.h"
#include "zbuffer.h"
#include "shader.h"

#define CINULL          0
#define CITEXTURE       1
//#define CILUM         2
#define CIRAYCAST       4
#define CI3DTEX         8
#define CITRANSPARENT   16
#define CIBITMAP        32
#define CISPOT          64
#define CIINVISO        128

#define MINDIM  5
#define MINBBOX 1.5

typedef enum colorenum {
   SHADOW       =  -1,
   DOT          =  0,
   WFBW         =  1,
   BW           =  2,
   CONSTANT     =  3,
   ALEX         =  4,
   GOURAUD      =  5,
   PHONG        =  6,
   PDOT         = 40,
   PWFBW        = 41,
   PBW          = 42,
   PCONSTANT    = 43,
   PALEX        = 44,
   PGOURAUD     = 45,
   PPHONG       = 46,
   RSOLID       = 91,
   IRRADIANT    = 92,
} colortype;


/* *************************************************************
************************************************************* */
class datatype {

   public:
      char     dataname[MAXSTRLEN];   // data file for object
      datatype *next;                 // ptr to next in list

      datatype() {
         dataname[0] = 0;
         next = (datatype *)NULL;
      }

      int read_data(char *filename);
};


/* *************************************************************
************************************************************* */


class texmap {

   public:
      char dataname[MAXSTRLEN], texname[MAXSTRLEN];
      texmap *next;

      texmap() {
         next = (texmap *)NULL;
      }

};


/* *************************************************************
   This is the base class for objects to be rendered by the system
************************************************************* */
class pc : public superclass {

   protected:
                                                                // pc
      void calc_color(float *surface, float *normal, light *lmain,
        eye *parm, texcolortype *texcolor, float *rcolor, float *acolor);
      void calc_colorb(float *surface, float *normal, light *lmain, light *spot,
        eye *parm, texcolortype *texcolor, float *rcolor, float *acolor);

      void calc_color2(float *surface, float *normal, light *lmain, eye *parm,
        texcolortype *texcolor, float *tcolor, float *rcolor, float *acolor);
      void calc_color2b(float *surface, float *normal, light *lmain, light *spot,
        eye *parm, texcolortype *texcolor, float *tcolor, float *rcolor, float *acolor);

     void calc_color3(shaderparamtype *sparam, int i, float *surface, float *normal,
        light *lmain, eye *parm, texcolortype *texcolor, float *rcolor, float *acolor);
     void calc_color3b(shaderparamtype *sparam, int i, float *surface, float *normal, light *lmain,
        light *spot, eye *parm, texcolortype *texcolor, float *rcolor, float *acolor);

      void calc_color(float *ambient, float *diffuse, float *rcolor, float *acolor) {
         copyarray3(acolor, ambient);
         copyarray3(rcolor, diffuse);
      }

      void calc_colorb(float *surface, float *normal, light *spot, eye *parm,
        texcolortype *texcolor, float *diffuse, float *rcolor, float *acolor);

      void calc_color2(texcolortype *texcolor, float *bcolor, float *tcolor,
        float *rcolor, float *acolor);
      void calc_color2b(float *surface, float *normal, light *spot, eye *parm,
        texcolortype *texcolor, float *bcolor, float *tcolor,
        float *rcolor, float *acolor);
      void pc::calc_color3(shaderparamtype *sparam, int i, float *surface, float *normal,
        texcolortype *texcolor, float *bcolor, float *rcolor, float *acolor);
      void calc_color3b(shaderparamtype *sparam, int i, float *surface, float *normal,
        light *spot, eye *parm, texcolortype *texcolor,
        float *bcolor, float *rcolor, float *acolor);

      virtual int bound_box(eye *parm) = 0;
      virtual int clip(eye *parm, int maxx, int maxy) = 0;
      virtual void transform(eye *parm) = 0;

   public:
                                                                // superclass
      sc_type query_category() { return OBJECT_CLASS; }
      int  parse(FILE *infile, char *token);
      void preprocess(void *data);
                                                                // pc
      shadetype *base_color;
      vector4f  bbox[2];                                        // box in screen space

      shaderlisttype *shaderlist;

      colortype mctype;             // coloring
      unsigned int      mcinfo;
      float     center[4];          // object center in wc
      float     size;               // size change
      float     rotate[4][4];       // rotation matrix
      float     world[4][4];
      float     iworld[4][4];
      int       id;                 // object id #

      float     splane[4];          // shadow plane
      pc        *shadptr;           // shadow obj

      shadelist *lob;

      char      texname2[MAXSTRLEN], texname3[MAXSTRLEN], colorname[MAXSTRLEN],
                filename[MAXSTRLEN];

      char      sflag, renderflag, boundflag, pc_padd;

      pc();
      int  read_ilum(char *filename, shadelist **itr);

      virtual int read_data(char filename[], datatype **dptr);
      virtual int read_data(FILE *infile);
      virtual int read_tex(char *filename, char *dfilename, texbase **tptr, texmap **tpptr);
      virtual int read_3dtex(char *filename, char *dfilename, shader *sptr, shaderlisttype **slptr);

      virtual void begin_scan() {}
      virtual void end_scan()   {}
      virtual void datacopy()   {}

      virtual int beamscan(spotlight *spot) = 0;
      virtual int scan(camera *cparm, light *lmain) = 0;

      virtual void beamrender(spotlight *spot) = 0;
      virtual void render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff) = 0;
      virtual void prender() = 0;
};


typedef pc * ppc;
typedef pc ** pppc;


/* *************************************************************
   This is the base class for objects to be rendered by the system
************************************************************* */
class basenull: public pc {

   protected:
                                                // pc
      int  bound_box(eye *parm);
      void transform(eye *parm);
      int clip(eye *parm, int maxx, int maxy);

   public:
                                                // superclass
      object_type query_whatami() { return BASENULL; };
                                                // pc
      int beamscan(spotlight *spot);
      int scan(camera *cparm, light *lmain);

      void beamrender(spotlight *spot);
      void render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff);
      void prender();
};


#endif

