



/* *************************************************************
   This function scans poligonal objects into a zbuffer
************************************************************* */


#include "polygon.h"
#include "global.h"
#include "memman.h"

/* *************************************************************
                replace w/ bucket sorter
************************************************************* */
void polygon::fillbucket(edgetype **head, edgetype *node) {

   edgetype **btr;
   edgetype *ptr;

   if (!*head) {
      *head = node;
      node->next = (edgetype *)NULL;
      return;
   }

   for (btr = head, ptr=*btr; ptr != NULL && node->starty > ptr->starty; btr = &ptr->next, ptr = *btr);

   node->next = *btr;
   *btr = node;
}


/* *************************************************************
************************************************************* */
void polygon::sortbucket(edgetype **head, edgetype *node) {

   edgetype **btr;
   edgetype *ptr;

   if (!*head) {
      *head = node;
      node->next = (edgetype *)NULL;
      return;
   }

   for (btr = head, ptr=*btr; ptr != NULL && node->start.point[0] > ptr->start.point[0]; btr = &ptr->next, ptr = *btr);

   node->next = *btr;
   *btr = node;
}


/* *************************************************************
   this function calculates the constant coloring by interpolating
   a point from which to calculate the polygon's color
************************************************************* */
void polygon::constcolor(eye *parm, light *lmain) {

   int  i, j;                           // looping variables
   float surface[3];                     // surface point
   float div;

   if (!lmain)
      return;

   for (i=0; i<ob->countobject; i++) {

      facedata[i].constcolor[0] = facedata[i].constcolor[1] = facedata[i].constcolor[2] = 0;

      if (ob->polynum[i]) {

         div = 1.0/ob->polynum[i];

         copyarray3(surface, ob->mvertex[ob->edgeptr[i][0]]);

         for (j=1; j<ob->polynum[i]; j++)
            addarray3(surface, ob->mvertex[ob->edgeptr[i][j]]);

         smultarray3(surface, div);
         lmain->intensity(surface, ob->normal[i], facedata[i].constcolor, parm, ob->shade[i].color, this);
//         addarray3(facedata[i].constcolor, ob->shade[i].ambient);
      }

   }

}


/* *************************************************************
   This procedure calculates linear interpolation in object space
                T = (pn+d)/(vn)
                pt = (0,0,0)
                ray = (x, y, -1)
           FAR: pt = (x, y, 0)
                ray = (0, 0, -1)
************************************************************* */
float polygon::engine(float *normal, float *middle, eye *parm, float *prenum, float *prediv) {

   if (parm->query_whatami() == FAR)
      if (abs(normal[2]) > CORRECT) {
         *prenum = (parm->imscale*normal[0])/normal[2];
         return *prediv = ((middle[0]*parm->imscale + parm->vrc[0])*normal[0] +
                           (middle[1]*parm->imscale + parm->vrc[2])*normal[1] +
                           normal[3])/(-normal[2]);
      }

      else {
         *prenum = 0.0;
         return *prediv = back;
      }

   *prenum = parm->imscale*normal[0];
   *prediv = (middle[0]*parm->imscale + parm->vrc[0])*normal[0] +
             (middle[1]*parm->imscale + parm->vrc[2])*normal[1] - normal[2];
   return (abs(*prediv) > CORRECT) ? normal[3]/(*prediv) : back;
}


/* **************************************************
************************************************** */
void polygon::polylt(spotlight *parm, int winx, int winy) {

   edgetype *qtr;               // temp pointer
   COMMON;

#define INTZ
#define POLYLT
#include "polyplate.c"
#undef POLYLT
#undef INTZ
}


/* **************************************************
************************************************** */
void polygon::polyltr(spotlight *parm, int winx, int winy) {

   edgetype *qtr;               // temp pointer
   vector4f surface;
   COMMON;

#define POLYLT
#include "polyplate.c"
#undef POLYLT
}


