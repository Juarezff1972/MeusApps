

#include <string.h>
#include "matrix.h"


/* *************************************************************
   Quaternion multiplication
************************************************************* */
void quatquatmult(vector4f y, vector4f x) {

   float x1, y1;                       // real part of quaternion
   vector3f x24, y24;               // complex part of quaternion

   x1 = x[0];
   y1 = y[0];

   x24[0] = x[1];
   x24[1] = x[2];
   x24[2] = x[3];

   y24[0] = y[1];
   y24[1] = y[2];
   y24[2] = y[3];


   x[0] = y1*x1 - dotproduct3(y24, x24);         // calc real part

   xproduct(&x[1], y24, x24);                   // calc complex part

   x[1] += y1*x24[0] + x1*y24[0];
   x[2] += y1*x24[1] + x1*y24[1];
   x[3] += y1*x24[2] + x1*y24[2];
}
//
/* *************************************************************
   convert euler to quaternion
************************************************************* */
void euler2quat(vector3f angle, vector4f quat) {

   vector3f s, c;
   float temp;
   float c01, s01, c0s1, c1s0;

   temp = -angle[0]*0.5f;
   s[0] = SIN(temp);
   c[0] = COS(temp);

   temp = -angle[1]*0.5f;
   s[1] = SIN(temp);
   c[1] = COS(temp);

   temp = -angle[2]*0.5f;
   s[2] = SIN(temp);
   c[2] = COS(temp);

   c01 = c[0]*c[1];
   s01 = s[0]*s[1];
   c0s1 = c[0]*s[1];
   c1s0 = c[1]*s[0];

   quat[0] = c01*c[2] + s01*s[2];
   quat[1] = c1s0*c[2] - c0s1*s[2];
   quat[2] = c0s1*c[2] + c1s0*s[2];
   quat[3] = c01*s[2] - s01*c[2];
}


/* *************************************************************
   calculate euler rotation

	czcy	-szcx+czsysx	szsx+czsycx
	szcy	czcx+szsysx		-czsx+szsycx
	-sy		cysx			cycx

   mx = [Z] * [Y] * [X]
************************************************************* */
void calc_euler(float mx[][4], float current[]) {

	float cx, sx, cy, sy, cz, sz;
	float szcx, szsx, czcx, czsx;

	float *t;

	t = &mx[0][0];

	cx = COS(current[0]);
	sx = SIN(current[0]);

	cy = COS(current[1]);
	sy = SIN(current[1]);

	cz = COS(current[2]);
	sz = SIN(current[2]);

	szsx = sz*sx;
	czcx = cz*cx;
	szcx = sz*cx;
	czsx = cz*sx;

	t[0] = cz*cy;
	t[1] = -szcx + czsx*sy;
	t[2] = szsx + czcx*sy;

	t[4] = sz*cy;
	t[5] = czcx + szsx*sy;
	t[6] = -czsx + szcx*sy;

	t[8] = -sy;
	t[9] = cy*sx;
	t[10] = cy*cx;

	t[3] = t[7] = t[11] = t[12] = t[13] = t[14] = 0.0f;
	t[15] = 1.0f;
}


/* *************************************************************
   calculate quaternion mx
************************************************************* */
void calc_quat(float mx[][4], float current[]) {

   vector4f vect, sqr;
   register float t1, t2, v1, v2;

   copyarray4(vect, current);
   normalize4(vect);

   sqr[1] = SQR(vect[1]);
   sqr[2] = SQR(vect[2]);
   sqr[3] = SQR(vect[3]);


   v1 = vect[1]*vect[2];
   v2 = vect[0]*vect[3];
   t1 = v1 + v2;
   t2 = v1 - v2;
   mx[0][1] = t1 + t1;
   mx[1][0] = t2 + t2;

   v1 = vect[1]*vect[3];
   v2 = vect[0]*vect[2];
   t1 = v1 - v2;
   t2 = v1 + v2;
   mx[0][2] = t1 + t1;
   mx[2][0] = t2 + t2;

   mx[0][3] = mx[1][3] = mx[2][3] = 0;

   v1 = vect[2]*vect[3];
   v2 = vect[0]*vect[1];
   t1 = v1 + v2;
   t2 = v1 - v2;
   mx[1][2] = t1 + t1;
   mx[2][1] = t2 + t2;


   t1 = sqr[2] + sqr[3];
   t2 = sqr[1] + sqr[3];
   mx[0][0] = 1 - (t1 + t1);
   mx[1][1] = 1 - (t2 + t2);

   t1 = sqr[1] + sqr[2];
   mx[2][2] = 1 - (t1 + t1);

   mx[3][0] = mx[3][1] = mx[3][2] = 0;
   mx[3][3] = 1;
}


