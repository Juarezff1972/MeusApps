



/* *************************************************************
   This file handles all the matrix/vector/math operations

   Reminder: Spherical Linear intERPolation (SLERP) can be
             done by splining the quaternions

************************************************************* */

#include <string.h>
#include "matrix.h"

/* *************************************************************
************************************************************* */
int similar3(float *x, float *y) {

   register float temp;

   temp = x[0] - y[0];
   if (abs(temp) > CORRECT)
      return 0;

   temp = x[1] - y[1];
   if (abs(temp) > CORRECT)
      return 0;

   temp = x[2] - y[2];
   return (abs(temp) <= CORRECT);
}


/* *************************************************************
   This procedure normalizes a vector and returns its magnitude
************************************************************* */
float normalize2(float x[]) {

   register float mag, imag;

   mag = magnitude2(x);

   if (mag) {
      imag = 1.0/mag;
      x[0] *= imag;
      x[1] *= imag;
   }

   return mag;
}


/* *************************************************************
   This procedure normalizes a vector and returns its magnitude
************************************************************* */
float normalize3(float x[]) {

   register float mag, imag;

   mag = magnitude3(x);

   if (mag) {
      imag = 1.0/mag;
      smultarray3(x, imag);
   }

   return mag;
}


/* *************************************************************
   This procedure normalizes a 4D vector and returns its magnitude
************************************************************* */
float normalize4(float x[]) {

   register float mag, imag;

   mag = magnitude4(x);

   if (mag) {
      imag = 1.0/mag;
      smultarray4(x, imag);
   }

   return mag;
}


/* *************************************************************
   This procedure normalizes a vector and returns its magnitude
************************************************************* */
void xproduct(float *a, float *x, float *y) {

   a[0] = x[1]*y[2] - x[2]*y[1];
   a[1] = x[2]*y[0] - x[0]*y[2];
   a[2] = x[0]*y[1] - x[1]*y[0];
}


/* *************************************************************
   this function calculates the transpose of a matrix
************************************************************* */
void transpose(float r[][4], float s[][4]) {

   register int    i, j;

   for (i=0; i<4; i++)
      for (j=0; j<4; j++)
         r[j][i] = s[i][j];
}


/* *************************************************************
   this function calculates the transpose of a matrix
************************************************************* */
void transpose(float r[][4]) {

   register int    i, j;
   register float  s;

   for (i=0; i<4; i++)
      for (j=0; j<i; j++) {
         s = r[j][i];
         r[j][i] = r[i][j];
         r[i][j] = s;
      }

}


/* *************************************************************
   This procedure multiplies 2 4X4 matricies
************************************************************* */
void matmatmult(float y[][4], float x[][4]) {

   vector4f z[4];
   register int    i, j;

   copymx4x4(z, x);

   for (i=0; i<4; i++)
      for (j=0; j<4; j++)
         x[i][j] = y[i][0]*z[0][j] + y[i][1]*z[1][j] + y[i][2]*z[2][j] + y[i][3]*z[3][j];
}
//
/* *************************************************************
   This procedure multiplies a 4X4 matrix with a vector
************************************************************* */
void matvecmult(float y[][4], float x[]) {

   vector4f z;

   copyarray4(z, x);

   x[0] = dotproduct4(y[0], z);
   x[1] = dotproduct4(y[1], z);
   x[2] = dotproduct4(y[2], z);
   x[3] = dotproduct4(y[3], z);
}


/* *************************************************************
   This function multiplies a vector . matrix
************************************************************* */
void vecmatmult(float y[][4], float x[]) {

   vector4f z;

   copyarray4(z, x);

   x[0] = y[0][0]*z[0] + y[1][0]*z[1] + y[2][0]*z[2] + y[3][0]*z[3];
   x[1] = y[0][1]*z[0] + y[1][1]*z[1] + y[2][1]*z[2] + y[3][1]*z[3];
   x[2] = y[0][2]*z[0] + y[1][2]*z[1] + y[2][2]*z[2] + y[3][2]*z[3];
   x[3] = y[0][3]*z[0] + y[1][3]*z[1] + y[2][3]*z[2] + y[3][3]*z[3];
}


