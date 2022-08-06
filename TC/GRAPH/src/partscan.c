




/* *************************************************************
************************************************************* */

#include <stdlib.h>
#include "particle.h"
#include "global.h"

/* *************************************************************
************************************************************* */
int particle::parse(FILE *infile, char *token) {

   switch (token[0]) {

      case 'l':
         if (!strcmp(token, "location")) {
            get_token(infile, token);
            center[0] = atof(token);
            get_token(infile, token);
            center[1] = atof(token);
            get_token(infile, token);
            center[2] = atof(token);

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
                     color[0] = atof(token)*255;
                     get_token(infile, token);
                     color[1] = atof(token)*255;
                     get_token(infile, token);
                     color[2] = atof(token)*255;
                     return 1;
                  }

                  return 0;

               default:
                  return 0;
            }

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
int particle::read_data(FILE *infile) {

   fscanf(infile, "%f %f %f", &center[0], &center[1], &center[2]);

   if (mctype == WFBW)
      return 0;

   fscanf(infile, "%f %f %f", &color[0], &color[1], &color[2]);
   smultarray3(color, 255);

   return 0;
}


/* *************************************************************
************************************************************* */
int particle::bound_box(eye *parm) {

   copyarray3(bbox[0], wwai);
   copyarray3(bbox[1], wwai);

   return (boundflag = 1);
}


/* *************************************************************
   this is the basic particle clipper
************************************************************* */
int particle::clip(eye *parm, int maxx, int maxy) {

   if (wwai[2] >= front)
      return 0;

   parm->map2screen(wwai);

   if (wwai[0] >= 0 && wwai[0] < maxx && wwai[1] >= 0 && wwai[1] < maxy)
      return 1;

   return 0;
}


/* *************************************************************
   This procedure puts the object in the edge table
************************************************************* */
void particle::transform(eye *parm) {

   matvecmulto(parm->transform, wwai);
}


/* *************************************************************
************************************************************* */
void particle::datacopy() {

   copyarray3(wwai, center);
   wwai[3] = 1;

   copyarray3(wcolor, color);
}


/* **************************************************
************************************************** */
void particle::render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff) {

   vector3f black = {0,0,0};
   int index;
   int a0, a1, a2, d0, d1, d2;
   float *color = wcolor;

   index = round(wwai[1]);

   if (INTERLACE && (index & 0x01))
      return;

#ifdef DOS
   index = zbuff->maxx*(zbuff->maxy-1) - index*zbuff->maxx + round(wwai[0]);
#else
   index = index*zbuff->maxx + round(wwai[0]);
#endif

   if (wwai[2] > zbuff->zbuff[index].zdata)
      if (mctype == GOURAUD)
         if (mcinfo & CITRANSPARENT) {
            set_datatr(zbuff->zbuff, index, wwai[2], color, black, a0, a1, a2, d0, d1, d2);
         }

         else {
            set_data(zbuff->zbuff, index, wwai[2], color, black, a0, a1, a2, d0, d1, d2);
         }

      else {
         zbuff->zbuff[index].zdata = wwai[2];
         *(unsigned int *)zbuff->zbuff[index].pixel   = 0xffffffff;
         *(unsigned int *)zbuff->zbuff[index].ambient = 0xffffffff;
      }

}


/* **************************************************
************************************************** */
int particle::scan(camera *cparm, light *lmain) {

   shadowpt *str;

   if (sflag && lmain && !NOSHADOW && !(mcinfo & CITRANSPARENT)) {
      str = new shadowpt;
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
void particle::prender() {

   int oy = round(wwai[1]);
   int ox = round(wwai[0]);

   gpline.insert(frame, ox, oy, ox, oy);
}


/* **************************************************
************************************************** */
int particle::beamscan(spotlight *lparm) {

   transform(lparm);         // transform objects

   if (!bound_box(lparm) || !clip(lparm, lparm->maxx, lparm->maxy))
      return 0;

   renderflag = 1;
   return 1;
}


/* **************************************************
************************************************** */
void particle::beamrender(spotlight *lparm) {

   int   i, j;

   j = round(wwai[0]);
   i = round(wwai[1]);

   if (wwai[2] > lparm->lbuff->lbuff.pdata[i][j].zdata) {
      lparm->lbuff->lbuff.pdata[i][j].zdata = wwai[2];
      lparm->lbuff->lbuff.pdata[i][j].idbuff = id;
   }

}

