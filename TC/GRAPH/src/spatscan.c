



/* *************************************************************
   This file scans surface patch data into the buffer
************************************************************* */
#include <stdlib.h>
#include <string.h>
#include "spatch.h"
#include "global.h"

/* *************************************************************
************************************************************* */
int spatch::bound_box(eye *parm) {

   int    i, j;
   float  extreme[2][2];
   float  d[2];
   vector4f box[8];

   copyarray3(bbox[0], ob->data[0]);
   copyarray3(bbox[1], ob->data[0]);

   for (i=0; i<ob->vsize; i++)
      for (j=0; j<3; j++)
         if (bbox[0][j] > ob->data[i][j])
            bbox[0][j] = ob->data[i][j];
         else if (bbox[1][j] < ob->data[i][j])
            bbox[1][j] = ob->data[i][j];

   if (bbox[0][2] > front)
      return 0;

   if (bbox[1][2] > front)
      return (boundflag = 1);

   make_box(bbox, box);

   for (i=0; i<8; i++)
      parm->map2screen(box[i]);

   copyarray2(extreme[0], box[0]);      // bounding box
   copyarray2(extreme[1], box[0]);

   for (i=1; i<8; i++)
      for (j=0; j<2; j++)
         if (extreme[0][j] > box[i][j])
            extreme[0][j] = box[i][j];
         else if (extreme[1][j] < box[i][j])
            extreme[1][j] = box[i][j];

   d[0] = extreme[1][0] - extreme[0][0];
   d[1] = extreme[1][1] - extreme[0][1];

   if (abs(d[0]) < MINBBOX  || abs(d[1]) < MINBBOX)
      return 0;

   if (abs(d[0]) < MINDIM  || abs(d[1]) < MINDIM) {
      switch (mctype) {
         case DOT:
         case PDOT:
            mctype = DOT;

         case WFBW:
         case PWFBW:
            mctype = WFBW;
            break;

         case BW:
         case PBW:
            mctype = BW;
            break;

         default:
            mctype = CONSTANT;
      }

      mcinfo &= CIRAYCAST;

      setup_transfer(4,1);
      pob->make_flat(extreme, bbox[1][2], parm);

      if (mctype == CONSTANT)
         for (i=0; i<pob->countobject; i++)
            pob->shade[i] = base_color;

      return (boundflag = 2);
   }

   return (boundflag = 1);
}


/* *************************************************************
************************************************************* */
int spatch::clip(eye *parm, int maxx, int maxy) {

   if (bbox[0][2] > front ||
        parm->zscale(bbox[0][0], bbox[0][2]) > parm->vrc[1] ||     // outside boundaries
        parm->zscale(bbox[1][0], bbox[0][2]) < parm->vrc[0] ||
        parm->zscale(bbox[0][1], bbox[0][2]) > parm->vrc[3] ||
        parm->zscale(bbox[1][1], bbox[0][2]) < parm->vrc[2])
      return 0;

   return 1;
}


