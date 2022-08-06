



/* *************************************************************
   This file handles all the matrix/vector/math operations

   Reminder: Spherical Linear intERPolation (SLERP) can be
             done by splining the quaternions

************************************************************* */

#include "matrix.h"

vector4f MCR[4]  = { -0.5,      1.5,     -1.5,      0.5,
                      1.0,     -2.5,      2.0,     -0.5,
                     -0.5,      0.0,      0.5,      0.0,
                      0.0,      1.0,      0.0,      0.0 };     // catmull-rom

vector4f MCRT[4] = { -0.5,      1.0,     -0.5,      0.0,       // Tcatmull-rom
                      1.5,     -2.5,      0.0,      1.0,
                     -1.5,      2.0,      0.5,      0.0,
                      0.5,     -0.5,      0.0,      0.0 };

vector4f MB[4]   = { -1.0/6.0,  0.5,     -0.5,      1.0/6.0,   // B-spline
                      0.5,     -1.0,      0.5,      0.0,
                     -0.5,      0.0,      0.5,      0.0,
                      1.0/6.0,  2.0/3.0,  1.0/6.0,  0.0 };

vector4f MBT[4]  = { -1.0/6.0,   0.5,    -0.5,       1.0/6.0,   // TB-spline
                     0.5,      -1.0,     0.0,       2.0/3.0,
                    -0.5,       0.5,     0.5,       1.0/6.0,
                     1.0/6.0,   0.0,     0.0,       0.0 };

/* *************************************************************
   this function calculates the current value of a surface patch
   spline - s is dy, t is dx
************************************************************* */
void sppatch(float s, float t, float in[][4], float out[], int ptr[], int stype) {

   vector4f T, S, temp;
   vector4f Q[4][4];
   int  i, j, k, l;

   for (j=0; j<4; j++)
      for (k=0; k<4; k++)
         for (l=0, i=ptr[k]; l<4; l++, i+=4)
            Q[j][k][l] = in[i][j];

   T[3] = 1;
   T[2] = t;
   T[1] = T[2]*T[2];
   T[0] = T[1]*T[2];

   S[3] = 1;
   S[2] = s;
   S[1] = S[2]*S[2];
   S[0] = S[1]*S[2];

   if (stype == M_CR) {
      matvecmult(MCRT, T);
      vecmatmult(MCR, S);
   }

   else {
      matvecmult(MBT, T);
      vecmatmult(MB, S);
   }

   for (j=0; j<4; j++) {
      temp[0] = dotproduct4(Q[j][0], T);
      temp[1] = dotproduct4(Q[j][1], T);
      temp[2] = dotproduct4(Q[j][2], T);
      temp[3] = dotproduct4(Q[j][3], T);
      out[j] = dotproduct4(S, temp);
   }

}


/* *************************************************************
   this function calculates the current value of a surface patch
   spline - s is dy, t is dx
************************************************************* */
void sppatch(float s, float t, vector4f **in, float *out, int stype) {

   vector4f T, S, temp;
   vector4f Q[4][4];
   int  j, k, l;

   for (j=0; j<4; j++)
      for (k=0; k<4; k++)
         for (l=0; l<4; l++)
            Q[j][k][l] = in[k][l][j];

   T[3] = 1;
   T[2] = t;
   T[1] = T[2]*T[2];
   T[0] = T[1]*T[2];

   S[3] = 1;
   S[2] = s;
   S[1] = S[2]*S[2];
   S[0] = S[1]*S[2];

   if (stype == M_CR) {
      matvecmult(MCRT, T);
      vecmatmult(MCR, S);
   }

   else {
      matvecmult(MBT, T);
      vecmatmult(MB, S);
   }

   for (j=0; j<4; j++) {
      temp[0] = dotproduct4(Q[j][0], T);
      temp[1] = dotproduct4(Q[j][1], T);
      temp[2] = dotproduct4(Q[j][2], T);
      temp[3] = dotproduct4(Q[j][3], T);
      out[j] = dotproduct4(S, temp);
   }

}


