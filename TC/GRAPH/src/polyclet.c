



/* *************************************************************
   This file creates the edge table
************************************************************* */


#include <string.h>
#include "polygon.h"
#include "memman.h"

int polygon::clip(eye *parm, int maxx, int maxy) { return 0; }     // use polyclet instead


/* *************************************************************
************************************************************* */
void polygon::post_clip(edgetype *ptr) {

   register edgetype *qtr, *rtr;
   register edgetype **btr;

   if (ptr->next == (edgetype *)NULL)
      return;

   for (qtr = ptr->next; qtr->next != (edgetype *)NULL;) {
      rtr = qtr->next;

      if (nsame3(qtr->end.point, rtr->start.point))
         if (similar3(qtr->end.point, rtr->start.point)) {
            copyarray3(qtr->end.point, rtr->start.point);

            qtr = rtr;
         }

         else {
            rtr = (edgetype *)control.pop(EDGETYPE);
            rtr->next = qtr->next;
            qtr->next = rtr;

            rtr->id = qtr->id;
            rtr->start = qtr->end;

            qtr = rtr->next;

            rtr->end = qtr->start;
         }

      else
         qtr = qtr->next;
   }

   if (ptr->next == qtr) {
      control.push(EDGETYPE, qtr);
      ptr->next = (edgetype *)NULL;
      return;
   }

   rtr = ptr->next;
   if (nsame3(qtr->end.point, rtr->start.point))
      if (similar3(qtr->end.point, rtr->start.point)) {
         copyarray3(qtr->end.point, rtr->start.point);
      }

      else {
         rtr = (edgetype *)control.pop(EDGETYPE);
         rtr->next = qtr->next;
         qtr->next = rtr;

         rtr->id = qtr->id;
         rtr->start = qtr->end;

         qtr = ptr->next;

         rtr->end = qtr->start;
      }

   for (btr = &ptr->next; *btr != (edgetype *)NULL;)    // delete "points"
      if (similar3((*btr)->start.point, (*btr)->end.point)) {
         qtr = *btr;
         *btr = qtr->next;
         control.push(EDGETYPE, qtr);
      }

      else
         btr = &(*btr)->next;
}


/* *************************************************************
        // can precalc length of edge...
************************************************************* */
int polygon::clip_greater(edgetype *ptr, int i, float plane, float *normal, eye *parm) {

   int j;
   vector3f d, n;
   float factor;
   float bound;
   edgetype *qtr, *rtr;
   int textureflag;
   int flag;
   float temp1, temp2;

   if (ptr->next == (edgetype *)NULL)
      return 0;

   textureflag = (mcinfo & CITEXTURE) && tob->query_tflag(ptr->next->id);

   flag = 0;
   bound = plane + CORRECT;

   for (qtr = ptr; qtr->next != (edgetype *)NULL;) {

      if (qtr->next->start.point[i] > bound) {
         flag = 1;

         if (qtr->next->end.point[i] > bound) {
            rtr = qtr->next;
            qtr->next = rtr->next;
            control.push(EDGETYPE, rtr);
         }

         else {
            qtr = qtr->next;
            for (j=0; j<3; j++) {
               d[j] = qtr->end.point[j] - qtr->start.point[j];
               n[j] = qtr->end.shadedata[j] - qtr->start.shadedata[j];
            }

            factor = (plane-qtr->start.point[i])/d[i];

            qtr->start.point[0] += factor*d[0];
            qtr->start.point[1] += factor*d[1];

            if (i==Z)
               qtr->start.point[2] = plane;
            else {
               qtr->start.point[2] = engine(normal, qtr->start.point, parm, &temp1, &temp2);
            }

            qtr->start.shadedata[0] += factor*n[0];
            qtr->start.shadedata[1] += factor*n[1];
            qtr->start.shadedata[2] += factor*n[2];

            if (textureflag) {
               qtr->start.uv[0] += factor*(qtr->end.uv[0]-qtr->start.uv[0]);
               qtr->start.uv[1] += factor*(qtr->end.uv[1]-qtr->start.uv[1]);
            }

         }

      }

      else {
         qtr = qtr->next;

         if (qtr->end.point[i] > bound) {
            flag = 1;

            for (j=0; j<3; j++) {
               d[j] = qtr->start.point[j] - qtr->end.point[j];
               n[j] = qtr->start.shadedata[j] - qtr->end.shadedata[j];
            }

            factor = (plane-qtr->end.point[i])/d[i];

            qtr->end.point[0] += factor*d[0];
            qtr->end.point[1] += factor*d[1];

            if (i==Z)
               qtr->end.point[2] = plane;
            else {
               qtr->end.point[2] = engine(normal, qtr->end.point, parm, &temp1, &temp2);
            }

            qtr->end.shadedata[0] += factor*n[0];
            qtr->end.shadedata[1] += factor*n[1];
            qtr->end.shadedata[2] += factor*n[2];

            if (textureflag) {
               qtr->end.uv[0] += factor*(qtr->start.uv[0]-qtr->end.uv[0]);
               qtr->end.uv[1] += factor*(qtr->start.uv[1]-qtr->end.uv[1]);
            }

         }

      }

   }

   return flag;
}


