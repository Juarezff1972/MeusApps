



/* *************************************************************
************************************************************* */

#include <stdio.h>
#include "matrix.h"

void build_rect_index(int index[6][4]) {

   index[0][0] = 0; index[0][1] = 1; index[0][2] = 2; index[0][3] = 3;
   index[1][0] = 4; index[1][1] = 5; index[1][2] = 6; index[1][3] = 7;
   index[2][0] = 0; index[2][1] = 4; index[2][2] = 5; index[2][3] = 1;
   index[3][0] = 1; index[3][1] = 5; index[3][2] = 6; index[3][3] = 2;
   index[4][0] = 2; index[4][1] = 6; index[4][2] = 7; index[4][3] = 3;
   index[5][0] = 3; index[5][1] = 7; index[5][2] = 4; index[5][3] = 0;
}


/* ***********************************************************************
                normal orders front, back left, top, right, bottom
                polygon orders : 0-3 front, 4-7 back, 0-4-5-1 left, etc
*********************************************************************** */
int line_rect_intersect(float *pt, float *v, vector4f *p, vector4f *n, float *flag) {

   vector4f ptemp[4];
   float t;
   float intersect[2];
   int ptr = 0;
   int index[6][4];
   int i;

   build_rect_index(index);

   for (i=0; i<6; i++) {
      copyarray4(ptemp[0], p[index[i][0]]);
      copyarray4(ptemp[1], p[index[i][0]]);
      copyarray4(ptemp[2], p[index[i][0]]);
      copyarray4(ptemp[3], p[index[i][0]]);

      if (line_poly_intersect(pt, v, n[i], ptemp, 4, &t)) {
         intersect[ptr] = t;
         ptr++;

         if (ptr == 2) {
            if (intersect[0]*intersect[1] > 0)       // outside the box
               if (intersect[0] > 0) {                     // face to box
                  if (intersect[0] < intersect[1])
                     *flag = intersect[0];
                  else
                     *flag = intersect[1];
                  return 1;
               }

            if (intersect[0] > intersect[1])               // inside box or face aw         if (intersect[0] > intersect[1])
               *flag = intersect[0];
            else
               *flag = intersect[1];
            return 1;
         }

      }

   }

   if (ptr == 1) {                              // should never happen unless I screwed the pooch
      *flag = intersect[0];
      printf("FUCKUP in LINE_RECT!!!\n");
      return 1;
   }

   return 0;
}


/* **********************************************************************
   This function calculates the distance between an objects center, and
   a plane.
*********************************************************************** */
int line_plane_intersect(float *plane, float *pt, float *vector, float *t) {

   float x;

   x = dotproduct3(plane, vector);

   if (not_zero(x)) {
      *t = -(plane[3] + dotproduct3(pt, plane))/x;
      return 1;
   }

   *t = 0;
   return 0;
}


/* **********************************************************************
        sphere -
        pt     - point on line
        vector - line vector
        NOTE:   if inside sphere, return positive t
*********************************************************************** */
int line_sphere_intersect(float radius, float *sphere, float *pt, float *vector, float *t) {

   float a, b, c, check, t1, t2;
   vector3f x;

   x[0] = pt[0]-sphere[0];
   x[1] = pt[1]-sphere[1];
   x[2] = pt[2]-sphere[2];

   a = dotproduct3(vector, vector);
   b = dotproduct3(x, vector);
   b += b;
   c = dotproduct3(x, x) - SQR(radius);

   check = SQR(b) - 4*a*c;

   if (check < 0) {
      *t = 0;
      return 0;
   }

   check = sqrt(check);
   a += a;

   if (a < 0) {
      a = -a;
      t1 = (b + check);
      t2 = (b - check);
   }

   else {
      t1 = -(b + check);
      t2 = (-b + check);
   }

   if (t1 < 0) {
      if (t2 < 0) {
         if (t1 < t2) {
            *t = t2/a;
            return 1;
         }

         *t = t1/a;
         return 1;
      }

      *t = t2/a;            // signs are different - we are inside
      return -1;
   }

   if (t2 < 0) {          // signs are different - we are inside
      *t = t1/a;
      return -1;
   }

   if (t1 < t2) {
      *t = t1/a;
      return 1;
   }

   *t = t2/a;

   return 1;
}


