/* *************************************************************
   this function calculates the current value of a surface patch
   spline - s is dy, t is dx
************************************************************* */
void sppatch(float s, float t, float Q[][4][4], float *out, int stype) {

   vector4f T, S, temp;
   int  j;

   T[3] = 1;
   T[2] = t;
   T[1] = T[2]*T[2];
   T[0] = T[1]*T[2];

   S[3] = 1;
   S[2] = s;
   S[1] = S[2]*S[2];
   S[0] = S[1]*S[2];

   if (stype == M_CR) {
      matvecmult(MCRT, T);
      vecmatmult(MCR, S);
   }

   else {
      matvecmult(MBT, T);
      vecmatmult(MB, S);
   }

   for (j=0; j<4; j++) {
      temp[0] = dotproduct4(Q[j][0], T);
      temp[1] = dotproduct4(Q[j][1], T);
      temp[2] = dotproduct4(Q[j][2], T);
      temp[3] = dotproduct4(Q[j][3], T);
      out[j] = dotproduct4(S, temp);
   }

}
//


/* *************************************************************
   This function calculates the patch normals
************************************************************* */
void spnormal(float s, float t, float in[][4], float out[], int ptr[], int stype) {

   vector4f ds, dt;
   vector4f S, T;
   vector4f Q[4][4];
   vector4f temp1, temp2;
   vector3f pt1, pt2;
   int   i, j, k, l;

   for (j=0; j<4; j++)
      for (k=0; k<4; k++)
         for (l=0, i=ptr[k]; l<4; l++, i+=4)
            Q[j][k][l] = in[i][j];

   T[3] = 1;
   T[2] = t;
   T[1] = SQR(T[2]);
   T[0] = T[1]*T[2];

   S[3] = 1;
   S[2] = s;
   S[1] = SQR(S[2]);
   S[0] = S[1]*S[2];

   ds[3] = 0;
   ds[2] = 1;
   ds[1] = s+s;
   ds[0] = 3*SQR(s);

   dt[3] = 0;
   dt[2] = 1;
   dt[1] = t+t;
   dt[0] = 3*SQR(t);

   if (stype == M_CR) {
      matvecmult(MCRT, T);
      vecmatmult(MCR, S);

      matvecmult(MCRT, dt);
      vecmatmult(MCR, ds);
   }

   else {
      matvecmult(MBT, T);
      vecmatmult(MB, S);

      matvecmult(MBT, dt);
      vecmatmult(MB, ds);
   }

   for (i=0; i<3; i++) {
      temp1[0] = dotproduct4(Q[i][0], T);
      temp1[1] = dotproduct4(Q[i][1], T);
      temp1[2] = dotproduct4(Q[i][2], T);
      temp1[3] = dotproduct4(Q[i][3], T);
   
      temp2[0] = dotproduct4(Q[i][0], dt);
      temp2[1] = dotproduct4(Q[i][1], dt);
      temp2[2] = dotproduct4(Q[i][2], dt);
      temp2[3] = dotproduct4(Q[i][3], dt);

      pt1[i] = dotproduct4(ds, temp1);
      pt2[i] = dotproduct4(S, temp2);
   }
      
   xproduct(out, pt1, pt2);
   normalize3(out);
}


/* *************************************************************
   This function calculates the patch normals
************************************************************* */
void spnormal(float s, float t, vector4f **in, float *out, int stype) {

   vector4f ds, dt;
   vector4f S, T;
   vector4f Q[4][4];
   vector4f temp1, temp2;
   vector3f pt1, pt2;
   int j, k, l;

   for (j=0; j<4; j++)
      for (k=0; k<4; k++)
         for (l=0; l<4; l++)
            Q[j][k][l] = in[k][l][j];

   T[3] = 1;
   T[2] = t;
   T[1] = SQR(t);
   T[0] = T[1]*T[2];

   S[3] = 1;
   S[2] = s;
   S[1] = SQR(s);
   S[0] = S[1]*S[2];

   ds[3] = 0;
   ds[2] = 1;
   ds[1] = s+s;
   ds[0] = 3*S[1];

   dt[3] = 0;
   dt[2] = 1;
   dt[1] = t+t;
   dt[0] = 3*T[1];

   if (stype == M_CR) {
      matvecmult(MCRT, T);
      vecmatmult(MCR, S);

      matvecmult(MCRT, dt);
      vecmatmult(MCR, ds);
   }

   else {
      matvecmult(MBT, T);
      vecmatmult(MB, S);

      matvecmult(MBT, dt);
      vecmatmult(MB, ds);
   }

   for (k=0; k<3; k++) {
      temp1[0] = dotproduct4(Q[k][0], T);
      temp1[1] = dotproduct4(Q[k][1], T);
      temp1[2] = dotproduct4(Q[k][2], T);
      temp1[3] = dotproduct4(Q[k][3], T);
   
      temp2[0] = dotproduct4(Q[k][0], dt);
      temp2[1] = dotproduct4(Q[k][1], dt);
      temp2[2] = dotproduct4(Q[k][2], dt);
      temp2[3] = dotproduct4(Q[k][3], dt);
   
      pt1[k] = dotproduct4(ds, temp1);
      pt2[k] = dotproduct4(S, temp2);
   }
   
   xproduct(out, pt1, pt2);
   normalize3(out);
}


