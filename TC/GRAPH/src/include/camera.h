



#if !defined __CAMERA_H
#define __CAMERA_H


#include "eye.h"


/* *************************************************************
************************************************************* */


class camera : public eye {

   public:
					// superclass
      sc_type query_category() { return CAMERA_CLASS; }
      object_type query_whatami() { return CAMERA; }
      int  parse(FILE *infile, char *token);
      void preprocess(void *data);

					// eye
      float zscale(float n, float d) { return -n/d; }

      void map2screen(float *v) {
         v[0] = -mscale * (v[0]/v[2] + vrc[0]);
         v[1] = -mscale * (v[1]/v[2] + vrc[2]);
      }

      void screen2map(float *v) {
         v[0] = -(v[0]*imscale + vrc[0]) * v[2];
         v[1] = -(v[1]*imscale + vrc[2]) * v[2];
      }

					// camera
      vector3f coa;			// center of attention

      camera() {
	 copyarray3(coa, location);
         coa[2] -= 1;

         next = (camera *)NULL;
      }

      camera(int x, int y);

      void  read_def(FILE *infile, unsigned int *x, unsigned int *y);
      void  read_data(FILE *infile);
      void  write_lst(FILE *outfile, int frameno);
      void  dump();
};


typedef camera *pcamera;

#endif

