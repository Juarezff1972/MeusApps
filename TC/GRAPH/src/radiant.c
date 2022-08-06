
#include <stdlib.h>
#include <string.h>

#include "radiant.h"
#include "global.h"

#define RADPOLY 1
#define RADSCAN 2

/* *************************************************************
************************************************************* */
int radiant::parse(FILE *infile, char *token) {

   switch (token[0]) {

      case 'h':
         if (!strcmp(token, "hollow")) {
            flagh = 1;
            return 1;
         }

         break;

      case 'i':
         if (!strcmp(token, "inner_radius")) {
            get_token(infile, token);
            inner_circle = atof(token);

            return 1;
         }

         break;

      case 'o':
         if (!strcmp(token, "outer_radius")) {
            get_token(infile, token);
            radius = atof(token);

            return 1;
         }

         break;

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

      default:
         break;
   }

   return pc::parse(infile, token);
}


void radiant::preprocess(void *data) {

   pc::preprocess(data);

   wratio = 1.0 - inner_circle/radius;
   sflag = 0;
}


int radiant::read_data(FILE *infile) {

   char junk[MAXSTRLEN], junk1[MAXSTRLEN];

   fscanf(infile, "%s %f %f %f", junk, &center[0], &center[1], &center[2]);
   fscanf(infile, "%s %f %s %f", junk, &radius, junk1, &inner_circle);

   return 0;
}


float radiant::radiance(float pt[]) {

   float i, k;
   float vector[2], sv[2];

   vector[0] = pt[0]-wcenter[0];
   vector[1] = pt[1]-wcenter[1];

   sv[0] = SQR(vector[0]);
   sv[1] = SQR(vector[1]);

   k = sv[0] + sv[1];
   i = (sqrwradius-k)*scale;                            // point outside perimeter

   if (i < 0)
      return -1.0;

   pt[2] = wcenter[2] + sqrt(i);

   if (pt[2] > front)
      return -1.0;

   if (k <= winner_circle)
      return (!flagh);

   i = wratio*sqrt(k);          // calc intensity

   if (tradius < i)
      return -1.0;

   i = (1.0-i/tradius)*HALFPI;
   return SIN(i) + 0.1;
}


int radiant::bound_box(eye *parm) {

   bbox[0][0] = wcenter[0] - wradius;
   bbox[0][1] = wcenter[1] - wradius;
   bbox[0][2] = wcenter[2] - wradius;

   bbox[1][0] = wcenter[0] + wradius;
   bbox[1][1] = wcenter[1] + wradius;
   bbox[1][2] = wcenter[2] + wradius;

   return (boundflag = 1);
}


int radiant::clip(eye *parm, int maxx, int maxy) {

   if (bbox[0][2] > front ||
       parm->zscale(bbox[0][0], bbox[0][2]) > parm->vrc[1] ||     // outside boundaries
       parm->zscale(bbox[1][0], bbox[0][2]) < parm->vrc[0] ||
       parm->zscale(bbox[0][1], bbox[0][2]) > parm->vrc[3] ||
       parm->zscale(bbox[1][1], bbox[0][2]) < parm->vrc[2])
      return 0;

   parm->map2screen(wcenter);                       // map to screen

   scale = parm->query_scale()/(-wcenter[2]);

   winner_circle *= scale;
   wradius       *= scale;
   tradius        = wradius - winner_circle;
   sqrwradius     = SQR(wradius);
   winner_circle *= winner_circle;
   scale = 1.0/SQR(scale);

   row[0] = round(wcenter[1] - wradius);
   row[1] = round(wcenter[1] + wradius);
   col[0] = round(wcenter[0] - wradius);
   col[1] = round(wcenter[0] + wradius);

   if (col[0] < 0)
      col[0] = 0;

   if (col[1] > maxx-1)
      col[1] = maxx-1;

   if (row[0] < 0)
      row[0] = 0;

   if (row[1] > maxy-1)
      row[1] = maxy-1;

   return 1;
}


void radiant::transform(eye *parm) {

   matvecmulto(parm->transform, wcenter);
}


void radiant::datacopy() {

   copyarray3(wcenter, center);
   wcenter[3] = 1;
   wradius = radius;
   winner_circle = inner_circle;

   if (mctype > BW)
      base_color = &lob->base;
}


