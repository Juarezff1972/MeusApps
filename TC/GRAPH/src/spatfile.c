



/* *************************************************************
   This file scans surface patch data into the buffer
************************************************************* */
#include <stdlib.h>
#include <string.h>
#include "spatch.h"


/* *************************************************************
************************************************************* */
int spatch::parse(FILE *infile, char *token) {

   switch (token[0]) {

      case 'f':
         if (!strcmp(token, "file")) {
            get_token(infile, filename);

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
void spatch::preprocess(void *data) {

   listtype *ltr = (listtype *)data;

   make_poly::preprocess(data);

   if (!read_data(filename, &ltr->dtr))
      exit(1);

   if ((mctype > BW && mctype <= PHONG) || mctype > PBW) {

      if (mcinfo & CITEXTURE)
         read_tex(texname2, filename, &ltr->tdtr, (texpolygon **)&ltr->tmap);
      else if (mcinfo & CI3DTEX)
         read_3dtex(texname3, filename, ltr->shade_list, &ltr->shltype);
   }

}


int spatch::read_3dtex(char *filename, char *dfilename, shader *sptr, shaderlisttype **slptr) {

   int i, j;
   char buffer[MAXSTRLEN];
   FILE *infile;
   int count;
   int objectcount = (dob->vrow-3)*(dob->vcol-3);

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
   shaderlist->next = *slptr;
   *slptr = shaderlist;

   fscanf(infile, "%s %f", buffer, &shaderlist->scale);
   lower_case(buffer);

   if (strcmp(buffer, "mono")) {                // ALL
      shaderlist->shade[0].read_data(infile, sptr);

      for (i=1; i<objectcount; i++)
         shaderlist->shade[i] = shaderlist->shade[0];
   }

   fscanf(infile, "%d", &count);

   for (; count>0; count--) {
      fscanf(infile, "%d %d", &i, &j);
      i = (i-1)*(dob->vcol-2) + j-1;

      shaderlist->shade[i].read_data(infile, sptr);
   }

   fclose(infile);
   return 1;
}

/* *************************************************************
************************************************************* */
int spatch::read_tex(char *filename, char *dfilename, texbase **tptr, texpolygon **tpptr) {

   FILE *infile;                        // file pointer
   int i, j, k, l, m;
   int coord[2][2];
   float corner[4][2];
   int count;
   char buffer[MAXSTRLEN], tempstr[MAXSTRLEN];             // generic string
   int patch;
   texbase *ptr;
   vector2f *pointer;
   int   start[2], end[2];
   double pt[2], d[2], current[2];
   int   vtr;

   int prow, pcol, srow, scol;

   prow = dob->vrow - 3;
   pcol = dob->vcol - 3;
   srow = dob->vrow - 2;
   scol = dob->vcol - 2;

   for (tob=*tpptr; tob!=(texpolygon *)NULL; tob=(texpolygon *)tob->next)
      if (!strcmp(filename, tob->texname) && !strcmp(dfilename, tob->dataname))
         return 0;

   if (!(infile = fopen(filename, "r"))) {
      printf("Could not access %s.\n", filename);
      return 0;
   }

   tob = new texpolygon;
   strcpy(tob->texname, filename);
   strcpy(tob->dataname, dfilename);

   tob->setup(prow*pcol);

   tob->next = *tpptr;
   *tpptr    = tob;

   pointer = new vector2f[scol*srow];

   fscanf(infile, "%d", &count);

   for (; count>0; count--) {

      fscanf(infile, "%s", buffer);
      lower_case(buffer);

      fscanf(infile, "%d %d %d %d", &coord[0][0], &coord[0][1], &coord[1][0], &coord[1][1]);

      if (!strcmp(buffer, "coord"))
         fscanf(infile, "%f %f %f %f", &corner[0][0], &corner[0][1], &corner[2][0], &corner[2][1]);

      fscanf(infile, "%s", tempstr);               // texture name

      for (ptr=*tptr; ptr!=(texbase *)NULL; ptr=ptr->next)
         if (!strcmp(tempstr, ptr->filename))
            break;

      if (ptr == (texbase *)NULL) {
         if (query_video(tempstr)) {
            ptr = new texvid;
            ptr->read_texture(tempstr, tptr);
         }

         else {
            ptr = new texture;
            ptr->read_texture(tempstr);
         }

         ptr->next = *tptr;
         *tptr = ptr;
      }

      coord[0][0]--;
      coord[0][1]--;
      coord[1][0]--;
      coord[1][1]--;

      if (!strcmp(buffer, "corner")) {
         if (coord[0][0] != coord[1][0] && coord[0][1] != coord[1][1]) {

            if (coord[0][0] > coord[1][0]) {
               start[0] = coord[1][0];
               end[0]   = coord[0][0];
            }

            else {
               start[0] = coord[0][0];
               end[0]   = coord[1][0];
            }

            if (coord[0][1] > coord[1][1]) {
               start[1] = coord[1][1];
               end[1]   = coord[0][1];
            }

            else {
               start[1] = coord[0][1];
               end[1]   = coord[1][1];
            }

            corner[1][0] = corner[0][0] = corner[3][1] = corner[0][1] = 0.0;
            corner[2][1] = corner[1][1] = (float)(ptr->maxy-1);
            corner[3][0] = corner[2][0] = (float)(ptr->maxx-1);

            pt[0] = corner[0][0];
            pt[1] = corner[0][1];
            d[0]  = (corner[2][0]-corner[0][0]) / ((double)(coord[1][0]-coord[0][0]));
            d[1]  = (corner[2][1]-corner[0][1]) / ((double)(coord[1][1]-coord[0][1]));

            for (j=start[1], current[1]=pt[1], i=j*scol; j<=end[1]; j++, current[1]+=d[1], i+=scol)
               for (k=start[0], current[0]=pt[0], vtr=i+k; k<=end[0]; k++, current[0]+=d[0], vtr++)
                   copyarray2(pointer[vtr], current);

            l = end[1]+start[1]-1;
            for (j=start[1], i=j*scol; j<end[1]; j++, i+=scol)
               for (k=start[0], vtr=i+k, m=vtr+scol+1, patch=(l-j)*pcol; k<end[0]; k++, vtr++, m++) {

                  tob->data[patch+k].setup(2);
                  tob->data[patch+k].ob = ptr;

                  tob->data[patch+k].uv[0][0] = pointer[vtr][0];
                  tob->data[patch+k].uv[0][1] = pointer[m][1];

                  tob->data[patch+k].uv[1][0] = pointer[m][0];
                  tob->data[patch+k].uv[1][1] = pointer[vtr][1];
               }

         }

      }

      else {
         corner[0][0]--;
         corner[0][1]--;
         corner[2][0]--;
         corner[2][1]--;

         if (coord[0][0] > coord[1][0]) {
            start[0] = coord[1][0];
            end[0]   = coord[0][0];
         }

         else {
            start[0] = coord[0][0];
            end[0]   = coord[1][0];
         }

         if (coord[0][1] > coord[1][1]) {
            start[1] = coord[1][1];
            end[1]   = coord[0][1];
         }

         else {
            start[1] = coord[0][1];
            end[1]   = coord[1][1];
         }

         pt[0] = corner[0][0];
         pt[1] = corner[0][1];
         d[0]  = (corner[2][0]-corner[0][0]) / ((double)(coord[1][0]-coord[0][0]));
         d[1]  = (corner[2][1]-corner[0][1]) / ((double)(coord[1][1]-coord[0][1]));

         for (j=start[1], current[1]=pt[1], i=j*scol; j<=end[1]; j++, current[1]+=d[1], i+=scol)
            for (k=start[0], current[0]=pt[0], vtr=i+k; k<=end[0]; k++, current[0]+=d[0], vtr++)
                copyarray2(pointer[vtr], current);

         l = end[1]+start[1]-1;
         for (j=start[1], i=j*scol; j<end[1]; j++, i+=scol)
            for (k=start[0], vtr=i+k, m=vtr+scol+1, patch=(l-j)*pcol; k<end[0]; k++, vtr++, m++) {

               tob->data[patch+k].setup(2);
               tob->data[patch+k].ob = ptr;
               tob->data[patch+k].uv[0][0] = pointer[vtr][0];
               tob->data[patch+k].uv[0][1] = pointer[m][1];
               tob->data[patch+k].uv[1][0] = pointer[m][0];
               tob->data[patch+k].uv[1][1] = pointer[vtr][1];
            }

      }

   }

   fclose(infile);
   delete [] pointer;
   return 1;
}


/* *************************************************************
   This function reads in the surface patch data
************************************************************* */
int spatch::read_data(char filename[], datatype **dptr) {

   int i;

   for (dob=(spatchtype *)*dptr; dob!=(spatchtype *)NULL; dob=(spatchtype *)dob->next)
      if (!strcmp(filename, dob->dataname))
         return (dob->vrow-3)*(dob->vcol-3);

   dob = new spatchtype;

   if ((i=dob->read_data(filename)) < 1) {
      delete dob;
      dob = (spatchtype *)NULL;
      return 0;
   }

   dob->next = *dptr;
   *dptr     = dob;
   return i;
}