/* *************************************************************
   This function converts surface patch data into polygonal data
************************************************************* */
void spatch::spline2poly(spatchtype *sob, texpolygon *stob, int xflag) {

   int    i, j, k, l, m;
   int    base;
   int    ptr[4];
   float  d[3];
   float  temp;
   int    rflag;                        // variable to keep track of splitting of patch
   pvector4f vtr[4];
   int    patch;
   vector2f uv[4];

   pob->countobject = pob->countvertex = pob->countedge = 0;

   base = l = 0;

   for (i=1; i<=sob->srow; i++)
      for (j=1, k=0; j<=sob->scol; k=j, j++, l++) {
         copyarray4(pob->mvertex[l], sob->mvertex[i][j]);

         if (mctype > PWFBW || (mctype > WFBW && mctype <= PHONG)) {
            if (mctype > PWFBW) {
               copyarray3(pob->gnormal[l], sob->gnormal[i][j]);
            }

            else {
               vtr[0] = &ob->mvertex[i-1][k];
               vtr[1] = &ob->mvertex[i][k];
               vtr[2] = &ob->mvertex[i+1][k];
               vtr[3] = &ob->mvertex[i+2][k];

               spnormal_base(vtr, (float *)pob->gnormal[l], M_CR);
            }

         }

      }

   pob->countvertex = l;

   for (i=patch=0, m=base; i<sob->prow; i++, m++)
      for (j=0; j<sob->pcol; j++, m++, patch++) {
         ptr[0] = m;
         ptr[1] = ptr[0] + 1;
         ptr[3] = ptr[0] + sob->scol;
         ptr[2] = ptr[3] + 1;

         k = 0;
         for (l=0; l<4; l++)
            if (!k || !similar3(pob->mvertex[pob->object[pob->countedge+k-1]], pob->mvertex[ptr[l]])) {
               pob->object[pob->countedge+k] = ptr[l];
               k++;
            }

         if (k == 3) {
            if (mctype > PBW || (mctype > BW && mctype <= PHONG))
               pob->shade[pob->countobject] = sob->mshade[i][j];

            pob->edgeptr[pob->countobject] = &pob->object[pob->countedge];

            if (xflag && stob->query_tflag(patch)) {

               pob->polynum[pob->countobject] = 4;

               pob->object[pob->countedge] = ptr[0];
               pob->countedge++;
               pob->object[pob->countedge] = ptr[1];
               pob->countedge++;
               pob->object[pob->countedge] = ptr[2];
               pob->countedge++;
               pob->object[pob->countedge] = ptr[3];
               pob->countedge++;

               uv[3][0] = uv[0][0] = stob->data[patch].uv[0][0];
               uv[1][1] = uv[0][1] = stob->data[patch].uv[0][1];
               uv[2][0] = uv[1][0] = stob->data[patch].uv[1][0];
               uv[3][1] = uv[2][1] = stob->data[patch].uv[1][1];

               tpob->direct2Dmap(pob->countobject, uv, stob->data[patch].ob, 4);
            }

            else {
               pob->countedge += 3;
               pob->polynum[pob->countobject] = 3;
            }

            pob->countobject++;
         }

         else
            if (k == 4) {
               d[0] = pob->mvertex[ptr[0]][0] - pob->mvertex[ptr[2]][0];
               d[1] = pob->mvertex[ptr[0]][1] - pob->mvertex[ptr[2]][1];
               d[2] = pob->mvertex[ptr[0]][2] - pob->mvertex[ptr[2]][2];

               temp = magnitude3(d);

               d[0] = pob->mvertex[ptr[1]][0] - pob->mvertex[ptr[3]][0];
               d[1] = pob->mvertex[ptr[1]][1] - pob->mvertex[ptr[3]][1];
               d[2] = pob->mvertex[ptr[1]][2] - pob->mvertex[ptr[3]][2];

               if ((rflag = (magnitude3(d) < temp))) {
                  k = ptr[0];
                  ptr[0] = ptr[1];
                  ptr[1] = ptr[2];
                  ptr[2] = ptr[3];
                  ptr[3] = k;
               }

               pob->edgeptr[pob->countobject] = &pob->object[pob->countedge];

               if (mctype > PBW || (mctype > BW && mctype <= PHONG))
                  pob->shade[pob->countobject] = pob->shade[pob->countobject+1] = sob->mshade[i][j];

               pob->object[pob->countedge] = ptr[0];
               pob->countedge++;
               pob->object[pob->countedge] = ptr[1];
               pob->countedge++;
               pob->object[pob->countedge] = ptr[2];
               pob->countedge++;

               if (xflag && stob->query_tflag(patch)) {

                  if (rflag) {
                     uv[2][0] = stob->data[patch].uv[0][0];
                     uv[0][1] = stob->data[patch].uv[0][1];
                     uv[1][0] = uv[0][0] = stob->data[patch].uv[1][0];
                     uv[2][1] = uv[1][1] = stob->data[patch].uv[1][1];
                  }

                  else {
                     uv[0][0] = stob->data[patch].uv[0][0];
                     uv[1][1] = uv[0][1] = stob->data[patch].uv[0][1];
                     uv[2][0] = uv[1][0] = stob->data[patch].uv[1][0];
                     uv[2][1] = stob->data[patch].uv[1][1];
                  }

                  tpob->direct2Dmap(pob->countobject, uv, stob->data[patch].ob, 3);
               }

               pob->polynum[pob->countobject] = 3;
               pob->countobject++;

               pob->edgeptr[pob->countobject] = &pob->object[pob->countedge];

               pob->object[pob->countedge] = ptr[2];
               pob->countedge++;
               pob->object[pob->countedge] = ptr[3];
               pob->countedge++;
               pob->object[pob->countedge] = ptr[0];
               pob->countedge++;

               if (xflag && stob->query_tflag(patch)) {
                  tpob->data[pob->countobject].setup(3);
                  tpob->data[pob->countobject].ob = stob->data[patch].ob;

                  if (rflag) {
                     uv[1][0] = uv[0][0] = stob->data[patch].uv[0][0];
                     uv[2][1] = uv[1][1] = stob->data[patch].uv[0][1];
                     uv[0][1] = stob->data[patch].uv[1][1];
                     uv[2][0] = stob->data[patch].uv[1][0];
                  }

                  else {
                     uv[2][0] = uv[1][0] = stob->data[patch].uv[0][0];
                     uv[1][1] = uv[0][1] = stob->data[patch].uv[1][1];
                     uv[0][0] = stob->data[patch].uv[1][0];
                     uv[2][1] = stob->data[patch].uv[0][1];
                  }

                  tpob->direct2Dmap(pob->countobject, uv, stob->data[patch].ob, 3);
               }

               pob->polynum[pob->countobject] = 3;
               pob->countobject++;
            }

      }

   if (xflag)
      tpob->countobject = pob->countobject;
}


