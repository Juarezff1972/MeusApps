
#include <string.h>
#include "datatype.h"

/* *************************************************************
************************************************************* */

void polytype::init() {

   borrowflag = 0;
   maxpolynum = 0;
   normal = mvertex = gnormal = (vector4f *)NULL;
   object = polynum = (int *)NULL;
   iarea = (float *)NULL;
   edgeptr = (int **)NULL;
   shade = (illtype *)NULL;
}


void polytype::dest() {

   if (mvertex != (vector4f *)NULL)
      delete [] mvertex;
   if (normal != (vector4f *)NULL)
      delete [] normal;
   if (gnormal != (vector4f *)NULL)
      delete [] gnormal;
   if (polynum != (int *)NULL)
      delete [] polynum;
   if (iarea != (float *)NULL)
      delete [] iarea;

   if (!borrowflag) {
      if (edgeptr != (int **)NULL)
         delete [] edgeptr;
      if (object != (int *)NULL)
         delete [] object;
   }

   if (shade != (illtype *)NULL)
      delete [] shade;

   init();
}


int polytype::build(int cvertex, int cobject, int cedge, int sflag, int bflag) {

   if (!cvertex || !cobject || !cedge)
      return 0;

   dest();

   countvertex = cvertex;
   countobject = cobject;
   countedge   = cedge;

   mvertex = new vector4f[countvertex];
   polynum = new int[countobject];
   normal  = new vector4f[countobject];
   gnormal = new vector4f[countvertex];
   iarea   = new float[countobject];

   if (!(borrowflag = bflag)) {
      edgeptr = new pint[countobject];
      object = new int[countedge];
   }

   if (sflag)
      shade = new illtype[countobject];

   return 1;
}

int polytype::read_data(char *filename) {


   FILE *infile;                        // file pointer
   int i, j;                           // looping var

   strcpy(dataname, filename);

   if (!(infile = fopen(filename, "r"))) {
      printf("Could not access %s....\n", filename);
      return 0;
   }

   fscanf(infile, "%d %d %d", &countvertex, &countobject, &countedge);

   mvertex = new vector4f[countvertex];
   normal  = new vector4f[countobject];
   gnormal = new vector4f[countvertex];
   object  = new int[countedge];
   iarea   = new float[countobject];
   polynum = new int[countobject];
   edgeptr = new pint[countobject];

   for (i=0; i<countvertex; i++) {
      fscanf(infile, "%f %f %f", &mvertex[i][0], &mvertex[i][1], &mvertex[i][2]);
      mvertex[i][3] = 1;
   }

   maxpolynum = 0;
   countedge = 0;
   for (i=0; i<countobject; i++) {
      fscanf(infile, "%d", &polynum[i]);

      if (maxpolynum < polynum[i])
         maxpolynum = polynum[i];

      edgeptr[i] = &object[countedge];

      countedge += polynum[i];

      for (j=0; j<polynum[i]; j++) {
         fscanf(infile, "%d", &edgeptr[i][j]);
         edgeptr[i][j]--;
      }

   }

   fclose(infile);

   calc_normal();
   calc_area();
   calc_vnormal();

   return countobject;
}


void polytype::calc_normal() {

   int i, j;
   vector3f v1, v2;

   for (i=0; i<countobject; i++) {
      for (j=0; j<3; j++) {
         v1[j] = mvertex[edgeptr[i][1]][j] - mvertex[edgeptr[i][0]][j];
         v2[j] = mvertex[edgeptr[i][2]][j] - mvertex[edgeptr[i][1]][j];
      }

      xproduct(normal[i], v2, v1);
      normalize3(normal[i]);
                                        // used for phong2  ax+by+cz+d
      normal[i][3] = -dotproduct3(normal[i], mvertex[edgeptr[i][0]]);
   }

}

void polytype::calc_vnormal() {

   int i, j;

   memset(gnormal, 0, countvertex << 4);    // 4 floats x 4 bytes

   for (i=0; i<countobject; i++)
      for (j=0; j<polynum[i]; j++)
         addarray3(gnormal[edgeptr[i][j]], normal[i]);

   for (i=0; i<countvertex; i++)
      normalize3(gnormal[i]);
}


void polytype::calc_area() {

   int i, j;
   vector3f temp;

   for (i=0; i<countobject; i++) {
      iarea[i] = 0;

      for (j=0; j<polynum[i]-1; j++) {
         xproduct(temp, mvertex[edgeptr[i][j]], mvertex[edgeptr[i][j+1]]);
         iarea[i] += dotproduct3(normal[i], temp);
      }

      xproduct(temp, mvertex[edgeptr[i][j]], mvertex[edgeptr[i][0]]);
      iarea[i] += dotproduct3(normal[i], temp);

      iarea[i] = 1.0/sqrt(abs(iarea[i]));
   }

}


void polytype::vtransform(vector4f *mx) {

   int i;

   for (i=0; i<countvertex; i++)
      matvecmultv(mx, mvertex[i]);
}


void polytype::ntransform(vector4f *mx) {

   int i;

   for (i=0; i<countobject; i++)
      matvecmultv(mx, normal[i]);
}