/* **********************************************************************
*********************************************************************** */
int line_ellipse_intersect(float *scale, float *pt, float *vector, float *t) {

   float a, b, c, check, t1, t2;
   vector3f x, iscale;

   iscale[0] = 1.0/SQR(scale[0]);
   iscale[1] = 1.0/SQR(scale[1]);
   iscale[2] = 1.0/SQR(scale[2]);

   x[0] = iscale[0]*vector[0];
   x[1] = iscale[1]*vector[1];
   x[2] = iscale[2]*vector[2];

   b = dotproduct3(pt, x);
   b += b;
   a = dotproduct3(vector, x);
   c = SQR(pt[0])*iscale[0] + SQR(pt[1])*iscale[1] + SQR(pt[2])*iscale[2] - 1;

   check = SQR(b) - 4*a*c;

   if (check < 0) {
      *t = 0;
      return 0;
   }

   check = sqrt(check);
   a += a;

   if (a < 0) {
      a = -a;
      t1 = (b + check);
      t2 = (b - check);
   }

   else {
      t1 = -(b + check);
      t2 = (-b + check);
   }

   if (t1 < 0) {
      if (t2 < 0) {
         if (t1 < t2) {
            *t = t2/a;
            return 1;
         }

         *t = t1/a;
         return 1;
      }

      *t = t2/a;            // signs are different - we are inside
      return -1;
   }

   if (t2 < 0) {          // signs are different - we are inside
      *t = t1/a;
      return -1;
   }

   if (t1 < t2) {
      *t = t1/a;
      return 1;
   }

   *t = t2/a;

   return 1;
}


/* **********************************************************************
*********************************************************************** */
int line_ellipse_intersecti(float *iscale, float *pt, float *vector, float *t) {

   float a, b, c, check, t1, t2;
   vector3f x;

   x[0] = iscale[0]*vector[0];
   x[1] = iscale[1]*vector[1];
   x[2] = iscale[2]*vector[2];

   b = dotproduct3(pt, x);
   b += b;
   a = dotproduct3(vector, x);
   c = SQR(pt[0])*iscale[0] + SQR(pt[1])*iscale[1] + SQR(pt[2])*iscale[2] - 1;

   check = SQR(b) - 4*a*c;

   if (check < 0) {
      *t = 0;
      return 0;
   }

   check = sqrt(check);
   a += a;

   if (a < 0) {
      a = -a;
      t1 = (b + check);
      t2 = (b - check);
   }

   else {
      t1 = -(b + check);
      t2 = (-b + check);
   }

   if (t1 < 0) {
      if (t2 < 0) {
         if (t1 < t2) {
            *t = t2/a;
            return 1;
         }

         *t = t1/a;
         return 1;
      }

      *t = t2/a;            // signs are different - we are inside
      return -1;
   }

   if (t2 < 0) {          // signs are different - we are inside
      *t = t1/a;
      return -1;
   }

   if (t1 < t2) {
      *t = t1/a;
      return 1;
   }

   *t = t2/a;

   return 1;
}


/* **********************************************************************
*********************************************************************** */
int line_poly_intersect(float *pt, float *vector, float *plane, vector4f *vertex, int count, float *point) {

   float t;

   if (!(line_plane_intersect(plane, pt, vector, &t)))
      return 0;

   point[0] = pt[0] + t*vector[0];
   point[1] = pt[1] + t*vector[1];
   point[2] = pt[2] + t*vector[2];

   return query_point_poly_intersect(point, plane, count, vertex);
}


/* **********************************************************************
*********************************************************************** */
int line_cpoly_intersect(float *pt, float *vector, float *plane, vector4f *vertex, int count, float *point) {

   float t;

   if (!(line_plane_intersect(plane, pt, vector, &t)))
      return 0;

   point[0] = pt[0] + t*vector[0];
   point[1] = pt[1] + t*vector[1];
   point[2] = pt[2] + t*vector[2];

   return query_point_cpoly_intersect(point, plane, count, vertex);
}


/* **********************************************************************
*********************************************************************** */
int query_poly_plane_intersect(vector4f *pt, int len, float *n) {

   int i;
   float t;

   t = dotproduct3(pt[0], n) + n[3];

   for (i=0; i<len; i++)
      if (t * (dotproduct3(pt[i], n) + n[3]) < CORRECT)
         return 1;

   return 0;
}


/* **********************************************************************
*********************************************************************** */
int query_point_poly_intersect(float *pt, float *normal, int count, vector4f *vertex) {

   float angle;
   int  i;
   float v0[3], v1[3], v2[3];

   if (abs(dotproduct3(pt, normal) + normal[3]) > CORRECT)
      return 0;

   copyarray3(v2, vertex[0]);
   subarray3(v2, pt);
   normalize3(v2);

   copyarray3(v0, v2);

   for (i=1, angle=0; i<count; i++) {
      copyarray3(v1, v2);
      copyarray3(v2, vertex[i]);
      subarray3(v2, pt);
      normalize3(v2);
      angle += ACOS(dotproduct3(v1, v2));
   }

   copyarray3(v1, v0);
   angle += ACOS(dotproduct3(v1, v2));

   return (angle > TWOPI2D-CORRECT3 && angle < TWOPI2D+CORRECT3);
}