/* **************************************************
   This procedure scans 2D grids into surface patches
   or polygonal structures.
************************************************** */
void spatch::convert(int i, int j, spatchtype *sob, texpolygon *stob, shaderlisttype *ssob) {

   int h, k, l, m, n;
   float u, v, t;
   pvector4f ptr[4];
   float SPLQ[4][4], SPLQ2[4][4], SPPQ[4][4][4];
   float dt[2], pt[2][2], start[2];
   float delta = 1.0/SPDELTA;
   int patch = i * ob->pcol + j;
   int subpatch;

   ptr[0] = &ob->mvertex[i][j];
   ptr[1] = &ob->mvertex[i+1][j];
   ptr[2] = &ob->mvertex[i+2][j];
   ptr[3] = &ob->mvertex[i+3][j];

   for (n=0; n<4; n++)
      for (m=0; m<4; m++)
         for (h=0; h<4; h++)
              SPPQ[n][m][h] = ptr[m][h][n];

                                                // top row
   for (n=0; n<4; n++)
      for (h=0; h<4; h++)
         SPLQ[n][h] = ptr[1][h][n];

   k=i+1;
   l=j+1;
   copyarray4(sob->mvertex[1][1], ob->mvertex[k][l]);

   if (mctype > PWFBW)
      spnormal(0, 0, SPPQ, (float *)&sob->gnormal[1][1], M_CR);

   for (n=1, m=2, t=delta; n<SPDELTA; n++, m++, t+=delta) {
      spline(t, SPLQ, (float *)&sob->mvertex[1][m], M_CR);
      sob->mvertex[1][m][3] = 1;

      if (mctype > PWFBW)
         spnormal(0, t, SPPQ, (float *)&sob->gnormal[1][m], M_CR);
   }

   l++;
   copyarray4(sob->mvertex[1][m], ob->mvertex[k][l]);
   if (mctype > PWFBW)
      spnormal(0, 1, SPPQ, (float *)&sob->gnormal[1][m], M_CR);

                                                // last row
   for (n=0; n<4; n++)
      for (h=0; h<4; h++)
         SPLQ[n][h] = ptr[2][h][n];

   k=i+2;
   l=j+1;
   h = 1+SPDELTA;
   copyarray4(sob->mvertex[h][1], ob->mvertex[k][l]);

   if (mctype > PWFBW)
      spnormal(1, 0, SPPQ, (float *)&sob->gnormal[h][1], M_CR);

   for (n=1, m=2, t=delta; n<SPDELTA; n++, m++, t+=delta) {
      spline(t, SPLQ, (float *)&sob->mvertex[h][m], M_CR);
      sob->mvertex[1][m][3] = 1;

      if (mctype > PWFBW)
         spnormal(1, t, SPPQ, (float *)&sob->gnormal[h][m], M_CR);
   }

   l++;
   copyarray4(sob->mvertex[h][m], ob->mvertex[k][l]);
   if (mctype > PWFBW)
      spnormal(1, 1, SPPQ, (float *)&sob->gnormal[h][m], M_CR);

                                        // rest
   for (n=0; n<4; n++)
      for (h=0; h<4; h++)
         SPLQ[n][h] = ptr[h][1][n];

   for (n=0; n<4; n++)
      for (h=0; h<4; h++)
         SPLQ2[n][h] = ptr[h][2][n];

   for (h=2, k=1, u=delta; k<SPDELTA; k++, u+=delta, h++) {
      spline(u, SPLQ, (float *)&sob->mvertex[h][1], M_CR);
      sob->mvertex[h][1][3] = 1;

      if (mctype > PWFBW)
         spnormal(u, 0, SPPQ, (float *)&sob->gnormal[h][1], M_CR);

      for (l=2,n=1, v=delta; n<SPDELTA; n++, v+=delta, l++) {
         sppatch(u, v, SPPQ, (float *)&sob->mvertex[h][l], M_CR);
         sob->mvertex[h][l][3] = 1;

         if (mctype > PWFBW)
            spnormal(u, v, SPPQ, (float *)&sob->gnormal[h][l], M_CR);
      }

      spline(u, SPLQ2, (float *)&sob->mvertex[h][l], M_CR);
      sob->mvertex[h][l][3] = 1;

      if (mctype > PWFBW)
         spnormal(u, 1, SPPQ, (float *)&sob->gnormal[h][l], M_CR);
   }

   if (mctype > PBW) {

      for (k=0; k<SPDELTA; k++)
         for (l=0; l<SPDELTA; l++)
            sob->mshade[k][l] = ob->mshade[i][j];

      if ((mcinfo & CITEXTURE) && tob->query_tflag(patch)) {
         copyarray2(start, tob->data[patch].uv[0]);
         dt[0] = (tob->data[patch].uv[1][0]-start[0])*delta;
         dt[1] = (tob->data[patch].uv[1][1]-start[1])*delta;

         for (subpatch = k=0, pt[0][1]=start[1], pt[1][1]=start[1]+dt[1]; k<SPDELTA; k++, pt[0][1]=pt[1][1], pt[1][1]+=dt[1])
            for (l=0, pt[0][0]=start[0], pt[1][0]=start[0]+dt[0]; l<SPDELTA; l++, subpatch++, pt[0][0]=pt[1][0], pt[1][0]+=dt[0]) {
               stob->data[subpatch].setup(SPDELTA);
               stob->data[subpatch].ob = tob->data[patch].ob;
               copyarray2(stob->data[subpatch].uv[0], pt[0]);
               copyarray2(stob->data[subpatch].uv[1], pt[1]);
            }

      }

      if (mcinfo & CI3DTEX) {
         m = sob->psize<<1;
         for (k=0; k<m; k++)
            ssob->shade[k] = shaderlist->shade[patch];
      }

   }

   spline2poly(sob, stob, (mcinfo & CITEXTURE));
}


