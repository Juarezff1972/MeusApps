

#include "sbfast.h"
#include "global.h"

#define SBCOMMON                \
   shaderparamtype sparam;      \
   int  i, j, k;                \
   vector4f surface, normal;    \
   vector4f pzero, pray;        \
   vector3f rcolor, acolor, tcolor;     \
   int  flag3t = mcinfo & CI3DTEX;      \
   int  flag2t = mcinfo & CITEXTURE;    \
   int  flagt = flag3t | flag2t;        \
   int  flagtr = mcinfo & CITRANSPARENT;        \
   float increment;             \
   texcolortype texcolor;       \
   vector4f isw[4];             \
   int index;                   \
   int a0, a1, a2, d0, d1, d2;  \
   int toprow = WINX*(WINY-1);  \
   pzero[0] = pzero[1] = pzero[2] = 0;  \
   pzero[3] = 1;                \
   matvecmulto(iworld, pzero);  \
                                \
   normal[3] = pray[3] = 0;     \
   surface[3] = 1;              \
                                \
   if (flag3t)                  \
      sparam.setup(world, iworld, frame, parm, this, shaderlist->scale);        \
                                \
   copymx4x4(isw, world);       \
   smultarray3(isw[0], isize);  \
   smultarray3(isw[1], isize);  \
   smultarray3(isw[2], isize);  \
   increment = (parm->vrc[1]+parm->vrc[1])*isize / (double)WINX;        \
                                \
   texcolor = *base_color;      \
                                \
   if (lmain)                   \
      lmain->set_ambient(texcolor.color.ka, texcolor.ambient, texcolor.color.lum);      \
   else                         \
      copyarray3(texcolor.ambient, texcolor.color.lum); \