/* **********************************************************************
********************************************************************** */
int query_point_cpoly_intersect(float *pt, float *normal, int count, vector4f *vertex) {

   int i;
   vector3f v1, v2;
   vector3f n;

   if (abs(dotproduct3(pt, normal) + normal[3]) > CORRECT)
      return 0;

   for (i=0; i<count; i++) {
      v1[0] = vertex[i][0]-pt[0];
      v1[1] = vertex[i][1]-pt[1];
      v1[2] = vertex[i][2]-pt[2];

      v2[0] = vertex[i+1][0]-pt[0];
      v2[1] = vertex[i+1][1]-pt[1];
      v2[2] = vertex[i+1][2]-pt[2];

      xproduct(n, v2, v1);

      if (abs(n[0]) > CORRECT && abs(n[1]) > CORRECT && abs(n[2]) > CORRECT &&  dotproduct3(n, normal) < 0)
         return 0;
   }

   v1[0] = vertex[i][0]-pt[0];
   v1[1] = vertex[i][1]-pt[1];
   v1[2] = vertex[i][2]-pt[2];

   v2[0] = vertex[i+1][0]-pt[0];
   v2[1] = vertex[i+1][1]-pt[1];
   v2[2] = vertex[i+1][2]-pt[2];

   xproduct(n, v2, v1);

   return (!(abs(n[0]) > CORRECT && abs(n[1]) > CORRECT && abs(n[2]) > CORRECT &&  dotproduct3(n, normal) < 0));
}


/* **********************************************************************
********************************************************************** */
int query_point_plane_intersect(float *pt, float *n) {

   float t;

   line_plane_intersect(n, pt, n, &t);

   if (t < -CORRECT)
      return 0;

   return 1;
}


/* **********************************************************************
********************************************************************** */
int query_point_sphere_intersect(float *pt, float *center, float r) {

   float sum[3];

   copyarray3(sum, pt);
   subarray3(sum, center);

   if (magnitude3(sum) - r < CORRECT)
      return 1;

   return 0;
}


/* ***********************************************************************
*********************************************************************** */
int query_point_rect_intersect(float *pt, vector4f *n) {

   float t;

   line_plane_intersect(n[0], pt, n[0], &t);
   if (t < -CORRECT)
      return 0;

   line_plane_intersect(n[1], pt, n[1], &t);
   if (t < -CORRECT)
      return 0;

   line_plane_intersect(n[2], pt, n[2], &t);
   if (t < -CORRECT)
      return 0;

   line_plane_intersect(n[3], pt, n[3], &t);
   if (t < -CORRECT)
      return 0;

   line_plane_intersect(n[4], pt, n[4], &t);
   if (t < -CORRECT)
      return 0;

   line_plane_intersect(n[5], pt, n[5], &t);
   if (t < -CORRECT)
      return 0;

   return 1;
}


/* **********************************************************************
********************************************************************** */
int query_sphere_sphere_intersect(float *c1, float r1, float *c2, float r2) {

   float sum[3];

   copyarray3(sum, c1);
   subarray3(sum, c2);

   if (magnitude3(sum) - (r1+r2) < CORRECT)
      return 1;

   return 0;
}


/* ***********************************************************************
*********************************************************************** */
int query_sphere_plane_intersect(float *c, float r, float *n) {

   float t;

   line_plane_intersect(n, c, n, &t);

   if (t + r > -CORRECT)
      return 1;

   return 0;
}


/* ***********************************************************************
*********************************************************************** */
int query_sphere_rect_intersect(float *c, float r, vector4f *n) {

   float t;

   line_plane_intersect(n[0], c, n[0], &t);
   if (abs(t) < CORRECT)
      return 1;
   if (t + r <= -CORRECT)
      return 0;

   line_plane_intersect(n[1], c, n[1], &t);
   if (abs(t) < CORRECT)
      return 1;
   if (t + r <= -CORRECT)
      return 0;

   line_plane_intersect(n[2], c, n[2], &t);
   if (abs(t) < CORRECT)
      return 1;
   if (t + r <= -CORRECT)
      return 0;

   line_plane_intersect(n[3], c, n[3], &t);
   if (abs(t) < CORRECT)
      return 1;
   if (t + r <= -CORRECT)
      return 0;

   line_plane_intersect(n[4], c, n[4], &t);
   if (abs(t) < CORRECT)
      return 1;
   if (t + r <= -CORRECT)
      return 0;

   line_plane_intersect(n[5], c, n[5], &t);
   if (t + r <= -CORRECT)
      return 0;

   return 1;
}

