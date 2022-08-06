


#include "convert.h"
#include "global.h"
#include "texture.h"


#define MAXBUFLEN	20000


/* *************************************************************
	read a surface's color - .mtl files
************************************************************* */
void wavefront::read_obj_material(char *token, int *countid) {

   FILE *infile;
   base_shade *mptr;
   int errorflag;

   if (!(infile = fopen(token, "r")))
      return;

   while ((errorflag = get_token(infile, token)))

      switch (token[0]) {

         case '\\':
            break;

         case 0:				// comment
         case '#':
         case '!':
         case '$':
            while ((errorflag=fgetc(infile)) != '\n' && errorflag != EOF);

            break;

         case 'K':
            if (!strcmp(token, "Ka")) {		// ambient
               get_token(infile, token);
               mptr->ambient[0] = atof(token);
               get_token(infile, token);
               mptr->ambient[1] = atof(token);
               get_token(infile, token);
               mptr->ambient[2] = atof(token);

               break;
            }

            if (!strcmp(token, "Kd")) {		// diffuse
               get_token(infile, token);
               mptr->diffuse[0] = atof(token);
               get_token(infile, token);
               mptr->diffuse[1] = atof(token);
               get_token(infile, token);
               mptr->diffuse[2] = atof(token);

               break;
            }

            if (!strcmp(token, "Ks")) {		// specular
               get_token(infile, token);
               mptr->specular[0] = atof(token);
               get_token(infile, token);
               mptr->specular[1] = atof(token);
               get_token(infile, token);
               mptr->specular[2] = atof(token);

               break;
            }

            break;

         case 'm':	// map_Kd - might be a later replacement for usemap.... ???
             break;

         case 'n':				// new color definition
            if (!strcmp(token, "newmtl")) {
               get_token(infile, token);

               mptr = new base_shade;
               mptr->next = surface_list;
               surface_list = mptr;
               mptr->id = *countid;
               (*countid)++;
               strcpy(mptr->name, token);

               break;
            }

            break;

         case 'N':
            if (!strcmp(token, "Ns")) {		// specular exponent - shinyness
               get_token(infile, token);
               mptr->specn = atof(token);

               break;
            }

            break;

         case 'r':	// refl
             break;

         default:	// map_Ka, map_Ks, map_d, sharpness, illum, d, vp
            break;
      }

   fclose(infile);
}


