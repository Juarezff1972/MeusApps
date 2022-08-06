




#include <stdlib.h>
#include "line.h"
#include "global.h"

/* *************************************************************
************************************************************* */
int line::parse(FILE *infile, char *token) {

   switch (token[0]) {

      case 'p':
         if (!strcmp(token, "point")) {
            get_token(infile, token);
            center[0] = atof(token);
            get_token(infile, token);
            center[1] = atof(token);
            get_token(infile, token);
            center[2] = atof(token);

            get_token(infile, token);
            wwi[0] = atof(token);
            get_token(infile, token);
            wwi[1] = atof(token);
            get_token(infile, token);
            wwi[2] = atof(token);

            return 1;
         }

         break;

      case 's':         // before base
         if (!strcmp(token, "shade")) {
            get_token(infile, token);
            lower_case(token);

            switch (token[0]) {

               case 'w':
                  if (!strcmp(token, "wfbw")) {
                     mctype = WFBW;
                     return 1;
                  }

                  return 0;

               case 'g':
                  if (!strcmp(token, "gouraud")) {
                     mctype = GOURAUD;

                     get_token(infile, token);
                     color[0][0] = atof(token)*255.0;
                     get_token(infile, token);
                     color[0][1] = atof(token)*255.0;
                     get_token(infile, token);
                     color[0][2] = atof(token)*255.0;

                     get_token(infile, token);
                     color[1][0] = atof(token)*255.0;
                     get_token(infile, token);
                     color[1][1] = atof(token)*255.0;
                     get_token(infile, token);
                     color[1][2] = atof(token)*255.0;

                     return 1;
                  }

                  return 0;

               default:
                  break;
            }

            return 0;
         }

         break;

      default:
         break;
   }

   return pc::parse(infile, token);
}


/* *************************************************************
   This function reads in the surface patch data
************************************************************* */
int line::read_data(FILE *infile) {

   fscanf(infile, "%f %f %f", &center[0], &center[1], &center[2]);

   if (mctype == GOURAUD)
      fscanf(infile, "%f %f %f", &color[0][0], &color[0][1], &color[0][2]);

   fscanf(infile, "%f %f %f", &wwi[0], &wwi[1], &wwi[2]);

   if (mctype == GOURAUD)
      fscanf(infile, "%f %f %f", &color[1][0], &color[1][1], &color[1][2]);

   return 0;
}


/* *************************************************************
************************************************************* */
int line::bound_box(eye *parm) {

   int i;

   copyarray3(bbox[0], wwai[0]);
   copyarray3(bbox[1], wwai[0]);

   for (i=0; i<3; i++)
      if (bbox[0][i] > wwai[1][i])
         bbox[0][i] = wwai[1][i];
      else if (bbox[1][i] < wwai[1][i])
         bbox[1][i] = wwai[1][i];

   return (boundflag = 1);
}


