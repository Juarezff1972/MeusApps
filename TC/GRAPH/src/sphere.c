

#include <stdlib.h>
#include <string.h>
#include "sphere.h"

extern int INTERLACE;

#define U 10
#define DU 0.1
#define V 16
#define DV 0.0625
                                // (U+1-2) * V + 2
#define COUNTVERTEX   146
                                // U * V
#define COUNTOBJECT   160
                                // 4 * U * V
#define COUNTEDGE     640


/* *************************************************************
************************************************************* */
int sphere::parse(FILE *infile, char *token) {

   switch (token[0]) {

      case 'a':
         if (!strcmp(token, "axis")) {
            get_token(infile, token);
            wscale[0] = atof(token);
            get_token(infile, token);
            wscale[1] = atof(token);
            get_token(infile, token);
            wscale[2] = atof(token);

            return 1;
         }

         if (strlen(token) < 6)
            break;

         switch (token[5]) {

            case 'x':
               if (!strcmp(token, "axis_x")) {
                  get_token(infile, token);
                  wscale[0] = atof(token);

                  return 1;
               }

               break;

            case 'y':
               if (!strcmp(token, "axis_y")) {
                  get_token(infile, token);
                  wscale[1] = atof(token);

                  return 1;
               }

               break;

            case 'z':
               if (!strcmp(token, "axis_z")) {
                  get_token(infile, token);
                  wscale[2] = atof(token);

                  return 1;
               }

               break;

            default:
               break;
         }

         break;

      case 'r':
         if (!strcmp(token, "radius")) {
            get_token(infile, token);
            wscale[0] = wscale[1] = wscale[2] = atof(token);

            return 1;
         }

         break;

      default:
         break;
   }

   return make_poly::parse(infile, token);
}


float sphere::calc_area() {

   return sphere_area((wscale[0]+wscale[1]+wscale[2])*0.33333333);
}


