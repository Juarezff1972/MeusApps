
#include "eye.h"
#include "texture.h"
#include "shading.h"
#include "memman.h"

#if !defined __LIGHT_H
#define __LIGHT_H

#define MAXLDIM 200


typedef enum lightclassenum {
        GLOBAL_LIGHT = 0, SPOT_LIGHT = 1
} lightclass;


/* *************************************************************
************************************************************* */

class light : public eye {

   protected:
                                                // light
      virtual void lighting(float *surface, float *gnormal, float *rcolor,
                          shadetype *shade, float *mIp, float *vlight, float dist2surface);

   public:
                                                // superclass
      sc_type query_category() { return LIGHT_CLASS; }
      int parse(FILE *infile, char *token);
      void preprocess(void *data) { eye::preprocess(data); }
                                                // light
      float     Ia[3], Ip[3];
      float     fatt[3];

      vector4f  transloc, transvec;

      light() {
         vpn[0] = vpn[1] = 0; vpn[2] = 1;

         Ia[0] = Ia[1] = Ia[2] = 0.3;
         Ip[0] = Ip[1] = Ip[2] = 0.7;
         fatt[0] = fatt[1] = fatt[2] = 0.0;
      }

      void set_ambient(float *ka, float *ambient, float *lum);

      virtual lightclass query_class() { return GLOBAL_LIGHT; }

      virtual void intensity(float *surface, float *gnormal, float *rcolor,
                     eye *parm, shadetype *shade, superclass *source) = 0;
      virtual void xform(eye *parm) = 0;
      virtual int read_data(FILE *infile, texbase **tptr) = 0;
      virtual void write_lst(FILE *outfile, int frameno) = 0;
};

typedef light * plight;

// **************************************************************************

class far_light : public light {

   public:
                                                        // superclass
      object_type query_whatami() { return FAR; }
      void preprocess(void *data) { light::preprocess(data); normalize3(vpn); }
      int parse(FILE *infile, char *token);
                                                        // eye
      float zscale(float n, float d) { return n; }
      void screen2map(float *v);
      void map2screen(float *v);
                                                        // light
      void intensity(float *surface, float *gnormal, float *rcolor,
                      eye *parm, shadetype *shade, superclass *source);
      void xform(eye *parm);
      int read_data(FILE *infile, texbase **tptr);
      void write_lst(FILE *outfile, int frameno);
                                                        // far_light
      far_light() {}
};


class point_light : public light {

   public:
                                                        // superclass
      object_type query_whatami() { return POINT; }
      void preprocess(void *data) { light::preprocess(data); }
      int parse(FILE *infile, char *token) { return light::parse(infile, token); }
                                                        // eye
      float zscale(float n, float d) { return -n/d; }
      void screen2map(float *v);
      void map2screen(float *v);
                                                        // light

      void intensity(float *surface, float *gnormal, float *rcolor,
                      eye *parm, shadetype *shade, superclass *source);
      void xform(eye *parm);
      int read_data(FILE *infile, texbase **tptr);
      void write_lst(FILE *outfile, int frameno);
                                                        // point_light
      point_light() {}
};

// **************************************************************************

class spotlight : public light {

   public:
                                                        // superclass
      void      preprocess(void *data);
      int       parse(FILE *infile, char *token);
                                                        // light
      lightclass query_class() { return SPOT_LIGHT; }
      int read_data(FILE *infile, texbase **tptr);
      void      xform(eye *parm);
                                                        // spotlight
      texbase   *tob;
      float     area;

      float length, width, distance2plane;
      char texname[MAXSTRLEN];

      int       maxx, maxy;
      float     ztscale;

      lightbufftype *lbuff;

      spotlight() {
         tob = (texbase *)NULL;
         area   = 1;

         length = width = distance2plane = 1.0;
         texname[0] = 0;

         lbuff   = (lightbufftype *)NULL;
      }

      int       init_buff();
};


class beam_far : public spotlight {

   public:
                                                // superclass
      object_type query_whatami() { return FAR; }
                                                // eye
      float zscale(float n, float d) { return n; }
      void screen2map(float *v);
      void map2screen(float *v);
                                                // light
      void intensity(float *surface, float *gnormal, float *rcolor,
                      eye *parm, shadetype *shade, superclass *source);
      void write_lst(FILE *outfile, int frameno);
                                                // beam_far
      beam_far() {}
};


class beam_point : public spotlight {

   public:
                                                // superclass
      object_type query_whatami() { return POINT; }
                                                // eye
      float zscale(float n, float d) { return -n/d; }
      void screen2map(float *v);
      void map2screen(float *v);
                                                // light
      void intensity(float *surface, float *gnormal, float *rcolor,
                      eye *parm, shadetype *shade, superclass *source);
      void write_lst(FILE *outfile, int frameno);
                                                // beam_point
      beam_point() {}
};


#define simple_light    \
                        \
   float NL;            \
                        \
   NL = dotproduct3(gnormal, vlight);   \
                        \
   if (NL <= 0)         \
      return;           \
                        \
   rcolor[0] += mIp[0]*shade->kp[0]*NL; \
   rcolor[1] += mIp[1]*shade->kp[1]*NL; \
   rcolor[2] += mIp[2]*shade->kp[2]*NL;

class simple_far_light : public far_light {

   protected:
                                                        // light
      void      lighting(float *surface, float *gnormal, float *rcolor,
                          shadetype *shade, float *mIp, float *vlight, float dist2surface);

   public:
                                                        // simple far light
      simple_far_light() {}
};


class simple_point_light : public point_light {

   protected:
                                                        // light
      void lighting(float *surface, float *gnormal, float *rcolor,
        shadetype *shade, float *mIp, float *vlight, float dist2surface);

   public:
                                                        // simple point light
      simple_point_light() {}
};


class simple_beam_far : public beam_far {

   protected:
                                                        // light
      void      lighting(float *surface, float *gnormal, float *rcolor,
                          shadetype *shade, float *mIp, float *vlight, float dist2surface);

   public:
                                                        // simple beam far
      simple_beam_far() {}
};


class simple_beam_point : public beam_point {

   protected:
                                                        // light
      void      lighting(float *surface, float *gnormal, float *rcolor,
                        shadetype *shade, float *mIp, float *vlight, float dist2surface);

   public:
                                                        // simple beam point
      simple_beam_point() {}
};


#endif