/* *************************************************************
************************************************************* */
int line::clip(eye *parm, int maxx, int maxy) {

   int      i, k;                            // loop variables
   float    d;

   if (bbox[1][2] < front &&                    // w/in boundaries
       parm->zscale(bbox[0][0], bbox[1][2]) > parm->vrc[0] &&      // outside boundaries
       parm->zscale(bbox[1][0], bbox[1][2]) < parm->vrc[1] &&
       parm->zscale(bbox[0][1], bbox[1][2]) > parm->vrc[2] &&
       parm->zscale(bbox[1][1], bbox[1][2]) < parm->vrc[3]) {

      parm->map2screen(wwai[0]);
      parm->map2screen(wwai[1]);

      return 1;
   }

   if (bbox[0][2] > front ||
       parm->zscale(bbox[0][0], bbox[0][2]) > parm->vrc[1] ||     // outside boundaries
       parm->zscale(bbox[1][0], bbox[0][2]) < parm->vrc[0] ||
       parm->zscale(bbox[0][1], bbox[0][2]) > parm->vrc[3] ||
       parm->zscale(bbox[1][1], bbox[0][2]) < parm->vrc[2])
      return 0;

   for (i=0; i<2; i++) {                // clip Z
      k = (!i);

      if (wwai[i][2] > front) {
         d = (-wwai[i][2]) / (wwai[k][2] - wwai[i][2]);

         wwai[i][0] += d * (wwai[k][0] - wwai[i][0]);
         wwai[i][1] += d * (wwai[k][1] - wwai[i][1]);
         wwai[i][2] = front;

         if (mctype == GOURAUD) {
            wcolor[k][0] += d * (wcolor[k][0] - wcolor[i][0]);
            wcolor[k][1] += d * (wcolor[k][1] - wcolor[i][1]);
            wcolor[k][2] += d * (wcolor[k][2] - wcolor[i][2]);
         }

         break;
      }

   }

   parm->map2screen(wwai[0]);
   parm->map2screen(wwai[1]);

   for (i=0; i<2; i++) {                // clip x
      k = (!i);

      if (wwai[i][0] < 0) {
         d = (-wwai[i][0]) / (wwai[k][0] - wwai[i][0]);

         wwai[i][0] = 0;
         wwai[i][1] += d * (wwai[k][1] - wwai[i][1]);
         wwai[i][2] += d * (wwai[k][2] - wwai[i][2]);

         if (mctype == GOURAUD) {
            wcolor[i][0] += d * (wcolor[k][0] - wcolor[i][0]);
            wcolor[i][1] += d * (wcolor[k][1] - wcolor[i][1]);
            wcolor[i][2] += d * (wcolor[k][2] - wcolor[i][2]);
         }

      }

      else if (wwai[i][0] > maxx) {
         d = (maxx-wwai[i][0]) / (wwai[k][0] - wwai[i][0]);

         wwai[i][0] = maxx;
         wwai[i][1] += d * (wwai[k][1] - wwai[i][1]);
         wwai[i][2] += d * (wwai[k][2] - wwai[i][2]);

         if (mctype == GOURAUD) {
            wcolor[i][0] += d * (wcolor[k][0] - wcolor[i][0]);
            wcolor[i][1] += d * (wcolor[k][1] - wcolor[i][1]);
            wcolor[i][2] += d * (wcolor[k][2] - wcolor[i][2]);
         }

      }

   }

   for (i=0; i<2; i++) {                // clip y
      k = (!i);

      if (wwai[i][1] < 0) {
         d = (-wwai[i][1]) / (wwai[k][1] - wwai[i][1]);

         wwai[i][0] += d * (wwai[k][0] - wwai[i][0]);
         wwai[i][1] = 0;
         wwai[i][2] += d * (wwai[k][2] - wwai[i][2]);

         if (mctype == GOURAUD) {
            wcolor[i][0] += d * (wcolor[k][0] - wcolor[i][0]);
            wcolor[i][1] += d * (wcolor[k][1] - wcolor[i][1]);
            wcolor[i][2] += d * (wcolor[k][2] - wcolor[i][2]);
         }

      }

      else if (wwai[i][1] > maxy) {

         d = (maxy-wwai[i][1]) / (wwai[k][1] - wwai[i][1]);

         wwai[i][0] += d * (wwai[k][0] - wwai[i][0]);
         wwai[i][1] = maxy;
         wwai[i][2] += d * (wwai[k][2] - wwai[i][2]);

         if (mctype == GOURAUD) {
            wcolor[i][0] += d * (wcolor[k][0] - wcolor[i][0]);
            wcolor[i][1] += d * (wcolor[k][1] - wcolor[i][1]);
            wcolor[i][2] += d * (wcolor[k][2] - wcolor[i][2]);
         }

      }

   }

   return 1;
}


/* *************************************************************
   This procedure puts the object in the edge table
************************************************************* */
void line::transform(eye *parm) {

   matvecmulto(parm->transform, wwai[0]);
   matvecmulto(parm->transform, wwai[1]);
}


