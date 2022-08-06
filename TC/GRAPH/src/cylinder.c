
#include <stdlib.h>
#include <string.h>
#include "cylinder.h"

extern int INTERLACE;

#define COUNTVERTEX   20
                // COUNTVERTEX/2
#define COUNTSIDE     10
                // countside + 2
#define COUNTOBJECT   12
                // 1.0/countside
#define ICOUNTSIDE    0.1
                // countside*4
#define COUNTSIDEEDGE 40
                // countsideedge + 2*countside
#define COUNTEDGE     60


/* *************************************************************
************************************************************* */
int cylinder::parse(FILE *infile, char *token) {

   switch (token[0]) {

      case 'l':
         if (!strcmp(token, "length")) {
            get_token(infile, token);
            length = atof(token);
            hlength = length * 0.5;

            return 1;
         }

         break;

      case 'r':
         if (!strcmp(token, "radius")) {
            get_token(infile, token);
            radius = atof(token);

            return 1;
         }

         break;

      default:
         break;
   }

   return make_poly::parse(infile, token);
}


/* *************************************************************
************************************************************* */
float cylinder::calc_area() {

   return cylinder_area(radius, length);
}


/* *************************************************************
************************************************************* */
void cylinder::pg(eye *parm, light *lmain, light *spot, zbuffer *zbuff) {

   int  i, j;
   float surface[4], pzero[4];
   float temp[2];
   vector4f normal;
   float pt[4];
   float t;
   int   side;
   int   flag;
   vector3f rcolor, acolor, tcolor;
   char  flag2t = mcinfo & CITEXTURE;
   char  flag3t = mcinfo & CI3DTEX;
   char  flagt  = flag2t || flag3t;
   char  flagtr = mcinfo & CITRANSPARENT;
   int   inflag;
   texcolortype texcolor;
   shaderparamtype sparam;
   int scanxy, index;
   int a0, a1, a2, d0, d1, d2;
   int toprow = zbuff->maxx*(zbuff->maxy-1);

   float piinverse = 1.0/PI;
   float ratio;
   float tx, ty;
   float di, dj, wi, wj, wt;

   float area;

   if (flag3t)
      sparam.setup(world, iworld, frame, parm, this, shaderlist->scale);
   else if (flag2t)
      area = sqrt((tob->maxx*tob->maxy)/(calc_area()*SQR(parm->query_scale())));

   ratio = 1.0/length;

   normal[3] = 0;

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

   di = dj =  1.0/parm->query_scale();

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

      for (j=col[0], index = scanxy + col[0], wj=wt, inflag = 0; j<=col[1]; j++, wj+=dj, index++) {
         surface[0] = wj;
         surface[1] = wi;
         surface[2] = -1;

         copyarray3(pt, surface);
         matvecmultv(iworld, pt);


         flag = line_cylinder_intersect(pzero, pt, hlength, radius, &t, &side);

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

               switch (side) {

                  case CYL_TOP:
                     normal[0] = normal[2] = 0;
                     normal[1] = 1;

                     break;

                  case CYL_BOTTOM:
                     normal[0] = normal[2] = 0;
                     normal[1] = -1;

                     break;

                  default:                      // CYL_SIDE
                     normal[0] = pzero[0] + t*pt[0];
                     normal[2] = pzero[2] + t*pt[2];
                     normal[1] = 0;
                     break;
               }

               matvecmultv(world, normal);
               normalize3(normal);

               if (flag2t) {
                  smultarray3(pt, t);
                  addarray3(pt, pzero);

                  switch(side) {

                     case CYL_TOP:
                        ty = 1;
                        break;

                     case CYL_BOTTOM:
                        ty = 0;
                        break;

                     default:
                        ty = (pt[1] + hlength)*ratio;
                        break;
                  }

                  normalize3(pt);

                  temp[0] = pt[0];
                  temp[1] = pt[2];
                  normalize2(temp);
                  tx = 0.5 * ((temp[0] >= 0.0) ? (ACOS(temp[1])*piinverse) : (2-ACOS(temp[1])*piinverse));

                  query_texel(tx, ty, tcolor, area*(-surface[2]));
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


void cylinder::lt(spotlight *spot) {

   int i, j;
   float surface[4], pzero[4];
   float t;
   int   side;
   int   flag;
   float wi, wj, wt, di, dj;
   int   inflag;

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
      pzero[3] = 1;
      matvecmulto(iworld, pzero);
      surface[3] = 0;
   }

   else {
      pzero[2] = -1;
      pzero[3] = 0;
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
            flag = line_cylinder_intersect(pzero, surface, hlength, radius, &t, &side);
         }

         else {
            surface[2] = 0;
            matvecmulto(iworld, surface);
            flag = line_cylinder_intersect(surface, pzero, hlength, radius, &t, &side);
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


void cylinder::transform(eye *parm) {

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

   length *= size;
   hlength *= size;
   radius *= size;

   inversemx(world, iworld);

   bbox[0][0] = -radius;
   bbox[0][1] = -hlength;
   bbox[0][2] = -radius;
   bbox[0][3] = 1;

   bbox[1][0] = radius;
   bbox[1][1] = hlength;
   bbox[1][2] = radius;
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
         else
            if (box[i][j] > bbox[1][j])
               bbox[1][j] = box[i][j];
   }

}


void cylinder::geo2poly(texpolygon **ptob, shaderlisttype *slist) {

   int i, j, k, l;
   float fi, fd;
   float x, z;
   float tx, td, tx2;
   vector2f uv[4];

   pob = new polytype;

   pob->build(COUNTVERTEX, COUNTOBJECT, COUNTEDGE, (mctype > BW && ptob != (texpolygon **)NULL), 0);
   pob->maxpolynum = COUNTSIDE;

   if (mcinfo & CITEXTURE && ptob != (texpolygon **)NULL) {
      *ptob = new texpolygon;
      (*ptob)->setup(COUNTOBJECT);
   }

   if (mcinfo & CI3DTEX && slist != (shaderlisttype *)NULL) {
      slist->scount = COUNTOBJECT;
      slist->shade = new shadertype[COUNTOBJECT];
   }

   for (i=0, j=COUNTSIDE, fi=-HALFPI, fd = TWOPI*ICOUNTSIDE; i < COUNTSIDE; i++, j++, fi += fd) {
      x = COS(fi);              // could precalc cos/sin in a table (only 10 entries each)
      z = -SIN(fi);

      pob->mvertex[j][0] = pob->mvertex[i][0] = x*radius;
      pob->mvertex[j][1] = -(pob->mvertex[i][1] = hlength);
      pob->mvertex[j][2] = pob->mvertex[i][2] = z*radius;
      pob->mvertex[j][3] = pob->mvertex[i][3] = 1;

      pob->gnormal[i][0] = x;
      pob->gnormal[i][1] = 0;
      pob->gnormal[i][2] = z;

      matvecmulto(world, pob->mvertex[i]);
      matvecmulto(world, pob->mvertex[j]);
      matvecmultv(world, pob->gnormal[i]);
      normalize3(pob->gnormal[i]);
      copyarray3(pob->gnormal[j], pob->gnormal[i]);
   }

   for (i=j=0, tx=0, tx2=td=texcoord[2][0]*ICOUNTSIDE, k=COUNTSIDE-1, l=COUNTSIDE; i<k; i++, l++, tx=tx2, tx2+=td) {
      pob->polynum[i] = 4;
      pob->edgeptr[i] = &pob->object[j];

      pob->object[j] = l+1;
      j++;
      pob->object[j] = l;
      j++;
      pob->object[j] = i;
      j++;
      pob->object[j] = i+1;
      j++;

      if (mcinfo & CITEXTURE && ptob != (texpolygon **)NULL) {
         uv[0][0] = tx2;
         uv[0][1] = 0;
         uv[1][0] = tx;
         uv[1][1] = 0;
         uv[2][0] = tx;
         uv[2][1] = texcoord[2][1];
         uv[3][0] = tx2;
         uv[3][1] = texcoord[2][1];
         (*ptob)->direct2Dmap(i, uv, tob, 4);
      }

   }

   pob->polynum[k] = 4;
   pob->edgeptr[k] = &pob->object[COUNTSIDEEDGE-4];
   l = COUNTSIDE-1;

   pob->object[COUNTSIDEEDGE-4] = COUNTSIDE;
   pob->object[COUNTSIDEEDGE-3] = COUNTVERTEX-1;
   pob->object[COUNTSIDEEDGE-2] = k;
   pob->object[COUNTSIDEEDGE-1] = 0;

   if (ptob != (texpolygon **)NULL && mcinfo & CITEXTURE) {
      uv[0][0] = tx2;
      uv[0][1] = 0;
      uv[1][0] = tx;
      uv[1][1] = 0;
      uv[2][0] = tx;
      uv[2][1] = texcoord[2][1];
      uv[3][0] = tx2;
      uv[3][1] = texcoord[2][1];
      (*ptob)->direct2Dmap(l, uv, tob, 4);
   }

   pob->polynum[COUNTSIDE] = COUNTSIDE;
   pob->edgeptr[COUNTSIDE] = &pob->object[COUNTSIDEEDGE];
   pob->polynum[COUNTSIDE+1] = COUNTSIDE;
   pob->edgeptr[COUNTSIDE+1] = &pob->object[COUNTSIDEEDGE+COUNTSIDE];

   for (i=COUNTSIDE-1, l=COUNTSIDEEDGE; i>-1; i--, l++)
      pob->object[l] = i;

   for (i=COUNTSIDE, j=COUNTSIDE<<1; i<j; i++, l++)
      pob->object[l] = i;

   if (ptob != (texpolygon **)NULL) {   // NULL signifies NO coloring
      if (mctype > BW)
         for (i=0; i<COUNTOBJECT; i++)
            pob->shade[i] = base_color;

      if (mcinfo & CI3DTEX) {
         for (i=0; i<COUNTOBJECT; i++)
            slist->shade[i] = shaderlist->shade[0];
         slist->scale = shaderlist->scale;
      }

   }

}

