

#include <string.h>
#include "geo.h"
#include "global.h"


/* *************************************************************
************************************************************* */
void geo::preprocess(void *data) {

   listtype *ltr = (listtype *)data;

   make_poly::preprocess(data);

   if ((mctype > BW && mctype <= PHONG) || mctype > PBW) {

      read_ilum(colorname, &ltr->itr);

      if (mcinfo & CITEXTURE)
         read_tex(texname2, filename, &ltr->tdtr, (texmap **)&ltr->tmap);
      else if (mcinfo & CI3DTEX)
         read_3dtex(texname3, filename, ltr->shade_list, &ltr->shltype);
   }

}


/* *************************************************************
        bbox is made by children
************************************************************* */
int geo::bound_box(eye *parm) {

   int    i, j;
   float  extreme[2][2];
   float  d[2];
   vector4f box[8];

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
      if (mctype > BW)
         mctype = CONSTANT;

      mcinfo &= CIRAYCAST;

      setup_transfer(4,1);

      pob->make_flat(extreme, bbox[1][2], parm);

      if (mctype == CONSTANT)
         for (i=0; i<pob->countobject; i++)
            pob->shade[i].color = base_color;

      return (boundflag = 2);
   }

   return (boundflag = 1);
}


/* *************************************************************
************************************************************* */
int geo::read_tex(char *filename, char *dfilename, texbase **tptr, texmap **tpptr) {

   for (tob=*tptr; tob!=(texbase *)NULL; tob=tob->next)
      if (!strcmp(filename, tob->filename))
         return 1;

   if (query_video(filename)) {
      tob = new texvid;
      tob->read_texture(filename, tptr);
   }

   else {
      tob = new texture;
      tob->read_texture(filename);
   }

   texcoord[1][0] = texcoord[0][0] = texcoord[3][1] = texcoord[0][1] = 0.0;
   texcoord[2][1] = texcoord[1][1] = (float)(tob->maxy-1);
   texcoord[3][0] = texcoord[2][0] = (float)(tob->maxx-1);

   tob->next = *tptr;
   *tptr = tob;
   return 1;
}


/* *************************************************************
************************************************************* */
int geo::read_3dtex(char *filename, char *dfilename, shader *sptr, shaderlisttype **slptr) {

   int i;
   char buffer[MAXSTRLEN];
   FILE *infile;
   int count;

   for (shaderlist=*slptr; shaderlist!=(shaderlisttype *)NULL; shaderlist=shaderlist->next)
      if (!strcmp(filename, shaderlist->shname) && !strcmp(dfilename, shaderlist->dataname))
         return 1;

   if (!(infile = fopen(filename, "r"))) {
      printf("Can't find file %s\n", filename);
      mcinfo &= ~CI3DTEX;
      return 0;
   }

   shaderlist = new shaderlisttype;
   strcpy(shaderlist->shname, filename);
   strcpy(shaderlist->dataname, dfilename);
   shaderlist->init(1);
   shaderlist->next = *slptr;
   *slptr = shaderlist;

   fscanf(infile, "%s %f", buffer, &shaderlist->scale);
   lower_case(buffer);

   if (strcmp(buffer, "mono"))                 //ALL
      shaderlist->shade->read_data(infile, sptr);

   fscanf(infile, "%d", &count);

   for (; count>0; count--) {
      fscanf(infile, "%d", &i);

      if (i == 1)
         shaderlist->shade->read_data(infile, sptr);
   }

   fclose(infile);
   return 1;
}



unsigned int geo::query_texel(float tx, float ty, float *c, float d) {

   float a, b;

   a = (tx - (int)tx)*texcoord[2][0];
   b = (ty - (int)ty)*texcoord[2][1];

   return tob->query_texel(a, b, c, frame, 1, d);
}


int geo::clip(eye *parm, int maxx, int maxy) {

   float corner[4];

   if (bbox[0][2] > front ||
       parm->zscale(bbox[0][0], bbox[0][2]) > parm->vrc[1] ||     // outside boundaries
       parm->zscale(bbox[1][0], bbox[0][2]) < parm->vrc[0] ||
       parm->zscale(bbox[0][1], bbox[0][2]) > parm->vrc[3] ||
       parm->zscale(bbox[1][1], bbox[0][2]) < parm->vrc[2])
      return 0;

   copyarray3(corner, bbox[0]);
   corner[3] = 1;

   parm->map2screen(corner);

   row[0] = round(corner[1]);
   col[0] = round(corner[0]);

   corner[0] = bbox[1][0];
   corner[1] = bbox[1][1];
   corner[2] = bbox[0][2];

   parm->map2screen(corner);

   row[1] = round(corner[1]);
   col[1] = round(corner[0]);

   corner[0] = bbox[0][0];
   corner[1] = bbox[0][1];
   corner[2] = bbox[1][2];

   parm->map2screen(corner);

   if (row[0] > corner[1])
      row[0] = round(corner[1]);
   if (col[0] > corner[0])
      col[0] = round(corner[0]);

   copyarray3(corner, bbox[1]);

   parm->map2screen(corner);

   if (row[1] < corner[1])
      row[1] = round(corner[1]);
   if (col[1] < corner[0])
      col[1] = round(corner[0]);

   if (col[0] < 0)
      col[0] = 0;

   if (col[1] >= maxx)
      col[1] = maxx-1;

   if (row[0] < 0)
      row[0] = 0;

   if (row[1] >= maxy)
      row[1] = maxy-1;

   return 1;
}


