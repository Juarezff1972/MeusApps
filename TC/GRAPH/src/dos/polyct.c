



/* *************************************************************
   This function scans poligonal objects into a zbuffer
************************************************************* */


#include "polygon.h"
#include "global.h"
#include "memman.h"


/* **************************************************
************************************************** */
void polygon::polyct(zbuffer *zbuff, int winx, int winy) {

   int i;

   for (i=0; i<ob->countobject; i++)
      if (et[i])
         polyct(i, zbuff);
}


/* **************************************************
************************************************** */
void polygon::polyct2(zbuffer *zbuff, int winx, int winy) {

   int i;
   unsigned int flagb = mcinfo & CIINVISO;

   for (i=0; i<ob->countobject; i++)
      if (et[i])
         if (tob->query_tflag(i))
            if (tob->data[i].ob->whatami == TEXTURE && !((texture *)tob->data[i].ob)->mipmap)        // no antialiasing
               if (flagb)
                  polyct2db(i, zbuff);
               else
                  polyct2d(i, zbuff);
            else
               polyct2(i, flagb, zbuff);
         else
            polyct(i, zbuff);
}