/* **************************************************
************************************************** */
void polygon::polywfbw(zbuffer *zbuff) {

   float    mtempz, mdeltaz;
   int      j;
   int      toprow = zbuff->maxx*(zbuff->maxy-1);
   COMMON;

   for (;et[ob->countobject] != (edgetype *)NULL; et[ob->countobject]=ptr->next, control.push(EDGETYPE, ptr)) {    // horizontal lines
      ptr = et[ob->countobject];

      scany = round(ptr->start.point[1]);
      if (INTERLACE && (scany & 0x01))
         continue;

#ifdef DOS
      scanxy = toprow-scany*zbuff->maxx;
#else
      scanxy = scany*zbuff->maxx;
#endif

      index = scanxy + round(ptr->start.point[0]);
      end   = scanxy + round(ptr->end.point[0]);

      for (; index<end; index++, ptr->start.point[2] += ptr->dz)
         if (ptr->start.point[2]>zbuff->zbuff[index].zdata) {
            zbuff->zbuff[index].zdata = ptr->start.point[2];
            *(unsigned int *)zbuff->zbuff[index].pixel   = 0xffffffff;
            *(unsigned int *)zbuff->zbuff[index].ambient = 0xffffffff;
         }

   }

   for (i=0; i<ob->countobject; i++)
      while (et[i]) {
         ptr = et[i];
         et[i] = et[i]->next;

         scany = ptr->starty;
#ifdef DOS
      scanxy = toprow-scany*zbuff->maxx;
#else
      scanxy = scany*zbuff->maxx;
#endif

         while (scany < zbuff->maxy) {               // scan all the lines
            start = round(ptr->start.point[0]);

            if (!INTERLACE || !(scany & 0x01)) {
#include "polywfbw.c"
            }

            scany++;
#ifdef DOS
            scanxy -= zbuff->maxx;
#else
            scanxy += zbuff->maxx;
#endif

            if (scany >= round(ptr->end.point[1]))
               break;

            ptr->start.point[0] += ptr->dx;
            ptr->start.point[2] += ptr->dz;
         }

         control.push(EDGETYPE, ptr);
      }

}


/* **************************************************
************************************************** */
void polygon::polybw(zbuffer *zbuff, int winx, int winy) {

   edgetype  *qtr;                     // temp pointer
   float     mtempz[2];
   float     mdeltaz;
   int       mtemp[3];
   float     sz;
   int       j;
   int       toprow = zbuff->maxx*(zbuff->maxy-1);
   COMMON;

   for (;et[ob->countobject] != (edgetype *)NULL; et[ob->countobject] = ptr->next, control.push(EDGETYPE, ptr)) {            // horizontal lines

      ptr = et[ob->countobject];

      scany = round(ptr->start.point[1]);

      if (INTERLACE && (scany & 0x01))
         continue;

      start = round(ptr->start.point[0]);
      end   = round(ptr->end.point[0]);

#ifdef DOS
      scanxy = toprow-scany*zbuff->maxx;
#else
      scanxy = scany*zbuff->maxx;
#endif
      index = scanxy + start;

      sz = ptr->start.point[2];

      for (; start<end; start++, index++) {

         if (sz>zbuff->zbuff[index].zdata) {
            zbuff->zbuff[index].zdata = sz+CORRECT;
            *(unsigned int *)zbuff->zbuff[index].ambient = 0xffffffff;
            *(unsigned int *)zbuff->zbuff[index].pixel   = 0xffffffff;
         }

         sz += ptr->dz;
      }

   }

#define INTZ
#define POLYBW
#include "polyplate.c"
#undef POLYBW
#undef INTZ
}


/* **************************************************
************************************************** */
void polygon::polybwr(eye *parm, zbuffer *zbuff, int winx, int winy) {

   edgetype  *qtr;                     // temp pointer
   int       mtemp[3];
   vector4f  surface;
   float     sz;
   int       j;
   float     prenum, prediv;
   int       toprow = zbuff->maxx*(zbuff->maxy-1);
   COMMON;

   for (;et[ob->countobject] != (edgetype *)NULL; et[ob->countobject] = ptr->next, control.push(EDGETYPE, ptr)) {            // horizontal lines

      ptr = et[ob->countobject];

      scany = round(ptr->start.point[1]);

      if (INTERLACE && (scany & 0x01))
         continue;

      start = round(ptr->start.point[0]);
      end   = round(ptr->end.point[0]);

#ifdef DOS
      scanxy = toprow-scany*zbuff->maxx;
#else
      scanxy = scany*zbuff->maxx;
#endif

      surface[1] = ptr->start.point[1];
      surface[0] = start - 1.0;

      index = scanxy + start;

      engine(ob->normal[ptr->id], surface, parm, &prenum, &prediv);

      for (; start<end; start++, index++) {
         engine_dx(ob->normal[ptr->id], prenum, prediv, sz);

         if (sz>zbuff->zbuff[index].zdata) {
            zbuff->zbuff[index].zdata = sz+CORRECT;
            *(unsigned int *)zbuff->zbuff[index].pixel   = 0xffffffff;
            *(unsigned int *)zbuff->zbuff[index].ambient = 0xffffffff;
         }

      }

   }

#define POLYBW
#include "polyplate.c"
#undef POLYBW
}


