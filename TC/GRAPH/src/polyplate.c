


#ifndef POLYTRANSPARENT
#ifdef POLYBEAM

#define POLYNTRANSBEAM

#endif
#endif

#ifdef POLY2DTEXTURE
   unsigned int tempcolor;
   vector3f tcolor;
   float mtempt[2];
   float mdeltat[2];
   unsigned int flagt, flagb = mcinfo & CIINVISO;
#endif

#ifndef POLYBW

   int toprow = winx*(winy-1);
#ifndef POLYWFBW
   float mtempz;

#ifdef INTZ
   float mdeltaz;
#endif

#endif

#ifndef INTZ
   float prenum, prediv;
#endif

#endif

   for (i=0; i<ob->countobject; i++)
      if (et[i]) {
         scany = et[i]->starty;
         scanfy = (float)scany;
         aet = (edgetype *)NULL;
#ifdef DOS
#ifdef POLYLT
         scanxy = scany*winx;
#else
         scanxy = toprow - scany*winx;
#endif
#else
         scanxy = scany*winx;
#endif

#ifdef POLY2DTEXTURE
         flagt = tob->query_tflag(i);
         tcolor[0] = tcolor[1] = tcolor[2] = 1.0;
#endif

#ifdef POLYCT
         texcolor = ob->shade[i];

#ifndef POLYTRANSPARENT
#ifndef POLYBEAM
#ifdef POLYNORMAL
         calc_color(texcolor.ambient, facedata[i].constcolor, rcolor, acolor);
         addarray3(rcolor, acolor);

#ifdef LINUX
         F2I(a0, acolor[0]);
         F2I(a1, acolor[1]);
         F2I(a2, acolor[2]);
         F2I(d0, rcolor[0]);
         F2I(d1, rcolor[1]);
         F2I(d2, rcolor[2]);
         WAIT();

         flatacolor[0] = BYTE_LUT(a0);
         flatacolor[1] = BYTE_LUT(a1);
         flatacolor[2] = BYTE_LUT(a2);

         flatdcolor[0] = BYTE_LUT(d0);
         flatdcolor[1] = BYTE_LUT(d1);
         flatdcolor[2] = BYTE_LUT(d2);
#endif

#ifdef DOS
         F2I(a0, acolor);
         F2I(a1, acolor+4);
         F2I(a2, acolor+8);
         F2I(d0, rcolor);
         F2I(d1, rcolor+4);
         F2I(d2, rcolor+8);
         WAIT();

         flatacolor[2] = BYTE_LUT(a0);
         flatacolor[1] = BYTE_LUT(a1);
         flatacolor[0] = BYTE_LUT(a2);

         flatdcolor[2] = BYTE_LUT(d0);
         flatdcolor[1] = BYTE_LUT(d1);
         flatdcolor[0] = BYTE_LUT(d2);
#endif

#ifdef SGI
         a0 = (int)acolor[0];
         a1 = (int)acolor[1];
         a2 = (int)acolor[2];
         d0 = (int)rcolor[0];
         d1 = (int)rcolor[1];
         d2 = (int)rcolor[2];

         flatacolor[3] = BYTE_LUT(a0);
         flatacolor[2] = BYTE_LUT(a1);
         flatacolor[1] = BYTE_LUT(a2);

         flatdcolor[3] = BYTE_LUT(d0);
         flatdcolor[2] = BYTE_LUT(d1);
         flatdcolor[1] = BYTE_LUT(d2);
#endif


#endif
#endif
#endif

#endif

#ifdef POLYAL
         texcolor = ob->shade[i];
#endif

#ifdef POLYGD
         texcolor = ob->shade[i];
#endif

#ifdef POLYPG
         texcolor = ob->shade[i];
#endif

         while (scany < winy) {

            while (et[i] && et[i]->starty == scany) {
               ptr = et[i];
               et[i] = et[i]->next;
               sortbucket(&aet, ptr);
            }

#ifndef POLYLT
            if (!INTERLACE || !(scany & 0x01)) {
#endif

#ifndef INTZ
               surface[1] = scanfy;
#endif

               for (ptr=aet; ptr!=(edgetype *)NULL; ptr=qtr->next) {

                  qtr   = ptr->next;
                  start = round(ptr->start.point[0]);
                  end   = round(qtr->start.point[0]);
#ifndef INTZ
                  surface[0] = start-1.0;
                  engine(ob->normal[i], surface, parm, &prenum, &prediv);
#endif

                  if (start != end)

#ifdef POLYLT
#include "polylt.c"
#endif
#ifdef POLYBW
#include "polybw.c"
#endif
#ifdef POLYCT
#include "polyct.c"
#endif
#ifdef POLYGD
#include "polygd.c"
#endif
#ifdef POLYAL
#include "polyal.c"
#endif
#ifdef POLYPG
#include "polypg.c"
#endif
#ifdef SHADCT
#include "shadct.c"
#endif

               }

#ifndef POLYLT
            }
#endif

            scanfy += 1.0;
            scany++;
#ifdef DOS
#ifdef POLYLT
            scanxy += winx;
#else
            scanxy -= winx;
#endif
#else
            scanxy += winx;
#endif

            if (scany >= winy)
               break;

            for (qtr = aet, aet = (edgetype *)NULL; qtr != (edgetype *)NULL;) {
               ptr = qtr;
               qtr = qtr->next;

               if (scany < round(ptr->end.point[1])) {
                  ptr->start.point[0] += ptr->dx;
#ifdef INTZ
                  ptr->start.point[2] += ptr->dz;
#endif

#ifdef POLY2DTEXTURE
                  addarray2(ptr->start.uv, ptr->dt);
#endif

#ifdef POLYPG
                  ptr->ds[1] += ptr->ds[3];
#endif
#ifdef POLYGD
                  addarray3(ptr->start.shadedata, ptr->ds);
#endif

                  sortbucket(&aet, ptr);
               }

               else
                  control.push(EDGETYPE, ptr);
            }

            if (aet == (edgetype *)NULL) {
               if (!et[i])
                  break;

               scany = et[i]->starty;
#ifdef DOS
#ifdef POLYLT
               scanxy = scany*winx;
#else
               scanxy = toprow - scany*winx;
#endif
#else
               scanxy = scany*winx;
#endif

               scanfy = (float)scany;
            }

         }

         while (aet != (edgetype *)NULL) {
            ptr=aet;
            aet=aet->next;
            control.push(EDGETYPE, ptr);
         }

         while (et[i] != (edgetype *)NULL) {
            ptr=et[i];
            et[i]=et[i]->next;
            control.push(EDGETYPE, ptr);
         }

      }


#ifdef POLYNTRANSBEAM
#undef POLYNTRANSBEAM
#endif

