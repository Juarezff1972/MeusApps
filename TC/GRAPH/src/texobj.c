


#include "texobj.h"

void texface::setup(int i) {

   if (uv != (vector2f *)NULL)
      delete [] uv;

   uv = new vector2f[i];
   count = i;
   memset(uv, 0, i<<3);
   ob = (texbase *)NULL;
}


void texface::calc_area() {

   int j;

   area = 0;

   for (j=0; j<count-1; j++)
      area += xproduct2(uv[j], uv[j+1]);

   area += xproduct2(uv[j], uv[0]);
   area = sqrt(abs(area));
}


void texface::make_unit() {

   int j;
   float x, y;

   x = 1.0/(ob->maxx - 1);
   y = 1.0/(ob->maxy - 1);

   for (j=0; j<count; j++) {
      uv[j][0] *= x;
      uv[j][1] *= y;
   }

}


void texpolygon::setup(int i) {

   if (data != (texface *)NULL)
      delete [] data;

   data = new texface[i];
   countobject = i;
   memset(data, 0, i*sizeof(texface));
}


unsigned int texpolygon::query_texel(int face, float x, float y, float *color, int frame, float d) {

   float a, b;
   unsigned int t;

   t = data[face].ob->maxx - 1;
   a = (x - (int)x)*t;
   if (a < 0)
      a += t;

   t = data[face].ob->maxy - 1;
   b = (y - (int)y)*t;
   if (b < 0)
      b += t;

   return data[face].ob->query_texel(a, b, color, frame, 1, d);
}


void texpolygon::perim2Dmap(int face, int *corner, texbase *tob, polytype *dob) {

   int h, i, j, k;
   double delta[4][2];
   double coord[4][2];
   double perim;
   double *dist;

   if (data[face].uv != (vector2f *)NULL)
      delete [] data[face].uv;

   data[face].setup(dob->polynum[face]+1);
   data[face].ob = tob;

   dist = new double[data[face].count];

   j = 0;
   for (i=0, h = 1; i<3; i++, h++)
      if (corner[h] - corner[j] < 0)
         j--;
      else
         j++;

   if (j > 0) {
      coord[0][0] = 0;
      coord[0][1] = 0;
      coord[1][0] = 0;
      coord[1][1] = tob->maxy-1;
      coord[2][0] = tob->maxx-1;
      coord[2][1] = coord[1][1];
      coord[3][0] = coord[2][0];
      coord[3][1] = 0;

      delta[0][0] = 0;
      delta[0][1] = coord[1][1];
      delta[1][0] = coord[2][0];
      delta[1][1] = 0;
      delta[2][0] = 0;
      delta[2][1] = -coord[1][1];
      delta[3][0] = -coord[2][0];
      delta[3][1] = 0;
   }

   else {
      i = corner[0]; corner[0] = corner[3]; corner[3] = i;
      i = corner[1]; corner[1] = corner[2]; corner[2] = i;

      coord[3][0] = 0;
      coord[3][1] = 0;
      coord[2][0] = 0;
      coord[2][1] = tob->maxy-1;
      coord[1][0] = tob->maxx-1;
      coord[1][1] = coord[2][1];
      coord[0][0] = coord[1][0];
      coord[0][1] = 0;

      delta[3][0] = 0;
      delta[3][1] = coord[2][1];
      delta[2][0] = coord[1][0];
      delta[2][1] = 0;
      delta[1][0] = 0;
      delta[1][1] = -coord[2][1];
      delta[0][0] = -coord[1][0];
      delta[0][1] = 0;
   }

   for (i=0; i<4; i++) {
      h = (i == 3) ? 0 : (i+1);

      perim = 0;
      for (j=corner[i], k=j+1, k=(k<dob->polynum[face]) ? k:0; j != corner[h]; j=k, k++, k = (k<dob->polynum[face]) ? k:0) {
         dist[j] = distance3(dob->mvertex[dob->edgeptr[face][j]], dob->mvertex[dob->edgeptr[face][k]]);
         perim += dist[j];
      }

      delta[i][0] /= perim;
      delta[i][1] /= perim;

      for (k=corner[i]; k!=corner[h]; k++, k= (k<dob->polynum[face]) ? k:0) {
         copyarray2(data[face].uv[k], coord[i]);
         coord[i][0] += delta[i][0] * dist[k];
         coord[i][1] += delta[i][1] * dist[k];
      }

   }

   copyarray2(data[face].uv[dob->polynum[face]], data[face].uv[0]);

   delete [] dist;
   data[face].make_unit();
   data[face].calc_area();
}