/* **********************************************************************
   Theory - if a plane intersects a rectangle, then it must intersect at
   LEAST 3 edges, therefore we can stop if we find one intersection.
   order of vertices - 1-4 one side, 5-8 opposite side where 1,5 match,
   2,6 match, etc. - 12 sets of compares

   new theory - if a plane intersects a rectangle, it must intersect with
   at least1 of the 4 cross-rectangle diagonals -
   only need to check 3 (2 creates a plane, 3 creates a space). (1,7 2,8 3,5)
********************************************************************** */
int query_plane_rect_intersect(float *n, vector4f *p) {

   float v[3];
   float t;

   copyarray3(v, p[6]);
   subarray3(v, p[0]);

   if (line_plane_intersect(n, p[0], v, &t))
      return !(t < 0 || t > 1);

   copyarray3(v, p[7]);
   subarray3(v, p[1]);

   if (line_plane_intersect(n, p[1], v, &t))
      return !(t < 0 || t > 1);

   copyarray3(v, p[4]);
   subarray3(v, p[2]);

   if (line_plane_intersect(n, p[2], v, &t))
      return !(t < 0 || t > 1);

   return 0;  // theoretically, the program should never get here!!!
}


/* **********************************************************************
********************************************************************** */
int query_plane_plane_intersect(float *n1, float *n2) {

   vector3f v1, v2;
   float temp, m1, m2;

   copyarray3(v1, n1);
   m1 = normalize3(v1);
   copyarray3(v2, n2);
   m2 = normalize3(v2);

   temp = dotproduct3(v1, v2);

   if (abs(temp) < 1-CORRECT)
      return 1;

   if (abs(m1) < CORRECT)
      return (abs(m2) < CORRECT);

   if (abs(m2) < CORRECT)               // magnitude of one vector is non existant - assume no intersection
      return 0;

   if (temp < 0) {
      temp = n1[3]/m1 + n2[3]/m2;
      return (abs(temp) < CORRECT);
   }

   temp = n1[3]/m1 - n2[3]/m2;
   return (abs(temp) < CORRECT);
}


