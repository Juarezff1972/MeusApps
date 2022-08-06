




#include "base.h"

#if !defined __SHADER_H
#define __SHADER_H

#include <string.h>

typedef enum {

   SURFACE    = 0,
   ATMOSPHERE = 1,
   DISPLACEMENT = 2
} shader_type;

class pc;

class shaderparamtype {

   public:
      vector4f in;              // pt in local coordinates
      vector4f normal;          // normal in world coordinates
      vector4f pt;              // pt in world coordinates (out)
      vector4f xform[4], ixform[4];
      int   frame;

      eye    *cam;
      pc     *ob;

      fractal_params params;
      float min, max;

      vector3f out;             // out color
      vector3f add;             // out additional color

      shaderparamtype() {
         params.H = params.lacunarity = params.octaves = 0;
         params.freq = (float *)NULL;
         cam = (eye *)NULL;
         ob = (pc *)NULL;
         min = max = 0;
      }

      ~shaderparamtype () {
         if (params.freq != (float *)NULL)
            delete [] params.freq;
      }

      void setup(vector4f *xf, vector4f *ixf, int f, eye *c, pc *o, float fscale);
      void set_point(float *local, float *n);
};


class shader {

   protected:
      void *shade_fcn, *init_fcn;
      shader *left, *right;
      int initflag;

   public:
      char name[MAXSTRLEN];

      shader()  { name[0] = 0; left = right = (shader *)NULL; initflag = 1; }
      ~shader() { if (left != (shader *)NULL) delete left; if (right != (shader *)NULL) delete right; }

      shader    *find(char *target);
      void      insert(shader *x);

#ifdef LINUX
      int       set(char *tmpstr, int func_call(void *), void init_call() = (void *)NULL);
#else
      int       set(char *tmpstr, void *func_call, void *init_call = (void *)NULL);
#endif

      int       query_data(void *x) { return ((int (*)(void *))shade_fcn)(x); }
      void      init() {
                   if (!initflag)
                      return;

                   initflag = 0;

                   if (init_fcn != (void *)NULL)
                      ((void (*)())init_fcn)();

      }

};


typedef shader *pshader;

class shadertype {

   public:
      shader **s;
      shader_type *stype;

      int scount;

      shadertype() { s = (shader **)NULL; stype = (shader_type *)NULL; scount = 0; }
      ~shadertype() { if (s != (shader **)NULL) delete [] s; if (stype != (shader_type)NULL) delete [] stype; }
      void read_data(FILE *infile, shader *sptr);

      shadertype &operator = (shadertype &x);
};


class shaderlisttype {

   public:
      shadertype        *shade;
      float             scale;
      char              shname[MAXSTRLEN];            // illumination file
      char              dataname[MAXSTRLEN];          // object file
      int               scount;
      shaderlisttype    *next;

      shaderlisttype() { shade = (shadertype *)NULL; scale = 1; }
      ~shaderlisttype() { if (shade != (shadertype *)NULL) delete [] shade; }

      void init(int x) {

         if (shade != (shadertype *)NULL)
            delete shade;
         shade = new shadertype[x];
         scount = x;
      }

};


void init_shader();

extern shader *shades;

#endif