void sbfast::sbfastpg_llurh(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dy) {

   int  h;
   float row;
                                           // calculate the ray-creep increment
   SBCOMMON;

   for (h = 0; h < WINY; h++) {

      start_flag = 1;
      row = h;

      if (!SILENT)
         printf("y1 %d\n", h);

      for (j=0; j<WINX && (i=(int)row)<WINY; j++, row += dy) {
#include "sbshade.c"
      }

      for (; j<WINX && (i=(int)row)<WINY; j++, row += dy)
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastpg_llurv(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dx) {

   int  h;
   float col;
                                           // calculate the ray-creep increment
   SBCOMMON;

   for (h=0; h<WINX; h++) {

      start_flag = 1;
      col = h;

      if (!SILENT)
         printf("x2 %d\n", h);

      for (i=0; i<WINY && (j=(int)col)<WINX; i++, col += dx) {
#include "sbshade.c"
      }

      for (; i<WINY && (j=(int)col)<WINX; i++, col += dx)
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastpg_lrulh(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dy) {

   int  h;
   float row;
                                           // calculate the ray-creep increment
   SBCOMMON;

   for (h = 1; h<WINY; h++) {

      start_flag = 1;
      row = h;

      if (!SILENT)
         printf("y3 %d\n", h);

      for (j=WINX-1; j>-1 && (i=(int)row)<WINY; j--, row -= dy) {  // dy is negative - go up
#include "sbshade.c"
      }

      for (; j>-1 && (i=(int)row)<WINY; j--, row -= dy)    // dy is negative - go up
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastpg_lrulv(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dx) {

   int  h;
   float col;
                                           // calculate the ray-creep increment
   SBCOMMON;

   for (h=0; h<WINX; h++) {

      start_flag = 1;
      col = h;

      if (!SILENT)
         printf("x4 %d\n", h);

      for (i=0; i<WINY && (j=(int)col)>-1; i++, col += dx) {  // dx is negative, go left
#include "sbshade.c"
      }

      for (; i<WINY && (j=(int)col)>-1; i++, col += dx)    // dx is negative, go left
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastpg_negh(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dy, float *up, float *horizon) {

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
   yaxis[2] = 1.0-WINY;

   copyarray2(column, up);
   incup = -up[1];

   h = 1;

   column[2] = -(up[0]*(WINX-1) + up[1]);

   line2d_line2d_intersect(column, horizon, curr);
   column[2] += incup;
   line2d_line2d_intersect(column, horizon, delta);
   subarray2(delta, curr);

   for (column[2] = -(up[0]*(WINX-1)+up[1]); h<WINY; h++, column[2]+=incup) {

      if (!SILENT)
         printf("y5 %d \n", h);

      start_flag = 1;

      copyarray2(intersect, curr);

      if (intersect[0] >= WINX-1) {
         intersect[0] = WINX-1;
         intersect[1] = h;
      }

      else {
         if (intersect[1] > WINY-1)
            line2d_line2d_intersect(column, yaxis, intersect);

         if (intersect[0] < 0)
            line2d_line2d_intersect(column, xzero, intersect);
      }

      g = j = (int)intersect[0];
      i = (int)(intersect[1] = h - (WINX-1-j)*dy);

      if (i > WINY-1) {
         j++;
         i = (int)(intersect[1] += dy);
      }

      do {
#include "sbshade.c"
      } while (0);

      copyarray4(op, old_pos);

      for (j++, row=intersect[1]+dy; j<WINX && (i=(int)row)>-1; j++, row+=dy) {
#include "sbshade.c"
      }

      for (; j<WINX && (i=(int)row)>-1; j++, row+=dy)
         sbbuffer->pdata[i][j] = 2;

      copyarray4(old_pos, op);

      for (j=g-1, row=intersect[1]-dy; j>-1 && (i=(int)row)<WINY; j--, row -= dy) {
#include "sbshade.c"
      }

      for (; j>-1 && (i=(int)row)<WINY; j--, row -= dy)
         sbbuffer->pdata[i][j] = 2;

      addarray2(curr, delta);
   }

}


void sbfast::sbfastpg_negv(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dx, float *up, float *horizon) {

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
   yaxis[2] = 1.0-WINY;

   copyarray2(column, up);
   incup = -up[0];

   h = 0;
   column[2] = 0;

   line2d_line2d_intersect(column, horizon, curr);
   column[2] += incup;
   line2d_line2d_intersect(column, horizon, delta);
   subarray2(delta, curr);

   for (column[2]=0; h<WINX; h++, column[2]+=incup) {
      if (!SILENT)
         printf("x6 %d\n", h);

      start_flag = 1;

      copyarray2(intersect, curr);

      if (intersect[1] <= 0) {
         intersect[0] = h;
         intersect[1] = 0;
      }

      else {
         if (intersect[1] > WINY-1)
            line2d_line2d_intersect(column, yaxis, intersect);

         if (intersect[0] < 0)
            line2d_line2d_intersect(column, xzero, intersect);
      }

      g = i = (int)intersect[1];
      j = (int)(intersect[0] = h + i*dx);

      if (j > WINX-1) {
         i++;
         j = (int)(intersect[0] += dx);
      }

      do {
#include "sbshade.c"
      } while (0);

      copyarray4(op, old_pos);

      for (i++, col=intersect[0]+dx; i<WINY && (j=(int)col)>-1; i++, col+=dx) {
#include "sbshade.c"
      }

      for (; i<WINY && (j=(int)col)>-1; i++, col+=dx)
         sbbuffer->pdata[i][j] = 2;

      copyarray4(old_pos, op);

      for (i=g-1, col=intersect[0]-dx; i>-1 && (j=(int)col)<WINX; i--, col -= dx) {
#include "sbshade.c"
      }

      for (; i>-1 && (j=(int)col)<WINX; i--, col -= dx)
         sbbuffer->pdata[i][j] = 2;

      addarray2(curr, delta);
   }

}


void sbfast::sbfastpg_posh(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dy, float *up, float *horizon) {

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
   yaxis[2] = 1.0-WINY;

   xaxis[0] = 1;
   xaxis[1] = 0;
   xaxis[2] = 1.0-WINX;

   copyarray2(column, up);
   incup = -up[1];

   h = 1;

   column[2] = -up[1];

   line2d_line2d_intersect(column, horizon, curr);
   column[2] += incup;
   line2d_line2d_intersect(column, horizon, delta);
   subarray2(delta, curr);

   for (column[2] = -up[1]; h<WINY; h++, column[2]+=incup) {
      if (!SILENT)
         printf("y7 %d \n", h);

      start_flag = 1;

      copyarray2(intersect, curr);

      if (intersect[0] <= 0) {
         intersect[0] = 0;
         intersect[1] = h;
      }

      else {
         if (intersect[1] > WINY-1)
            line2d_line2d_intersect(column, yaxis, intersect);

         if (intersect[0] > WINX-1)
            line2d_line2d_intersect(column, xaxis, intersect);
      }

      g = j = (int)intersect[0];
      i = (int)(intersect[1] = h+j*dy);

      do {
#include "sbshade.c"
      } while (0);

      copyarray4(op, old_pos);

      for (j++, row=intersect[1]+dy; j<WINX && (i=(int)row)<WINY; j++, row+=dy) {
#include "sbshade.c"
      }

      for (; j<WINX && (i=(int)row)<WINY; j++, row+=dy)
         sbbuffer->pdata[i][j] = 2;

      copyarray4(old_pos, op);

      for (j=g-1, row=intersect[1]-dy; j>-1 && (i=(int)row)>-1; j--, row -= dy) {
#include "sbshade.c"
      }

      for (; j>-1 && (i=(int)row)>-1; j--, row -= dy)
         sbbuffer->pdata[i][j] = 2;

      addarray2(curr, delta);
   }

}


void sbfast::sbfastpg_posv(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dx, float *up, float *horizon) {

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
   yaxis[2] = 1.0-WINY;

   xaxis[0] = 1;
   xaxis[1] = 0;
   xaxis[2] = 1.0-WINX;

   copyarray2(column, up);
   incup = -up[0];

   h = 0;

   column[2] = 0;

   line2d_line2d_intersect(column, horizon, curr);
   column[2] += incup;
   line2d_line2d_intersect(column, horizon, delta);
   subarray2(delta, curr);

   for (column[2] = 0; h<WINX; h++, column[2]+=incup) {
      if (!SILENT)
         printf("x8 %d\n", h);

      start_flag = 1;

      copyarray2(intersect, curr);

      if (intersect[1] <= 0) {
         intersect[0] = h;
         intersect[1] = 0;
      }

      else {
         if (intersect[1] > WINY-1)
            line2d_line2d_intersect(column, yaxis, intersect);

         if (intersect[0] > WINX-1)
            line2d_line2d_intersect(column, xaxis, intersect);
      }

      g = i = (int)intersect[1];
      j = (int)(intersect[0] = h + i*dx);

      do {
#include "sbshade.c"
      } while (0);

      copyarray4(op, old_pos);

      for (i++, col=intersect[0]+dx; i<WINY && (j=(int)col)<WINX; i++, col+=dx) {
#include "sbshade.c"
      }

      for (; i<WINY && (j=(int)col)<WINX; i++, col+=dx)
         sbbuffer->pdata[i][j] = 2;

      copyarray4(old_pos, op);

      for (i=g-1, col=intersect[0]-dx; i>-1 && (j=(int)col)>-1; i--, col -= dx) {
#include "sbshade.c"
      }

      for (; i>-1 && (j=(int)col)>-1; i--, col -= dx)
         sbbuffer->pdata[i][j] = 2;

      addarray2(curr, delta);
   }

}


void sbfast::sbfastpg_horizontal(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int dflag) {

   int up, down, vinc;
                                           // calculate the ray-creep increment
   SBCOMMON;

   if (dflag) {
      up   = WINX;
      down = 0;
      vinc = 1;
   }

   else {
      up = -1;
      down = WINX-1;
      vinc = -1;
   }

   for (i=0; i<WINY; i++) {

      start_flag = 1;

      if (!SILENT)
         printf("y9 %d\n", i);

      for (j=down; j != up; j+=vinc) {
#include "sbshade.c"
      }

      for (; j != up; j+=vinc)
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastpg_vertical(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int dflag) {

   int up, down, vinc;
                                           // calculate the ray-creep increment
   SBCOMMON;

   if (dflag) {
      up   = WINY;
      down = 0;
      vinc = 1;
   }

   else {
      up = -1;
      down = WINY-1;
      vinc = -1;
   }

   for (j=0; j<WINX; j++) {

      start_flag = 1;

      if (!SILENT)
         printf("xa %d\n", j);

      for (i=down; i != up; i+=vinc) {
#include "sbshade.c"
      }

      for (; i != up; i+=vinc)
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastpg_horizontald(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int origin) {

   vector4f op;
                                           // calculate the ray-creep increment
   SBCOMMON;

   if (origin < 0)              // shouldnt do this, but in case of fp error...
      origin = 0;
   else
      if (origin >= WINX)
         origin = WINX-1;

   for (i=0; i<WINY; i++) {

      if (!SILENT)
         printf("yb %d\n", i);

      start_flag = 1;

      j = origin;

do {
#include "sbshade.c"
} while (0);

      copyarray4(op, old_pos);

      for (j--; j > -1; j--) {
#include "sbshade.c"
      }

      for (; j > -1; j--)
         sbbuffer->pdata[i][j] = 2;

      copyarray4(old_pos, op);

      for (j=origin+1; j<WINX; j++) {
#include "sbshade.c"
      }

      for (; j<WINX; j++)
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastpg_verticald(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int origin) {

   vector4f op;
                                           // calculate the ray-creep increment
   SBCOMMON;

   if (origin < 0)              // shouldnt do this, but in case of fp error...
      origin = 0;
   else if (origin >= WINY)
      origin = WINY-1;

   for (j=0; j<WINX; j++) {

      if (!SILENT)
         printf("xc %d\n", j);

      start_flag = 1;

      i = origin;

do {
#include "sbshade.c"
} while (0);

      copyarray4(op, old_pos);

      for (i--; i > -1; i--) {
#include "sbshade.c"
      }

      for (; i > -1; i--)
         sbbuffer->pdata[i][j] = 2;

      copyarray4(old_pos, op);

      for (i=origin+1; i<WINY; i++) {
#include "sbshade.c"
      }

      for (; i<WINY; i++)
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastpg_ullrh(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dy) {

   int  h;
   float row;
                                           // calculate the ray-creep increment
   SBCOMMON;

   for (h=0; h<WINY; h++) {

      start_flag = 1;
      row = h;

      if (!SILENT)
         printf("yd %d\n", h);

      for (j=0; j<WINX && (i=(int)row)>-1; j++, row += dy) {  // dy is negative - go down
#include "sbshade.c"
      }

      for (; j<WINX && (i=(int)row)>-1; j++, row += dy)    // dy is negative - go down
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastpg_ullrv(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dx) {

   int  h;
   float col;
                                           // calculate the ray-creep increment
   SBCOMMON;

   for (h=0; h<WINX; h++) {

      start_flag = 1;
      col = h;

      if (!SILENT)
         printf("xe %d\n", h);

      for (i=WINY-1; i>-1 && (j=(int)col)<WINX; i--, col -= dx) {  // dx is negative, go right
#include "sbshade.c"
      }

      for (; i>-1 && (j=(int)col)<WINX; i--, col -= dx)    // dx is negative, go right
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastpg_urllh(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dy) {

   int  h;
   float row;
                                           // calculate the ray-creep increment
   SBCOMMON;

   for (h=0; h<WINY; h++) {

      start_flag = 1;
      row = h;

      if (!SILENT)
         printf("yf %d\n", h);

      for (j=WINX-1; j>-1 && (i=(int)row)>-1; j--, row -= dy) {
#include "sbshade.c"
      }

      for (; j>-1 && (i=(int)row)>-1; j--, row -= dy)
         sbbuffer->pdata[i][j] = 2;
   }

}


void sbfast::sbfastpg_urllv(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dx) {

   int  h;
   float col;
                                           // calculate the ray-creep increment
   SBCOMMON;

   for (h=0; h < WINX; h++) {

      start_flag = 1;
      col = h;

      if (!SILENT)
         printf("xg %d\n", h);

      for (i=WINY-1; i>-1 && (j=(int)col)>-1; i--, col -= dx) {
#include "sbshade.c"
      }

      for (; i>-1 && (j=(int)col)>-1; i--, col -= dx)
         sbbuffer->pdata[i][j] = 2;
   }

}

/*
void sbfast::sbfastpg_postscan(zbuffer *zbuff) {

   int i, j, k, l, m, n;
   int count;
   float z;
   vector3f acolor, dcolor;
   int  flagtr = mcinfo & CITRANSPARENT;
   int o = (ditherflag ? 2 : 1);
   int index, subindex;
   int a0, a1, a2, d0, d1, d2;
   int toprow = WINX*(WINY-1);
   float icount;
   float icount_lut[10] = { 0, 1.0, 0.5, 0.3333, 0.25, 0.2, 1.0/6.0, 1.0/7.0,
                            0.125, 1.0/9.0 };

   for (i=0; i < WINY; i++)
      for ( j = (ditherflag ? (i & 0x01) : 0); j < WINX; j += o) {
         index = i*zbuff->maxx + j;

         if (!sbbuffer->data[index]) {

            acolor[0] = acolor[1] = acolor[2] = 0.0;
            dcolor[0] = dcolor[1] = dcolor[2] = 0.0;
            count = 0;
            z     = 0.0;

            if (i) {
               k = 0;
               m = i-1;
            }

            else {
               k = 1;
               m = i;
            }

            for (; k<3 && m<WINY; k++, m++) {
               l = (j ? 0 : 1);
               n = j-l;

               for (; l<3 && n<WINX; l++, n++) {
                  subindex = m*zbuff->maxx + n;

                  if ((k != 1 || l != 1) &&sbbuffer->data[subindex] == 1) {
                     count++;
#ifdef SGI
                     addarray3(acolor, (((char *)zbuff->zbuff[subindex].ambient)+1));
                     addarray3(dcolor, (((char *)zbuff->zbuff[subindex].pixel)+1));
#else
                     addarray3(acolor, zbuff->zbuff[subindex].ambient);
                     addarray3(dcolor, zbuff->zbuff[subindex].pixel);
#endif
                     z += zbuff->zbuff[subindex].zdata;
                  }

               }

            }

            if (count) {
               icount = icount_lut[count];
               z *= icount;
#ifdef SGI
               if (z > zbuff->zbuff[index].zdata) {
                  zbuff->zbuff[index].zdata = z;
                  acolor[0] *= icount;
                  a0 = (int)acolor[0];
                  acolor[1] *= icount;
                  a1 = (int)acolor[1];
                  acolor[2] *= icount;
                  a2 = (int)acolor[2];
                  dcolor[0] *= icount;
                  d0 = (int)dcolor[0];
                  dcolor[1] *= icount;
                  d1 = (int)dcolor[1];
                  dcolor[2] *= icount;
                  d2 = (int)dcolor[2];
                  WAIT();

                  if (flagtr) {
                     if (a0 > zbuff->zbuff[index].ambient[3])
                        zbuff->zbuff[index].ambient[3] = a0;
                     if (a1 > zbuff->zbuff[index].ambient[2])
                        zbuff->zbuff[index].ambient[2] = a1;
                     if (a2 > zbuff->zbuff[index].ambient[1])
                        zbuff->zbuff[index].ambient[1] = a2;
                     if (d0 > zbuff->zbuff[index].pixel[3])
                        zbuff->zbuff[index].pixel[3] = d0;
                     if (d1 > zbuff->zbuff[index].pixel[2])
                        zbuff->zbuff[index].pixel[2] = d1;
                     if (d2 > zbuff->zbuff[index].pixel[1])
                        zbuff->zbuff[index].pixel[1] = d2;
                  }

                  else {
                     zbuff->zbuff[index].ambient[3] = BYTE_LUT(a0);
                     zbuff->zbuff[index].ambient[2] = BYTE_LUT(a1);
                     zbuff->zbuff[index].ambient[1] = BYTE_LUT(a2);
                     zbuff->zbuff[index].pixel[3] = BYTE_LUT(d0);
                     zbuff->zbuff[index].pixel[2] = BYTE_LUT(d1);
                     zbuff->zbuff[index].pixel[1] = BYTE_LUT(d2);
                  }

               }
#endif

#ifdef LINUX
               if (z > zbuff->zbuff[index].zdata) {
                  zbuff->zbuff[index].zdata = z;
                  acolor[0] *= icount;
                  F2I(a0, acolor[0]);
                  acolor[1] *= icount;
                  F2I(a1, acolor[1]);
                  acolor[2] *= icount;
                  F2I(a2, acolor[2]);
                  dcolor[0] *= icount;
                  F2I(d0, dcolor[0]);
                  dcolor[1] *= icount;
                  F2I(d1, dcolor[1]);
                  dcolor[2] *= icount;
                  F2I(d2, dcolor[2]);
                  WAIT();

                  if (flagtr) {
                     if (a0 > zbuff->zbuff[index].ambient[0])
                        zbuff->zbuff[index].ambient[0] = a0;
                     if (a1 > zbuff->zbuff[index].ambient[1])
                        zbuff->zbuff[index].ambient[1] = a1;
                     if (a2 > zbuff->zbuff[index].ambient[2])
                        zbuff->zbuff[index].ambient[2] = a2;
                     if (d0 > zbuff->zbuff[index].pixel[0])
                        zbuff->zbuff[index].pixel[0] = d0;
                     if (d1 > zbuff->zbuff[index].pixel[1])
                        zbuff->zbuff[index].pixel[1] = d1;
                     if (d2 > zbuff->zbuff[index].pixel[2])
                        zbuff->zbuff[index].pixel[2] = d2;
                  }

                  else {
                     zbuff->zbuff[index].ambient[0] = BYTE_LUT(a0);
                     zbuff->zbuff[index].ambient[1] = BYTE_LUT(a1);
                     zbuff->zbuff[index].ambient[2] = BYTE_LUT(a2);
                     zbuff->zbuff[index].pixel[0] = BYTE_LUT(d0);
                     zbuff->zbuff[index].pixel[1] = BYTE_LUT(d1);
                     zbuff->zbuff[index].pixel[2] = BYTE_LUT(d2);
                  }

               }
#endif
#ifdef DOS

               if (z > zbuff->zbuff[index].zdata) {
                  zbuff->zbuff[index].zdata = z;
                  acolor[0] *= icount;
                  F2I(a0, acolor);
                  acolor[1] *= icount;
                  F2I(a1, acolor+4);
                  acolor[2] *= icount;
                  F2I(a2, acolor+8);
                  dcolor[0] *= icount;
                  F2I(d0, dcolor);
                  dcolor[1] *= icount;
                  F2I(d1, dcolor+4);
                  dcolor[2] *= icount;
                  F2I(d2, dcolor+8);
                  WAIT();

                  if (flagtr) {
                     if (a0 > zbuff->zbuff[index].ambient[2])
                        zbuff->zbuff[index].ambient[2] = a0;
                     if (a1 > zbuff->zbuff[index].ambient[1])
                        zbuff->zbuff[index].ambient[1] = a1;
                     if (a2 > zbuff->zbuff[index].ambient[0])
                        zbuff->zbuff[index].ambient[0] = a2;
                     if (d0 > zbuff->zbuff[index].pixel[2])
                        zbuff->zbuff[index].pixel[2] = d0;
                     if (d1 > zbuff->zbuff[index].pixel[1])
                        zbuff->zbuff[index].pixel[1] = d1;
                     if (d2 > zbuff->zbuff[index].pixel[0])
                        zbuff->zbuff[index].pixel[0] = d2;
                  }

                  else {
                     zbuff->zbuff[index].ambient[2] = BYTE_LUT(a0);
                     zbuff->zbuff[index].ambient[1] = BYTE_LUT(a1);
                     zbuff->zbuff[index].ambient[0] = BYTE_LUT(a2);
                     zbuff->zbuff[index].pixel[2] = BYTE_LUT(d0);
                     zbuff->zbuff[index].pixel[1] = BYTE_LUT(d1);
                     zbuff->zbuff[index].pixel[0] = BYTE_LUT(d2);
                  }

               }
#endif

            }

         }

      }

}
*/


void sbfast::dither(zbuffer *zbuff) {

   int i, j, k;
   int count;
   float icount;
   float z;
   vector3f acolor, dcolor;
   int  flagtr = mcinfo & CITRANSPARENT;
   int index;
   int pointer[4];
   int a0, a1, a2, d0, d1, d2;
   int toprow = WINX*(WINY-1);
   float icount_lut[5] = {  0, 1.0, 0.5, 0.3333, 0.25 };

   if (!ditherflag)
      return;

   for (i=1; i < WINY-1; i++)
      for (j=!(i & 0x01); j < WINX-1; j+=2) {
#ifdef DOS
         index = toprow - i*zbuff->maxx + j;
#else
         index = i*zbuff->maxx + j;
#endif
         if (sbbuffer->data[index] == 4) {

            pointer[0] = index-1;
            pointer[1] = pointer[0] + 2;
            pointer[2] = pointer[1] - (zbuff->maxx + 1);
            pointer[3] = pointer[2] + (zbuff->maxx<<1);

            acolor[0] = acolor[1] = acolor[2] = 0.0;
            dcolor[0] = dcolor[1] = dcolor[2] = 0.0;
            count = 0;
            z = 0.0;

            for (k=0; k<4; k++)
               if (sbbuffer->data[pointer[k]] == 1) {
                  count++;

#ifdef SGI
                     addarray3(acolor, (((unsigned char *)zbuff->zbuff[pointer[k]].ambient)+1));
                     addarray3(dcolor, (((unsigned char *)zbuff->zbuff[pointer[k]].pixel)+1));
#else
                     addarray3(acolor, zbuff->zbuff[pointer[k]].ambient);
                     addarray3(dcolor, zbuff->zbuff[pointer[k]].pixel);
#endif
                  z += zbuff->zbuff[pointer[k]].zdata;
               }


            if (count) {
               icount = icount_lut[count];
               z *= icount;

#ifdef SGI
               if (z > zbuff->zbuff[index].zdata) {
                  zbuff->zbuff[index].zdata = z;
                  acolor[0] *= icount;
                  a0 = (int)acolor[0];
                  acolor[1] *= icount;
                  a1 = (int)acolor[1];
                  acolor[2] *= icount;
                  a2 = (int)acolor[2];
                  dcolor[0] *= icount;
                  d0 = (int)dcolor[0];
                  dcolor[1] *= icount;
                  d1 = (int)dcolor[1];
                  dcolor[2] *= icount;
                  d2 = (int)dcolor[2];
                  WAIT();

                  if (flagtr) {
                     if (a0 > zbuff->zbuff[index].ambient[3])
                        zbuff->zbuff[index].ambient[3] = a0;
                     if (a1 > zbuff->zbuff[index].ambient[2])
                        zbuff->zbuff[index].ambient[2] = a1;
                     if (a2 > zbuff->zbuff[index].ambient[1])
                        zbuff->zbuff[index].ambient[1] = a2;
                     if (d0 > zbuff->zbuff[index].pixel[3])
                        zbuff->zbuff[index].pixel[3] = d0;
                     if (d1 > zbuff->zbuff[index].pixel[2])
                        zbuff->zbuff[index].pixel[2] = d1;
                     if (d2 > zbuff->zbuff[index].pixel[1])
                        zbuff->zbuff[index].pixel[1] = d2;
                  }

                  else {
                     zbuff->zbuff[index].ambient[3] = BYTE_LUT(a0);
                     zbuff->zbuff[index].ambient[2] = BYTE_LUT(a1);
                     zbuff->zbuff[index].ambient[1] = BYTE_LUT(a2);
                     ((unsigned char *)&zbuff->data[index])[3] = BYTE_LUT(d0);
                     ((unsigned char *)&zbuff->data[index])[2] = BYTE_LUT(d1);
                     ((unsigned char *)&zbuff->data[index])[1] = BYTE_LUT(d2);
                  }

               }
#endif

#ifdef LINUX
               if (z > zbuff->zbuff[index].zdata) {
                  zbuff->zbuff[index].zdata = z;
                  acolor[0] *= icount;
                  F2I(a0, acolor[0]);
                  acolor[1] *= icount;
                  F2I(a1, acolor[1]);
                  acolor[2] *= icount;
                  F2I(a2, acolor[2]);
                  dcolor[0] *= icount;
                  F2I(d0, dcolor[0]);
                  dcolor[1] *= icount;
                  F2I(d1, dcolor[1]);
                  dcolor[2] *= icount;
                  F2I(d2, dcolor[2]);
                  WAIT();

                  if (flagtr) {
                     if (a0 > zbuff->zbuff[index].ambient[0])
                        zbuff->zbuff[index].ambient[0] = a0;
                     if (a1 > zbuff->zbuff[index].ambient[1])
                        zbuff->zbuff[index].ambient[1] = a1;
                     if (a2 > zbuff->zbuff[index].ambient[2])
                        zbuff->zbuff[index].ambient[2] = a2;
                     if (d0 > zbuff->zbuff[index].pixel[0])
                        zbuff->zbuff[index].pixel[0] = d0;
                     if (d1 > zbuff->zbuff[index].pixel[1])
                        zbuff->zbuff[index].pixel[1] = d1;
                     if (d2 > zbuff->zbuff[index].pixel[2])
                        zbuff->zbuff[index].pixel[2] = d2;
                  }

                  else {
                     zbuff->zbuff[index].ambient[0] = BYTE_LUT(a0);
                     zbuff->zbuff[index].ambient[1] = BYTE_LUT(a1);
                     zbuff->zbuff[index].ambient[2] = BYTE_LUT(a2);
                     zbuff->zbuff[index].pixel[0] = BYTE_LUT(d0);
                     zbuff->zbuff[index].pixel[1] = BYTE_LUT(d1);
                     zbuff->zbuff[index].pixel[2] = BYTE_LUT(d2);
                  }

               }
#endif
#ifdef DOS
               if (z > zbuff->zbuff[index].zdata) {
                  zbuff->zbuff[index].zdata = z;
                  acolor[0] *= icount;
                  F2I(a0, acolor);
                  acolor[1] *= icount;
                  F2I(a1, acolor+4);
                  acolor[2] *= icount;
                  F2I(a2, acolor+8);
                  dcolor[0] *= icount;
                  F2I(d0, dcolor);
                  dcolor[1] *= icount;
                  F2I(d1, dcolor+4);
                  dcolor[2] *= icount;
                  F2I(d2, dcolor+8);
                  WAIT();

                  if (flagtr) {
                     if (a0 > zbuff->zbuff[index].ambient[2])
                        zbuff->zbuff[index].ambient[2] = a0;
                     if (a1 > zbuff->zbuff[index].ambient[1])
                        zbuff->zbuff[index].ambient[1] = a1;
                     if (a2 > zbuff->zbuff[index].ambient[0])
                        zbuff->zbuff[index].ambient[0] = a2;
                     if (d0 > zbuff->zbuff[index].pixel[2])
                        zbuff->zbuff[index].pixel[2] = d0;
                     if (d1 > zbuff->zbuff[index].pixel[1])
                        zbuff->zbuff[index].pixel[1] = d1;
                     if (d2 > zbuff->zbuff[index].pixel[0])
                        zbuff->zbuff[index].pixel[0] = d2;
                  }

                  else {
                     zbuff->zbuff[index].ambient[2] = BYTE_LUT(a0);
                     zbuff->zbuff[index].ambient[1] = BYTE_LUT(a1);
                     zbuff->zbuff[index].ambient[0] = BYTE_LUT(a2);
                     zbuff->zbuff[index].pixel[2] = BYTE_LUT(d0);
                     zbuff->zbuff[index].pixel[1] = BYTE_LUT(d1);
                     zbuff->zbuff[index].pixel[0] = BYTE_LUT(d2);
                  }

               }
#endif

            }

         }

      }

}