/* **********************************************************************
Cube Theory - if 2 cubes intersect each other, then at least one corner
of one cube is within another cube

Rectangle Theory 1: if 2 rectangles intersect, then there exists at
least 1 plane from each rectangle st the intersection of the planes (a line)
intersects both polygons that represent those planes.

Rectangle Theory 2: if bounding spheres that bound the rectangles
do not intersect, the rectangles do not intersect.

Rectangle Theory 3: if a corner of a rectangle is w/in the other,
there is an intersection

Rectangle Theory 4: if 2 rectangles intersect, then the edge of one
must intersect the face of another unless one completely encloses the
other. (caught by Theory 3)

********************************************************************** */
int query_rect_rect_intersect(vector4f *n1, vector4f *p1, vector4f *n2, vector4f *p2) {

   float bsr[2];
   float center[2][4];
   float tr, dist;
   float t[4];
   int i;

   copyarray4(center[0], p1[0]);             // if bs's dont intersect,
   addarray3(center[0], p1[6]);              //   rect's dont intersect
   smultarray3(center[0], 0.5);

   copyarray3(t, center[0]);
   subarray3(t, p1[0]);
   bsr[0] = magnitude3(t);

   copyarray4(center[1], p2[0]);
   addarray3(center[1], p2[6]);
   smultarray3(center[1], 0.5);

   copyarray3(t, center[1]);
   subarray3(t, p2[0]);
   bsr[1] = magnitude3(t);

   if (!query_sphere_sphere_intersect(center[0], bsr[0], center[1], bsr[1]))
      return 0;

   if (!query_sphere_rect_intersect(center[0], bsr[0], n2)) // check sphere/rect
      return 0;

   if (!query_sphere_rect_intersect(center[1], bsr[1], n1)) // check sphere/rect
      return 0;

   // *** bounding spheres/rects intersect ***
                                        // now see if a vertex(pt) of one
                                        // intersects the other
   for (i=0; i<8; i++)
      if (query_point_rect_intersect(p1[i], n2) ||
          query_point_rect_intersect(p2[i], n1))
         return 1;

   // *** each rect does not enclose a vertex of the other ***
                                        // now check to see if edge of one
                                        // intersects the other
   for (i=0; i<3; i++) {
      copyarray3(t, p1[i+1]);
      subarray3(t, p1[i]);
      tr = magnitude3(t);

      if (line_rect_intersect(p1[i], t, p2, n2, &dist))
         if (dist <= tr)
            return 1;

      copyarray3(t, p2[i+1]);
      subarray3(t, p2[i]);
      tr = magnitude3(t);

      if (line_rect_intersect(p2[i], t, p1, n1, &dist))
         if (dist <= tr)
            return 1;

      copyarray3(t, p1[i+5]);
      subarray3(t, p1[i+4]);
      tr = magnitude3(t);

      if (line_rect_intersect(p1[i+4], t, p2, n2, &dist))
         if (dist <= tr)
            return 1;

      copyarray3(t, p2[i+5]);
      subarray3(t, p2[i+4]);
      tr = magnitude3(t);

      if (line_rect_intersect(p2[i+4], t, p1, n1, &dist))
         if (dist <= tr)
            return 1;
   }

   copyarray3(t, p1[0]);
   subarray3(t, p1[3]);
   tr = magnitude3(t);

   if (line_rect_intersect(p1[3], t, p2, n2, &dist))
      if (dist <= tr)
         return 1;

   copyarray3(t, p2[0]);
   subarray3(t, p2[3]);
   tr = magnitude3(t);

   if (line_rect_intersect(p2[3], t, p1, n1, &dist))
      if (dist <= tr)
         return 1;

   copyarray3(t, p1[4]);
   subarray3(t, p1[7]);
   tr = magnitude3(t);

   if (line_rect_intersect(p1[7], t, p2, n2, &dist))
      if (dist <= tr)
         return 1;

   copyarray3(t, p2[4]);
   subarray3(t, p2[7]);
   tr = magnitude3(t);

   if (line_rect_intersect(p2[7], t, p1, n1, &dist))
      if (dist <= tr)
         return 1;

   for (i=0; i<4; i++) {
      copyarray3(t, p1[i+4]);
      subarray3(t, p1[i]);
      tr = magnitude3(t);

      if (line_rect_intersect(p1[i], t, p2, n2, &dist))
         if (dist <= tr)
            return 1;

      copyarray3(t, p2[i+4]);
      subarray3(t, p2[i]);
      tr = magnitude3(t);

      if (line_rect_intersect(p2[i], t, p1, n1, &dist))
         if (dist <= tr)
            return 1;
   }

   return 0;
}


/* **********************************************************************

        eq:     Ax + By + C = 0

*********************************************************************** */
int line2d_line2d_intersect(float *v1, float *v2, float *pt) {

   float t1, t2;

   if (abs(v1[0]) < CORRECT) {
      if (abs(v1[1]) < CORRECT || abs(v2[0]) < CORRECT)
         return 0;

      pt[1] = -v1[2]/v1[1];
      pt[0] = -(v2[1]*pt[1] + v2[2])/v2[0];

      return 1;
   }

   if (abs(v1[1]) < CORRECT) {
      if (abs(v1[0]) < CORRECT || abs(v2[1]) < CORRECT)
         return 0;

      pt[0] = -v1[2]/v1[0];
      pt[1] = -(v2[0]*pt[0] + v2[2])/v2[1];
      return 1;
   }

   if (abs(v2[0]) < CORRECT) {
      if (abs(v2[1]) < CORRECT || abs(v1[0]) < CORRECT)
         return 0;

      pt[1] = -v2[2]/v2[1];
      pt[0] = -(v1[1]*pt[1] + v1[2])/v1[0];
      return 1;
   }

   if (abs(v2[1]) < CORRECT) {
      if (abs(v2[0]) < CORRECT || abs(v1[1]) < CORRECT)
         return 0;

      pt[0] = -v2[2]/v2[0];
      pt[1] = -(v1[0]*pt[0] + v1[2])/v1[1];
      return 1;
   }

   t1 = v1[1]*v2[0] - v2[1]*v1[0];

   if (abs(t1) < CORRECT)
      return 0;

   t2 = v2[1]*v1[2] - v1[1]*v2[2];
   pt[0] = t2/t1;
   pt[1] = -(v2[0]*pt[0] + v2[2])/v2[1];

   t1 = v1[0]*pt[0] + v1[1]*pt[1] + v2[2];
   return (abs(t1) < CORRECT);
}


