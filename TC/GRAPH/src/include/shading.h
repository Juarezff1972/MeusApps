



/* *************************************************************
   This file contains all the global data types for my programs
************************************************************* */

#if !defined (__SHADING_H)
#define __SHADING_H

#include "matrix.h"
#include "pstring.h"

class shadetype {

   public:
      int index;
      vector3f ka, kp, ks;
      float specn;                        // power of cosine
      vector3f lum;

      shadetype();

      void read_data(FILE *infile);
      void write_data(FILE *outfile);
};

typedef shadetype * pshadetype;
typedef shadetype ** ppshadetype;


class illtype {

   public:
      shadetype *color;
      vector3f ambient;

      illtype operator = (shadetype *x);
};

typedef illtype *pilltype;


class texcolortype {

   public:

      shadetype color;
      vector3f  ambient;

      texcolortype operator = (illtype &x);
      texcolortype operator = (shadetype &x);
};


class shadelist {

   public:
      char shadename[MAXSTRLEN];

      shadetype base;

      int count;
      shadetype *list;

      shadelist *next;

      shadelist() {

         list = (shadetype *)NULL;
         next = (shadelist *)NULL;
      }

      ~shadelist() {

         if (list != (shadetype *)NULL)
            delete [] list;

         if (next != (shadelist *)NULL)
            delete next;
      }

      int read_data(char *filename);
      int write_data(char *filename);
};


#endif