/* *************************************************************
        roll -  x       axis from back -> front
        pitch - y       axis from 9 oclock -> 3 oclock (looking from top)
        yaw -   z       axis represents -height of object

        mx[      cpcy,          -cpsy,           sp
                 srspcy+cpsy    -srspsy+cpcy    -srcp
                -crspcy+spsy     crspsy+spcy     crcp   ]

        mx = [R] * [P] * [Y]

        Assumption: matrix is a pure rotation matrix    calc xyz/rpy mx
************************************************************* */
void decompose_zyx(vector4f mx[4], float *roll, float *pitch, float *yaw) {

   float temp, tempc, temps;

   if (abs(mx[0][2]) > ICORRECT) {

      if (mx[0][2] > 0) {
         *pitch = (float)HALFPI;
         temp = 1.0f;
      }
		
      else {
         *pitch = (float)(-HALFPI);
         temp = -1.0f;
      }

      *yaw = 0.0f;

      if ((tempc = -mx[2][0]*temp) > ICORRECT)
         *roll = 0.0f;
      else if (tempc < -ICORRECT)
         *roll = (float)PI;
      else if ((temps = mx[1][0]*temp) > ICORRECT)
         *roll = (float)HALFPI;
      else if (temps < -ICORRECT)
         *roll = (float)(-HALFPI);
      else
         *roll = (float)atan2(temps, tempc);

      return;
   }

   *pitch = ASIN(mx[0][2]);
   temp = 1.0f/COS(*pitch);

   if ((tempc = mx[0][0]*temp) > ICORRECT)
      *yaw = 0.0f;
   else if  (tempc < -ICORRECT)
      *yaw = (float)PI;
   else if ((temps = -mx[0][1]*temp) > ICORRECT)
      *yaw = (float)HALFPI;
   else if (temps < -ICORRECT)
      *yaw = (float)-HALFPI;
   else
      *yaw = (float)atan2(temps, tempc);

   if ((tempc = mx[2][2]*temp) > ICORRECT)
      *roll = 0.0f;
   else if (tempc < -ICORRECT)
      *roll = (float)PI;
   else if ((temps = -mx[1][2]*temp) > ICORRECT)
      *roll = (float)HALFPI;
   else if (temps < -ICORRECT)
      *roll = (float)(-HALFPI);
   else
      *roll = (float)atan2(temps, tempc);
}


/* *************************************************************
                        DIS local body coord

        roll -  x       axis from back -> front
        pitch - y       axis from 9 oclock -> 3 oclock (looking from top)
        yaw -   z       axis represents -height of object

        mx[     cpcy    cysrsp - sycr   cycrsp + sysr
                sycp    sysrsp + cycr   sycrsp - cysr
                -sp     srcp            crcp            ]

        mx * v

        Assumption: matrix is a pure rotation matrix    calc xyz/rpy mx
************************************************************* */
void decompose_xyz(vector4f mx[4], float *roll, float *pitch, float *yaw) {

   double temp, tempc, temps;

   if (abs(mx[2][0]) > ICORRECT) {
      *pitch = (-mx[2][0] > 0) ? (float)HALFPI : (float)(-HALFPI);
      *yaw = 0.0f;

      if ((tempc = mx[1][1]) > ICORRECT)
         *roll = 0.0f;
      else if (tempc < -ICORRECT)
         *roll = (float)PI;
      else if ((temps = -mx[1][2]) > ICORRECT)
         *roll = (float)HALFPI;
      else if (temps < -ICORRECT)
         *roll = (float)(-HALFPI);
      else
         *roll = (float)atan2(temps, tempc);

      return;
   }

   *pitch = ASIN(-mx[2][0]);
   temp = 1.0/COS(*pitch);

   if ((tempc = mx[0][0]*temp) > ICORRECT)
      *yaw = 0.0f;
   else if  (tempc < -ICORRECT)
      *yaw = (float)PI;
   else if ((temps = mx[1][0]*temp) > ICORRECT)
      *yaw = (float)HALFPI;
   else if (temps < -ICORRECT)
      *yaw = (float)(-HALFPI);
   else
      *yaw = (float)atan2(temps, tempc);

   if ((tempc = mx[2][2]*temp) > ICORRECT)
      *roll = 0.0f;
   else if (tempc < -ICORRECT)
      *roll = (float)PI;
   else if ((temps = mx[2][1]*temp) > ICORRECT)
      *roll = (float)HALFPI;
   else if (temps < -ICORRECT)
      *roll = (float)(-HALFPI);
   else
      *roll = (float)atan2(temps, tempc);
}