/* **********************************************************************
   This function reflects an vector about a normal to a plane
*********************************************************************** */
void reflect(float normal[], float vector[]) {

   float mag;
   int    i;

   mag = abs(dotproduct3(normal, vector));
   mag += mag;

   for (i=0; i<3; i++)
      vector[i] += mag*normal[i];

   normalize3(vector);
}


/* **********************************************************************
********************************************************************** */
int sphere_sphere_reflectn3vf(float *src, float *dest, float *normal) {

   copyarray3(normal, dest);
   subarray3(normal, src);

   return !(normalize3(normal) < CORRECT);
}


/* **********************************************************************
********************************************************************** */
int sphere_plane_reflectn3vf(float *n, float *normal) {

   copyarray3(normal, n);
   normalize3(normal);

   return 1;
}


/* **********************************************************************
          normal orders front, back left, top, right, bottom
          polygon orders : 0-3 front, 4-7 back, 0-4-5-1 left, etc
********************************************************************** */
int sphere_rect_reflectn3vf(float *c, vector4f *p, vector4f *n, float *normal) {

   float t, dt;
   float ct = -1;
   float pt[3];
   vector4f ptemp[4];

   int index[6][4];
   int i;

   build_rect_index(index);

   for (i=0; i<6; i++) {
      line_plane_intersect(n[i], c, n[i], &t); // back face
      dt = abs(t);

      if (ct < 0 || dt < ct) {
         pt[0] = c[0] + t*n[i][0];
         pt[1] = c[1] + t*n[i][1];
         pt[2] = c[2] + t*n[i][2];

         copyarray4(ptemp[0], p[index[i][0]]);
         copyarray4(ptemp[1], p[index[i][1]]);
         copyarray4(ptemp[2], p[index[i][2]]);
         copyarray4(ptemp[3], p[index[i][3]]);

         if (query_point_poly_intersect(pt, n[i], 4, ptemp)) { // if point in polygon
            ct = dt;
            copyarray3(normal, n[i]);
         }

      }

   }

   return !(ct < 0);    // If here, then cant determine (most cases sphere is nowhere near rectangloid

                        // possible addition - calc closest point to surface and calc that normal
}


/* **********************************************************************
          normal orders front, back left, top, right, bottom
          polygon orders : 0-3 front, 4-7 back, 0-4-5-1 left, etc

#1 method: first detect is returned...
#2 method:  detect which sides are "hit", then average all the surface normals

if average < CORRECT,
then return -1

                OR

***** calc closest surface to closest vertex(?)

#3 method: weighted sides - weight is based upon closest/ intersect/ penetrate

********* #4 average vertexes, line rectangle intersect...

                first object reflects off second object

                bouncen         - first rectangle normals
                bouncep         - first rectangle verticies
                surfacep        - second rectangle verticies
                surfacen        - second rectangle normals
                normal          - returned normal that reflection needs
********************************************************************** */
int rect_rect_reflectn3vf(vector4f *bouncep, vector4f *surfacep, vector4f *surfacen, float *normal) {

   vector3f temp;
   int i;

   copyarray3(temp, bouncep[0]);

   for (i=1; i<8; i++)
      addarray3(temp, bouncep[i]);

   smultarray3(temp, 0.125);

   return sphere_rect_reflectn3vf(temp, surfacep, surfacen, normal);
}


/* **********************************************************************

if no intersect, reflect = average of closest vertex
if intersect, reflect = average of intersect

        n              plane normal
        surfacep       verticies in rectangle
        surfacen       normals of rectangle
        normal         returned reflection normal

********************************************************************** */
int plane_rect_reflectn3vf(float *n, vector4f *surfacep, vector4f *surfacen, float *normal) {

   vector3f current;
   int index[6][4];
   char intersect_flag = 0;
   int i, j, last;
   vector4f poly[4];
   float dist[9];
   float temp;

   vector3f vn[8];

   build_rect_index(index);
   current[0] = current[1] = current[2] = 0;

   for (i=0; i<6; i++) {
      copyarray3(poly[0], surfacep[index[i][0]]);
      copyarray3(poly[1], surfacep[index[i][1]]);
      copyarray3(poly[2], surfacep[index[i][2]]);
      copyarray3(poly[3], surfacep[index[i][3]]);

      if (query_poly_plane_intersect(poly, 4, n)) {
         intersect_flag = 1;
         addarray3(current, surfacen[i]);
         last = i;
      }

   }

   if (intersect_flag) {
      if (magnitude3(current) < CORRECT) {
         copyarray3(normal, surfacen[last]);
      }

      else
         copyarray3(normal, current);

      normalize3(normal);
      return 1;
   }

   for (i=0; i<8; i++)
      vn[i][0] = vn[i][1] = vn[i][2] = 0;

   for (i=0; i<6; i++)
      for (j=0; j<4; j++)
         addarray3(vn[index[i][j]], surfacen[i]);

   current[0] = current[1] = current[2] = 0.0;
   dist[8] = dist[0] = dotproduct3(n, surfacep[0]) + n[3];

   last = 0;

   for (i=1; i<8; i++) {
      temp = dotproduct3(n, surfacep[i]) + n[3];
      temp = abs(temp);

      dist[i] = temp;

      if (temp < dist[8]) {
         dist[8] = temp;
         last = i;
      }

   }

   for (i=0; i<8; i++)
      if (abs(dist[i]-dist[8]) < CORRECT)
         addarray3(current, vn[i]);

   if (magnitude3(current) < CORRECT) {
      copyarray3(normal, vn[last]);
   }

   else
      copyarray3(normal, current);

   normalize3(normal);
   return 1;
}