/* *************************************************************
************************************************************* */
void line::datacopy() {

   copyarray3(wwai[0], center);
   wwai[0][3] = 1;

   copyarray3(wwai[1], wwi);
   wwai[1][3] = 1;

   if (mctype == GOURAUD) {
      copyarray3(wcolor[0], color[0]);
      copyarray3(wcolor[1], color[1]);
   }

}


/* **************************************************
************************************************** */
void line::render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff) {

   int    h, i, j, k, l;
   int    scany;
   float  delta[2][3];
   float  pt[3];
   float  m;
   vector3f c, dc, hc;
   int flagtr = CITRANSPARENT & mcinfo;
   int scanxy, index;
   int a0, a1, a2, d0, d1, d2;
   vector3f black = { 0, 0, 0 };
   int toprow = WINX*(WINY-1);

   j = (wwai[0][1] <= wwai[1][1]);
   i = (!j);                            // i is the smaller y

   h = (int)wwai[j][1];
   scany = (int)wwai[i][1];

   if (scany == h)
      h++;

   m = 1.0/(h-scany);

#ifdef DOS
   scanxy = toprow - scany * zbuff->maxx;
#else
   scanxy = scany * zbuff->maxx;
#endif

   delta[0][0] = (wwai[j][0] - wwai[i][0])*m;
   delta[0][1] = (wwai[j][2] - wwai[i][2])*m;

   switch (mctype) {

      case GOURAUD:

         dc[0] = (wcolor[j][0] - wcolor[i][0])*m;
         dc[1] = (wcolor[j][1] - wcolor[i][1])*m;
         dc[2] = (wcolor[j][2] - wcolor[i][2])*m;

         m = 1.0/abs(delta[0][0]);
         delta[1][2] = delta[0][1]*m;
         hc[0] = dc[0]*m;
         hc[1] = dc[1]*m;
         hc[2] = dc[2]*m;

         do {
            if (!INTERLACE || !(scany & 0x01)) {
               if (delta[0][0] > 0) {
                  pt[0] = wwai[i][0];
                  pt[1] = pt[0] + delta[0][0];
                  pt[2] = wwai[i][2];

                  copyarray3(c, wcolor[i]);
               }

               else {
                  pt[1] = wwai[i][0];
                  pt[0] = pt[1] + delta[0][0];
                  pt[2] = wwai[i][2] + delta[0][1];

                  c[0] = wcolor[i][0] + dc[0];
                  c[1] = wcolor[i][1] + dc[1];
                  c[2] = wcolor[i][2] + dc[2];
               }

               l = (int)pt[0];
               k = (int)pt[1];
               index = scanxy + l;

               do {
                  if (pt[2] > zbuff->zbuff[index].zdata)
                     if (flagtr) {
                        set_datatr(zbuff->zbuff, index, pt[2], c, black, a0, a1, a2, d0, d1, d2);
                     }

                     else {
                        set_data(zbuff->zbuff, index, pt[2], c, black, a0, a1, a2, d0, d1, d2);
                     }

                  pt[2] += delta[1][2];
                  addarray3(c, hc);
                  l++; index++;
               } while (l<k);

            }

            wwai[i][0] += delta[0][0];
            wwai[i][2] += delta[0][1];
            addarray3(wcolor[i], dc);

            scany++;

#ifdef DOS
            scanxy-=zbuff->maxx;
#else
            scanxy+=zbuff->maxx;
#endif

         }  while (scany < h);

         break;

      default:  //case WFBW:

         delta[1][2] = delta[0][1]/abs(delta[0][0]);

         do {

            if (!INTERLACE || !(scany & 0x01)) {
               if (delta[0][0] > 0) {
                  pt[0] = wwai[i][0];
                  pt[1] = pt[0] + delta[0][0];
                  pt[2] = wwai[i][2];
               }

               else {
                  pt[1] = wwai[i][0];
                  pt[0] = pt[1] + delta[0][0];
                  pt[2] = wwai[i][2] + delta[0][1];
               }

               l = (int)pt[0];
               k = (int)pt[1];
               index = l + scanxy;

               do {
                  if (pt[2] > zbuff->zbuff[index].zdata) {
                     zbuff->zbuff[index].zdata = pt[2];
                     *(unsigned int *)zbuff->zbuff[index].pixel   = 0xffffffff;
                     *(unsigned int *)zbuff->zbuff[index].ambient = 0xffffffff;
                  }

                  pt[2] += delta[1][2];
                  l++, index++;
               } while (l<k);

            }

            wwai[i][0] += delta[0][0];
            wwai[i][2] += delta[0][1];

            scany++;
#ifdef DOS
            scanxy-=zbuff->maxx;
#else
            scanxy+=zbuff->maxx;
#endif

         }  while (scany < h);

         break;
   }

}