void sphere::pg(eye *parm, light *lmain, light *spot, zbuffer *zbuff) {

   int  i, j;
   float surface[4], pzero[4];
   float temp[2];
   vector4f normal;
   float pt[4];
   float t;
   int   flag;
   vector3f rcolor, acolor, tcolor;
   int  flag2t = mcinfo & CITEXTURE;
   int  flag3t = mcinfo & CI3DTEX;
   int  flagt  = flag2t || flag3t;
   int  flagtr = mcinfo & CITRANSPARENT;
   int  inflag;
   int  index, scanxy;
   int a0, a1, a2, d0, d1, d2;
   int toprow = zbuff->maxx*(zbuff->maxy-1);

   texcolortype texcolor;
   shaderparamtype sparam;

   float piinverse = 1.0/PI;
   float tx, ty;
   float di, dj, wi, wj, wt;
   float area;

   vector3f iwscale;

   iwscale[0] = 1.0/wscale[0];
   iwscale[1] = 1.0/wscale[1];
   iwscale[2] = 1.0/wscale[2];

   normal[3] = 0;

   if (flag3t) {
      t = iwscale[0];

      if (iwscale[1] > t)
         t = iwscale[1];
      if (wscale[2] > t)
         t = iwscale[2];

      sparam.setup(world, iworld, frame, parm, this, shaderlist->scale*t);
   }

   else if (flag2t)
      area = sqrt((tob->maxx*tob->maxy)/(calc_area()*SQR(parm->query_scale())));

   multarray3(iwscale, iwscale);

   texcolor = *base_color;

   if (lmain)
      lmain->set_ambient(texcolor.color.ka, texcolor.ambient, texcolor.color.lum);
   else
      copyarray3(texcolor.ambient, texcolor.color.lum);

   pzero[0] = pzero[1] = pzero[2] = 0;
   pzero[3] = 1;

   matvecmulto(iworld, pzero);

   surface[0] = col[0];
   surface[1] = row[0];
   surface[2] = -1;
   surface[3] = 1;
   parm->screen2map(surface);

   di = dj = 1.0/parm->query_scale();

   wt = surface[0];
   wi = surface[1];

#ifdef DOS
   scanxy = toprow - row[0] * zbuff->maxx;
   for (i=row[0]; i<=row[1]; i++, wi+=di, scanxy -= zbuff->maxx) {
#else
   scanxy = row[0] * zbuff->maxx;
   for (i=row[0]; i<=row[1]; i++, wi+=di, scanxy += zbuff->maxx) {
#endif

      if (INTERLACE && (i & 0x01))
         continue;

      for (j=col[0], index = col[0] + scanxy, wj=wt, inflag = 0; j<=col[1]; j++, wj+=dj, index++) {

         surface[0] = wj;
         surface[1] = wi;
         surface[2] = -1;

         copyarray3(pt, surface);
         matvecmultv(iworld, pt);

         flag = line_ellipse_intersecti(iwscale, pzero, pt, &t);

         if (!flag) {
            if (inflag)
               break;
         }

         else {
            inflag = 1;

            if (-t>zbuff->zbuff[index].zdata) {

               surface[0] *= t;
               surface[1] *= t;
               surface[2] = -t;

               if (flag > 0) {
                  copyarray3(normal, surface);
                  subarray3(normal, wcenter);
               }

               else {
                  copyarray3(normal, wcenter);
                  subarray3(normal, surface);
               }

               matvecmultv(normalcalc, normal);
               normalize3(normal);

               if (flag2t) {
                  smultarray3(pt, t);
                  addarray3(pt, pzero);

                  normalize3(pt);

                  ty = ACOS(-pt[1])*piinverse;

                  temp[0] = pt[0];
                  temp[1] = pt[2];
                  normalize2(temp);

                  tx = 0.5 * ((temp[0] >= 0.0) ? (ACOS(temp[1])*piinverse) : (2-ACOS(temp[1])*piinverse));

                  query_texel(tx, ty, tcolor, area*(-surface[2]));

                  if (lmain)
                     lmain->set_ambient(texcolor.color.ka, texcolor.ambient, texcolor.color.lum);
               }

               if (!flagtr && spot) {
                  if (flag3t)
                     calc_color3b(&sparam, 0, surface, normal, lmain, spot, parm, &texcolor, rcolor, acolor);
                  else if (flag2t)
                     calc_color2b(surface, normal, lmain, spot, parm, &texcolor, tcolor, rcolor, acolor);
                  else
                     calc_colorb(surface, normal, lmain, spot, parm, &texcolor, rcolor, acolor);
               }

               else {
                  if (flag3t)
                     calc_color3(&sparam, 0, surface, normal, lmain, parm, &texcolor, rcolor, acolor);
                  else if (flag2t)
                     calc_color2(surface, normal, lmain, parm, &texcolor, tcolor, rcolor, acolor);
                  else
                     calc_color(surface, normal, lmain, parm, &texcolor, rcolor, acolor);
               }

               if (flagtr) {
                  set_datatr(zbuff->zbuff, index, surface[2], acolor, rcolor, a0, a1, a2, d0, d1, d2);
               }

               else {
                  set_data(zbuff->zbuff, index, surface[2], acolor, rcolor, a0, a1, a2, d0, d1, d2);
               }

            }

         }

      }

   }

}


void sphere::lt(spotlight *spot) {

   int i, j;
   float surface[4], pzero[4];
   float t;
   int   flag;
   float di, dj, wi, wj, wt;
   vector3f iwscale;
   int  inflag;

   iwscale[0] = 1.0/SQR(wscale[0]);
   iwscale[1] = 1.0/SQR(wscale[1]);
   iwscale[2] = 1.0/SQR(wscale[2]);

   surface[0] = col[0];
   surface[1] = row[0];
   surface[2] = -1;
   surface[3] = 1;
   copyarray3(pzero, surface);

   spot->screen2map(surface);

   pzero[0] += 1;
   pzero[1] += 1;
   pzero[3] = 0;
   spot->screen2map(pzero);

   dj = pzero[0] - surface[0];
   di = pzero[1] - surface[1];
   wt = surface[0];
   wi = surface[1];

   pzero[0] = pzero[1] = 0;

   if (spot->query_whatami() == POINT) {
      pzero[2] = 0;
      pzero[3]   = 1;
      matvecmulto(iworld, pzero);
      surface[3] = 0;
   }

   else {
      pzero[2] = -1;
      pzero[3]   = 0;
      matvecmultv(iworld, pzero);
      surface[3] = 1;
   }

   for (i=row[0]; i<=row[1]; i++, wi+=di)
      for (j=col[0], wj=wt, inflag = 0; j<=col[1]; j++, wj+=dj) {

         surface[0] = wj;
         surface[1] = wi;

         if (spot->query_whatami() == POINT) {
            surface[2] = -1;
            matvecmultv(iworld, surface);
            flag = line_ellipse_intersecti(iwscale, pzero, surface, &t);
         }

         else {
            surface[2] = 0;
            matvecmulto(iworld, surface);
            flag = line_ellipse_intersect(wscale, surface, pzero, &t);
         }

         if (!flag) {
            if (inflag)
               break;
         }

         else {
            inflag = 1;

            if (-t>spot->lbuff->lbuff.pdata[i][j].zdata) {
               spot->lbuff->lbuff.pdata[i][j].zdata = -t;
               spot->lbuff->lbuff.pdata[i][j].idbuff = id;
            }  

         }

      }

}


void sphere::transform(eye *parm) {

   int i, j;
   vector4f box[8];

   copymx4x4(world, rotate);

   world[0][3] += center[0];
   world[1][3] += center[1];
   world[2][3] += center[2];

   matmatmulto(parm->transform, world);

   wcenter[0] = world[0][3];
   wcenter[1] = world[1][3];
   wcenter[2] = world[2][3];
   wcenter[3] = 1;

   smultarray3(wscale, size);

   inversemx(world, iworld);

   copymx4x4(normalcalc, iworld);
   sdivarray4(normalcalc[0], wscale[0]);
   sdivarray4(normalcalc[1], wscale[1]);
   sdivarray4(normalcalc[2], wscale[2]);
   matmatmult(world, normalcalc);

   bbox[0][0] = -wscale[0];
   bbox[0][1] = -wscale[1];
   bbox[0][2] = -wscale[2];
   bbox[0][3] = 1;

   bbox[1][0] = wscale[0];
   bbox[1][1] = wscale[1];
   bbox[1][2] = wscale[2];
   bbox[1][3] = 1;

   make_box(bbox, box);

   matvecmulto(world, box[0]);
   copyarray3(bbox[0], box[0]);
   copyarray3(bbox[1], box[0]);

   for (i=1; i<8; i++) {
      matvecmulto(world, box[i]);

      for (j=0; j<3; j++)
         if (box[i][j] < bbox[0][j])
            bbox[0][j] = box[i][j];
         else if (box[i][j] > bbox[1][j])
               bbox[1][j] = box[i][j];
   }

}


void sphere::geo2poly(texpolygon **ptob, shaderlisttype *slist) {

   unsigned int i, j, k, l;
   float fi, fdi, fj, fdj;
   float r;
   float tx, ty;
   vector4f mx[4];
   vector2f uv[4];

   pob = new polytype;

   pob->build(COUNTVERTEX, COUNTOBJECT, COUNTEDGE, (mctype > BW && ptob != (texpolygon **)NULL), 0);
   pob->maxpolynum = 4;

   if (mcinfo & CITEXTURE && ptob != (texpolygon **)NULL) {
      *ptob = new texpolygon;
      (*ptob)->setup(COUNTOBJECT);
   }

   if (mcinfo & CI3DTEX && slist != (shaderlisttype *)NULL) {
      slist->scount = COUNTOBJECT;
      slist->shade = new shadertype[COUNTOBJECT];
   }

   init_mx(mx);
   mx[0][0] = wscale[0];
   mx[1][1] = wscale[1];
   mx[2][2] = wscale[2];

   matmatmulto(world, mx);
   copymx4x4(world, mx);

   inversemx(world, iworld);

   // optimization - do only 1/8 of below then calc the symetrical parts

   for (k=i=0, fdi = DU+DU, fi = -1 + fdi; i < U-1; i++, fi += fdi) {
      r = 1.0 - SQR(fi);

      r = (r > CORRECT) ? sqrt(r) : 0;

      for (j=0, fj=-HALFPI, fdj = TWOPI*DV; j < V; j++, fj += fdj, k++) {
         pob->gnormal[k][0] = pob->mvertex[k][0] = COS(fj)*r;
         pob->gnormal[k][1] = pob->mvertex[k][1] = fi;
         pob->gnormal[k][2] = pob->mvertex[k][2] = -SIN(fj)*r;
         pob->gnormal[k][3] = pob->mvertex[k][3] = 1;

         matvecmulto(world, pob->mvertex[k]);
         matvecmultv(world, pob->gnormal[k]);
         normalize3(pob->gnormal[k]);
      }

   }

   pob->gnormal[k][0] = pob->mvertex[k][0] = 0;
   pob->gnormal[k][1] = pob->mvertex[k][1] = 1;
   pob->gnormal[k][2] = pob->mvertex[k][2] = 0;
   pob->gnormal[k][3] = pob->mvertex[k][3] = 1;
   matvecmulto(world, pob->mvertex[k]);
   matvecmultv(world, pob->gnormal[k]);
   normalize3(pob->gnormal[k]);
   k++;
   pob->gnormal[k][0] = pob->mvertex[k][0] = 0;
   pob->gnormal[k][1] = pob->mvertex[k][1] = -1;
   pob->gnormal[k][2] = pob->mvertex[k][2] = 0;
   pob->gnormal[k][3] = pob->mvertex[k][3] = 1;
   matvecmulto(world, pob->mvertex[k]);
   matvecmultv(world, pob->gnormal[k]);
   normalize3(pob->gnormal[k]);

   for (i=k=l=0, ty=fdi=DU*texcoord[2][1]; i < U-2; i++, ty+=fdi, k++) {
      for (j=0, tx=0, fdj=DV*texcoord[2][0]; j < V-1; j++, k++, tx+=fdj) {

         pob->polynum[k] = 4;
         pob->edgeptr[k] = &pob->object[l];

         pob->object[l] = k;
         l++;
         pob->object[l] = k+V;
         l++;
         pob->object[l] = k+V+1;
         l++;
         pob->object[l] = k+1;
         l++;

         if (mcinfo & CITEXTURE && ptob != (texpolygon **)NULL) {
            uv[0][0] = tx;
            uv[0][1] = ty;
            uv[1][0] = tx;
            uv[1][1] = ty+fdi;
            uv[2][0] = tx+fdj;
            uv[2][1] = ty+fdi;
            uv[3][0] = tx+fdj;
            uv[3][1] = ty;
            (*ptob)->direct2Dmap(k, uv, tob, 4);
         }

      }

      pob->polynum[k] = 4;
      pob->edgeptr[k] = &pob->object[l];

      pob->object[l] = k;
      l++;
      pob->object[l] = k+V;
      l++;
      pob->object[l] = k+1;
      l++;
      pob->object[l] = k+1-V;
      l++;

      if (mcinfo & CITEXTURE && ptob != (texpolygon **)NULL) {
         uv[0][0] = tx;
         uv[0][1] = ty;
         uv[1][0] = tx;
         uv[1][1] = ty+fdi;
         uv[2][0] = texcoord[2][0];
         uv[2][1] = ty+fdi;
         uv[3][0] = texcoord[2][0];
         uv[3][1] = ty;
         (*ptob)->direct2Dmap(k, uv, tob, 4);
      }

   }
                                // top patches
   for (j=0, tx=0, fdj=DV*texcoord[2][0], ty=texcoord[2][1]-fdi; j < V-1; j++, k++, tx+=fdj) {

      pob->polynum[k] = 4;
      pob->edgeptr[k] = &pob->object[l];

      pob->object[l] = k+1;
      l++;
      pob->object[l] = k;
      l++;
      pob->object[l] = COUNTVERTEX-2;
      l++;
      pob->object[l] = COUNTVERTEX-2;
      l++;

      if (mcinfo & CITEXTURE && ptob != (texpolygon **)NULL) {
         uv[0][0] = tx+fdj;
         uv[0][1] = ty;
         uv[1][0] = tx;
         uv[1][1] = ty;
         uv[2][0] = tx;
         uv[2][1] = texcoord[2][1];
         uv[3][0] = tx+fdj;
         uv[3][1] = texcoord[2][1];
         (*ptob)->direct2Dmap(k, uv, tob, 4);
      }

   }

   pob->polynum[k] = 4;
   pob->edgeptr[k] = &pob->object[l];

   pob->object[l] = k+1-V;
   l++;
   pob->object[l] = k;
   l++;
   pob->object[l] = COUNTVERTEX-2;
   l++;
   pob->object[l] = COUNTVERTEX-2;
   l++;

   if (mcinfo & CITEXTURE && ptob != (texpolygon **)NULL) {
      uv[0][0] = texcoord[2][0];
      uv[0][1] = ty;
      uv[1][0] = tx;
      uv[1][1] = ty;
      uv[2][0] = tx;
      uv[2][1] = texcoord[2][1];
      uv[3][0] = texcoord[2][0];
      uv[3][1] = texcoord[2][1];
      (*ptob)->direct2Dmap(k, uv, tob, 4);
   }

   k++;
                                        // bottom
   for (j=0, tx=0, fdj=DV*texcoord[2][0], ty=texcoord[2][1]-fdi; j < V-1; j++, k++, tx+=fdj) {

      pob->polynum[k] = 4;
      pob->edgeptr[k] = &pob->object[l];

      pob->object[l] = j;
      l++;
      pob->object[l] = j+1;
      l++;
      pob->object[l] = COUNTVERTEX-1;
      l++;
      pob->object[l] = COUNTVERTEX-1;
      l++;

      if (mcinfo & CITEXTURE && ptob != (texpolygon **)NULL) {
         uv[0][0] = tx;
         uv[0][1] = fdi;
         uv[1][0] = tx+fdj;
         uv[1][1] = fdi;
         uv[2][0] = tx+fdj;
         uv[2][1] = 0.0;
         uv[3][0] = tx;
         uv[3][1] = 0.0;
         (*ptob)->direct2Dmap(k, uv, tob, 4);
      }

   }

   pob->polynum[k] = 4;
   pob->edgeptr[k] = &pob->object[l];

   pob->object[l] = j;
   l++;
   pob->object[l] = 0;
   l++;
   pob->object[l] = COUNTVERTEX-1;
   l++;
   pob->object[l] = COUNTVERTEX-1;
   l++;

   if (mcinfo & CITEXTURE && ptob != (texpolygon **)NULL) {
      uv[0][0] = tx;
      uv[0][1] = fdi;
      uv[1][0] = texcoord[2][0];
      uv[1][1] = fdi;
      uv[2][0] = texcoord[2][0];
      uv[2][1] = 0.0;
      uv[3][0] = tx;
      uv[3][1] = 0.0;
      (*ptob)->direct2Dmap(k, uv, tob, 4);
   }

   if (ptob != (texpolygon **)NULL) {   // NULL signifies NO coloring
      if (mctype > BW)
         for (i=0; i<COUNTOBJECT; i++)
            pob->shade[i] = base_color;

      if (mcinfo & CI3DTEX) {
         slist->scale = shaderlist->scale;
         for (i=0; i<COUNTOBJECT; i++)
            slist->shade[i] = shaderlist->shade[0];
      }

   }

}

