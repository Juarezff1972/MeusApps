

#include "sbfast.h"
#include "global.h"

#define SBCOMMON        \
        vector4f pzero, pray;           \
        vector4f surface, normal;       \
        float increment;                \
        int i, j, k;                    \
                        \
        increment = (isize*(lparm->vrc[1]+lparm->vrc[1]))/(double)lparm->maxx;  \
                        \
        pray[3] = 0;    \
        surface[3] = 1; \
        pzero[3] = 1;   \
                        \
        if (lparm->query_whatami() == POINT) {  \
                pzero[0] = pzero[1] = pzero[2] = 0;     \
                matvecmulto(iworld, pzero);             \
        }               \
                        \
        else {          \
                pray[0] = pray[1] = 0;          \
                pray[2] = -1;                   \
                matvecmultv(iworld, pray);      \
        }


void sbfast::sbfastbm_llurh(spotlight *lparm, float dy) {

   int  h;
   float row;
                                           // calculate the ray-creep increment
   SBCOMMON;

   for (h=0; h<lparm->maxy; h++) {

      start_flag = 1;
      row = h;

      if (!SILENT)
         printf("y1 %d\n", h);

      for (j=0; j<lparm->maxx && (i=(int)row)<lparm->maxy; j++, row+=dy) {
#include "sblt.c"
      }

      for (; j<lparm->maxx && (i=(int)row)<lparm->maxy; j++, row+=dy)
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastbm_llurv(spotlight *lparm, float dx) {

   int  h;
   float col;
                                           // calculate the ray-creep increment
   SBCOMMON;

   for (h=0; h<lparm->maxx; h++) {

      start_flag = 1;
      col = h;

      if (!SILENT)
         printf("x %d\n", h);

      for (i=0; i<lparm->maxy && (j=(int)col)<lparm->maxx; i++, col+=dx) {
#include "sblt.c"
      }

      for (; i<lparm->maxy && (j=(int)col)<lparm->maxx; i++, col+=dx)
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastbm_lrulh(spotlight *lparm, float dy) {

   int  h;
   float row;
                                           // calculate the ray-creep increment
   SBCOMMON;

   for (h=1; h<lparm->maxy; h++) {

      start_flag = 1;
      row = h;

      if (!SILENT)
         printf("y %d\n", h);

      for (j=lparm->maxx-1; j>-1 && (i=(int)row)<lparm->maxy; j--, row -= dy) {  // dy is negative - go up
#include "sblt.c"
      }

      for (; j>-1 && (i=(int)row)<lparm->maxy; j--, row -= dy)    // dy is negative - go up
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastbm_lrulv(spotlight *lparm, float dx) {

   int  h;
   float col;
                                           // calculate the ray-creep increment
   SBCOMMON;

   for (h=0; h<lparm->maxx; h++) {

      start_flag = 1;
      col = h;

      if (!SILENT)
         printf("x %d\n", h);

      for (i=0; i<lparm->maxy && (j=(int)col)>-1; i++, col += dx) {  // dx is negative, go left
#include "sblt.c"
      }

      for (; i<lparm->maxy && (j=(int)col)>-1; i++, col += dx)    // dx is negative, go left
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastbm_negh(spotlight *lparm, float dy, float *up, float *horizon) {

   int  g, h;
   float intersect[2];
   float row;
   float column[3];
   float incup;
   float delta[2], curr[2];
   float yaxis[3];
   float xzero[3] = {1,0,0};
   float op[4];
                                           // calculate the ray-creep increment
   SBCOMMON;

   yaxis[0] = 0;
   yaxis[1] = 1;
   yaxis[2] = 1.0-lparm->maxy;

   copyarray2(column, up);
   incup = -up[1];

   h = 1;
   column[2] = -(up[0]*(WINX-1) + up[1]);

   line2d_line2d_intersect(column, horizon, curr);
   column[2] += incup;
   line2d_line2d_intersect(column, horizon, delta);
   subarray2(delta, curr);

   for (column[2] = -(up[0]*(WINX-1)+up[1]); h<lparm->maxy; h++, column[2]+=incup) {
      if (!SILENT)
         printf("y %d\n", h);

      start_flag = 1;

      copyarray2(intersect, curr);

      if (intersect[0] >= lparm->maxx-1) {
         intersect[0] = lparm->maxx-1;
         intersect[1] = h;
      }

      else {
         if (intersect[1] > lparm->maxy-1)
            line2d_line2d_intersect(column, yaxis, intersect);

         if (intersect[0] < 0)
            line2d_line2d_intersect(column, xzero, intersect);
      }

      g = j = (int)intersect[0];
      i = (int)(intersect[1] = h - (lparm->maxx-1-j)*dy);

      if (i > lparm->maxy-1) {
         j++;
         i = (int)(intersect[1] += dy);
      }

      do {
#include "sblt.c"
      } while (0);

      copyarray4(op, old_pos);

      for (j++, row=intersect[1]+dy; j<lparm->maxx && (i=(int)row)>-1; j++, row+=dy) {
#include "sblt.c"
      }

      for (; j<lparm->maxx && (i=(int)row)>-1; j++, row+=dy)
         sbbuffer->pdata[i][j] = 2;

      copyarray4(old_pos, op);

      for (j=g-1, row=intersect[1]-dy; j>-1 && (i=(int)row)<lparm->maxy; j--, row -= dy) {
#include "sblt.c"
      }

      for (; j>-1 && (i=(int)row)<lparm->maxy; j--, row -= dy)
         sbbuffer->pdata[i][j] = 2;

      addarray2(curr, delta);
   }

}


void sbfast::sbfastbm_negv(spotlight *lparm, float dx, float *up, float *horizon) {

   int  g, h;
   float intersect[2];
   float col;
   float column[3];
   float incup;
   float delta[2], curr[2];
   float yaxis[3];
   float xzero[3] = {1,0,0};
   float op[4];
                                           // calculate the ray-creep increment
   SBCOMMON;

   yaxis[0] = 0;
   yaxis[1] = 1;
   yaxis[2] = 1.0-lparm->maxy;

   copyarray2(column, up);
   incup = -up[0];

   h = 0;
   column[2] = 0;

   line2d_line2d_intersect(column, horizon, curr);
   column[2] += incup;
   line2d_line2d_intersect(column, horizon, delta);
   subarray2(delta, curr);

   for (column[2]=0; h<lparm->maxx; h++, column[2]+=incup) {
      if (!SILENT)
         printf("x %d\n", h);

      start_flag = 1;

      copyarray2(intersect, curr);

      if (intersect[1] <= 0) {
         intersect[0] = h;
         intersect[1] = 0;
      }

      else {
         if (intersect[1] > lparm->maxy-1)
            line2d_line2d_intersect(column, yaxis, intersect);

         if (intersect[0] < 0)
            line2d_line2d_intersect(column, xzero, intersect);
      }

      g = i = (int)intersect[1];
      j = (int)(intersect[0] = h + i*dx);

      if (j > lparm->maxx-1) {
         i++;
         j = (int)(intersect[0] += dx);
      }

      do {
#include "sblt.c"
      } while (0);

      copyarray4(op, old_pos);

      for (i++, col=intersect[0]+dx; i<lparm->maxy && (j=(int)col)>-1; i++, col+=dx) {
#include "sblt.c"
      }

      for (; i<lparm->maxy && (j=(int)col)>-1; i++, col+=dx)
         sbbuffer->pdata[i][j] = 2;

      copyarray4(old_pos, op);

      for (i=g-1, col=intersect[0]-dx; i>-1 && (j=(int)col)<lparm->maxx; i--, col-=dx) {
#include "sblt.c"
      }

      for (; i>-1 && (j=(int)col)<lparm->maxx; i--, col-=dx)
         sbbuffer->pdata[i][j] = 2;

      addarray2(curr, delta);
   }

}


void sbfast::sbfastbm_posh(spotlight *lparm, float dy, float *up, float *horizon) {

   int  g, h;
   float intersect[2];
   float row;
   float column[3];
   float incup;
   float delta[2], curr[2];
   float yaxis[3];
   float xaxis[3];
   float op[4];
                                           // calculate the ray-creep increment
   SBCOMMON;

   yaxis[0] = 0;
   yaxis[1] = 1;
   yaxis[2] = 1.0-lparm->maxy;

   xaxis[0] = 1;
   xaxis[1] = 0;
   xaxis[2] = 1.0-lparm->maxx;

   copyarray2(column, up);
   incup = -up[1];

   h = 0;

   column[2] = 0;

   line2d_line2d_intersect(column, horizon, curr);
   column[2] += incup;
   line2d_line2d_intersect(column, horizon, delta);
   subarray2(delta, curr);

   for (column[2] = 0; h<lparm->maxy; h++, column[2]+=incup) {
      if (!SILENT)
         printf("y %d\n", h);

      start_flag = 1;

      copyarray2(intersect, curr);

      if (intersect[0] <= 0) {
         intersect[0] = 0;
         intersect[1] = h;
      }

      else {
         if (intersect[1] > lparm->maxy-1)
            line2d_line2d_intersect(column, yaxis, intersect);

         if (intersect[0] > lparm->maxx-1)
            line2d_line2d_intersect(column, xaxis, intersect);
      }

      g = j = (int)intersect[0];
      i = (int)(intersect[1] = h+j*dy);

      do {
#include "sblt.c"
      } while (0);

      copyarray4(op, old_pos);

      for (j++, row=intersect[1]+dy; j<lparm->maxx && (i=(int)row)<lparm->maxy; j++, row+=dy) {
#include "sblt.c"
      }

      for (; j<lparm->maxx && (i=(int)row)<lparm->maxy; j++, row+=dy)
         sbbuffer->pdata[i][j] = 2;

      copyarray4(old_pos, op);

      for (j=g-1, row=intersect[1]-dy; j>-1 && (i=(int)row)>-1; j--, row -= dy) {
#include "sblt.c"
      }

      for (; j>-1 && (i=(int)row)>-1; j--, row -= dy)
         sbbuffer->pdata[i][j] = 2;

      addarray2(curr, delta);
   }

}


void sbfast::sbfastbm_posv(spotlight *lparm, float dx, float *up, float *horizon) {

   int  g, h;
   float intersect[2];
   float col;
   float column[3];
   float incup;
   float delta[2], curr[2];
   float yaxis[3];
   float xaxis[3];
   float op[4];
                                           // calculate the ray-creep increment
   SBCOMMON;

   yaxis[0] = 0;
   yaxis[1] = 1;
   yaxis[2] = 1.0-lparm->maxy;

   xaxis[0] = 1;
   xaxis[1] = 0;
   xaxis[2] = 1.0-lparm->maxx;

   copyarray2(column, up);
   incup = -up[0];

   h = 0;

   column[2] = 0;

   line2d_line2d_intersect(column, horizon, curr);
   column[2] += incup;
   line2d_line2d_intersect(column, horizon, delta);
   subarray2(delta, curr);

   for (column[2] = 0; h<lparm->maxx; h++, column[2]+=incup) {
      if (!SILENT)
         printf("x %d\n", h);

      start_flag = 1;

      copyarray2(intersect, curr);

      if (intersect[1] <=0) {
         intersect[0] = h;
         intersect[1] = 0;
      }

      else {
         if (intersect[1] > lparm->maxy-1)
            line2d_line2d_intersect(column, yaxis, intersect);

         if (intersect[0] > lparm->maxx-1)
            line2d_line2d_intersect(column, xaxis, intersect);
      }

      g = i = (int)intersect[1];
      j = (int)(intersect[0] = h + i*dx);

      do {
#include "sblt.c"
      } while (0);

      copyarray4(op, old_pos);

      for (i++, col=intersect[0]+dx; i<lparm->maxy && (j=(int)col)<lparm->maxx; i++, col+=dx) {
#include "sblt.c"
      }

      for (; i<lparm->maxy && (j=(int)col)<lparm->maxx; i++, col+=dx)
         sbbuffer->pdata[i][j] = 2;

      copyarray4(old_pos, op);

      for (i=g-1, col=intersect[0]-dx; i>-1 && (j=(int)col)>-1; i--, col -= dx) {
#include "sblt.c"
      }

      for (; i>-1 && (j=(int)col)>-1; i--, col -= dx)
         sbbuffer->pdata[i][j] = 2;

      addarray2(curr, delta);
   }

}


void sbfast::sbfastbm_horizontal(spotlight *lparm, int dflag) {

   int up, down, vinc;
                                           // calculate the ray-creep increment
   SBCOMMON;

   if (dflag) {
      up   = lparm->maxx;
      down = 0;
      vinc = 1;
   }

   else {
      up = -1;
      down = lparm->maxx-1;
      vinc = -1;
   }

   for (i=0; i<lparm->maxy; i++) {

      start_flag = 1;

      if (!SILENT)
         printf("y %d\n", i);
      for (j=down; j != up; j+=vinc)
#include "sblt.c"

      for (; j != up; j+=vinc)
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastbm_vertical(spotlight *lparm, int dflag) {

   int up, down, vinc;
                                           // calculate the ray-creep increment
   SBCOMMON;

   if (dflag) {
      up   = lparm->maxy;
      down = 0;
      vinc = 1;
   }

   else {
      up = -1;
      down = lparm->maxy-1;
      vinc = -1;
   }

   for (j=0; j<lparm->maxx; j++) {

      start_flag = 1;

      if (!SILENT)
         printf("x %d\n", j);
      for (i=down; i != up; i+=vinc)
#include "sblt.c"

      for (; i != up; i+=vinc)
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastbm_horizontald(spotlight *lparm, int origin) {

   float op[4];
                                           // calculate the ray-creep increment
   SBCOMMON;

   if (origin < 0)              // shouldnt do this, but in case of fp error...
      origin = 0;
   else
      if (origin >= lparm->maxx)
         origin = lparm->maxx-1;

   for (i=0; i<lparm->maxy; i++) {

      if (!SILENT)
         printf("y %d\n", i);

      start_flag = 1;

      j = origin;

      do {
#include "sblt.c"
      } while (0);

      copyarray4(op, old_pos);

      for (j--; j > -1; j--)
#include "sblt.c"

      for (; j > -1; j--)
         sbbuffer->pdata[i][j] = 2;

      copyarray4(old_pos, op);

      for (j=origin+1; j<lparm->maxx; j++)
#include "sblt.c"

      for (; j<lparm->maxx; j++)
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastbm_verticald(spotlight *lparm, int origin) {

   vector4f op;
                                           // calculate the ray-creep increment
   SBCOMMON;

   if (origin < 0)              // shouldnt do this, but in case of fp error...
      origin = 0;
   else
      if (origin >= lparm->maxy)
         origin = lparm->maxy-1;

   for (j=0; j<lparm->maxx; j++) {
      if (!SILENT)
         printf("x %d\n", j);

      start_flag = 1;

      i = origin;

      do {
#include "sblt.c"
      } while (0);

      copyarray4(op, old_pos);

      for (i--; i > -1; i--)
#include "sblt.c"

      for (; i > -1; i--)
         sbbuffer->pdata[i][j] = 2;

      copyarray4(old_pos, op);

      for (i=origin+1; i<lparm->maxy; i++)
#include "sblt.c"

      for (; i<lparm->maxy; i++)
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastbm_ullrh(spotlight *lparm, float dy) {

   int  h;
   float row;
                                           // calculate the ray-creep increment
   SBCOMMON;

   for (h=0; h<lparm->maxy; h++) {

      start_flag = 1;
      row = h;

      if (!SILENT)
         printf("y %d\n", h);

      for (j=0; j<lparm->maxx && (i=(int)row)>-1; j++, row += dy) {  // dy is negative - go down
#include "sblt.c"
      }

      for (; j<lparm->maxx && (i=(int)row)>-1; j++, row += dy)   // dy is negative - go down
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastbm_ullrv(spotlight *lparm, float dx) {

   int  h;
   float col;
                                           // calculate the ray-creep increment
   SBCOMMON;

   for (h=0; h<lparm->maxx; h++) {

      start_flag = 1;
      col = h;

      if (!SILENT)
         printf("x %d\n", h);

      for (i=lparm->maxy-1; i>-1 && (j=(int)col)<lparm->maxx; i--, col -= dx) {  // dx is negative, go right
#include "sblt.c"
      }

      for (; i>-1 && (j=(int)col)<lparm->maxx; i--, col -= dx)    // dx is negative, go right
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastbm_urllh(spotlight *lparm, float dy) {

   int  h;
   float row;
                                           // calculate the ray-creep increment
   SBCOMMON;

   for (h=0; h<lparm->maxy; h++) {

      start_flag = 1;
      row = h;

      if (!SILENT)
         printf("y %d\n", h);

      for (j=lparm->maxx-1; j>-1 && (i=(int)row)>-1; j--, row -= dy) {
#include "sblt.c"
      }

      for (; j>-1 && (i=(int)row)>-1; j--, row -= dy)
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastbm_urllv(spotlight *lparm, float dx) {

   int  h;
   float col;
                                           // calculate the ray-creep increment
   SBCOMMON;

   for (h=0; h < lparm->maxx; h++) {

      start_flag = 1;
      col = h;

      if (!SILENT)
         printf("x %d\n", h);

      for (i=lparm->maxy-1; i>-1 && (j=(int)col)>-1; i--, col -= dx) {
#include "sblt.c"
      }

      for (; i>-1 && (j=(int)col)>-1; i--, col -= dx)
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastbm_postscan(spotlight *lparm) {

                                           // calculate the ray-creep increment
   SBCOMMON;

   for (i=0; i < lparm->maxy; i++)
      for (j=0; j < lparm->maxx; j++)
         if (!sbbuffer->pdata[i][j]) {
            start_flag = 1;
#include "sblt.c"
         }

}