#ifndef DOS
/* **************************************************
************************************************** */
void polygon::polyct(zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   unsigned char flatacolor[4], flatdcolor[4];
   COMMON;
   SHADECOMMON;

#define POLYNORMAL
#define INTZ
#define POLYCT
#include "polyplate.c"
#undef POLYCT

#undef INTZ
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polyct2(zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   COMMON;
   SHADECOMMON;

#define POLY2DTEXTURE
#define INTZ
#define POLYCT
#include "polyplate.c"
#undef POLYCT
#undef INTZ
#undef POLY2DTEXTURE
}


#endif
/* **************************************************
************************************************** */
void polygon::polyctr(eye *parm, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   unsigned char flatacolor[4], flatdcolor[4];
   COMMON;
   SHADECOMMON;

#define POLYNORMAL
#define POLYCT
#include "polyplate.c"
#undef POLYCT
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polyctr2(eye *parm, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   COMMON;
   SHADECOMMON;

#define POLY2DTEXTURE
#define POLYCT
#include "polyplate.c"
#undef POLYCT
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polyct3(eye *parm, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   COMMON;
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define INTZ
#define POLYCT
#include "polyplate.c"
#undef POLYCT
#undef INTZ
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polyctr3(eye *parm, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   COMMON;
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define POLYCT
#include "polyplate.c"
#undef POLYCT
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polyctt(zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   COMMON;
   SHADECOMMON;

#define POLYNORMAL
#define POLYTRANSPARENT
#define INTZ
#define POLYCT
#include "polyplate.c"
#undef POLYCT
#undef INTZ
#undef POLYTRANSPARENT
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polyctt2(zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   COMMON;
   SHADECOMMON;

#define POLY2DTEXTURE
#define POLYTRANSPARENT
#define INTZ
#define POLYCT
#include "polyplate.c"
#undef POLYCT
#undef INTZ
#undef POLYTRANSPARENT
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polycttr(eye *parm, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   COMMON;
   SHADECOMMON;

#define POLYNORMAL
#define POLYTRANSPARENT
#define POLYCT
#include "polyplate.c"
#undef POLYCT
#undef POLYTRANSPARENT
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polycttr2(eye *parm, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   COMMON;
   SHADECOMMON;

#define POLY2DTEXTURE
#define POLYTRANSPARENT
#define POLYCT
#include "polyplate.c"
#undef POLYCT
#undef POLYTRANSPARENT
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polyctt3(eye *parm, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   COMMON;
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define POLYTRANSPARENT
#define INTZ
#define POLYCT
#include "polyplate.c"
#undef POLYCT
#undef INTZ
#undef POLYTRANSPARENT
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polycttr3(eye *parm, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   COMMON;
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define POLYTRANSPARENT
#define POLYCT
#include "polyplate.c"
#undef POLYCT
#undef POLYTRANSPARENT
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polyctb(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   COMMON;
   SHADECOMMON;

#define POLYNORMAL
#define POLYBEAM
#define INTZ
#define POLYCT
#include "polyplate.c"
#undef POLYCT
#undef INTZ
#undef POLYBEAM
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polyctb2(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   COMMON;
   SHADECOMMON;

#define POLY2DTEXTURE
#define POLYBEAM
#define INTZ
#define POLYCT
#include "polyplate.c"
#undef POLYCT
#undef INTZ
#undef POLYBEAM
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polyctbr(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   COMMON;
   SHADECOMMON;

#define POLYNORMAL
#define POLYBEAM
#define POLYCT
#include "polyplate.c"
#undef POLYCT
#undef POLYBEAM
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polyctbr2(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   COMMON;
   SHADECOMMON;

#define POLY2DTEXTURE
#define POLYBEAM
#define POLYCT
#include "polyplate.c"
#undef POLYCT
#undef POLYBEAM
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polyctb3(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   COMMON;
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define POLYBEAM
#define INTZ
#define POLYCT
#include "polyplate.c"
#undef POLYCT
#undef INTZ
#undef POLYBEAM
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polyctbr3(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   COMMON;
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define POLYBEAM
#define POLYCT
#include "polyplate.c"
#undef POLYCT
#undef POLYBEAM
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polygd(zbuffer *zbuff, int winx, int winy) {

   vector3f gcolor;
   float    mtemp;
   vector3f mdeltac;                    // delta color stuff
   COMMON
   SHADECOMMON;

#define POLYNORMAL
#define INTZ
#define POLYGD
#include "polyplate.c"
#undef POLYGD
#undef INTZ
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polygd2(zbuffer *zbuff, int winx, int winy) {

   vector3f gcolor;
   float    mtemp;
   vector3f mdeltac;                    // delta color stuff
   COMMON
   SHADECOMMON;

#define POLY2DTEXTURE
#define INTZ
#define POLYGD
#include "polyplate.c"
#undef POLYGD
#undef INTZ
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polygdr(eye *parm, zbuffer *zbuff, int winx, int winy) {

   vector3f gcolor;
   float    mtemp;
   vector3f mdeltac;                    // delta color stuff
   COMMON
   SHADECOMMON;

#define POLYNORMAL
#define POLYGD
#include "polyplate.c"
#undef POLYGD
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polygdr2(eye *parm, zbuffer *zbuff, int winx, int winy) {

   vector3f gcolor;
   float    mtemp;
   vector3f mdeltac;                    // delta color stuff
   COMMON
   SHADECOMMON;

#define POLY2DTEXTURE
#define POLYGD
#include "polyplate.c"
#undef POLYGD
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polygd3(eye *parm, zbuffer *zbuff, int winx, int winy) {

   vector3f gcolor;
   float    mtemp;
   vector3f mdeltac;                    // delta color stuff
   COMMON
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define INTZ
#define POLYGD
#include "polyplate.c"
#undef POLYGD
#undef INTZ
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polygdr3(eye *parm, zbuffer *zbuff, int winx, int winy) {

   vector3f gcolor;
   float    mtemp;
   vector3f mdeltac;                    // delta color stuff
   COMMON
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define POLYGD
#include "polyplate.c"
#undef POLYGD
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polygdt(zbuffer *zbuff, int winx, int winy) {

   vector3f gcolor;
   float    mtemp;
   vector3f mdeltac;                    // delta color stuff
   COMMON
   SHADECOMMON;

#define POLYNORMAL
#define POLYTRANSPARENT
#define INTZ
#define POLYGD
#include "polyplate.c"
#undef POLYGD
#undef INTZ
#undef POLYTRANSPARENT
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polygdt2(zbuffer *zbuff, int winx, int winy) {

   vector3f gcolor;
   float    mtemp;
   vector3f mdeltac;                    // delta color stuff
   COMMON
   SHADECOMMON;

#define POLY2DTEXTURE
#define POLYTRANSPARENT
#define INTZ
#define POLYGD
#include "polyplate.c"
#undef POLYGD
#undef INTZ
#undef POLYTRANSPARENT
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polygdtr(eye *parm, zbuffer *zbuff, int winx, int winy) {

   vector3f gcolor;
   float    mtemp;
   vector3f mdeltac;                    // delta color stuff
   COMMON
   SHADECOMMON;

#define POLYNORMAL
#define POLYTRANSPARENT
#define POLYGD
#include "polyplate.c"
#undef POLYGD
#undef POLYTRANSPARENT
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polygdtr2(eye *parm, zbuffer *zbuff, int winx, int winy) {

   vector3f gcolor;
   float    mtemp;
   vector3f mdeltac;                    // delta color stuff
   COMMON
   SHADECOMMON;

#define POLY2DTEXTURE
#define POLYTRANSPARENT
#define POLYGD
#include "polyplate.c"
#undef POLYGD
#undef POLYTRANSPARENT
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polygdt3(eye *parm, zbuffer *zbuff, int winx, int winy) {

   vector3f gcolor;
   float    mtemp;
   vector3f mdeltac;                    // delta color stuff
   COMMON
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define POLYTRANSPARENT
#define INTZ
#define POLYGD
#include "polyplate.c"
#undef POLYGD
#undef INTZ
#undef POLYTRANSPARENT
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polygdtr3(eye *parm, zbuffer *zbuff, int winx, int winy) {

   vector3f gcolor;
   float    mtemp;
   vector3f mdeltac;                    // delta color stuff
   COMMON
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define POLYTRANSPARENT
#define POLYGD
#include "polyplate.c"
#undef POLYGD
#undef POLYTRANSPARENT
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polygdb(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy) {

   vector3f gcolor;
   float    mtemp;
   vector3f mdeltac;                    // delta color stuff
   COMMON
   SHADECOMMON;

#define POLYNORMAL
#define INTZ
#define POLYBEAM
#define POLYGD
#include "polyplate.c"
#undef POLYGD
#undef POLYBEAM
#undef INTZ
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polygdb2(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy) {

   vector3f gcolor;
   float    mtemp;
   vector3f mdeltac;                    // delta color stuff
   COMMON
   SHADECOMMON;

#define POLY2DTEXTURE
#define INTZ
#define POLYBEAM
#define POLYGD
#include "polyplate.c"
#undef POLYGD
#undef POLYBEAM
#undef INTZ
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polygdbr(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy) {

   vector3f gcolor;
   float    mtemp;
   vector3f mdeltac;                    // delta color stuff
   COMMON
   SHADECOMMON;

#define POLYNORMAL
#define POLYBEAM
#define POLYGD
#include "polyplate.c"
#undef POLYGD
#undef POLYBEAM
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polygdbr2(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy) {

   vector3f gcolor;
   float    mtemp;
   vector3f mdeltac;                    // delta color stuff
   COMMON
   SHADECOMMON;

#define POLY2DTEXTURE
#define POLYBEAM
#define POLYGD
#include "polyplate.c"
#undef POLYGD
#undef POLYBEAM
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polygdb3(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy) {

   vector3f gcolor;
   float    mtemp;
   vector3f mdeltac;                    // delta color stuff
   COMMON
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define INTZ
#define POLYBEAM
#define POLYGD
#include "polyplate.c"
#undef POLYGD
#undef POLYBEAM
#undef INTZ
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polygdbr3(eye *parm, light *spot, zbuffer *zbuff, int winx, int winy) {

   vector3f gcolor;
   float    mtemp;
   vector3f mdeltac;                    // delta color stuff
   COMMON
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define POLYBEAM
#define POLYGD
#include "polyplate.c"
#undef POLYGD
#undef POLYBEAM
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polyal(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float     mtemp;
   COMMON;
   SHADECOMMON;

#define POLYNORMAL
#define INTZ
#define POLYAL
#include "polyplate.c"
#undef POLYAL
#undef INTZ
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polyal2(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float     mtemp;
   COMMON;
   SHADECOMMON;

#define POLY2DTEXTURE
#define INTZ
#define POLYAL
#include "polyplate.c"
#undef POLYAL
#undef INTZ
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polyalr(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float     mtemp;
   COMMON;
   SHADECOMMON;

#define POLYNORMAL
#define POLYAL
#include "polyplate.c"
#undef POLYAL
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polyalr2(eye *parm, light *lmain,zbuffer *zbuff, int winx, int winy) {

   float     mtemp;
   COMMON;
   SHADECOMMON;

#define POLY2DTEXTURE
#define POLYAL
#include "polyplate.c"
#undef POLYAL
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polyal3(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float     mtemp;
   COMMON;
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define INTZ
#define POLYAL
#include "polyplate.c"
#undef POLYAL
#undef INTZ
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polyalr3(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float     mtemp;
   COMMON;
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define POLYAL
#include "polyplate.c"
#undef POLYAL
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polyalt(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float     mtemp;
   COMMON;
   SHADECOMMON;

#define POLYNORMAL
#define POLYTRANSPARENT
#define INTZ
#define POLYAL
#include "polyplate.c"
#undef POLYAL
#undef INTZ
#undef POLYTRANSPARENT
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polyalt2(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float     mtemp;
   COMMON;
   SHADECOMMON;

#define POLY2DTEXTURE
#define POLYTRANSPARENT
#define INTZ
#define POLYAL
#include "polyplate.c"
#undef POLYAL
#undef INTZ
#undef POLYTRANSPARENT
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polyaltr(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float     mtemp;
   COMMON;
   SHADECOMMON;

#define POLYNORMAL
#define POLYTRANSPARENT
#define POLYAL
#include "polyplate.c"
#undef POLYAL
#undef POLYTRANSPARENT
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polyaltr2(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float     mtemp;
   COMMON;
   SHADECOMMON;

#define POLY2DTEXTURE
#define POLYTRANSPARENT
#define POLYAL
#include "polyplate.c"
#undef POLYAL
#undef POLYTRANSPARENT
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polyalt3(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float     mtemp;
   COMMON;
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define POLYTRANSPARENT
#define INTZ
#define POLYAL
#include "polyplate.c"
#undef POLYAL
#undef INTZ
#undef POLYTRANSPARENT
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polyaltr3(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float     mtemp;
   COMMON;
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define POLYTRANSPARENT
#define POLYAL
#include "polyplate.c"
#undef POLYAL
#undef POLYTRANSPARENT
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polyalb(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy) {

   float     mtemp;
   COMMON;
   SHADECOMMON;

#define POLYNORMAL
#define INTZ
#define POLYBEAM
#define POLYAL
#include "polyplate.c"
#undef POLYAL
#undef POLYBEAM
#undef INTZ
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polyalb2(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy) {

   float     mtemp;
   COMMON;
   SHADECOMMON;

#define POLY2DTEXTURE
#define INTZ
#define POLYBEAM
#define POLYAL
#include "polyplate.c"
#undef POLYAL
#undef POLYBEAM
#undef INTZ
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polyalbr(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy) {

   float     mtemp;
   COMMON;
   SHADECOMMON;

#define POLYNORMAL
#define POLYBEAM
#define POLYAL
#include "polyplate.c"
#undef POLYAL
#undef POLYBEAM
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polyalbr2(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy) {

   float     mtemp;
   COMMON;
   SHADECOMMON;

#define POLY2DTEXTURE
#define POLYBEAM
#define POLYAL
#include "polyplate.c"
#undef POLYAL
#undef POLYBEAM
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polyalb3(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy) {

   float     mtemp;
   COMMON;
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define INTZ
#define POLYBEAM
#define POLYAL
#include "polyplate.c"
#undef POLYAL
#undef POLYBEAM
#undef INTZ
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polyalbr3(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy) {

   float     mtemp;
   COMMON;
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define POLYBEAM
#define POLYAL
#include "polyplate.c"
#undef POLYAL
#undef POLYBEAM
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polypg(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   vector3f normal, snormal, enormal;
   vector4f temp;
   float      s, e, is, ie;
   COMMON;
   SHADECOMMON;

#define POLYNORMAL
#define INTZ
#define POLYPG
#include "polyplate.c"
#undef POLYPG
#undef INTZ
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polypg2(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   vector3f normal, snormal, enormal;
   vector4f temp;
   float      s, e, is, ie;
   COMMON;
   SHADECOMMON;

#define POLY2DTEXTURE
#define INTZ
#define POLYPG
#include "polyplate.c"
#undef POLYPG
#undef INTZ
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polypgr(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   vector3f normal, snormal, enormal;
   vector4f temp;
   float      s, e, is, ie;
   COMMON;
   SHADECOMMON;

#define POLYNORMAL
#define POLYPG
#include "polyplate.c"
#undef POLYPG
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polypgr2(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   vector3f normal, snormal, enormal;
   vector4f temp;
   float      s, e, is, ie;
   COMMON;
   SHADECOMMON;

#define POLY2DTEXTURE
#define POLYPG
#include "polyplate.c"
#undef POLYPG
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polypg3(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   vector3f normal, snormal, enormal;
   vector4f temp;
   float      s, e, is, ie;
   COMMON;
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define INTZ
#define POLYPG
#include "polyplate.c"
#undef POLYPG
#undef INTZ
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polypgr3(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   vector3f normal, snormal, enormal;
   vector4f temp;
   float      s, e, is, ie;
   COMMON;
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define POLYPG
#include "polyplate.c"
#undef POLYPG
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polypgt(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   vector3f normal, snormal, enormal;
   vector4f temp;
   float      s, e, is, ie;
   COMMON;
   SHADECOMMON;

#define POLYNORMAL
#define POLYTRANSPARENT
#define INTZ
#define POLYPG
#include "polyplate.c"
#undef POLYPG
#undef INTZ
#undef POLYTRANSPARENT
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polypgt2(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   vector3f normal, snormal, enormal;
   vector4f temp;
   float      s, e, is, ie;
   COMMON;
   SHADECOMMON;

#define POLY2DTEXTURE
#define POLYTRANSPARENT
#define INTZ
#define POLYPG
#include "polyplate.c"
#undef POLYPG
#undef INTZ
#undef POLYTRANSPARENT
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polypgtr(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   vector3f normal, snormal, enormal;
   vector4f temp;
   float      s, e, is, ie;
   COMMON;
   SHADECOMMON;

#define POLYNORMAL
#define POLYTRANSPARENT
#define POLYPG
#include "polyplate.c"
#undef POLYPG
#undef POLYTRANSPARENT
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polypgtr2(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   vector3f normal, snormal, enormal;
   vector4f temp;
   float      s, e, is, ie;
   COMMON;
   SHADECOMMON;

#define POLY2DTEXTURE
#define POLYTRANSPARENT
#define POLYPG
#include "polyplate.c"
#undef POLYPG
#undef POLYTRANSPARENT
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polypgt3(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   vector3f normal, snormal, enormal;
   vector4f temp;
   float      s, e, is, ie;
   COMMON;
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define POLYTRANSPARENT
#define INTZ
#define POLYPG
#include "polyplate.c"
#undef POLYPG
#undef INTZ
#undef POLYTRANSPARENT
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polypgtr3(eye *parm, light *lmain, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   vector3f normal, snormal, enormal;
   vector4f temp;
   float      s, e, is, ie;
   COMMON;
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define POLYTRANSPARENT
#define POLYPG
#include "polyplate.c"
#undef POLYPG
#undef POLYTRANSPARENT
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polypgb(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   vector3f normal, snormal, enormal;
   vector4f temp;
   float      s, e, is, ie;
   COMMON;
   SHADECOMMON;

#define POLYNORMAL
#define INTZ
#define POLYBEAM
#define POLYPG
#include "polyplate.c"
#undef POLYPG
#undef POLYBEAM
#undef INTZ
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polypgb2(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   vector3f normal, snormal, enormal;
   vector4f temp;
   float      s, e, is, ie;
   COMMON;
   SHADECOMMON;

#define POLY2DTEXTURE
#define INTZ
#define POLYBEAM
#define POLYPG
#include "polyplate.c"
#undef POLYPG
#undef POLYBEAM
#undef INTZ
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polypgbr(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   vector3f normal, snormal, enormal;
   vector4f temp;
   float      s, e, is, ie;
   COMMON;
   SHADECOMMON;

#define POLYNORMAL
#define POLYBEAM
#define POLYPG
#include "polyplate.c"
#undef POLYPG
#undef POLYBEAM
#undef POLYNORMAL
}


/* **************************************************
************************************************** */
void polygon::polypgbr2(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   vector3f normal, snormal, enormal;
   vector4f temp;
   float      s, e, is, ie;
   COMMON;
   SHADECOMMON;

#define POLY2DTEXTURE
#define POLYBEAM
#define POLYPG
#include "polyplate.c"
#undef POLYPG
#undef POLYBEAM
#undef POLY2DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polypgb3(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   vector3f normal, snormal, enormal;
   vector4f temp;
   float      s, e, is, ie;
   COMMON;
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define INTZ
#define POLYBEAM
#define POLYPG
#include "polyplate.c"
#undef POLYPG
#undef POLYBEAM
#undef INTZ
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::polypgbr3(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int winx, int winy) {

   float mtemp;
   vector3f normal, snormal, enormal;
   vector4f temp;
   float      s, e, is, ie;
   COMMON;
   SHADECOMMON;
   SHADE3D;

#define POLY3DTEXTURE
#define POLYBEAM
#define POLYPG
#include "polyplate.c"
#undef POLYPG
#undef POLYBEAM
#undef POLY3DTEXTURE
}


/* **************************************************
************************************************** */
void polygon::beamrender(spotlight *spot) {

   if (mcinfo & CIRAYCAST)
      polyltr(spot, spot->maxx, spot->maxy);
   else
      polylt(spot, spot->maxx, spot->maxy);
}


/* **************************************************
************************************************** */
void polygon::render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff) {

   unsigned int mask;

   mask = mcinfo & ~(CIBITMAP | CIINVISO);

   if (!(mcinfo & CITRANSPARENT) && spot)
      mask |= CISPOT;

   switch (mctype) {

      case PHONG:
         switch (mask) {
            case 0x00:                  // basic
               polypg(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x01:                  // 2D texture
               polypg2(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x04:                  // raycast
               polypgr(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x05:                  // raycast, 2D texture
               polypgr2(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x08:                  // 3D texture
               polypg3(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x0C:                  // 3D texture, raycast
               polypgr3(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x10:                  // transparent;
               polypgt(lmain, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x11:                  // transparent, 2D texture;
               polypgt2(lmain, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x14:                  // transparent, raycast;
               polypgtr(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x15:                  // transparent, raycast, 2D texture;
               polypgtr2(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x18:                  // transparent, 3D texture
               polypgt3(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x1C:                  // transparent, raycast, 3D texture
               polypgtr3(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x40:                  // beam;
               polypgb(cparm, lmain, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x41:                  // beam, 2D texture;
               polypgb2(cparm, lmain, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x44:                  // beam, raycast;
               polypgbr(cparm, lmain, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x45:                  // beam, raycast, 2D texture;
               polypgbr2(cparm, lmain, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x48:                  // 3D texture
               polypgb3(cparm, lmain, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x4C:                  // 3D texture, raycast
               polypgbr3(cparm, lmain, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            default:
               printf("BAD phong shader %d\n", mask);
               return;
         }

      case ALEX:
         switch (mask) {
            case 0x00:                  // basic
               polyal(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x01:                  // 2D texture
               polyal2(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x04:                  // raycast
               polyalr(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x05:                  // raycast, 2D texture
               polyalr2(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x08:                  // 3D texture
               polyal3(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x0C:                  // 3D texture, raycast
               polyalr3(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x10:                  // transparent;
               polyalt(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x11:                  // transparent, 2D texture;
               polyalt2(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x14:                  // transparent, raycast;
               polyaltr(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x15:                  // transparent, raycast, 2D texture;
               polyaltr2(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x18:                  // transparent, 3D texture
               polyalt3(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x1C:                  // transparent, raycast, 3D texture
               polyaltr3(cparm, lmain, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x40:                  // beam;
               polyalb(cparm, lmain, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x41:                  // beam, 2D texture;
               polyalb2(cparm, lmain, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x44:                  // beam, raycast;
               polyalbr(cparm, lmain, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x45:                  // beam, raycast, 2D texture;
               polyalbr2(cparm, lmain, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x48:                  // 3D texture
               polyalb3(cparm, lmain, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x4C:                  // 3D texture, raycast
               polyalbr3(cparm, lmain, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            default:
               printf("BAD alex shader %d\n", mask);
               return;
         }

      case GOURAUD:

         switch (mask) {
            case 0x00:                  // basic
               polygd(zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x01:                  // 2D texture
               polygd2(zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x04:                  // raycast
               polygdr(cparm, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x05:                  // raycast, 2D texture
               polygdr2(cparm, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x08:                  // 3D texture
               polygd3(cparm, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x0C:                  // 3D texture, raycast
               polygdr3(cparm, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x10:                  // transparent;
               polygdt(zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x11:                  // transparent, 2D texture;
               polygdt2(zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x14:                  // transparent, raycast;
               polygdtr(cparm, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x15:                  // transparent, raycast, 2D texture;
               polygdtr2(cparm, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x18:                  // transparent, 3D texture
               polygdt3(cparm, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x1C:                  // transparent, raycast, 3D texture
               polygdtr3(cparm, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x40:                  // beam;
               polygdb(cparm, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x41:                  // beam, 2D texture;
               polygdb2(cparm, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x44:                  // beam, raycast;
               polygdbr(cparm, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x45:                  // beam, raycast, 2D texture;
               polygdbr2(cparm, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x48:                  // 3D texture
               polygdb3(cparm, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x4C:                  // 3D texture, raycast
               polygdbr3(cparm, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            default:
               printf("BAD gouraud shader %d\n", mask);
               return;
         }

      case CONSTANT:

         switch (mask) {
            case 0x00:                  // basic
               polyct(zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x01:                  // 2D texture
               polyct2(zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x04:                  // raycast
               polyctr(cparm, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x05:                  // raycast, 2D texture
               polyctr2(cparm, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x08:                  // 3D texture
               polyct3(cparm, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x0C:                  // 3D texture, raycast
               polyctr3(cparm, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x10:                  // transparent;
               polyctt(zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x11:                  // transparent, 2D texture;
               polyctt2(zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x14:                  // transparent, raycast;
               polycttr(cparm, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x15:                  // transparent, raycast, 2D texture;
               polycttr2(cparm, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x18:                  // transparent, 3D texture
               polyctt3(cparm, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x1C:                  // transparent, raycast, 3D texture
               polycttr3(cparm, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x40:                  // beam;
               polyctb(cparm, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x41:                  // beam, 2D texture;
               polyctb2(cparm, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x44:                  // beam, raycast;
               polyctbr(cparm, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x45:                  // beam, raycast, 2D texture;
               polyctbr2(cparm, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x48:                  // 3D texture
               polyctb3(cparm, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;

            case 0x4C:                  // 3D texture, raycast
               polyctbr3(cparm, spot, zbuff, zbuff->maxx, zbuff->maxy);
               return;
            default:
               printf("BAD constant shader %d\n", mask);
               return;
         }

      case BW:
         if (mcinfo & CIRAYCAST)
            polybwr(cparm, zbuff, zbuff->maxx, zbuff->maxy);
         else
            polybw(zbuff, zbuff->maxx, zbuff->maxy);

         return;

      default:
         polywfbw(zbuff);
         return;
   }

}


/* **************************************************
   This function scans polygonal data and puts it in a zbuffer.
************************************************** */
void polygon::prender() {

   register int i;                           // loop counter
   edgetype  *ptr, *qtr;

   for (ptr=et[ob->countobject]; ptr!=(edgetype *)NULL; qtr=ptr, ptr=ptr->next, control.push(EDGETYPE, qtr))
      gpline.insert(frame, round(ptr->start.point[0]), round(ptr->start.point[1]), round(ptr->end.point[0]), round(ptr->start.point[1]));

   for (i=0; i<ob->countobject; i++)                    // scan all the lines
      for (ptr=et[i]; ptr !=(edgetype *)NULL; qtr=ptr, ptr=ptr->next, control.push(EDGETYPE, qtr))
         gpline.insert(frame, round(ptr->start.point[0]), round(ptr->start.point[1]),round(ptr->end.point[0]), round(ptr->end.point[1]));
}

