

#if !defined __MEMMAN_H
#define __MEMMAN_H

#include "matrix.h"
#include "map.h"

class pointtype {

   public:
      vector3f point;
      vector2f uv;
      vector3f shadedata;
};


typedef struct edgestruct {             // struct of edge table data
   int      id;                         // polygon #

   pointtype start, end;

   float    dx, dz;
   vector2f dt;                         // dtx/dty, multiscan dtx/dty
   vector4f ds;                         // phong/gouraud delta

   int starty;

   struct edgestruct *next;             // next edge
} edgetype;


typedef edgetype *pedgetype;
typedef edgetype **ppedgetype;

typedef struct lightbuffstruct {

   maplexel lbuff;
   struct lightbuffstruct *next;
} lightbufftype;


typedef enum {EDGETYPE, LIGHTBUFF} memmantype;


class memman {

   protected:
      edgetype *edgetype_list;
      lightbufftype *lightbuff_list;

   public:
      memman();
      ~memman();

      void *pop(memmantype item);
      void push(memmantype item, void *mem);
};

extern memman control;

#endif