/* *************************************************************
   This procedure puts the object in the edge table
************************************************************* */
void spatch::transform(eye *parm) {

   int   i, j;                          // looping vars

   copymx4x4(world, rotate);

   for (i=0; i<3; i++)
      for (j=0; j<3; j++)
         world[i][j] *= size;

   world[0][3] += center[0];
   world[1][3] += center[1];
   world[2][3] += center[2];

   matmatmulto(parm->transform, world);
   inversemx(world, iworld);

   for (i=0; i<ob->vsize; i++)
      matvecmulto(world, ob->data[i]);
}


/* *************************************************************
   This function copies the basic object data into a composite
   structure for manipulation
************************************************************* */
void spatch::datacopy() {

   int i;                          // looping variables

   ob->build(dob->vrow, dob->vcol, 1, (i=(mctype > BW && mctype <= PHONG) || mctype > PBW));

   memcpy(ob->data, dob->data, ob->vsize*sizeof(vector4f));

   if (i) {
      base_color = &lob->base;

      for (i=0; i<ob->psize; i++)
         ob->shade[i] = base_color;

      for (i=0; i<lob->count; i++)
         ob->shade[lob->list[i].index] = &lob->list[i];
   }

}


/* **************************************************
   This procedure scans 2D grids into surface patches
   or polygonal structures.
************************************************** */
int spatch::scan(camera *cparm, light *lmain) {

   int       i, l;
   int       texflag = mcinfo & CITEXTURE;
   int       flag3d = mcinfo & CI3DTEX;
   shadertype *str;

   transform(cparm);

   if (!ob->psize || !(i=bound_box(cparm)))
      return 0;

   if (i == 2) {
      polyob.setup_patch(pob, mctype, mcinfo, id, sflag, splane, bbox, base_color);
      renderflag = SPATPOLY;
      i = polyob.scan(cparm, lmain);
      shadptr = polyob.shadptr;
      polyob.shadptr = (pc *)NULL;
      return i;
   }

   if (!clip(cparm, WINX, WINY))
      return 0;

   if (mctype <= PHONG) {                // display data as polygons
      setup_transfer(ob->srow*ob->scol, ob->psize);
      spline2poly(ob, tob, texflag);
      polyob.setup_patch(pob, mctype, mcinfo, id, sflag, splane, bbox, base_color);

      if (texflag)
         polyob.setup_texture(tpob);

      if (flag3d) {
         i = ob->psize<<1;
         slist.scount = i;
         slist.scale = shaderlist->scale;

         slist.shade = new shadertype[i];

         for (i=0, str = slist.shade; i<ob->psize; i++, str += 2)
           *(str+1) =  *str = shaderlist->shade[i];

         polyob.setup_texture3d(&slist, world, iworld);
      }

      renderflag = SPATPOLY;
      i = polyob.scan(cparm, lmain);
      shadptr = polyob.shadptr;
      polyob.shadptr = (pc *)NULL;
      return i;
   }

   if (sflag && lmain && !NOSHADOW && !(mcinfo & CITRANSPARENT))
      shadptr = new basenull;

   cptr = cparm;
   lptr = lmain;

   ptr.build(SPDELTA + 3, SPDELTA + 3, mctype > PWFBW, mctype > PBW);

   setup_transfer(ptr.vsize, ptr.psize);

   if (texflag) {
      ttr.setup(ptr.psize);
      polyob.setup_texture(tpob);
   }

   l = ptr.psize<<1;

   if (flag3d) {
      slist.scount = 0;
      slist.shade = new shadertype[l];
      polyob.setup_texture3d(&slist, world, iworld);
   }

   polyob.setup_patch(pob, mctype, mcinfo, id, sflag, splane, bbox, base_color);

   renderflag = SPATREND;
   return 1;
}