/* *************************************************************
************************************************************* */
int polygon::clip_lesser(edgetype *ptr, int i, float plane, float *normal, eye *parm) {

   int j;
   vector3f d, n;
   float factor;
   float bound;
   edgetype *qtr, *rtr;
   int textureflag;
   int flag;
   float temp1, temp2;

   if (ptr->next == (edgetype *)NULL)
      return 0;

   textureflag = (mcinfo & CITEXTURE) && tob->query_tflag(ptr->next->id);

   flag = 0;
   bound = plane - CORRECT;

   for (qtr = ptr; qtr->next != (edgetype *)NULL;) {

      if (qtr->next->start.point[i] < bound) {
         flag = 1;

         if (qtr->next->end.point[i] < bound) {
            rtr = qtr->next;
            qtr->next = rtr->next;
            control.push(EDGETYPE, rtr);
         }

         else {
            qtr = qtr->next;
            for (j=0; j<3; j++) {
               d[j] = qtr->end.point[j] - qtr->start.point[j];
               n[j] = qtr->end.shadedata[j] - qtr->start.shadedata[j];
            }

            factor = (plane-qtr->start.point[i])/d[i];

            qtr->start.point[0] += factor*d[0];
            qtr->start.point[1] += factor*d[1];

            if (i==Z)
               qtr->start.point[2] = plane;
            else {
               qtr->start.point[2] = engine(normal, qtr->start.point, parm, &temp1, &temp2);
            }

            qtr->start.shadedata[0] += factor*n[0];
            qtr->start.shadedata[1] += factor*n[1];
            qtr->start.shadedata[2] += factor*n[2];

            if (textureflag) {
               qtr->start.uv[0] += factor*(qtr->end.uv[0]-qtr->start.uv[0]);
               qtr->start.uv[1] += factor*(qtr->end.uv[1]-qtr->start.uv[1]);
            }

         }

      }

      else {
         qtr = qtr->next;

         if (qtr->end.point[i] < bound) {
            flag = 1;

            for (j=0; j<3; j++) {
               d[j] = qtr->start.point[j] - qtr->end.point[j];
               n[j] = qtr->start.shadedata[j] - qtr->end.shadedata[j];
            }

            factor = (plane-qtr->end.point[i])/d[i];

            qtr->end.point[0] += factor*d[0];
            qtr->end.point[1] += factor*d[1];

            if (i==Z)
               qtr->end.point[2] = plane;
            else {
               qtr->end.point[2] = engine(normal, qtr->end.point, parm, &temp1, &temp2);
            }

            qtr->end.shadedata[0] += factor*n[0];
            qtr->end.shadedata[1] += factor*n[1];
            qtr->end.shadedata[2] += factor*n[2];

            if (textureflag) {
               qtr->end.uv[0] += factor*(qtr->start.uv[0]-qtr->end.uv[0]);
               qtr->end.uv[1] += factor*(qtr->start.uv[1]-qtr->end.uv[1]);
            }

         }

      }

   }

   return flag;
}


