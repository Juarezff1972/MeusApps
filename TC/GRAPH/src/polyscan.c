

/* *************************************************************
   This function scans poligonal objects into a zbuffer
************************************************************* */


#include <string.h>

#include "polygon.h"
#include "global.h"
#include "memman.h"

/* *************************************************************
************************************************************* */
void make_poly::setup_transfer(int vcount, int pcount) {

   int i, j;

   i = pcount << 1;
   j = pcount << 3;

   pob = new polytype;

   pob->build(vcount, i, j, (mctype > PBW || (mctype > BW && mctype <=PHONG)), 0);
   pob->maxpolynum = 4;

   pob->countvertex = 0;
   pob->countobject = 0;
   pob->countedge   = 0;

   if (mcinfo & CITEXTURE) {
      tpob = new texpolygon;
      tpob->countobject = 0;

      tpob->data = new texface[i];
      memset(tpob->data, 0, i*sizeof(texface));
   }

}


/* *************************************************************
************************************************************* */
int polygon::bound_box(eye *parm) {

   int    i, j;
   float  extreme[2][2];
   float  d[2];
   vector4f box[8];

   copyarray3(bbox[0], ob->mvertex[0]);
   copyarray3(bbox[1], ob->mvertex[0]);

   for (i=1; i<ob->countvertex; i++)
      for (j=0; j<3; j++)
         if (bbox[0][j] > ob->mvertex[i][j])
            bbox[0][j] = ob->mvertex[i][j];
         else if (bbox[1][j] < ob->mvertex[i][j])
            bbox[1][j] = ob->mvertex[i][j];

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
      mcinfo &= CIRAYCAST;

      if (mctype > CONSTANT)
         mctype = CONSTANT;

      if (bflag) {
         if (ob->countvertex > 3 && ob->countobject > 0 && ob->countedge > 3) {
            ob->make_flat(extreme, bbox[1][2], parm);
            bflag = 0;
         }

         if (mctype == CONSTANT)
            for (i=0; i<ob->countobject; i++)
               ob->shade[i] = base_color;
      }

   }

   return (boundflag = 1);
}


/* *************************************************************
************************************************************* */
void polygon::setup_texture(texpolygon *stob) {

   tob     = stob;
}


/* *************************************************************
************************************************************* */
void polygon::setup_texture3d(shaderlisttype *stob, float sworld[][4], float siworld[][4])  {

   shaderlist = stob;
   copymx4x4(world, sworld);
   copymx4x4(iworld, siworld);
}


/* *************************************************************
   This function converts surface patch data into polygonal data
************************************************************* */
void polygon::setup_patch(polytype *sob, colortype sctype, unsigned scinfo,
                          int sid, int ssflag, float  ssplane[],
                          float  box[][4], shadetype *c) {

   base_color = c;
   copyarray4(bbox[0], box[0]);
   copyarray4(bbox[1], box[1]);

   ob = sob;
   ob->calc_normal();
   ob->calc_area();

   if (sctype >=PDOT)
      sctype = (colortype)(sctype - PDOT + DOT);

   mctype = sctype;
   mcinfo = scinfo;
   id     = sid;
   size   = 1;
   center[0] = center[1] = center[2] = 0;
   init_mx(rotate);

   bbflag = tflag  = 0;

   sflag = (ssflag) ? 2 : 0;

   bflag = (mctype > WFBW);

   copyarray4(splane, ssplane);

   facedata = new faceinfo[ob->countobject];
}


/* *************************************************************
   This procedure takes the basic object data, and creates a composite
   structure to be worked on.
************************************************************* */
void polygon::datacopy() {

   int i, j;               // looping variables

   ob->build(dob->countvertex, dob->countobject, dob->countedge, (mctype > BW), 1);
   ob->maxpolynum = dob->maxpolynum;

   i = ob->countvertex<<4;
   j = ob->countobject<<2;

   memcpy(ob->mvertex, dob->mvertex, i);
   memcpy(ob->polynum, dob->polynum, j);
   memcpy(ob->normal, dob->normal, j<<2);
   memcpy(ob->gnormal, dob->gnormal, i);
   memcpy(ob->iarea, dob->iarea, j);

   ob->object = dob->object;
   ob->edgeptr = dob->edgeptr;

   if (mctype > BW) {
      base_color = &lob->base;

      for (i=0; i<ob->countobject; i++)
         ob->shade[i] = base_color;

      for (i=0; i<lob->count; i++)
         ob->shade[lob->list[i].index] = &lob->list[i];
   }

   if (mcinfo & CITEXTURE)
      ob->scale_area(size);

   bflag = (mctype > WFBW);
}


/* **************************************************
************************************************** */
void polygon::begin_scan() {

   ob = new polytype;
}


/* **************************************************
************************************************** */
void polygon::end_scan() {

   if (et != (edgetype **)NULL) {
      delete [] et;
      et = (edgetype **)NULL;
   }

   if (ob != (polytype *)NULL) {
      delete ob;
      ob = (polytype *)NULL;
   }

}


/* **************************************************
   This function scans polygonal data and puts it in a zbuffer.
************************************************** */
int polygon::scan(camera *cparm, light *lmain) {

   int       i;                        // loop counter
   shadow    *str;

   if (sflag && lmain!=(light *)NULL && !NOSHADOW && !(mcinfo & CITRANSPARENT)) {
      str = new shadow;
      str->begin_scan();

      str->datacopy2(this, cparm, lmain, sflag-1);
      shadptr = str;
   }

   if (tflag)
      transform(cparm);                  // transform objects

   if (!ob->countobject || (bbflag && !bound_box(cparm)))
      return 0;

   if (bflag)
      removeside();

   et = new pedgetype[ob->countobject+1];
   memset(et, 0, (ob->countobject+1)*sizeof(pedgetype));

   if (mctype > BW)
      if (lmain != (light *)NULL) {
         for (i=0; i<ob->countobject; i++)
            if (ob->polynum[i])
               lmain->set_ambient(ob->shade[i].color->ka, ob->shade[i].ambient, ob->shade[i].color->lum);
      }

      else
         for (i=0; i<ob->countobject; i++)
            if (ob->polynum[i])
                copyarray3(ob->shade[i].ambient, ob->shade[i].color->lum);

   if (!polyclet(cparm, lmain, WINX, WINY))
      return 0;

   renderflag = 1;
   return 1;
}


/* **************************************************
   This function scans polygonal data and puts it in a zbuffer.
************************************************** */
int polygon::beamscan(spotlight *spot) {

   edgetype     *ptr;
   colortype    nctype;
   unsigned int ncinfo;

   if (mctype < BW || (mcinfo & CIBITMAP))
      return 0;

   nctype = mctype;
   ncinfo = mcinfo;

   mctype = BW;
   mcinfo = CINULL;

   if (tflag)
      transform(spot);                  // transform objects

   if (!ob->countobject || (bbflag && !bound_box(spot))) {
      mctype = nctype;
      mcinfo = ncinfo;
      return 0;
   }

   if (bflag)
      removeside();

   et = new pedgetype[ob->countobject+1];
   memset(et, 0, (ob->countobject+1)<<2);

   if (!polyclet(spot, (light *)NULL, spot->maxx, spot->maxy)) {
      mctype = nctype;
      mcinfo = ncinfo;
      return 0;
   }
   for (;et[ob->countobject] != (edgetype *)NULL; ptr = et[ob->countobject], et[ob->countobject] = ptr->next,  control.push(EDGETYPE, ptr));     // delete horizontal lines

   mctype = nctype;
   mcinfo = ncinfo;
   renderflag = 1;

   return 1;
}
