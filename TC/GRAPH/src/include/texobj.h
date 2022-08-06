



#if !defined __TEXOBJ_H
#define __TEXOBJ_H

#include "base.h"
#include "datatype.h"

class texface {

   public:
      texbase  *ob;
      vector2f *uv;
      int      count;
      float    area;

      texface() {
         uv = (vector2f *)NULL;
      }

      ~texface() {
         if (uv != (vector2f *)NULL)
            delete [] uv;
      }

      void setup(int i);
      void calc_area();
      void make_unit();
};


class texpolygon : public texmap {

   public:
      int       countobject;
      texface   *data;

      texpolygon() {
         data = (texface *)NULL;
      }

      ~texpolygon() {
         if (data != (texface *)NULL)
            delete [] data;

      }

      int query_tflag(int i) { return (data[i].ob != (texbase *)NULL); }
      void query_uvmap(int i, int j, float *uv) { copyarray2(uv, data[i].uv[j]); }

      void setup(int i);
      unsigned int query_texel(int face, float x, float y, float *color, int frame, float d);

      void perim2Dmap(int face, int *corner, texbase *tob, polytype *dob);
      void direct2Dmap(int face, vector2f *uv, texbase *tob, int vcount);
      void affinemap(int face, vector2f *uv, texbase *tob, polytype *dob);
};


/* ****************************************************************
**************************************************************** */


class sbmap {

   public:
      texbase  *ob;
      int      repeatflag;
      float    xscale, yscale;
      float    min[2], max[2];
      int      flaginv;
      float    xsign, ysign;
      float    area;
      void     setup(vector4f *pt);
      unsigned int query_texel(float x, float y, float *c, int frame, float d);
};


class texsbfast : public texmap {

   public:
      int       countobject;
      sbmap     *data;

      texsbfast() {
         data  = (sbmap *)NULL;
      }

      ~texsbfast() {
         if (data != (sbmap *)NULL)
            delete [] data;
      }

      void setup(int x) {

         if (data != (sbmap *)NULL)
            delete [] data;

         data = new sbmap[x];
      }

      int query_texel(float x, float y, float *c, int frame, float d);
};


#endif