/* **********************************************************************
        NOTE:   if inside, return positive
********************************************************************** */
int line_cylinder_intersect(vector4f pt, vector4f v, float length, float radius, float *t, int *side) {

   float a, b, c, check, t1, t2;
   float topt, bottomt;
   int tflag, bflag, t1flag, t2flag, iflag;
   vector4f plane;
   float rsquare;

   rsquare = SQR(radius);

   plane[0] = plane[2] = 0;
   plane[1] = 1;
   plane[3] = length;
   bflag = line_plane_intersect(plane, pt, v, &bottomt);

   if (bflag) {
      a = pt[0] + bottomt*v[0];
      b = pt[2] + bottomt*v[2];

      if (SQR(a) + SQR(b) > rsquare)
         bflag = 0;
   }

   plane[3] = -length;
   tflag = line_plane_intersect(plane, pt, v, &topt);

   if (tflag) {
      a = pt[0] + topt*v[0];
      b = pt[2] + topt*v[2];

      if (SQR(a) + SQR(b) > rsquare)
         tflag = 0;
   }

   if (tflag && bflag) {
      *t = topt;
      *side = CYL_TOP;

      if (topt < 0) {
         if (bottomt < 0) {
            if (topt < bottomt) {
               *t = bottomt;
               *side = CYL_BOTTOM;
            }

            return 1;
         }

         *t = bottomt;          // signs are different
         *side = CYL_BOTTOM;
         return -1;
      }

      if (bottomt > 0) {
         if (topt > bottomt) {
            *t = bottomt;
            *side = CYL_BOTTOM;
         }

         return 1;
      }

      else                      // signs are different
         return -1;
   }

   b = pt[0]*v[0] + pt[2]*v[2];
   b += b;
   c = SQR(pt[0]) + SQR(pt[2]) - SQR(radius);
   a = SQR(v[0]) + SQR(v[2]);

   check = SQR(b) - 4*a*c;

   if (check < 0) {
      *t = 0;
      return 0;
   }

   check = sqrt(check);
   a += a;

   t1 = (-b - check)/a;
   t2 = (-b + check)/a;

   a = pt[1] + t1*v[1];
   t1flag = (a < length && a > -length);

   a = pt[1] + t2*v[1];
   t2flag = (a < length && a > -length);

   if (!(t1flag || t2flag)) {
      *t = 0;
      return 0;
   }

   iflag = -1;

   *side = CYL_SIDE;

   if (t1flag) {
      iflag = (t1 > 0);
      *t = t1;
   }

   if (t2flag) {

      if (iflag > -1) {
         if ((t2>0) == iflag) {
            if (*t < 0) {
               if (t2 > *t)
                  *t = t2;
            }

            else
               if (t2 < *t)
                  *t = t2;

            return 1;
         }

         if (t2 > *t)   // signs are different
            *t = t2;

         return -1;
      }

      iflag = (t2 > 0);
      *t = t2;
   }

   if (tflag) {
      if ((topt>0) == iflag) {
         if (*t < 0) {
            if (topt > *t) {
               *side = CYL_TOP;
               *t = topt;
            }

         }

         else
            if (topt < *t) {
               *side = CYL_TOP;
               *t = topt;
            }

         return 1;
      }

      if (topt > *t) {  // different signs
         *side = CYL_TOP;
         *t = topt;
      }

      return -1;
   }

   if (bflag) {
      if ((bottomt>0) == iflag) {
         if (*t < 0) {
            if (bottomt > *t) {
               *side = CYL_BOTTOM;
               *t = bottomt;
            }

         }

         else
            if (bottomt < *t) {
               *side = CYL_BOTTOM;
               *t = bottomt;
            }

         return 1;
      }

      if (bottomt > *t) {               // different signs
         *side = CYL_BOTTOM;
         *t = bottomt;
      }

      return -1;
   }

   *t = 0;
   return 0;
}