void polytype::vntransform(vector4f *mx) {

   int i;

   for (i=0; i<countvertex; i++)
      matvecmultv(mx, gnormal[i]);
}


void polytype::scale_area(float scale) {

   int i;
   float iscale = 1.0/scale;

   for (i=0; i<countobject; i++)
      iarea[i] *= iscale;
}


void polytype::make_flat(vector2f *extreme, float depth, eye *parm) {

   int sflag = (shade != (illtype *)NULL);

   dest();

   build(4, 1, 4, sflag, 0);
   maxpolynum = 4;

   countvertex = 4;
   countobject = 1;
   countedge   = 4;

   mvertex[0][0] = extreme[0][0];
   mvertex[0][1] = extreme[0][1];
   mvertex[0][2] = depth;
   mvertex[0][3] = 1;
   parm->screen2map(mvertex[0]);

   mvertex[1][0] = extreme[0][0];
   mvertex[1][1] = extreme[1][1];
   mvertex[1][2] = depth;
   mvertex[1][3] = 1;
   parm->screen2map(mvertex[1]);

   mvertex[2][0] = extreme[1][0];
   mvertex[2][1] = extreme[1][1];
   mvertex[2][2] = depth;
   mvertex[2][3] = 1;
   parm->screen2map(mvertex[2]);

   mvertex[3][0] = extreme[1][0];
   mvertex[3][1] = extreme[0][1];
   mvertex[3][2] = depth;
   mvertex[3][3] = 1;
   parm->screen2map(mvertex[3]);

   object[0] = 0;
   object[1] = 1;
   object[2] = 2;
   object[3] = 3;

   normal[0][0] = 0;
   normal[0][1] = 0;
   normal[0][2] = 1;
   normal[0][3] = -depth;

   polynum[0] = 4;
   edgeptr[0] = object;
}


void spatchtype::init() {

   ndata = data = (vector4f *)NULL;
   gnormal = mvertex = (pvector4f *)NULL;
   shade = (illtype *)NULL;
   mshade = (illtype **)NULL;
}

void spatchtype::dest() {

   if (mvertex != (pvector4f *)NULL) {
      delete [] data;
      delete [] mvertex;
   }

   if (gnormal != (pvector4f *)NULL) {
      delete [] ndata;
      delete [] gnormal;
   }

   if (mshade != (illtype **)NULL) {
      delete [] shade;
      delete [] mshade;
   }

   init();
}

int spatchtype::build(int crow, int ccol, int nflag, int sflag) {

   int i, j;

   if (!crow || !ccol)
      return 0;

   dest();

   vrow = crow;
   vcol = ccol;

   srow = vrow - 2;
   scol = vcol - 2;

   prow = vrow - 3;
   pcol = vcol - 3;

   vsize = vrow*vcol;
   psize = prow*pcol;

   data = new vector4f[vsize];
   mvertex = new pvector4f[vrow];

   for (i=j=0; i<vrow; i++, j+=vcol)
      mvertex[i] = &data[j];

   if (nflag) {
      ndata = new vector4f[vsize];
      gnormal = new pvector4f[vrow];

      for (i=j=0; i<vrow; i++, j+=vcol)
         gnormal[i] = &ndata[j];
   }

   if (sflag) {
      shade  = new illtype[psize];
      mshade = new pilltype[prow];

      for (i=j=0; i<prow; i++, j+=pcol)
         mshade[i] = &shade[j];
   }

   return 1;
}


int spatchtype::read_data(char *filename) {

   FILE *infile;
   int i, size;

   if (!(infile = fopen(filename, "r"))) {
      printf("Could not access %s.\n", filename);
      return 0;
   }

   strcpy(dataname, filename);

   fscanf(infile, "%d %d", &vrow, &vcol);

   size = vrow * vcol;
   data = new vector4f[size];

   for (i=0; i<size; i++) {
      fscanf(infile, "%f %f %f", &data[i][0], &data[i][1], &data[i][2]);
      data[i][3] = 1;
   }

   fclose(infile);
   return (vrow-3)*(vcol-3);
}


void listtype::init(int x) {

   int i;

   dtr = (datatype *)NULL;
   itr = (shadelist *)NULL;
   tdtr = (texbase *)NULL;
   tmap = (texmap *)NULL;
   shltype = (shaderlisttype *)NULL;

   if (header != (pc **)NULL)
      delete [] header;
   header = new ppc[x];

   if (camdtr != (camera **)NULL)
      delete [] camdtr;
   camdtr = new pcamera[x];

   if (lightdtr != (light **)NULL)
      delete [] lightdtr;
   lightdtr = new plight[x];

   if (beamdtr != (light **)NULL)
      delete [] beamdtr;
   beamdtr = new plight[x];

   if (raddtr != (pc **)NULL)
      delete [] raddtr;
   raddtr = new ppc[x];

   for (i=0; i<x; i++) {
      header[i] = (pc *)NULL;
      camdtr[i] = (camera *)NULL;
      lightdtr[i] = (light *)NULL;
      beamdtr[i] = (light *)NULL;
      raddtr[i] = (pc *)NULL;
   }

   shade_list = shades;
}
