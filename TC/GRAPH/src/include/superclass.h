
#include <stdio.h>

#if !defined __SUPERCLASS_H
#define __SUPERCLASS_H

typedef enum sc_enum { LIGHT_CLASS, CAMERA_CLASS, OBJECT_CLASS } sc_type;

typedef enum object_enum {
        CAMERA, POINT, FAR, BASENULL, POLYGON, SPATCH,
        PARTICLE, LINE, RADIANT, SPHERE, CYLINDER, CONE, SBFAST
} object_type;

class superclass {

   public:
      sc_type category;
      int frame;
      superclass *next;

      superclass() {
         frame = 0;
         next = (superclass *)NULL;
      }

      virtual sc_type query_category() = 0;
      virtual object_type query_whatami() = 0;
      virtual int parse(FILE *infile, char *token);
      virtual void  preprocess(void *data);
};

#endif

