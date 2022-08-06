


/* ***************************************************************
   this file handles the routines to manipulate polygons
**************************************************************** */

#include <string.h>
#include "polygon.h"


/* *************************************************************
   this function removes back face surfaces
************************************************************* */
void polygon::removeside() {

   register int i;

   for (i=0; i<ob->countobject; i++)
      if (ob->normal[i][3] < 0) // precalc the dotprod in transform()
         ob->polynum[i] = 0;

/*
   register int i, j;

   for (i=0; i<ob->countobject; i++) {
      for (j=ob->polynum[i]-1; j>-1; j--)
         if (ob->mvertex[ob->edgeptr[i][j]][2] < 0) {
            if (dotproduct3(ob->normal[i], ob->mvertex[ob->edgeptr[i][j]]) > 0)
               ob->polynum[i] = 0;

            break;
         }

      if (j < 0)
         ob->polynum[i] = 0;
   }
*/

}


/* *************************************************************
   This procedure puts the object in the edge table
************************************************************* */
void polygon::transform(eye *parm) {

   register int i;                           // looping vars
   vector3f temp, temp2, oldcenter;

   for (i=0; i<ob->countobject; i++)            // rotate face normals
      matvecmultv(rotate, ob->normal[i]);

   if (mctype > ALEX)                           // rotate vertex normals
      for (i=0; i<ob->countvertex; i++)
         matvecmultv(rotate, ob->gnormal[i]);

   copymx4x4(world, rotate);

   world[0][3] += center[0];
   world[1][3] += center[1];
   world[2][3] += center[2];

   smultarray3(world[0], size);
   smultarray3(world[1], size);
   smultarray3(world[2], size);

   if (!(mcinfo & CIBITMAP)) {          // remove rotation for bitmaps :)
      matmatmulto(parm->transform, world);

      for (i=0; i<ob->countvertex; i++)
         matvecmulto(world, ob->mvertex[i]);

      for (i=0; i<ob->countobject; i++) {               // rotate face normals
         matvecmultv(parm->transform, ob->normal[i]);
         ob->normal[i][3] = -dotproduct3(ob->normal[i], ob->mvertex[ob->edgeptr[i][0]]);
      }

      if (mctype > ALEX)                                // rotate vertex normals
         for (i=0; i<ob->countvertex; i++)
            matvecmultv(parm->transform, ob->gnormal[i]);

      inversemx(world, iworld);

      return;
   }

   oldcenter[0] = temp[0] = world[0][3];
   oldcenter[1] = temp[1] = world[1][3];
   oldcenter[2] = temp[2] = world[2][3];
   matvecmulto(parm->transform, temp);

   subeqarray3(temp2, temp, oldcenter);

   world[0][3] += temp2[0];
   world[1][3] += temp2[1];
   world[2][3] += temp2[2];

   for (i=0; i<ob->countvertex; i++)    // translate bitmap wrt camera
      matvecmulto(world, ob->mvertex[i]);

   for (i=0; i<ob->countobject; i++)            // calc "D"
      ob->normal[i][3] = -dotproduct3(ob->normal[i], ob->mvertex[ob->edgeptr[i][0]]);

   world[0][3] = temp[0];
   world[1][3] = temp[1];
   world[2][3] = temp[2];

   inversemx(world, iworld);

}