void texpolygon::affinemap(int face, vector2f *uv, texbase *tob, polytype *dob) {

   vector3f u, v;
   vector3f v1, v2;
   vector3f ov1, ov2;
   vector3f mxu, mxv;
   float a, s, t;
   int i;

   data[face].setup(dob->polynum[face]+1);
   data[face].ob = tob;

   copyarray2(data[face].uv[0], uv[0]);
   copyarray2(data[face].uv[1], uv[1]);
   copyarray2(data[face].uv[2], uv[2]);
//   copyarray2(data[face].uv[dob->polynum[face]], uv[0]);

   if (dob->polynum[face] > 3) {

      copyarray3(u, uv[1]);
      copyarray3(v, uv[2]);
      subarray3(u, uv[0]);
      subarray3(v, uv[1]);

      copyarray3(v1,dob->mvertex[dob->edgeptr[face][1]]);
      copyarray3(v2,dob->mvertex[dob->edgeptr[face][2]]);
      subarray3(v1, dob->mvertex[dob->edgeptr[face][0]]);
      subarray3(v2, dob->mvertex[dob->edgeptr[face][1]]);

      a = 1.0/dotproduct3(v1, v1);
      t = dotproduct3(v1, v2)*a;

      copyarray3(ov1, v1);
      smultarray3(ov1, -t);
      addarray3(ov1, v2);

      s = 1.0/dotproduct3(ov1, ov1);

      ov1[1] = (v[0] - t*u[0])*s;
      ov2[1] = (v[1] - t*u[1])*s;

      ov1[0] = u[0]*a - ov1[1]*t;
      ov2[0] = u[1]*a - ov2[1]*t;

      mxu[0] = ov1[0]*v1[0] + ov1[1] * v2[0];
      mxu[1] = ov1[0]*v1[1] + ov1[1] * v2[1];
      mxu[2] = ov1[0]*v1[2] + ov1[1] * v2[2];

      mxv[0] = ov2[0]*v1[0] + ov2[1] * v2[0];
      mxv[1] = ov2[0]*v1[1] + ov2[1] * v2[1];
      mxv[2] = ov2[0]*v1[2] + ov2[1] * v2[2];

      for (i=3; i<dob->polynum[face]; i++) {
         copyarray3(v1, dob->mvertex[dob->edgeptr[face][i]]);
         subarray3(v1, dob->mvertex[dob->edgeptr[face][0]]);
         data[face].uv[i][0] = dotproduct3(v1, mxu) + uv[0][0];
         data[face].uv[i][1] = dotproduct3(v1, mxv) + uv[0][1];
      }

   }

   copyarray2(data[face].uv[dob->polynum[face]], data[face].uv[0]);

   data[face].make_unit();
   data[face].calc_area();
}


void texpolygon::direct2Dmap(int face, vector2f *uv, texbase *tob, int vcount) {

   data[face].setup(vcount+1);
   data[face].ob = tob;
   memcpy(data[face].uv, uv, vcount<<3);
   copyarray2(data[face].uv[vcount], uv[0]);
   data[face].make_unit();
   data[face].calc_area();
}


/* *****************************************************************
***************************************************************** */

void sbmap::setup(vector4f *pt) {

   float size[2];

   if (ob == (texbase *)NULL)
      return;

   min[0] = max[0] = pt[0][0];

   if (pt[2][0] < min[0])
      min[0] = pt[2][0];
   else
      if (pt[2][0] > max[0])
         max[0] = pt[2][0];

   min[1] = max[1] = pt[0][1];

   if (pt[2][1] < min[1])
      min[1] = pt[2][1];
   else
      if (pt[2][1] > max[1])
         max[1] = pt[2][1];

   size[0] = max[0] - min[0];
   size[1] = max[1] - min[1];

   xscale = 1.0/size[0];
   yscale = 1.0/size[1];

   if ((flaginv = (abs(pt[0][0] - pt[3][0]) > CORRECT))) {
      xsign = pt[2][0] - pt[0][0];
      ysign = pt[2][1] - pt[0][1];
   }

   else {
      ysign = pt[2][0] - pt[0][0];
      xsign = pt[2][1] - pt[0][1];
   }

   area = sqrt((ob->maxx*ob->maxy)/(size[0]*size[1]));
}


unsigned int sbmap::query_texel(float x, float y, float *c, int frame, float d) {

   float a, b, e;
   unsigned int z;
   int xx, yy;

   if (repeatflag && (x < min[0] || x > max[0] || y < min[1] || y > max[1]))
      return 0;

   xx = ob->maxx - 1;
   yy = ob->maxy - 1;

   a = (x-min[0]) * xscale;
   a -= ((int)a);

//   if (a < 0)           // bug in sgi  (int)(-x) = roundup(x)
//      a += 1.0;

   b = (y-min[1]) * yscale;
   b -= ((int)b);

//   if (b < 0)           // bug in sgi  (int)(-x) = roundup(x)
//      b += 1.0;

   if (flaginv) {
      a = xx * ((xsign < 0) ? (1 - a) : a);
      b = yy * ((ysign < 0) ? (1 - b) : b);
   }

   else {
      e = xx * ((xsign < 0) ? (1 - b) : b);
      b = yy * ((ysign < 0) ? (1 - a) : a);

      a = e;
   }

   z = ob->query_texel(a, b, c, frame, 1, d*area);

   return (z > 0) ? z : 1;
}


int texsbfast::query_texel(float x, float y, float *c, int frame, float d) {

   int i, j;

   for (i=j=0; i<countobject; i++)
      if (!data[i].repeatflag) {
         if (data[i].query_texel(x,y,c, frame, d))
            return 1;
      }

      else
         if (!j && data[i].query_texel(x,y,c, frame, d))
            j = 1;

   return j;
}