/* **************************************************
************************************************** */
void spatch::render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff) {

   int          i, j, k, l;
   int          texflag = mcinfo & CITEXTURE;
   int          flag3d = mcinfo & CI3DTEX;

   switch (renderflag) {

      case SPATPOLY:
         polyob.render(cparm, lmain, spot, zbuff);
         return;

      case SPATREND:
         l = ptr.psize<<1;

         for (i=0; i<ob->prow; i++)
            for (j=0; j<ob->pcol; j++) {

               if (flag3d)
                  for (k=0; k<l; k++)
                     slist.shade[k].s = (shader **)NULL;

               if (texflag) {
                  if (ttr.query_tflag(0)) {
                     for (k=0; k<ttr.countobject; k++)
                        delete [] ttr.data[k].uv;

                     memset(ttr.data, 0, ttr.countobject*sizeof(texface));
                  }

                  if (tpob->query_tflag(0)) {
                     for (k=0; k<tpob->countobject; k++)
                        delete [] tpob->data[k].uv;

                     memset(tpob->data, 0, tpob->countobject*sizeof(texface));
                  }

               }

               convert(i, j, &ptr, &ttr, &slist);
               polyob.ob->calc_normal();

               if (polyob.scan(cparm, lmain))
                  polyob.render(cparm, lmain, spot, zbuff);

               if (polyob.shadptr != (pc *)NULL) {
                  if (shadptr && polyob.shadptr->scan(cparm, lmain)) {
                     polyob.shadptr->next = shadptr->next;
                     shadptr->next = polyob.shadptr;
                  }

                  else
                     delete polyob.shadptr;

                  polyob.shadptr = (pc *)NULL;
               }

            }

         return;
   }

}


