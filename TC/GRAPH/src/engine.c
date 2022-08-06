

#include <string.h>
#include "pixcon.h"
#include "memman.h"

void penciltest(camera *cparm, light *lparm, pc *object_list, pc *rad_list) {

   pc           *ptr, *qtr;
   light        *lctr;
   camera       *ctr;
   pc           *shadptr = (pc *)NULL;

   for (lctr=lparm; lctr!=(light *)NULL; lctr=(light *)lctr->next)
      lctr->xform(cparm);

                // SCAN
                                                // scan objects
   for (ptr = object_list; ptr!=(pc *)NULL; ptr = (pc *)ptr->next) {
      ptr->begin_scan();
      ptr->datacopy();
      ptr->scan(cparm, lparm);
   }
                                                // build shadow list
   for (ptr = object_list; ptr!=(pc *)NULL; ptr = (pc *)ptr->next)
      if (ptr->shadptr != (pc *)NULL) {
         for (qtr=ptr->shadptr; qtr->next != (pc *)NULL; qtr=(pc *)qtr->next);
         qtr->next = shadptr;
         shadptr = ptr->shadptr;
      }
                                                // scan shadow list
   for (ptr = shadptr; ptr!=(pc *)NULL; ptr=(pc *)ptr->next)
      ptr->scan(cparm, lparm);
                                                // scan transparent objects
   for (ptr = rad_list; ptr!=(pc *)NULL; ptr = (pc *)ptr->next) {
      ptr->begin_scan();
      ptr->datacopy();
      ptr->scan(cparm, lparm);
   }

                // RENDER
                                                // render object list
   for (; object_list; ptr=object_list, object_list=(pc *)object_list->next, delete ptr) {
      if (object_list->renderflag)
         object_list->prender();

      object_list->end_scan();
   }
                                                // render shadow list
   for (; shadptr != (pc *)NULL; ptr=shadptr, shadptr=(pc *)shadptr->next, delete ptr) {
      if (shadptr->renderflag)
         shadptr->prender();

      shadptr->end_scan();
   }
                                                // render transparent list
   for (; rad_list != (pc *)NULL; ptr=rad_list, rad_list=(pc *)rad_list->next, delete ptr) {
      if (rad_list->renderflag)
         rad_list->prender();

      rad_list->end_scan();
   }

   for (; cparm!=(camera *)NULL; ctr=cparm, cparm=(camera *)cparm->next, delete ctr);
   for (; lparm!=(light *)NULL; lctr=lparm, lparm=(light *)lparm->next, delete lctr);
}