/* *************************************************************
   this function initiates an edge in the edge table
************************************************************* */
int polygon::prepare_edge(int polynum, edgetype *ptr, eye *parm, light *lmain) {

   vector3f temp, surface;
   int      i;
   float    deltay;
   float    theta;
   float    *ftr = ptr->start.point;

   deltay = 1.0 / (ptr->end.point[1] - ftr[1]);

   ptr->dx  = (ptr->end.point[0] - ftr[0]) * deltay;
   ptr->dz  = (ptr->end.point[2] - ftr[2]) * deltay;

   switch (mctype) {
      case PHONG:                                       // phong

         theta = dotproduct3(ptr->start.shadedata, ptr->end.shadedata);

         if (theta > 1-CORRECT) {
            ptr->ds[0] = HALFPI;        // theta
            ptr->ds[1] = 0.0;           // psi
            ptr->ds[2] = 1.0;           // 1.0/sin(theta);
            ptr->ds[3] = 0.0;           // dpsi
            break;
         }

         ptr->ds[0] = (theta < CORRECT-1) ? 0.00447214315061 : ACOS(theta);     // theta
         ptr->ds[1] = 0.0;              // psi
         ptr->ds[2] = 1.0/SIN(ptr->ds[0]);      // 1.0/sin(theta);
         ptr->ds[3] = deltay*ptr->ds[0];        // dpsi

         break;

      case GOURAUD:
         if (!lmain)
            break;

         copyarray3(temp, ptr->start.shadedata);
         copyarray3(ptr->start.shadedata, ob->shade[polynum].ambient);
         copyarray3(surface, ftr);
         parm->screen2map(surface);
         lmain->intensity(surface, temp, ptr->start.shadedata, parm, ob->shade[polynum].color, this);

         copyarray3(temp, ptr->end.shadedata);
         copyarray3(ptr->end.shadedata, ob->shade[polynum].ambient);
         copyarray3(surface, ptr->end.point);
         parm->screen2map(surface);
         lmain->intensity(surface, temp, ptr->end.shadedata, parm, ob->shade[polynum].color, this);

         ptr->ds[0] = (ptr->end.shadedata[0] - ptr->start.shadedata[0]) * deltay;
         ptr->ds[1] = (ptr->end.shadedata[1] - ptr->start.shadedata[1]) * deltay;
         ptr->ds[2] = (ptr->end.shadedata[2] - ptr->start.shadedata[2]) * deltay;

         break;

      default:
         break;
   }

   ftr[1] = (i = round(ftr[1]));

   if ((mcinfo & CITEXTURE) && tob->query_tflag(ptr->id)) {
       ptr->dt[0] = (ptr->end.uv[0] - ptr->start.uv[0]) * deltay;
       ptr->dt[1] = (ptr->end.uv[1] - ptr->start.uv[1]) * deltay;
   }

   return i;
}


/* *************************************************************
   this procedure creates the main edge table
************************************************************* */
void polygon::add_edge_table(eye *parm, light *lmain) {

   edgetype *ptr;                              // ptr to edge table
   int     *pointer = (int *)NULL;             // ptr to vertices
   int     i, j, k;                            // loop variables
   int     top, bottom;
   int     b, t;
   int     textureflag = mcinfo & CITEXTURE;

   k = ob->maxpolynum;

   if (!k)
      return;

   pointer = new int[k+1];

   for (i=0; i<ob->countobject; i++)
      if (ob->polynum[i]) {
         for (j=0; j<ob->polynum[i]; j++)
            pointer[j] = ob->edgeptr[i][j];

         pointer[ob->polynum[i]] = pointer[0];

         for (j=0; j<ob->polynum[i]; j++)
            if (round(ob->mvertex[pointer[j]][1]) !=
                round(ob->mvertex[pointer[j+1]][1])) {

               ptr = (edgetype *)control.pop(EDGETYPE);
               ptr->id = i;

               if (ob->mvertex[pointer[j]][1] < ob->mvertex[pointer[j+1]][1]) {
                  bottom   = pointer[b=j];
                  top      = pointer[t=j+1];
               }

               else {
                  bottom = pointer[b=j+1];
                  top    = pointer[t=j];
               }

               copyarray3(ptr->start.point, ob->mvertex[bottom]);
               copyarray3(ptr->start.shadedata, ob->gnormal[bottom]);

               copyarray3(ptr->end.point, ob->mvertex[top]);
               copyarray3(ptr->end.shadedata, ob->gnormal[top]);

               if (textureflag && tob->query_tflag(i)) {
                  tob->query_uvmap(i, b, ptr->start.uv);
                  tob->query_uvmap(i, t, ptr->end.uv);
               }

               ptr->starty = prepare_edge(i, ptr, parm, lmain);

               fillbucket(&et[i], ptr);
            }

            else if (mctype < CONSTANT && mctype > SHADOW &&
                round(ob->mvertex[pointer[j]][0]) !=
                round(ob->mvertex[pointer[j+1]][0])) { // handles horizontal lines

               ptr = (edgetype *)control.pop(EDGETYPE);

               if (ob->mvertex[pointer[j]][0] < ob->mvertex[pointer[j+1]][0]) {
                  bottom   = pointer[j];
                  top      = pointer[j+1];
               }

               else {
                  bottom   = pointer[j+1];
                  top      = pointer[j];
               }

               copyarray3(ptr->start.point, ob->mvertex[bottom]);
               ptr->end.point[0] = ob->mvertex[top][0];
               ptr->dx = ptr->end.point[0] - ptr->start.point[0];
               ptr->dz = (ob->mvertex[top][2] - ptr->start.point[2]) / ptr->dx;

               ptr->next = et[ob->countobject];
               et[ob->countobject] = ptr;
            }

      }

   delete [] pointer;
}