/* *************************************************************
************************************************************* */
void rotate_mx_x(vector4f mx[4], float x) {

   register float cx, sx;
   vector4f temp1, temp2;

   copyarray4(temp1, mx[1]);
   copyarray4(temp2, mx[2]);

   cx = COS(x);
   sx = SIN(x);

   mx[1][0] = cx*temp1[0] - sx*temp2[0];
   mx[1][1] = cx*temp1[1] - sx*temp2[1];
   mx[1][2] = cx*temp1[2] - sx*temp2[2];
   mx[1][3] = cx*temp1[3] - sx*temp2[3];

   mx[2][0] = sx*temp1[0] + cx*temp2[0];
   mx[2][1] = sx*temp1[1] + cx*temp2[1];
   mx[2][2] = sx*temp1[2] + cx*temp2[2];
   mx[2][3] = sx*temp1[3] + cx*temp2[3];
}


/* *************************************************************
************************************************************* */
void rotate_mx_y(vector4f mx[4], float y) {

   vector4f temp0, temp2;
   register float cy, sy;

   copyarray4(temp0, mx[0]);
   copyarray4(temp2, mx[2]);

   cy = COS(y);
   sy = SIN(y);

   mx[0][0] = cy*temp0[0] + sy*temp2[0];
   mx[0][1] = cy*temp0[1] + sy*temp2[1];
   mx[0][2] = cy*temp0[2] + sy*temp2[2];
   mx[0][3] = cy*temp0[3] + sy*temp2[3];

   mx[2][0] = -sy*temp0[0] + cy*temp2[0];
   mx[2][1] = -sy*temp0[1] + cy*temp2[1];
   mx[2][2] = -sy*temp0[2] + cy*temp2[2];
   mx[2][3] = -sy*temp0[3] + cy*temp2[3];
}


/* *************************************************************
************************************************************* */
void rotate_mx_z(vector4f mx[4], float z) {

   vector4f temp0, temp1;
   register float cz, sz;

   copyarray4(temp0, mx[0]);
   copyarray4(temp1, mx[1]);

   cz = COS(z);
   sz = SIN(z);

   mx[0][0] = cz*temp0[0] - sz*temp1[0];
   mx[0][1] = cz*temp0[1] - sz*temp1[1];
   mx[0][2] = cz*temp0[2] - sz*temp1[2];
   mx[0][3] = cz*temp0[3] - sz*temp1[3];

   mx[1][0] = sz*temp0[0] + cz*temp1[0];
   mx[1][1] = sz*temp0[1] + cz*temp1[1];
   mx[1][2] = sz*temp0[2] + cz*temp1[2];
   mx[1][3] = sz*temp0[3] + cz*temp1[3];
}


/* *************************************************************
        axis  = z
        start = x
************************************************************* */
void rotate_vector(float *start, float *axis, float rad, float *sol) {

   vector4f mx[4], r[4];

   init_mx(mx);

   xproduct(mx[1], axis, start);
   normalize3(mx[1]);
   copyarray3(mx[2], axis);
   normalize3(mx[2]);
   xproduct(mx[0], mx[1], mx[2]);
   transpose(mx);

   init_mx(r);
   rotate_mx_z(r, rad);

   matmatmulto(mx, r);

   sol[0] = r[0][0];
   sol[1] = r[1][0];
   sol[2] = r[2][0];
}