void colorize(camera *cparm, light *lparm, light *bparm, pc *object_list, pc *rad_list, int frame, mapul *mapbuffer, zbuffer *zbuff) {

   light        *lctr;
   camera       *ctr;
   pc           *ptr, *qtr;
   pc           **btr;
   pc           *prescan;
   pc           *nobox;
   pc           *shadptr = (pc *)NULL;

                // BEAM
   for (lctr=bparm; lctr!=(light *)NULL; lctr=(light *)lctr->next) {

      ((spotlight *)lctr)->lbuff = (lightbufftype *)control.pop(LIGHTBUFF);
      ((spotlight *)lctr)->init_buff();

      nobox = prescan = (pc *)NULL;

      for (ptr=object_list; ptr != (pc *)NULL; ptr=(pc *)ptr->next) {
         ptr->begin_scan();
         ptr->datacopy();
         ptr->beamscan((spotlight *)lctr);
      }

      for (ptr=object_list; ptr != (pc *)NULL; ptr=(pc *)ptr->next) {
         if (ptr->renderflag)
            ptr->beamrender((spotlight *)lctr);

         ptr->end_scan();
         ptr->renderflag = ptr->boundflag = 0;
      }

      lctr->xform(cparm);
   }

   for (lctr=lparm; lctr!=(light *)NULL; lctr=(light *)lctr->next)
      lctr->xform(cparm);

                // SCAN
   for (ptr = object_list; ptr!=(pc *)NULL; ptr = (pc *)ptr->next) {
      ptr->begin_scan();
      ptr->datacopy();
      ptr->scan(cparm, lparm);
   }

   prescan = nobox = (pc *)NULL;

   while (object_list != (pc *)NULL) {
      ptr = object_list;
      object_list = (pc *)object_list->next;

      if (ptr->shadptr != (pc *)NULL) {
         for (qtr=ptr->shadptr; qtr->next != (pc *)NULL; qtr=(pc *)qtr->next);
         qtr->next = shadptr;
         shadptr = ptr->shadptr;
      }

      if (ptr->renderflag)
         if (!ptr->boundflag) {
            ptr->next = nobox;
            nobox = ptr;
         }

         else {
            for (btr = &prescan; *btr != (pc *)NULL && (*btr)->bbox[0][2] > ptr->bbox[0][2]; btr = (pc **)(&(*btr)->next));
            ptr->next = *btr;
            *btr = ptr;
         }

      else {
         ptr->end_scan();
         delete ptr;
      }

   }

   for (ptr = shadptr; ptr!=(pc *)NULL; ptr=(pc *)ptr->next)
      ptr->scan(cparm, lparm);

   for (ptr = rad_list; ptr!=(pc *)NULL; ptr = (pc *)ptr->next) {
      ptr->begin_scan();
      ptr->datacopy();
      ptr->scan(cparm, lparm);
   }

                // RENDER
   zbuff->initbuff(mapbuffer->data, mapbuffer->pdata, WINX, WINY);

/*
zbuff->data = NULL;
zbuff->pdata = NULL;
return;
*/

   if (prescan)
      do {

         prescan->render(cparm, lparm, bparm, zbuff);
         prescan->end_scan();

         ptr=prescan;
         prescan=(pc *)prescan->next;
         delete ptr;
      } while (prescan);

   for (; nobox != (pc *)NULL; ptr=nobox, nobox = (pc *)nobox->next, delete ptr) {
      nobox->render(cparm, lparm, bparm, zbuff);
      nobox->end_scan();
   }

   for (;shadptr != (pc *)NULL; ptr=shadptr, shadptr=(pc *)shadptr->next, delete ptr) {

      if (shadptr->renderflag)
         shadptr->render(cparm, (light *)NULL, (light *)NULL, zbuff);

      shadptr->end_scan();
   }

   for (;rad_list != (pc *)NULL; ptr=rad_list, rad_list=(pc *)rad_list->next, delete ptr) {

      if (rad_list->renderflag)
         rad_list->render(cparm, lparm, bparm, zbuff);

      rad_list->end_scan();
   }

#ifndef DOS
   zbuff->compress();
#endif

   if (ANTIALIAS)
      if (ANTIALIAS == 1)                           // anti aliasing
         zbuff->antialias(SANTIALIAS);
      else
         zbuff->antialias_edge(SANTIALIAS);

   if (GAMMA)
      zbuff->gammabuff(PGAMMA);

   if (RGBFILE)
      zbuff->save_rgb(SRGBFILE, frame);

   for (; cparm!=(camera *)NULL; ctr=cparm, cparm=(camera *)cparm->next, delete ctr);

   for (; lparm!=(light *)NULL;  lctr=lparm, lparm=(light *)lparm->next, delete lctr)
      if (lparm->query_class() == SPOT_LIGHT) {
         control.push(LIGHTBUFF, ((spotlight *)lparm)->lbuff);
         ((spotlight *)lparm)->lbuff = (lightbufftype *)NULL;
      }

   zbuff->pdata = NULL;
   zbuff->data = NULL;
}


void spawn(camera *cparm, light *lparm, light *bparm, pc *object_list, pc *rad_list, int frame, mapul *mapbuffer, zbuffer *zbuff) {

   if (PENCILTEST)
      penciltest(cparm, lparm, object_list, rad_list);
   else
      colorize(cparm, lparm, bparm, object_list, rad_list, frame, mapbuffer, zbuff);
}

