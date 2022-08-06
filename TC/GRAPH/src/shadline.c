



/* *************************************************************
************************************************************* */


#include "shadline.h"
extern int INTERLACE;

/* *************************************************************
************************************************************* */
int shadline::point2plane(float *psplane, light *lparm) {

   float  vector[3];
   float  t;

   if (lparm->query_whatami() == POINT) {
      vector[0] = wwai[0][0] - lparm->location[0];
      vector[1] = wwai[0][1] - lparm->location[1];
      vector[2] = wwai[0][2] - lparm->location[2];
   }

   else {
      vector[0] = -lparm->transvec[0];
      vector[1] = -lparm->transvec[1];
      vector[2] = -lparm->transvec[2];
   }

   line_plane_intersect(psplane, wwai[0], vector, &t);

   if (t <=0)
      return 0;

   wwai[0][0] += t*vector[0];
   wwai[0][1] += t*vector[1];
   wwai[0][2] += t*vector[2];

   if (lparm->query_whatami() == POINT) {
      vector[0] = wwai[1][0] - lparm->location[0];
      vector[1] = wwai[1][1] - lparm->location[1];
      vector[2] = wwai[1][2] - lparm->location[2];
   }

   line_plane_intersect(psplane, wwai[1], vector, &t);

   if (t <=0)
      return 0;

   wwai[1][0] += t*vector[0];
   wwai[1][1] += t*vector[1];
   wwai[1][2] += t*vector[2];

   return 1;
}


/* *************************************************************
************************************************************* */
int shadline::datacopy2(line *pob, light *lparm) {

   copyarray4(wwai[0], pob->wwai[0]);

   sflag  = 0;
   mctype = pob->mctype;
   id = -pob->id;

   copyarray4(wwai[1], pob->wwai[1]);

   return point2plane(pob->splane, lparm);
}


/* **************************************************
************************************************** */
void shadline::render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff) {

   int    i, j, k;
   int    scany;
   float  delta[2][2];
   float  pt[3];
   float  m;
   float  dz;
   vector3f black = {0,0,0};
   vector3f white = {1,1,1};
   int index;
   int h;
   int toprow = zbuff->maxx*(zbuff->maxy-1);

   j = (wwai[0][1] < wwai[1][1]);
   i = !j;

   h = (int)wwai[j][1];
   scany = (int)wwai[i][1];

   if (scany == h)
      h++;

   m = 1.0/(h - scany);
   delta[0][0] = (wwai[j][0] - wwai[i][0])*m;
   delta[0][1] = (wwai[j][2] - wwai[i][2])*m;

   dz = delta[0][1]/abs(delta[0][0]);

   do {

      if (!INTERLACE || !(scany & 0x01)) {
         if (delta[0][0] > 0) {
            pt[0] = wwai[i][0];
            pt[1] = pt[0] + delta[0][0];
            pt[2] = wwai[i][2];
         }

         else {
            pt[1] = wwai[i][0];
            pt[0] = pt[1] + delta[0][0];
            pt[2] = wwai[i][2] + delta[0][1];
         }

#ifdef DOS
         index = toprow - scany*zbuff->maxx;
#else
         index = scany*zbuff->maxx;
#endif

         k = index + round(pt[1]);
         index += round(pt[0]);

         do {
            if (pt[2] > zbuff->zbuff[index].zdata) {
               zbuff->zbuff[index].zdata = pt[2];

               if (mctype == WFBW)
                  *(unsigned int *)zbuff->zbuff[index].pixel = *(unsigned int *)zbuff->zbuff[index].ambient = 0xffffffff;
               else
                  *(unsigned int *)zbuff->zbuff[index].pixel = *(unsigned int *)zbuff->zbuff[index].ambient;
            }

            pt[2] += dz;
            index++;
         } while (index < k);

      }

      wwai[i][0] += delta[0][0];
      wwai[i][2] += delta[0][1];

      scany++;
   } while (scany < h);

}
