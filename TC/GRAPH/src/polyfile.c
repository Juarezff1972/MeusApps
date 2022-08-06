

#include <stdlib.h>
#include <string.h>
#include "polygon.h"

/* *************************************************************
************************************************************* */
int polygon::parse(FILE *infile, char *token) {

   vector4uc temp;

   switch (token[0]) {

      case 'f':
         if (!strcmp(token, "file")) {
            get_token(infile, filename);

            return 1;
         }

         break;

      case 'i':
         if (!strcmp(token, "inviso")) {
            get_token(infile, token);
            temp[3] = atoi(token);
            get_token(infile, token);
            temp[2] = atoi(token);
            get_token(infile, token);
            temp[1] = atoi(token);
            temp[0] = 0;
            inviso = *(int *)temp;
            mcinfo |= CIINVISO;

            return 1;
         }

         break;

      default:
         break;
   }

   return pc::parse(infile, token);
}


/* *************************************************************
************************************************************* */
void polygon::preprocess(void *data) {

   int i, j;
   listtype *ltr = (listtype *)data;

   pc::preprocess(data);

   if (!read_data(filename, &ltr->dtr))
      exit(1);

   if ((mctype > BW && mctype <= PHONG) || mctype > PBW) {

      if (mcinfo & CITEXTURE)
         read_tex(texname2, filename, &ltr->tdtr, (texpolygon **)&ltr->tmap);

      else if (mcinfo & CI3DTEX)
         read_3dtex(texname3, filename, ltr->shade_list, &ltr->shltype);
   }

   facedata = new faceinfo[dob->countobject];
}


/* *************************************************************
   This procedure reads in the object data.  File format is:

   M                                    - # of vertices   m
   N                                    - # of polygons   n
   X1 Y1 Z1                             - x y z coordinates for vertex 1
    - - -
   XM YM ZM                             - x y z coordinates for vertex N
   C1 V1 ... Vc1                        - # of vertices in this polygon, and
    - - -                                   pointers to vertex list...
   CN VN ... VcN
************************************************************* */
int polygon::read_data(char *filename, datatype **dptr) {

   int i;

   for (dob=(polytype *)*dptr; dob != (polytype *)NULL; dob=(polytype *)dob->next)
      if (!strcmp(filename, dob->dataname))
         return dob->countobject;

   dob = new polytype;

   if (!(i=dob->read_data(filename))) {
      delete dob;
      dob = (polytype *)NULL;
      return 0;
   }

   dob->next = *dptr;
   *dptr     = dob;
   return i;
}


/* *************************************************************
************************************************************* */
int polygon::read_tex(char *filename, char *dfilename, texbase **tptr, texpolygon **tpptr) {

   FILE *infile;                        // file pointer
   int i, j, k;
   int count;
   char tempstr[MAXSTRLEN];             // generic string
   int  coordflag;
   int corner[4];
   texbase *tmap;
   vector2f *uv;

   for (tob=*tpptr; tob!=(texpolygon *)NULL; tob=(texpolygon *)tob->next)
      if (!strcmp(filename, tob->texname) && !strcmp(dfilename, tob->dataname))
         return 1;

   if (!(infile = fopen(filename, "r"))) {
      printf("Can't find file %s\n", filename);
      mcinfo &= ~CITEXTURE;
      return 0;
   }

   tob = new texpolygon;
   strcpy(tob->texname, filename);
   strcpy(tob->dataname, dfilename);

   tob->next = *tpptr;
   *tpptr    = tob;

   tob->setup(dob->countobject);

   fscanf(infile, "%s", tempstr);
   lower_case(tempstr);

   if (strcmp(tempstr, "mono")) {

      fscanf(infile, "%s", tempstr);
      lower_case(tempstr);

      fscanf(infile, "%d %d %d %d", &corner[0], &corner[1], &corner[2], &corner[3]);
      corner[0]--;
      corner[1]--;
      corner[2]--;
      corner[3]--;

      fscanf(infile, "%s", tempstr);               // texture name
      for (tmap=*tptr; tmap!=(texbase *)NULL; tmap=tmap->next)
         if (!strcmp(tempstr, tmap->filename))
            break;

      if (tmap == (texbase *)NULL) {

         if (query_video(tempstr)) {
            tmap = new texvid;
            tmap->read_texture(tempstr, tptr);
         }

         else {
            tmap = new texture;
            tmap->read_texture(tempstr);
         }

         tmap->next = *tptr;
         *tptr = tmap;
      }

      for (j=0; j<dob->countobject; j++)
         tob->perim2Dmap(j, corner, tmap, dob);
   }

   fscanf(infile, "%d", &count);

   for (; count>0; count--) {
      fscanf(infile, "%d", &j);
      j--;

      fscanf(infile, "%s", tempstr);
      lower_case(tempstr);

      if (!strcmp(tempstr, "coord")) {
         coordflag = 1;
         i = dob->polynum[j];
         uv = new vector2f[i];

         for (k=0; k<i; k++)
            fscanf(infile, "%f %f", &uv[k][0], &uv[k][1]);
      }

      else if (!strcmp(tempstr, "affine")) {
         coordflag = 2;
         uv = new vector2f[3];

         fscanf(infile, "%f %f", &uv[0][0], &uv[0][1]);
         fscanf(infile, "%f %f", &uv[1][0], &uv[1][1]);
         fscanf(infile, "%f %f", &uv[2][0], &uv[2][1]);
      }

      else if (!strcmp(tempstr, "corner")) {

         coordflag = 0;
         fscanf(infile, "%d %d %d %d", &corner[0], &corner[1], &corner[2], &corner[3]);
         corner[0]--;
         corner[1]--;
         corner[2]--;
         corner[3]--;
      }

      else {
         printf("Invalid 2D texture method \"%s\"\n", tempstr);
         fclose(infile);
         exit(0);
      }

      fscanf(infile, "%s", tempstr);               // texture name
      for (tmap=*tptr; tmap!=(texbase *)NULL; tmap=tmap->next)
         if (!strcmp(tempstr, tmap->filename))
            break;

      if (tmap == (texbase *)NULL) {

         if (query_video(tempstr)) {
            tmap = new texvid;
            tmap->read_texture(tempstr, tptr);
         }

         else {
            tmap = new texture;
            tmap->read_texture(tempstr);
         }

         tmap->next = *tptr;
         *tptr = tmap;
      }

      switch (coordflag) {

         case 1:
            tob->direct2Dmap(j, uv, tmap, dob->polynum[j]);
            delete [] uv;
            break;

         case 2:
            tob->affinemap(j, uv, tmap, dob);
            delete [] uv;
            break;

         default:
            tob->perim2Dmap(j, corner, tmap, dob);
      }

   }

   fclose(infile);
   return 1;
}


int polygon::read_3dtex(char *filename, char *dfilename, shader *sptr, shaderlisttype **slptr) {

   int i, count;
   char buffer[MAXSTRLEN];
   FILE *infile;
   int objectcount = dob->countobject;

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
   shaderlist->init(objectcount);
   shaderlist->next   = *slptr;
   *slptr       = shaderlist;

   fscanf(infile, "%s %f", buffer, &shaderlist->scale);
      lower_case(buffer);

   if (strcmp(buffer, "mono")) {                        // ALL
      shaderlist->shade[0].read_data(infile, sptr);

      for (i=1; i<objectcount; i++)
         shaderlist->shade[i] = shaderlist->shade[0];
   }

   fscanf(infile, "%d", &count);

   for (; count>0; count--) {
      fscanf(infile, "%d", &i);
      shaderlist->shade[i-1].read_data(infile, sptr);
   }

   fclose(infile);
   return 1;
}