/* **************************************************
************************************************** */
int line::scan(camera *cparm, light *lmain) {

   shadline *str;

   if (sflag && lmain && !NOSHADOW && !(mcinfo & CITRANSPARENT)) {
      str = new shadline;
      str->begin_scan();
      if (str->datacopy2(this, lmain))
         shadptr = str;
      else {
         str->end_scan();
         delete str;
      }

   }

   transform(cparm);         // transform objects

   if (!bound_box(cparm) || !clip(cparm, WINX, WINY))
      return 0;

   renderflag = 1;
   return 1;
}


/* **************************************************
************************************************** */
void line::prender() {

   gpline.insert(frame, round(wwai[0][0]), round(wwai[0][1]), round(wwai[1][0]), round(wwai[1][1]));
}


/* **************************************************
************************************************** */
int line::beamscan(spotlight *spot) {

   colortype nctype;
   unsigned int ncinfo;

   nctype = mctype;
   ncinfo = mcinfo;

   mctype = WFBW;
   ncinfo = CINULL;

   transform(spot);         // transform objects

   if (!bound_box(spot) || !clip(spot, spot->maxx, spot->maxy)) {
      mcinfo = ncinfo;
      mctype = nctype;

      return 0;
   }

   mcinfo = ncinfo;
   mctype = nctype;

   renderflag = 1;
   return 1;
}


/* **************************************************
************************************************** */
void line::beamrender(spotlight *spot) {

   int    h, i, j, k, l;
   int    scany;
   float  delta[2][6];
   float  pt[6];
   float  m;

   j = (wwai[0][1] <= wwai[1][1]);
   i = (!j);

   h = (int)wwai[j][1];
   scany = (int)wwai[i][1];

   if (scany == h)
      h++;

   m = 1.0/(h-scany);
   delta[0][0] = (wwai[j][0] - wwai[i][0])*m;
   delta[0][1] = (wwai[j][2] - wwai[i][2])*m;

   delta[1][2] = delta[0][1]/abs(delta[0][0]);

   do {

      if (delta[0][0] > 0) {
         pt[0] = wwai[i][0];
         pt[1] = pt[0] + delta[0][0];
         pt[2] = wwai[i][2];
      }

      else {
         pt[1] = wwai[i][0];
         pt[0] = pt[1] + delta[0][0];
         pt[2] = wwai[i][2] + delta[0][1];
      }

      l = (int)pt[0];
      k = (int)pt[1];

      do {
         if (pt[2] > spot->lbuff->lbuff.pdata[scany][l].zdata) {
            spot->lbuff->lbuff.pdata[scany][l].zdata  = pt[2];
            spot->lbuff->lbuff.pdata[scany][l].idbuff = id;
         }

         pt[2] += delta[1][2];
         l++;
      } while (l<k);

      wwai[i][0] += delta[0][0];
      wwai[i][2] += delta[0][1];
      scany++;
   }  while (scany < h);

}