/* **********************************************************************
   0 - top
   1 - bottom
********************************************************************** */
int line_cone_intersect(vector4f pt, vector4f v, float length, float *radius, float *t, int *side) {

   float a, b, c, d, e, f, check, t1, t2;
   float topt, bottomt;
   int tflag, bflag, t1flag, t2flag, iflag;
   vector4f plane;

   plane[0] = plane[2] = 0;
   plane[1] = 1;
   plane[3] = length;
   bflag = line_plane_intersect(plane, pt, v, &bottomt);

   if (bflag) {
      a = pt[0] + bottomt*v[0];
      b = pt[2] + bottomt*v[2];

      if (SQR(a) + SQR(b) > SQR(radius[1]))
         bflag = 0;
   }

   plane[3] = -length;
   tflag = line_plane_intersect(plane, pt, v, &topt);

   if (tflag) {
      a = pt[0] + topt*v[0];
      b = pt[2] + topt*v[2];

      if (SQR(a) + SQR(b) > SQR(radius[0]))
         tflag = 0;
   }

   if (tflag && bflag) {
      *t = topt;
      *side = CYL_TOP;

      if (topt < 0) {
         if (bottomt < 0) {
            if (topt < bottomt) {
               *t = bottomt;
               *side = CYL_BOTTOM;
            }

            return 1;
         }

         *t = bottomt;          // signs are different
         *side = CYL_BOTTOM;
         return -1;
      }

      if (bottomt > 0) {
         if (topt > bottomt) {
            *t = bottomt;
            *side = CYL_BOTTOM;
         }

         return 1;
      }

      return -1;                // signs are different
   }

   d = (radius[0] - radius[1])/(2*length);
   e = 0.5*(radius[1] + radius[0]);
   f = SQR(d);

   check = d*e;
   c = f*pt[1];
   b = pt[0]*v[0] + pt[2]*v[2] - v[1]*(c + check);
   b += b;
   c = SQR(pt[0]) + SQR(pt[2]) - (pt[1]*(c + check + check) + SQR(e));
   a = SQR(v[0]) + SQR(v[2]) - SQR(v[1])*f;

   check = SQR(b) - 4*a*c;

   if (check < 0) {
      *t = 0;
      return 0;
   }

   check = sqrt(check);
   a += a;

   t1 = (-b - check)/a;
   t2 = (-b + check)/a;

   a = pt[1] + t1*v[1];
   t1flag = (a < length && a > -length);

   a = pt[1] + t2*v[1];
   t2flag = (a < length && a > -length);

   if (!(t1flag || t2flag)) {
      *t = 0;
      return 0;
   }

   iflag = -1;

   *side = CYL_SIDE;

   if (t1flag) {
      iflag = (t1 > 0);
      *t = t1;
   }

   if (t2flag) {
      if (iflag > -1) {
         if ((t2>0) == iflag) {
            if (*t < 0) {
               if (t2 > *t)
                  *t = t2;
            }

            else
               if (t2 < *t)
                  *t = t2;

            return 1;
         }

         if (t2 > *t)   // signs are different
            *t = t2;

         return -1;
      }

      iflag = (t2 > 0);
      *t = t2;
   }

   if (tflag) {
      if ((topt>0) == iflag) {
         if (*t < 0) {
            if (topt > *t) {
               *side = CYL_TOP;
               *t = topt;
            }

         }

         else
            if (topt < *t) {
               *side = CYL_TOP;
               *t = topt;
            }

         return 1;
      }

      if (topt > *t) {  // different signs
         *side = CYL_TOP;
         *t = topt;
      }

      return -1;
   }

   if (bflag) {
      if ((bottomt>0) == iflag) {
         if (*t < 0) {
            if (bottomt > *t) {
               *side = CYL_BOTTOM;
               *t = bottomt;
            }

         }

         else
            if (bottomt < *t) {
               *side = CYL_BOTTOM;
               *t = bottomt;
            }

         return 1;
      }

      if (bottomt > *t) {               // different signs
         *side = CYL_BOTTOM;
         *t = bottomt;
      }

      return -1;
   }

   *t = 0;
   return 0;
}