void radiant::render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff) {

   int   i, j;
   float intens;
   vector4f pt;
   texcolortype texcolor;
   vector4f normal, ycenter;
   vector3f ca, cd;
   vector3f black = {0,0,0};
   int scanxy, index;
   int a0, a1, a2, d0, d1, d2;
   int toprow = zbuff->maxx*(zbuff->maxy-1);

   if (renderflag == RADPOLY) {
      r.render(cparm, lmain, spot, zbuff);
      return;
   }

   pt[3] = 1;

#ifdef DOS
   scanxy = toprow - row[0]*zbuff->maxx;
#else
   scanxy = row[0]*zbuff->maxx;
#endif

   switch (mctype) {
      case IRRADIANT:

#ifdef DOS
         for (i=row[0], pt[1] = (float)i; i<=row[1]; i++, pt[1] += 1.0, scanxy-=zbuff->maxx) {
#else
         for (i=row[0], pt[1] = (float)i; i<=row[1]; i++, pt[1] += 1.0, scanxy+=zbuff->maxx) {
#endif
            if (INTERLACE && (i & 0x01))
               continue;

            for (j=col[0], index = scanxy + col[0], pt[0] = (float)j; j<=col[1]; j++, index++, pt[0] += 1.0) {
               intens = radiance(pt);

               if (intens > 0 && pt[2] > zbuff->zbuff[index].zdata) {
                  ca[0] = base_color->lum[0]*intens;
                  ca[1] = base_color->lum[1]*intens;
                  ca[2] = base_color->lum[2]*intens;

                  set_datatr(zbuff->zbuff, index, pt[2], ca, black, a0, a1, a2, d0, d1, d2);
               }

            }

         }

         return;

      case RSOLID:              // pseudo 3D - needs work

         pt[3] = 1;
         copyarray4(ycenter, wcenter);
         cparm->screen2map(ycenter);

         texcolor = *base_color;
         if (lmain)
            lmain->set_ambient(texcolor.color.ka, texcolor.ambient, texcolor.color.lum);
         else
            copyarray3(texcolor.ambient, texcolor.color.lum);

#ifdef DOS
         for (i=row[0], pt[1] = (float)i; i<=row[1]; i++, pt[1] += 1.0, scanxy-=zbuff->maxx) {
#else
         for (i=row[0], pt[1] = (float)i; i<=row[1]; i++, pt[1] += 1.0, scanxy+=zbuff->maxx) {
#endif
            if (INTERLACE && (i & 0x01))
               continue;

            for (j=col[0], index = col[0] + scanxy, pt[0] = (float)j; j<=col[1]; j++, index++, pt[0] += 1.0) {
               intens = radiance(pt);

               if (intens > 0 && pt[2]>zbuff->zbuff[index].zdata) {
                  copyarray3(black, pt);
                  cparm->screen2map(black);

                  normal[0] = black[0] - ycenter[0];
                  normal[1] = black[1] - ycenter[1];
                  normal[2] = black[2] - ycenter[2];
                  normalize3(normal);

                  cd[0] = cd[1] = cd[2] = 0;
                  copyarray3(ca, texcolor.ambient);

                  if (lmain)
                     lmain->intensity(black, normal, cd, cparm, &texcolor.color, this);

                  smultarray3(ca, intens);
                  smultarray3(cd, intens);
                  set_datatr(zbuff->zbuff, index, pt[2], ca, cd, a0, a1, a2, d0, d1, d2);
               }

            }

         }

         return;

      default:
         return;
   }

}


int radiant::scan(camera *cparm, light *lmain) {

   if (mctype == WFBW) {
      r.frame = frame;
      r.id = id;

      r.sflag = 0;

      copyarray3(r.center, center);
      r.size = 1;
      init_mx(r.rotate);
      r.mctype = WFBW;
      r.mcinfo = CINULL;
      r.wscale[0] = r.wscale[1] = r.wscale[2] = radius;

      r.scan(cparm, lmain);

      renderflag = RADPOLY;
      return 1;
   }

   transform(cparm);

   if (!bound_box((eye *)NULL) || !clip(cparm, WINX, WINY))
      return 0;

   renderflag = RADSCAN;
   return 1;
}


int radiant::beamscan(spotlight *spot) {

   return 0;
}


void radiant::beamrender(spotlight *spot) {

}


void radiant::prender() {

   r.prender();
}