/* *************************************************************
   this procedure creates the main edge table
************************************************************* */
void polygon::add_edge_table(edgetype *ptr, int i, eye *parm, light *lmain) {

   vector3f temp;

   if (round(ptr->start.point[1]) != round(ptr->end.point[1])) {
      ptr->starty = prepare_edge(i, ptr, parm, lmain);
      fillbucket(&et[i], ptr);

      return;
   }

   if (mctype > SHADOW && mctype < CONSTANT && round(ptr->start.point[0]) != round(ptr->end.point[0])) { // handles horizontal lines
      if (ptr->start.point[0] > ptr->end.point[0]) {
         copyarray3(temp, ptr->start.point);
         copyarray3(ptr->start.point, ptr->end.point);
         copyarray3(ptr->end.point, temp);
      }

      ptr->dx = ptr->end.point[0] - ptr->start.point[0];
      ptr->dz = (ptr->end.point[2]-ptr->start.point[2]) / ptr->dx;

      ptr->next = et[ob->countobject];
      et[ob->countobject] = ptr;
      return;
   }

   control.push(EDGETYPE, ptr);
}


/* *************************************************************
************************************************************* */
edgetype *polygon::edgelist(int i) {

   edgetype *ptr;
   edgetype **btr;
   register edgetype *qtr;
   register int j, l;
   int k;
   int textureflag = mcinfo & CITEXTURE;
   int *pointer, *counter;

   k = ob->polynum[i];
   pointer = new int[k+1];
   counter = new int[k];

   for (j=0; j<k; j++) {
      pointer[j] = ob->edgeptr[i][j];
      counter[j] = j+1;
   }

   pointer[k] = pointer[0];
   counter[k-1] = 0;

   btr = &ptr;

   for (j=0; j<k;) {
      qtr = (edgetype *)control.pop(EDGETYPE);
      *btr = qtr;
      btr = &qtr->next;

      qtr->id = i;

      if (textureflag && tob->query_tflag(i)) {
         tob->query_uvmap(i, j, qtr->start.uv);
         tob->query_uvmap(i, counter[j], qtr->end.uv);
      }

      l = pointer[j];
      copyarray3(qtr->start.point, ob->mvertex[l]);
      copyarray3(qtr->start.shadedata, ob->gnormal[l]);
      j++;
      l = pointer[j];
      copyarray3(qtr->end.point, ob->mvertex[l]);
      copyarray3(qtr->end.shadedata, ob->gnormal[l]);
   }

   qtr->next = (edgetype *)NULL;

   delete [] counter;
   delete [] pointer;
   return ptr;
}