/* *************************************************************
   This function calculates the patch normals
************************************************************* */
void spnormal(float s, float t, float Q[][4][4], float *out, int stype) {

   vector4f ds, dt;
   vector4f S, T;
   vector4f temp1, temp2;
   vector3f pt1, pt2;
   int k;

   T[3] = 1;
   T[2] = t;
   T[1] = SQR(t);
   T[0] = T[1]*T[2];

   S[3] = 1;
   S[2] = s;
   S[1] = SQR(s);
   S[0] = S[1]*S[2];

   ds[3] = 0;
   ds[2] = 1;
   ds[1] = s+s;
   ds[0] = 3*S[1];

   dt[3] = 0;
   dt[2] = 1;
   dt[1] = t+t;
   dt[0] = 3*T[1];

   if (stype == M_CR) {
      matvecmult(MCRT, T);
      vecmatmult(MCR, S);

      matvecmult(MCRT, dt);
      vecmatmult(MCR, ds);
   }

   else {
      matvecmult(MBT, T);
      vecmatmult(MB, S);

      matvecmult(MBT, dt);
      vecmatmult(MB, ds);
   }

   for (k=0; k<3; k++) {
      temp1[0] = dotproduct4(Q[k][0], T);
      temp1[1] = dotproduct4(Q[k][1], T);
      temp1[2] = dotproduct4(Q[k][2], T);
      temp1[3] = dotproduct4(Q[k][3], T);
   
      temp2[0] = dotproduct4(Q[k][0], dt);
      temp2[1] = dotproduct4(Q[k][1], dt);
      temp2[2] = dotproduct4(Q[k][2], dt);
      temp2[3] = dotproduct4(Q[k][3], dt);
   
      pt1[k] = dotproduct4(ds, temp1);
      pt2[k] = dotproduct4(S, temp2);
   }
   
   xproduct(out, pt1, pt2);
   normalize3(out);
}


/* *************************************************************
   This function calculates the normal at s = t = 0
************************************************************* */
void spnormal_base(vector4f **in, float out[], int stype) {

   vector4f S, T;
   vector4f Q[4][4];
   vector4f temp1, temp2;
   vector3f pt1, pt2;
   int   i, j, k, l;

   if (stype == M_CR) {
      pt1[0] = -in[0][1][0] + in[2][1][0];
      pt1[1] = -in[0][1][1] + in[2][1][1];
      pt1[2] = -in[0][1][2] + in[2][1][2];

      pt2[0] = -in[1][0][0] + in[1][2][0];
      pt2[1] = -in[1][0][1] + in[1][2][1];
      pt2[2] = -in[1][0][2] + in[1][2][2];
   }

   else {
      for (j=0; j<4; j++)
         for (k=0; k<4; k++)
            for (l=0; l<4; l++)
               Q[j][k][l] = in[k][l][j];

      S[0] = T[0] = MB[3][0];
      S[1] = T[1] = MB[3][1];
      S[2] = T[2] = MB[3][2];

      for (i=0; i<3; i++) {
         temp1[0] = dotproduct3(Q[i][0], T);
         temp1[2] = dotproduct3(Q[i][2], T);

         temp2[0] = 0.5f*(-Q[i][0][0] + Q[i][0][2]);
         temp2[1] = 0.5f*(-Q[i][1][0] + Q[i][1][2]);
         temp2[2] = 0.5f*(-Q[i][2][0] + Q[i][2][2]);

         pt1[i] = 0.5f*(-temp1[0] + temp1[2]);
         pt2[i] = dotproduct3(S, temp2);
      }

   }

   xproduct(out, pt1, pt2);
   normalize3(out);
}