/* **************************************************
   This procedure scans 2D grids into surface patches
   or polygonal structures.
************************************************** */
void spatch::prender() {

   int          i, j;
   spatchtype   ptr;

   switch (renderflag) {

      case SPATPOLY:
         polyob.prender();
         return;

      case SPATREND:
         ptr.build(SPDELTA + 3, SPDELTA + 3, 0, 0);

         setup_transfer(ptr.vsize, ptr.psize);

         for (i=0; i<ob->prow; i++)
            for (j=0; j<ob->pcol; j++) {
               convert(i, j, &ptr, (texpolygon *)NULL, (shaderlisttype *)NULL);
               polyob.setup_patch(pob, PWFBW, CINULL, id, sflag, splane, bbox, base_color);
               if (polyob.scan(cptr, lptr))
                  polyob.prender();

               if (polyob.shadptr != (pc *)NULL) {
                  if (shadptr && polyob.shadptr->scan(cptr, lptr)) {
                     polyob.shadptr->next = shadptr->next;
                     shadptr->next = polyob.shadptr;
                  }

                  else
                     delete polyob.shadptr;

                  polyob.shadptr = (pc *)NULL;
               }

            }

         return;
   }

}


/* **************************************************
************************************************** */
int spatch::beamscan(spotlight *lparm) {

   int        i;
   colortype  nctype;
   unsigned int ncinfo;

   if (mctype < BW || (mctype > PHONG && mctype < PBW))
      return 0;

   nctype = mctype;
   ncinfo = mcinfo;

   mcinfo = CINULL;
   mctype = (mctype <= PHONG) ? BW : PBW;

   transform(lparm);

   if (!ob->psize || !(i=bound_box(lparm))) {
      mctype = nctype;
      mcinfo = ncinfo;
      return 0;
   }

   if (i == 2) {
      polyob.setup_patch(pob, mctype, mcinfo, id, sflag, splane, bbox, base_color);
      mctype = nctype;
      mcinfo = ncinfo;
      renderflag = SPATPOLY;
      return polyob.beamscan(lparm);
   }

   if (!clip(lparm, lparm->maxx, lparm->maxy)) {
      mctype = nctype;
      mcinfo = ncinfo;
      return 0;
   }

   if (mctype == BW) {                // display data as polygons
      setup_transfer(ob->srow*ob->scol, ob->psize);
      spline2poly(ob, (texpolygon *)NULL, 0);
      polyob.setup_patch(pob, mctype, mcinfo, id, sflag, splane, bbox, base_color);

      mctype = nctype;
      mcinfo = ncinfo;
      renderflag = SPATPOLY;
      return polyob.beamscan(lparm);
   }

   mctype = nctype;
   mcinfo = ncinfo;
   renderflag = SPATREND;
   return 1;
}


/* **************************************************
************************************************** */
void spatch::beamrender(spotlight *lparm) {

   int        i, j;
   colortype  nctype;
   unsigned int ncinfo;
   spatchtype ptr;

   switch (renderflag) {

      case SPATPOLY:
         polyob.beamrender(lparm);
         return;

      case SPATREND:
         nctype = mctype;
         ncinfo = mcinfo;

         mcinfo = CINULL;
         mctype = (mctype <= PHONG) ? BW : PBW;

         ptr.build(SPDELTA + 3, SPDELTA + 3, mctype > PWFBW, 0);

         setup_transfer(ptr.vsize, ptr.psize);

         for (i=0; i<ob->prow; i++)
            for (j=0; j<ob->pcol; j++) {
               convert(i, j, &ptr, (texpolygon *)NULL, (shaderlisttype *)NULL);
               polyob.setup_patch(pob, mctype, mcinfo, id, sflag, splane, bbox, base_color);
               if (polyob.beamscan(lparm))
                  polyob.beamrender(lparm);
            }

         mctype = nctype;
         mcinfo = ncinfo;
         return;
   }

}