/* *************************************************************
   This procedure multiplies 2 4X4 matricies
************************************************************* */
void matmatmulto(float y[][4], float x[][4]) {

   vector4f z[3];
   register int i;

   copyarray4(z[0], x[0]);
   copyarray4(z[1], x[1]);
   copyarray4(z[2], x[2]);

   for (i=0; i<3; i++) {
      x[i][0] = y[i][0]*z[0][0] + y[i][1]*z[1][0] + y[i][2]*z[2][0];
      x[i][1] = y[i][0]*z[0][1] + y[i][1]*z[1][1] + y[i][2]*z[2][1];
      x[i][2] = y[i][0]*z[0][2] + y[i][1]*z[1][2] + y[i][2]*z[2][2];
      x[i][3] = y[i][0]*z[0][3] + y[i][1]*z[1][3] + y[i][2]*z[2][3] + y[i][3];
   }

}


/* *************************************************************
   This procedure multiplies a 4X4 matrix with a vector
************************************************************* */
void matvecmultv(float y[][4], float x[]) {

   vector3f z;

   copyarray3(z, x);

   x[0] = dotproduct3(y[0], z);
   x[1] = dotproduct3(y[1], z);
   x[2] = dotproduct3(y[2], z);
}


/* *************************************************************
   This procedure multiplies a 4X4 matrix with a vector
************************************************************* */
void matvecmulto(float y[][4], float x[]) {

   vector3f z;

   copyarray3(z, x);

   x[0] = dotproduct3(y[0], z) + y[0][3];
   x[1] = dotproduct3(y[1], z) + y[1][3];
   x[2] = dotproduct3(y[2], z) + y[2][3];
}


/* *************************************************************
   This function multiplies a vector . matrix
************************************************************* */
void vecmatmultv(float y[][4], float x[]) {

   vector3f z;

   copyarray3(z, x);

   x[0] = y[0][0]*z[0] + y[1][0]*z[1] + y[2][0]*z[2];
   x[1] = y[0][1]*z[0] + y[1][1]*z[1] + y[2][1]*z[2];
   x[2] = y[0][2]*z[0] + y[1][2]*z[1] + y[2][2]*z[2];
}


/* *************************************************************
   This function multiplies a vector . matrix
************************************************************* */
void vecmatmulto(float y[][4], float x[]) {

   vector3f z;

   copyarray3(z, x);

   x[0] = y[0][0]*z[0] + y[1][0]*z[1] + y[2][0]*z[2];
   x[1] = y[0][1]*z[0] + y[1][1]*z[1] + y[2][1]*z[2];
   x[2] = y[0][2]*z[0] + y[1][2]*z[1] + y[2][2]*z[2];
   x[3] = y[0][3]*z[0] + y[1][3]*z[1] + y[2][3]*z[2] + y[3][3];
}


/* *************************************************************
   this procedure swaps two vectors
************************************************************* */
void swap4(float mx1[], float mx2[]) {

   float temp;
   register int i;

   for (i=0; i<4; i++) {
      temp   = mx1[i];
      mx1[i] = mx2[i];
      mx2[i] = temp;
   }

}
// 
/* *************************************************************
   this procedure calculates the inverse of a matrix
************************************************************* */
void inversemxrt(float mx[][4], float mxinverse[][4]) {

   vector4f temp;

   transpose(mxinverse, mx);

   copyarray4(temp, mxinverse[3]);

   mxinverse[3][0] = mxinverse[3][1] = mxinverse[3][2] = 0.0;

   matvecmulto(mxinverse, temp);

   mxinverse[0][3] = -temp[0];
   mxinverse[1][3] = -temp[1];
   mxinverse[2][3] = -temp[2];
}



