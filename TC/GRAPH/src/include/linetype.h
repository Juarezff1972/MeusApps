

#if !defined __LINETYPE_H
#define __LINETYPE_H

#include <stdio.h>

typedef struct line_struct {

   unsigned int pt[2];
   struct line_struct *next;
} line_type;

typedef line_type *pline_type;


class linelist {

   public:
      pline_type *list;
      int count;

      void dest();
      void build(int i);

      linelist() {
         count = 0;
         list = (pline_type *)NULL;
      }

      ~linelist() { dest(); }

#ifndef DOS
      int bitblt(int i);
#endif
      int insert(int i, line_type *x);
      int insert(int i, int x1, int y1, int x2, int y2);
};

#endif
