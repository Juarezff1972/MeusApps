


#include "matrix.h"

/* *************************************************************

  function: cylinder_area - returns the surface area of a cylinder

  in:	radius - radius of the cylinder
		length - length of the cylinder

  out:

  return: surface area
************************************************************* */
float cylinder_area(float radius, float length) {

   register float temp;

   temp = (float)(PI*radius*length);
   return temp + temp;
}


/* *************************************************************

  function:	make_box - make a bounding box out of min/max extents:

  in: in - 2 vector4f arrays containing min, max extents

  out: out - 8 vector4f verticies of the bounding box

  return:
************************************************************* */
void make_box(vector4f *in, vector4f *out) {

	float *box;

	box = &out[0][0];

   box[0]  = box[4]  = box[16] = box[20] = in[0][0];
   box[1]  = box[13] = box[17] = box[29] = in[0][1];
   box[2]  = box[6]  = box[10] = box[14] = in[1][2];
   box[5]  = box[9]  = box[21] = box[25] = in[1][1];
   box[8]  = box[12] = box[24] = box[28] = in[1][0];
   box[18] = box[22] = box[26] = box[30] = in[0][2];
   box[31] = box[27] = box[23] = box[19] = box[15] = box[11] = box[7] = box[3] = 1.0f;
}