/* *************************************************************
   this procedure calculates the inverse of a matrix
************************************************************* */
int inversemx(float a[][4],float b[][4]) {

   register double det, idet;

   b[0][0] = a[1][1]*a[2][2] - a[2][1]*a[1][2];
   b[0][1] = a[0][1]*a[2][2] - a[2][1]*a[0][2];
   b[0][2] = a[0][1]*a[1][2] - a[1][1]*a[0][2];

   b[1][0] = a[1][0]*a[2][2] - a[2][0]*a[1][2];
   b[1][1] = a[0][0]*a[2][2] - a[2][0]*a[0][2];
   b[1][2] = a[0][0]*a[1][2] - a[1][0]*a[0][2];

   b[2][0] = a[1][0]*a[2][1] - a[2][0]*a[1][1];
   b[2][1] = a[0][0]*a[2][1] - a[2][0]*a[0][1];
   b[2][2] = a[0][0]*a[1][1] - a[1][0]*a[0][1];

   det = a[0][0]*b[0][0] - a[1][0]*b[0][1] + a[2][0]*b[0][2];

   if (abs(det) < CORRECT)
      return inversemx2(a, b);

   det = 1.0/det;
   idet = -det;

   b[0][0] *= det;
   b[0][1] *= idet;
   b[0][2] *= det;

   b[1][0] *= idet;
   b[1][1] *= det;
   b[1][2] *= idet;

   b[2][0] *= det;
   b[2][1] *= idet;
   b[2][2] *= det;

   b[0][3] = -(b[0][0]*a[0][3] + b[0][1]*a[1][3] + b[0][2]*a[2][3]);
   b[1][3] = -(b[1][0]*a[0][3] + b[1][1]*a[1][3] + b[1][2]*a[2][3]);
   b[2][3] = -(b[2][0]*a[0][3] + b[2][1]*a[1][3] + b[2][2]*a[2][3]);

   b[3][0] = b[3][1] = b[3][2] = 0.0;
   b[3][3] = 1;

   return 1;
}


/* *************************************************************
   this procedure calculates the inverse of a matrix
************************************************************* */
int inversemx2(float mx[][4], float mxinverse[][4]) {

   vector4f tempmx[4];
   register int i, j, k;
   register float temp;

   init_mx(mxinverse);

   copymx4x4(tempmx, mx);

   for (i=0; i<4; i++) {
      if (!not_zero(tempmx[i][i]))
         for (j=i+1; (j<4 && !not_zero(tempmx[i][i])); j++)
            if (tempmx[j][i]) {
               swap4(tempmx[i], tempmx[j]);
               swap4(mxinverse[i], mxinverse[j]);
            }

      if (!not_zero(tempmx[i][i]))
         return 0;

      temp = 1.0/tempmx[i][i];

      for (j=i+1; j<4; j++)
         tempmx[i][j] *= temp;

      smultarray4(mxinverse[i], temp);

      for (j=i+1; j<4; j++) {
         for (k=i+1; k<4; k++)
            tempmx[j][k] -= tempmx[j][i]*tempmx[i][k];

         mxinverse[j][0] -= tempmx[j][i]*mxinverse[i][0];
         mxinverse[j][1] -= tempmx[j][i]*mxinverse[i][1];
         mxinverse[j][2] -= tempmx[j][i]*mxinverse[i][2];
         mxinverse[j][3] -= tempmx[j][i]*mxinverse[i][3];
      }

   }

   for (i=3; i>-1; i--)
      for (j=i-1; j>-1; j--) {
         mxinverse[j][0] -= tempmx[j][i]*mxinverse[i][0];
         mxinverse[j][1] -= tempmx[j][i]*mxinverse[i][1];
         mxinverse[j][2] -= tempmx[j][i]*mxinverse[i][2];
         mxinverse[j][3] -= tempmx[j][i]*mxinverse[i][3];
      }

   return 1;
}

