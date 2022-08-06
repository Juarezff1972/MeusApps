#include <stdio.h>
#include "memman.h"


memman::memman() {
   edgetype_list = (edgetype *)NULL;
   lightbuff_list = (lightbufftype *)NULL;
}


memman::~memman() {

   edgetype *etr;
   lightbufftype *ltr;

   for (;edgetype_list != (edgetype *)NULL; etr=edgetype_list, edgetype_list=edgetype_list->next, delete etr);
   for (;lightbuff_list != (lightbufftype *)NULL; ltr = lightbuff_list, lightbuff_list=lightbuff_list->next, delete ltr);
}


void *memman::pop(memmantype item) {

   edgetype *etr;
   lightbufftype *ltr;

   switch (item) {

      case EDGETYPE:
         if (edgetype_list != (edgetype *)NULL) {
            etr = edgetype_list;
            edgetype_list = edgetype_list->next;
            return etr;
         }

         return new edgetype;

      default:          // LIGHTBUFF

         if (lightbuff_list != (lightbufftype *)NULL) {
            ltr = lightbuff_list;
            lightbuff_list = lightbuff_list->next;
            return ltr;
         }

         return new lightbufftype;
   }

}


void memman::push(memmantype item, void *mem) {

   edgetype *etr;
   lightbufftype *ltr;

   switch (item) {

      case EDGETYPE:
         etr = (edgetype *)mem;
         etr->next = edgetype_list;
         edgetype_list = etr;
         return;

      default:          // LIGHTBUFF
         ltr = (lightbufftype *)mem;
         ltr->next = lightbuff_list;
         lightbuff_list = ltr;
         return;
   }

}

memman control;