/* **************************************************
************************************************** */
void geo::datacopy() {

   if (lob != (shadelist *)NULL)
      base_color = &lob->base;
};


/* **************************************************
************************************************** */
int geo::beamscan(spotlight *lparm) {

   int i;
   colortype  nctype;
   unsigned int ncinfo;

   if (mctype < BW)
      return 0;

   nctype = mctype;
   ncinfo = mcinfo;

   ncinfo = CINULL;
   mctype = (mctype <= PHONG) ? BW : PBW;

   transform(lparm);

   if (!(i=bound_box(lparm))) {
      mctype = nctype;
      mcinfo = ncinfo;
      return 0;
   }

   if (i==2) {
      polyob.setup_patch(pob, BW, mcinfo, id, 0, splane, bbox, base_color);

      mctype = nctype;
      mcinfo = ncinfo;
      renderflag = SCANPOLY;

      return polyob.beamscan(lparm);
   }

   if (!clip(lparm, lparm->maxx, lparm->maxy)) {
      mctype = nctype;
      mcinfo = ncinfo;
      return 0;
   }

   if (mctype < PALEX) {
      geo2poly((texpolygon **)NULL, (shaderlisttype *)NULL);
      polyob.setup_patch(pob, BW, mcinfo, id, 0, splane, bbox, base_color);
      mctype = nctype;
      mcinfo = ncinfo;

      renderflag = SCANPOLY;

      return polyob.beamscan(lparm);
   }

   nctype = mctype;
   ncinfo = mcinfo;
   renderflag = SCANCAST;
   return 1;
}


/* **************************************************
************************************************** */
void geo::beamrender(spotlight *lparm) {

   switch (renderflag) {

      case SCANPOLY:
         polyob.beamrender(lparm);
         return;

      case SCANCAST:
         lt(lparm);
         return;
   }

}


/* **************************************************
************************************************** */
int geo::scan(camera *cparm, light *lmain) {

   int i;
   shadow    *str;
   char texflag = (char)(mcinfo & CITEXTURE);
   char flag3d = (char)(mcinfo & CI3DTEX);

   transform(cparm);

   if (!(i=bound_box(cparm)))
      return 0;

   if (i == 2) {
      polyob.setup_patch(pob, mctype, mcinfo, id, sflag, splane, bbox, base_color);
      renderflag = SCANPOLY;
      i = polyob.scan(cparm, lmain);
      shadptr = polyob.shadptr;
      return i;
   }

   if (!clip(cparm, WINX, WINY))
      return 0;

   if (mctype < PALEX) {
      geo2poly(&tpob, &slist);
      polyob.setup_patch(pob, mctype, mcinfo, id, sflag, splane, bbox, base_color);

      if (texflag)
         polyob.setup_texture(tpob);

      if (flag3d)
         polyob.setup_texture3d(&slist, world, iworld);

      renderflag = SCANPOLY;
      i = polyob.scan(cparm, lmain);
      shadptr = polyob.shadptr;
      return i;
   }

   if (sflag && lmain!=(light *)NULL && !NOSHADOW && !(mcinfo & CITRANSPARENT)) {
      geo2poly((texpolygon **)NULL, (shaderlisttype *)NULL);
      polyob.setup_patch(pob, ALEX, 0, id, 1, splane, bbox, base_color);

      str = new shadow;
      str->begin_scan();

      if (str->datacopy2(&polyob, cparm, lmain, 1))
         shadptr = str;
      else {
         str->end_scan();
         delete str;
      }

      polyob.end_scan();
   }

   renderflag = SCANCAST;
   return 1;
}


/* **************************************************
************************************************** */
void geo::render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff) {

   switch (renderflag) {

      case SCANPOLY:
         polyob.render(cparm, lmain, spot, zbuff);
         return;

      case SCANCAST:
         pg(cparm, lmain, spot, zbuff);
         return;
   }

}


/* **************************************************
************************************************** */
void geo::prender() {

   polyob.prender();
}


/* **************************************************
************************************************** */
void geo::begin_scan() {

   tpob = (texpolygon *)NULL;
}


/* **************************************************
************************************************** */
void geo::end_scan() {

   if (tpob != (texpolygon *)NULL)
      delete [] tpob;

   if (slist.shade != (shadertype *)NULL) {
      delete [] slist.shade;
      slist.shade = (shadertype *)NULL;
   }

   if (renderflag == SCANPOLY)
      polyob.end_scan();
}
