
#ifdef DOS

#else
#include <GL/gl.h>
#endif

#include <string.h>
#include "linetype.h"


void linelist::dest() {

   line_type *ptr;

   if (count) {
      for (count--; count > -1; count--)
         for (; list[count] != (line_type *)NULL; ptr=list[count], list[count]=ptr->next, delete ptr);

      delete [] list;

      count = 0;
      list = (line_type **)NULL;
   }

}


void linelist::build(int i) {

   if (count)
      dest();

   list = new pline_type[i];
   count = i;

   memset(list, 0, i<<2);
}


#ifndef DOS

int linelist::bitblt(int i) {

   line_type *ptr;

   if (i >= count)
      return 0;

   glColor3f(1.0, 1.0, 1.0);

   glBegin(GL_LINES);

      for (ptr=list[i]; ptr!=(line_type *)NULL; ptr=ptr->next) {
         glVertex2i(ptr->pt[0] >> 16, ptr->pt[0] & 0xffff);
         glVertex2i(ptr->pt[1] >> 16, ptr->pt[1] & 0xffff);
      }

   glEnd();

   return 1;
}

#endif

int linelist::insert(int i, line_type *x) {

   if (i >= count)
      return 0;

   x->next = list[i];
   list[i] = x;

   return 1;
}


int linelist::insert(int i, int x1, int y1, int x2, int y2) {

   line_type *ptr;

   if (i >= count)
      return 0;

   ptr = new line_type;
   ptr->pt[0] = (x1<<16) | y1;
   ptr->pt[1] = (x2<<16) | y2;

   ptr->next = list[i];

   list[i] = ptr;

   return 1;
}