/* *************************************************************
   this function is the main clipping routine
************************************************************* */
int polygon::polyclet(eye *parm, light *lmain, int maxx, int maxy) {

   float        extreme[2][4];
   int          i, j, k;
   edgetype     ptr;
   edgetype     *qtr, *pptr;
   float        pscale;

   int textureflag = mcinfo & CITEXTURE;

   if (bbox[0][2] > front ||
       parm->zscale(bbox[0][0], bbox[0][2]) > parm->vrc[1] ||     // outside boundaries
       parm->zscale(bbox[1][0], bbox[0][2]) < parm->vrc[0] ||
       parm->zscale(bbox[0][1], bbox[0][2]) > parm->vrc[3] ||
       parm->zscale(bbox[1][1], bbox[0][2]) < parm->vrc[2])    {

      ob->countobject = 0;
      return 0;
   }

   if (textureflag) {
      pscale = 1.0/parm->query_scale();

      for (i=0; i<ob->countobject; i++)
         if (tob->query_tflag(i))
            facedata[i].area = tob->data[i].area * ob->iarea[i] * pscale;
   }

   if (mctype == CONSTANT)
      constcolor(parm, lmain);

   if (bbox[1][2] < front &&
       parm->zscale(bbox[0][0], bbox[1][2]) > parm->vrc[0] &&      // outside boundaries
       parm->zscale(bbox[1][0], bbox[1][2]) < parm->vrc[1] &&
       parm->zscale(bbox[0][1], bbox[1][2]) > parm->vrc[2] &&
       parm->zscale(bbox[1][1], bbox[1][2]) < parm->vrc[3]) {

      for (i=0; i<ob->countvertex; i++)
         parm->map2screen(ob->mvertex[i]);     // map to screen

      add_edge_table(parm, lmain);
      return 1;
   }

   for (i=0; i<ob->countobject; i++)
      if (ob->polynum[i]) {

         copyarray3(extreme[0], ob->mvertex[ob->edgeptr[i][0]]);   // bound polygon box
         copyarray3(extreme[1], ob->mvertex[ob->edgeptr[i][0]]);   // bound polygon box

         for (j=1; j<ob->polynum[i]; j++) {
            for (k=0; k<3; k++)
               if (extreme[0][k] > ob->mvertex[ob->edgeptr[i][j]][k])
                  extreme[0][k] = ob->mvertex[ob->edgeptr[i][j]][k];
               else if (extreme[1][k] < ob->mvertex[ob->edgeptr[i][j]][k])
                  extreme[1][k] = ob->mvertex[ob->edgeptr[i][j]][k];
         }

         if (extreme[0][2] > front ||            // outside boundaries
             parm->zscale(extreme[0][0], extreme[0][2]) > parm->vrc[1] ||
             parm->zscale(extreme[1][0], extreme[0][2]) < parm->vrc[0] ||
             parm->zscale(extreme[0][1], extreme[0][2]) > parm->vrc[3] ||
             parm->zscale(extreme[1][1], extreme[0][2]) < parm->vrc[2])
            ob->polynum[i] = 0;

         else { // This part basicly clips verticies and creates edge table

            ptr.next = edgelist(i);

            if (extreme[1][2] > front && clip_greater(&ptr, Z, front, ob->normal[i], parm)) {
               post_clip(&ptr);
               extreme[1][2] = front;
            }

            if (ptr.next != (edgetype *)NULL) {

               parm->map2screen(ptr.next->start.point);

               copyarray2(extreme[0], ptr.next->start.point);
               copyarray2(extreme[1], ptr.next->start.point);

               for (qtr=ptr.next; qtr!=(edgetype *)NULL; qtr=qtr->next) { // map to screen
                  parm->map2screen(qtr->end.point);

                  if (qtr->next != (edgetype *)NULL)
                     copyarray3(qtr->next->start.point, qtr->end.point);

                  for (k=0; k<2; k++)
                     if (extreme[0][k] > qtr->end.point[k])
                        extreme[0][k] = qtr->end.point[k];
                     else if (extreme[1][k] < qtr->end.point[k])
                        extreme[1][k] = qtr->end.point[k];
               }

               if (extreme[0][0] < 0.0 && clip_lesser(&ptr, X, 0.0, ob->normal[i], parm))
                  post_clip(&ptr);

               if (extreme[1][0] > maxx-1 && clip_greater(&ptr, X, (float )(maxx-1), ob->normal[i], parm))
                  post_clip(&ptr);

               if (extreme[0][1] < 0.0 && clip_lesser(&ptr, Y, 0.0, ob->normal[i], parm))
                  post_clip(&ptr);

               if (extreme[1][1] > maxy-1 && clip_greater(&ptr, Y, (float )(maxy-1), ob->normal[i], parm))
                  post_clip(&ptr);

               for (qtr=ptr.next; qtr!=(edgetype *)NULL; qtr=ptr.next) {
                  ptr.next = qtr->next;

                  if (qtr->start.point[1] > qtr->end.point[1]) {
                     pptr = (edgetype *)control.pop(EDGETYPE);
                     pptr->id = i;
                     pptr->start = qtr->end;
                     pptr->end = qtr->start;

                     control.push(EDGETYPE, qtr);
                     qtr = pptr;
                  }

                  add_edge_table(qtr, i, parm, lmain);
               }

            }

         }

      }

   return 1;
}
