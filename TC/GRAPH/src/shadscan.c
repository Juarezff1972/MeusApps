



/* *************************************************************
   This function scans poligonal objects into a zbuffer
************************************************************* */

#include <string.h>
#include "shadow.h"
#include "global.h"
#include "memman.h"

/* *************************************************************
************************************************************* */
void shadow::object2plane(light *lparm) {

   int    i, j, k;
   int    flag;
   vector3f vector;
   float  t;
   char   *clip;
   int    xflag = 0;

   clip = new char[ob->countvertex];

   if (lparm->query_whatami() == FAR) {
      vector[0] = -lparm->transvec[0];
      vector[1] = -lparm->transvec[1];
      vector[2] = -lparm->transvec[2];
   }

   for (i=0; i<ob->countvertex; i++) {
      if (lparm->query_whatami() == POINT) {
         vector[0] = ob->mvertex[i][0] - lparm->transloc[0];
         vector[1] = ob->mvertex[i][1] - lparm->transloc[1];
         vector[2] = ob->mvertex[i][2] - lparm->transloc[2];
      }

      flag = line_plane_intersect(splane, ob->mvertex[i], vector, &t);

      if (!(clip[i] = (t < 0 || !flag))) {
         ob->mvertex[i][0] += t*vector[0];
         ob->mvertex[i][1] += t*vector[1];
         ob->mvertex[i][2] += t*vector[2];

         if (xflag) {
            for (j=0; j<3; j++)
               if (ob->mvertex[i][j] < bbox[0][j])
                  bbox[0][j] = ob->mvertex[i][j];
               else if (ob->mvertex[i][j] > bbox[1][j])
                  bbox[1][j] = ob->mvertex[i][j];
         }

         else {
            xflag = 1;
            copyarray4(bbox[0], ob->mvertex[i]);
            copyarray4(bbox[1], ob->mvertex[i]);
         }

      }

   }

   for (i=0; i<ob->countobject; i++)
      if (ob->polynum[i])
         for (k=0; k<ob->polynum[i]; k++)
            if (clip[ob->edgeptr[i][k]]) {
               ob->polynum[i] = 0;
               break;
            }

   delete [] clip;
}


/* **************************************************
************************************************** */
void shadow::render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff) {

   switch (mctype) {

      case SHADOW:
         if (mcinfo & CIRAYCAST)
            shadctr(cparm, zbuff, zbuff->maxx, zbuff->maxy);
         else
            shadct(zbuff, zbuff->maxx, zbuff->maxy);
         break;

      case BW:
         if (mcinfo & CIRAYCAST)
            polybwr(cparm, zbuff, zbuff->maxx, zbuff->maxy);
         else
            polybw(zbuff, zbuff->maxx, zbuff->maxy);

         break;

      default:
         polywfbw(zbuff);
         break;
   }

}


/* **************************************************
************************************************** */
void shadow::shadct(zbuffer *zbuff, int winx, int winy) {

   COMMON;

   edgetype *qtr;               // temp pointer

#define POLYNORMAL
#define INTZ
#define SHADCT
#include "polyplate.c"
#undef SHADCT
#undef INTZ
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void shadow::shadctr(eye *parm, zbuffer *zbuff, int winx, int winy) {

   COMMON;

   edgetype *qtr;               // temp pointer
   vector4f surface;

#define POLYNORMAL
#define SHADCT
#include "polyplate.c"
#undef SHADCT
#undef POLYNORMAL
}


/* *************************************************************
   This procedure takes the basic object data, and creates a composite
   structure to be worked on.
************************************************************* */
int shadow::datacopy2(polygon *pob, camera *cparm, light *lparm, int invert) {

   int    i;                          // looping variables

   ob->build(pob->ob->countvertex, pob->ob->countobject, pob->ob->countedge, 0, 0);
   ob->maxpolynum = pob->ob->maxpolynum;

   memcpy(ob->mvertex, pob->ob->mvertex, ob->countvertex<<4);
   memcpy(ob->polynum, pob->ob->polynum, ob->countobject<<2);
   memcpy(ob->object, pob->ob->object, ob->countedge<<2);

   ob->edgeptr[0] = ob->object;
   for (i=1; i<ob->countobject; i++)
      ob->edgeptr[i] = ob->edgeptr[i-1] + ob->polynum[i-1];

   size = 1;
   init_mx(rotate);
   center[0] = center[1] = center[2] = 0;
   id = -pob->id;

   if (invert) {
      for (i=0; i<ob->countvertex; i++)
         matvecmulto(cparm->Tinverse, ob->mvertex[i]);
   }

   copyarray4(splane, pob->splane);

   sflag = 0;

   mcinfo = pob->mcinfo & CIRAYCAST;

   if (pob->mctype > BW) {
      mctype = SHADOW;
      bflag = pob->bflag;
   }

   else {
      mctype = pob->mctype;
      bflag = (mctype <= WFBW) ? 0 : pob->bflag;
   }

   object2plane(lparm);

   ob->calc_normal();

   return 1;
}