/* *************************************************************
	read a wavefront .obj file
************************************************************* */
int wavefront::read_data(char *filename) {

   FILE *infile;
   char buffer[MAXBUFLEN], token[MAXSTRLEN], texname[MAXSTRLEN];
   float texdim[2];

   vertexdata *vtlist = (vertexdata *)NULL;

   int texcount = 0;
   int vercount = 0;
   int errorflag, texflag;
   vector3f point;
   char *cptr, *sptr;

   int i, j, k;
   base_shade *mtr = (base_shade *)NULL;
   charlist *clist = (charlist *)NULL;
   charlist *fptr;
   objectlist *otr;
   texture tob;
   int countid = 1;

   if (!(infile=fopen(filename, "r"))) {
      printf("Cannot open %s.  Aborting...\n", filename);
      exit(1);
   }

   printf("Reading...\n");

   texcount = vercount = 0;

   // the following while loop just counts the # of vertices
   while ((errorflag = get_token(infile, token)))

      switch (token[0]) {

         case '\\':
            break;

         case 0:				// comment
         case '#':
         case '!':
         case '$':
            while ((errorflag=fgetc(infile)) != '\n' && errorflag != EOF);

            break;

         case 'f':
            if (!strcmp(token, "f") || !strcmp(token, "fo")) {	// polygon face

               fgets(buffer, MAXBUFLEN, infile);

               break;
            }

            break;

         case 'g':					// group?
            break;

         case 'm':
            if (!strcmp(token, "mtllib")) {
               fscanf(infile, "%s", token);

               break;
            }

            break;

         case 'u':
            if (!strcmp(token, "usemtl")) {
               fscanf(infile, "%s", token);

               break;
            }

            if (!strcmp(token, "usemap")) {
               fscanf(infile, "%s", texname);

               break;
            }

            break;
 
         case 'v':

            if (!strcmp(token, "v")) {		// vertex

               vercount++;

               get_tokena(infile, token);
               get_tokena(infile, token);
               get_tokena(infile, token);

               break;
            }

            if (!strcmp(token, "vn")) {		// vertex normal
               get_tokena(infile, token);
               get_tokena(infile, token);
               get_tokena(infile, token);

               break;
            }

            if (!strcmp(token, "vt")) {		// vertex texture (0..1)
               texcount++;

               get_tokena(infile, token);
               get_tokena(infile, token);

               break;
            }

            break;

         default:	// bevel bmat bsp bzp c_interp cdc con cstype ctech curv curv2 d_interp deg end hole l lod maplib mg o p param res s scrv shadow_obj sp stech step surf trace_obj trim usemap vp
            break;
      }

   rewind(infile);

   // allocate temporary vertex/texture mem

   if (vercount > texcount)
      vtlist = new vertexdata[vercount];
   else
      vtlist = new vertexdata[texcount];

   vercount = texcount = 0;

   vertexcount = edgecount = polycount = 0;
   texname[0] = 0;

   // check if there is a .mtl file of the same name, and process it

   if (!strcmp(&filename[strlen(filename)-4], ".obj")) {
      strcpy(buffer, filename);
      strcpy(&buffer[strlen(buffer)-4], ".mtl");

      clist = new charlist;
      strcpy(clist->x, buffer);

      read_obj_material(buffer, &countid);
   }

   while ((errorflag = get_token(infile, token)))

      switch (token[0]) {

         case '\\':
            break;

         case 0:				// comment to eoln
         case '#':
         case '!':
         case '$':
            while ((errorflag=fgetc(infile)) != '\n' && errorflag != EOF);

            break;

         case 'f':
            if (!strcmp(token, "f") || !strcmp(token, "fo")) {	// polygon face

               fgets(buffer, MAXBUFLEN, infile);
               cptr = buffer;

               // count # of vertices in face
               for (texflag = j = 0; (errorflag = sscanf(cptr, "%s%n", token, &i)) && errorflag != EOF; j++) {
                  cptr += i;

                  if ((sptr=strchr(token, '/')) != NULL && (k = (int)strtol(sptr+1, NULL, 10)))
                     texflag = 1;
               }

               otr = new objectlist;
               polycount++;
               otr->id = polycount;
               otr->rnflag = reverse_normal;
               otr->vptr = new vertexdata[j];

               if (texflag)
                  strcpy(otr->texname, texname);

               otr->lw = mtr;
               otr->lw->count++;

               otr->polynum = j;
               edgecount += j;

               if (ob == (objectlist *)NULL)
                  ob = otr;
               else
                  ob->insert_id(otr);

               otr->vptr = new vertexdata[j];

               cptr = buffer;

               // process vertex indices,
               // format: f v/t/n v/t/n v/t/n .....
               for (i=j-1; i>-1; i--) {
                 sscanf(cptr, "%s%n", token, &k);
                 cptr += k;
                  k = (int)strtol(token, NULL, 10) - 1;

                  otr->vptr[i].index = vtlist[k].index;

                  if (texflag && (sptr=strchr(token, '/')) != NULL && (k = (int)strtol(sptr+1, NULL, 10))) {
                     k--;
                     otr->vptr[i].t[0] = texdim[0]*vtlist[k].t[0];
                     otr->vptr[i].t[1] = texdim[1]*vtlist[k].t[1];
                  }

               }

               break;
            }

            break;

         case 'g':					// group?
            break;

         case 'm':
            if (!strcmp(token, "mtllib")) {		// .mtl file
               fscanf(infile, "%s", token);		// format: "mtllib <.mtl>"
               for (fptr = clist; fptr != (charlist *)NULL && strcmp(fptr->x, token); fptr = fptr->next);

               if (fptr != (charlist *)NULL)
                  break;

               fptr = new charlist;
               strcpy(fptr->x, token);
               fptr->next = clist;
               clist = fptr;

               read_obj_material(token, &countid);

               break;
            }

            break;

         case 'u':
            if (!strcmp(token, "usemtl")) {		// set current colors
               fscanf(infile, "%s", token);		// format: "usemtl <surface name>"

               if (surface_list != (base_shade *)NULL)
                  mtr = (base_shade *)surface_list->find(token);
               else
                  mtr = (base_shade *)NULL;

               if (mtr == (base_shade *)NULL) {
                  printf("Error... Cannot find material %s... Aborting...\n", token);
                  fclose(infile);
                  exit(1);
               }

               break;
            }

            if (!strcmp(token, "usemap")) {		// set current texture map
               fscanf(infile, "%s", texname);		// format: "usemap <texname>"

               if (!tob.read_texture(texname,1)) {
                  printf("Warning: Cannot access texture %s -> therefore cannot scale properly...\n", texname);
                  texdim[0] = texdim[1] = 1.0;
               }

               else {
                  texdim[0] = tob.maxx - 1;
                  texdim[1] = tob.maxy - 1;
               }

               break;
            }

            break;
 
         case 'v':

            if (!strcmp(token, "v")) {			// vertex
							// format: "v <x> <y> <z>"
               get_tokena(infile, token);
               point[0] = atof(token);
               get_tokena(infile, token);
               point[1] = atof(token);
               get_tokena(infile, token);
               point[2] = atof(token);

               if (head == (vertexlist *)NULL) {
                  head = new vertexlist;
                  copyarray3(head->pt, point);
                  vtlist[vercount].index = head->id = vertexcount =  1;
               }

               else
                  vtlist[vercount].index = head->insert(point, &vertexcount);

               vercount++;

               break;
            }

            if (!strcmp(token, "vn")) {			// vertex normal
               get_tokena(infile, token);		// format "vn <dx> <dy> <dz>"
               get_tokena(infile, token);
               get_tokena(infile, token);
               break;
            }

            if (!strcmp(token, "vt")) {			// vertex texture (0..1)
               get_tokena(infile, token);		// format "vt <tx> <ty>"
               vtlist[texcount].t[0] = atof(token);

               get_tokena(infile, token);
               vtlist[texcount].t[1] = atof(token);

               texcount++;
               break;
            }

            break;

         default:	// bevel bmat bsp bzp c_interp cdc con cstype ctech curv curv2 d_interp deg end hole l lod maplib mg o p param res s scrv shadow_obj sp stech step surf trace_obj trim usemap vp
            break;
      }

   fclose(infile);

   if (vtlist != (vertexdata *)NULL)
      delete [] vtlist;

   return 1;
}
