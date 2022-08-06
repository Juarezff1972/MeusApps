



/* *************************************************************
   This function scans poligonal objects into a zbuffer
************************************************************* */


#include "shadowpt.h"
extern int INTERLACE;

/* *************************************************************
************************************************************* */
int shadowpt::point2plane(float psplane[], light *lparm) {

   float  vector[3];
   float  t;

   if (lparm->query_whatami() == POINT) {
      vector[0] = wwai[0] - lparm->location[0];
      vector[1] = wwai[1] - lparm->location[1];
      vector[2] = wwai[2] - lparm->location[2];
   }

   else {
      vector[0] = -lparm->transvec[0];
      vector[1] = -lparm->transvec[1];
      vector[2] = -lparm->transvec[2];
   }

   line_plane_intersect(psplane, wwai, vector, &t);

   if (t <=0)
      return 0;

   wwai[0] += t*vector[0];
   wwai[1] += t*vector[1];
   wwai[2] += t*vector[2];

   return 1;
}


/* *************************************************************
   This procedure takes the basic object data, and creates a composite
   structure to be worked on.
************************************************************* */
int shadowpt::datacopy2(particle *pob, light *lparm) {

   copyarray4(wwai, pob->wwai);

   sflag  = 0;
   mctype = pob->mctype;
   id = -pob->id;

   return point2plane(pob->splane, lparm);
}


/* **************************************************
************************************************** */
void shadowpt::render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff) {

   int    index;

   index = round(wwai[1]);

   if (INTERLACE && (index & 0x01))
      return;

#ifdef DOS
   index =  (zbuff->maxy - 1 - index)*zbuff->maxx + round(wwai[0]);
#else
   index = index*zbuff->maxx + round(wwai[0]);
#endif

   if (wwai[2] > zbuff->zbuff[index].zdata) {
      zbuff->zbuff[index].zdata = wwai[2];

      if (mctype == WFBW)
         *(unsigned int *)zbuff->zbuff[index].pixel = *(unsigned int *)zbuff->zbuff[index].ambient = 0xffffffff;
      else
         *(unsigned int *)zbuff->zbuff[index].pixel = *(unsigned int *)zbuff->zbuff[index].ambient;
   }

}
