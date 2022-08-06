



/* *************************************************************
   This file handles all the matrix/vector/math operations

   Reminder: Spherical Linear intERPolation (SLERP) can be
             done by splining the quaternions

************************************************************* */

#include <string.h>
#include "matrix.h"

float *cos_table = (float *)NULL;
float *sin_table = (float *)NULL;
float *asin_table = (float *)NULL;

double trigfactor = MAX_TRIG_SIZE/TWOPI;
double atrigfactor = MAX_TRIG_SIZE*0.5;

void init_sin_cos() {

   register int i;

   if (sin_table != (float *)NULL)
      delete [] sin_table;

   sin_table = new float[MAX_SIN_COS_TBL_SIZE];

   cos_table = &sin_table[COS_START];

   for (i=0 ; i <COS_START; i++)
      sin_table[i] = (float)sin((i*TWOPI)/MAX_TRIG_SIZE_PLUS);

   for (i=0 ; i <MAX_TRIG_SIZE_PLUS; i++)
      cos_table[i] =  (float)cos((i*TWOPI)/MAX_TRIG_SIZE_PLUS);
}


void init_asin_acos() {

   register int i;

   if (asin_table != (float *)NULL)
      delete [] asin_table;

   asin_table = new float[MAX_TRIG_SIZE_PLUS];

   for (i=0 ; i <MAX_TRIG_SIZE_PLUS; i++)
      asin_table[i] = (float)asin((((float)(i+i))/MAX_TRIG_SIZE_PLUS) - 1);
}


// ***************************************************************************

int mantissa_lut[65536];
int imantissa_lut[65536];
int bit_table[2] = { 0, 0x8000 };

void init_sqrt() {

   float   x;
   int     i;
   int     *bits = (int *)&x;

   for (i = 0; i < 32768; i++)
   {
		// exponent 0
	  *bits = (i | (0x7F<<15)) << 8;
      x = (float)sqrt(x);
      mantissa_lut[i] = *bits & 0x7FFF00;

		// exponent 1
      *bits = (i | (1<<22)) << 8;
      x = (float)sqrt(x);
      mantissa_lut[i + 32768] = *bits & 0x7FFF00;

      *bits = (i | (0x7F<<15)) << 8;
      x = 1.0f / (float)sqrt(x);
      imantissa_lut[i] = *bits & 0x7FFF00;

      *bits = (i |(1<<22)) << 8;
      x = 1.0f / (float)sqrt(x);
      imantissa_lut[i + 32768] = *bits & 0x7FFF00;
   }

}


// ***************************************************************************
unsigned char byte_lut[1024];
unsigned char *pbyte_lut = &byte_lut[128];

void init_byte() {

   unsigned int i;

   memset(byte_lut, 0, 128);

   for (i=0; i<256; i++)
      pbyte_lut[i] = (unsigned char)i;

   memset(&byte_lut[384], 255, 640);
}

// ***************************************************************************
float t_0_255_0_1[256];
float *table_0_255_0_1 = t_0_255_0_1;

void init_0_255_0_1() {

   unsigned int i;
   for (i=0; i<256; i++)
      table_0_255_0_1[i] = i/255.0f;
}


// ***************************************************************************

float pow_lut[MAX_POW_N*(MAX_POW_X+1)];
float *ppow_lut[MAX_POW_N];

void init_pow() {

   unsigned int i, j;

   for (i=j=0; i<MAX_POW_N; i++, j+= MAX_POW_X+1)
      ppow_lut[i] = &pow_lut[j];

   for (i=0; i<MAX_POW_N; i++)
      for (j=0; j<=MAX_POW_X; j++)
         ppow_lut[i][j] = pow((float)(j*0.01), (float)(i+1));
}

// ***************************************************************************

void init_lut(int fmask) {

   if (fmask & MASK_SIN_COS)
      init_sin_cos();

   if (fmask & MASK_ASIN_ACOS)
      init_asin_acos();

   if (fmask & MASK_SQRT)
      init_sqrt();

   if (fmask & MASK_BYTE)
      init_byte();

   if (fmask & MASK_POW)
      init_pow();

   if (fmask & MASK_0_255_0_1)
      init_0_255_0_1();
}