/* *************************************************************
   This procedure calculates the current placement of objects
   using Catmull-Rom or B splines
************************************************************* */
void spline(float s, float in[][4], float out[], int i, int stype) {

   vector4f Q[4];
   vector4f T;
   int j, k;

   for (j=0; j<4; j++)
      for (k=0; k<4; k++)
         Q[j][k] = in[i+k][j];

   T[3] = 1;
   T[2] = s;
   T[1] = T[2]*T[2];
   T[0] = T[1]*T[2];

   if (stype == M_CR) {                           // Catmull-Rom spline
      matvecmult(MCR, Q[0]);
      out[0] = dotproduct4(T, Q[0]);
      matvecmult(MCR, Q[1]);
      out[1] = dotproduct4(T, Q[1]);
      matvecmult(MCR, Q[2]);
      out[2] = dotproduct4(T, Q[2]);
      matvecmult(MCR, Q[3]);
      out[3] = dotproduct4(T, Q[3]);

      return;
   }

   matvecmult(MB, Q[0]);                                                // B - Spline
   out[0] = dotproduct4(T, Q[0]);
   matvecmult(MB, Q[1]);
   out[1] = dotproduct4(T, Q[1]);
   matvecmult(MB, Q[2]);
   out[2] = dotproduct4(T, Q[2]);
   matvecmult(MB, Q[3]);
   out[3] = dotproduct4(T, Q[3]);
}


/* *************************************************************
   This procedure calculates the current placement of objects
   using Catmull-Rom or B splines
************************************************************* */
void splinet(float s, vector4f *in, float *out, int stype) {

   vector4f Q[4];
   vector4f T;

   transpose(Q, in);

   T[3] = 1;
   T[2] = s;
   T[1] = T[2]*T[2];
   T[0] = T[1]*T[2];

   if (stype == M_CR) {                           // Catmull-Rom spline
      matvecmult(MCR, Q[0]);
      out[0] = dotproduct4(T, Q[0]);
      matvecmult(MCR, Q[1]);
      out[1] = dotproduct4(T, Q[1]);
      matvecmult(MCR, Q[2]);
      out[2] = dotproduct4(T, Q[2]);
      matvecmult(MCR, Q[3]);
      out[3] = dotproduct4(T, Q[3]);

      return;
   }

   matvecmult(MB, Q[0]);                                                // B-Spline
   out[0] = dotproduct4(T, Q[0]);
   matvecmult(MB, Q[1]);
   out[1] = dotproduct4(T, Q[1]);
   matvecmult(MB, Q[2]);
   out[2] = dotproduct4(T, Q[2]);
   matvecmult(MB, Q[3]);
   out[3] = dotproduct4(T, Q[3]);
}


/* *************************************************************
   This procedure calculates the current placement of objects
   using Catmull-Rom or B splines
************************************************************* */
void spline(float s, float Q[][4], float *out, int stype) {

   vector4f T, temp;

   T[3] = 1;
   T[2] = s;
   T[1] = T[2]*T[2];
   T[0] = T[1]*T[2];

   if (stype == M_CR) {                           // Catmull-Rom spline
      copyarray4(temp, Q[0]);
      matvecmult(MCR, temp);
      out[0] = dotproduct4(T, temp);
      copyarray4(temp, Q[1]);
      matvecmult(MCR, temp);
      out[1] = dotproduct4(T, temp);
      copyarray4(temp, Q[2]);
      matvecmult(MCR, temp);
      out[2] = dotproduct4(T, temp);
      copyarray4(temp, Q[3]);
      matvecmult(MCR, temp);
      out[3] = dotproduct4(T, temp);

      return;
   }

   copyarray4(temp, Q[0]);
   matvecmult(MB, temp);                                                // B-Spline
   out[0] = dotproduct4(T, temp);
   copyarray4(temp, Q[1]);
   matvecmult(MB, temp);
   out[1] = dotproduct4(T, temp);
   copyarray4(temp, Q[2]);
   matvecmult(MB, temp);
   out[2] = dotproduct4(T, temp);
   copyarray4(temp, Q[3]);
   matvecmult(MB, temp);
   out[3] = dotproduct4(T, temp);
}

