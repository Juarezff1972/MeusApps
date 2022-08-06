

#include "convert.h"
#include "global.h"

int reverse_normal = 0;

int get_tokena(FILE *infile, char *token) {

   int errorflag;

   while (1) {
      if (!(errorflag = get_token(infile, token)))
         return 0;

      if (token[0] != '%')
         return errorflag;
   }

}


int get_ca(FILE *infile, char x) {

   int errorflag;

   while (1) {
      if ((errorflag = fgetc(infile)) == EOF || errorflag == x)
         break;

      if (errorflag == '%')
         while (1)
            if ((errorflag = fgetc(infile)) == EOF || errorflag == '\n')
               break;
   }

   return errorflag;
}


// ********************************************************************


void base_shade::write_data(FILE *outfile) {

   fprintf(outfile, "KA %f %f %f\n", ambient[0], ambient[1], ambient[2]);
   fprintf(outfile, "KP %f %f %f\n", diffuse[0], diffuse[1], diffuse[2]);
   fprintf(outfile, "KS %f %f %f\n", specular[0], specular[1], specular[2]);
   fprintf(outfile, "SPECN %f\n", specn);
   fprintf(outfile, "LUM %f %f %f\n\n", luminate[0], luminate[1], luminate[2]);
}


void base_shade::write_notes(FILE *outfile) {

   if (trans > 0.0)
      fprintf(outfile, "   TRANSPARENCY - %f\n", trans);
}


// ********************************************************************


list *list::get_node() {

   list *x;

   if (back != (list *)NULL) {
      x = back->get_node();
      if (x==back)
         back = (list *)NULL;
      return x;
   }

   if (next != (list *)NULL) {
      x = next->get_node();
      if (x==next)
         next = (list *)NULL;
      return x;
   }

   return this;
}


list *list::find_id(int x) {

   list *y;

   if (id == x)
      return this;

   if (back != (list *)NULL) {
      y = back->find_id(x);
      if (y != (list *)NULL)
         return y;
   }

   if (next != (list *)NULL) {
      y = next->find_id(x);
      if (y != (list *)NULL)
         return y;
   }

   return (list *)NULL;
}


void list::insert_id(list *x) {

   if (x->id == id)
      delete x;
   else if (x->id < id)
      if (back == (list *)NULL)
         back = x;
      else
         back->insert_id(x);
   else
      if (next == (list *)NULL)
         next = x;
      else
         next->insert_id(x);
}


// ********************************************************************


int vertexlist::insert(float *point, int *vertexcount) {

   int flag;

   if (abs(point[0] - pt[0]) < CORRECT)
      if (abs(point[1] - pt[1]) < CORRECT)
         if (abs(point[2] - pt[2]) < CORRECT)
            return id;
         else
            flag = (point[2] < pt[2]);
      else
         flag = (point[1] < pt[1]);
   else
      flag = (point[0] < pt[0]);

   if (flag) {
      if (back == (list *)NULL) {
         back = new vertexlist;

         (*vertexcount)++;
         back->id = *vertexcount;
         copyarray3(((vertexlist *)back)->pt, point);
         return back->id;
      }

      return ((vertexlist *)back)->insert(point, vertexcount);
   }

   if (next == (list *)NULL) {
      next = new vertexlist;
      (*vertexcount)++;
      next->id = *vertexcount;
      copyarray3(((vertexlist *)next)->pt, point);
      return next->id;
   }

   return ((vertexlist *)next)->insert(point, vertexcount);
}


void vertexlist::write_data(FILE *outfile) {

   if (back != (list *)NULL)
      back->write_data(outfile);

   fprintf(outfile, "%f %f %f\n", pt[0], pt[1], pt[2]);

   if (next != (list *)NULL)
      next->write_data(outfile);
}


// ********************************************************************


void objectlist::adjust_totals(int *polycount, int *edgecount) {

   if (back != (list *)NULL)
      ((objectlist *)back)->adjust_totals(polycount, edgecount);

   if (lw != (lw_shade *)NULL && lw->flag_dbl_side)
      flag_dbl_side = 1;

   if (flag_dbl_side) {
      (*polycount)++;
      (*edgecount) += polynum;
      other_id = *polycount;
      if (lw != (lw_shade *)NULL)
         lw->count++;
   }

   if (next != (list *)NULL)
      ((objectlist *)next)->adjust_totals(polycount, edgecount);
}


void objectlist::tex_count(int *count) {

   if (back != (list *)NULL)
      ((objectlist *)back)->tex_count(count);

   if (texname[0]) {
      (*count) += 1;

      if (flag_dbl_side)
         (*count) += 1;
   }

   if (next != (list *)NULL)
      ((objectlist *)next)->tex_count(count);
}


void objectlist::write_dbl_side(FILE *outfile) {

   int i;

   if (back != (list *)NULL)
      ((objectlist *)back)->write_dbl_side(outfile);

   if (flag_dbl_side) {
      fprintf(outfile, "%d ", polynum);

      if (!rnflag)
         for (i=polynum-1; i>-1; i--)
            fprintf(outfile, "%d ", vptr[i].index);
      else
         for (i=0; i<polynum; i++)
            fprintf(outfile, "%d ", vptr[i].index);

      fprintf(outfile, "\n");
   }

   if (next != (list *)NULL)
      ((objectlist *)next)->write_dbl_side(outfile);
}


void objectlist::write_ilm(FILE *outfile, int base) {

   if (back != (list *)NULL)
      ((objectlist *)back)->write_ilm(outfile, base);

   if (lw != (lw_shade *)NULL && base != lw->id) {
      fprintf(outfile, "# %d\n", id);

      lw->write_data(outfile);

      if (flag_dbl_side) {
         fprintf(outfile, "# %d\n", other_id);
         lw->write_data(outfile);
      }

   }

   if (next != (list *)NULL)
      ((objectlist *)next)->write_ilm(outfile, base);
}


void objectlist::write_tex(FILE *outfile) {

   int i;

   if (back != (list *)NULL)
      ((objectlist *)back)->write_tex(outfile);

   if (texname[0]) {
      fprintf(outfile, "%d COORD\n", id);

      if (!rnflag)
         for (i=0; i<polynum; i++)
            fprintf(outfile, "\t%f %f\n", vptr[i].t[0], vptr[i].t[1]);
      else
         for (i=polynum-1; i>-1; i--)
            fprintf(outfile, "\t%f %f\n", vptr[i].t[0], vptr[i].t[1]);

      fprintf(outfile, "\t%s\n", texname);

      if (flag_dbl_side) {
         fprintf(outfile, "%d COORD\n", other_id);

         if (!rnflag)
            for (i=polynum-1; i>-1; i--)
               fprintf(outfile, "\t%f %f\n", vptr[i].t[0], vptr[i].t[1]);
         else
            for (i=0; i<polynum; i++)
               fprintf(outfile, "\t%f %f\n", vptr[i].t[0], vptr[i].t[1]);

         fprintf(outfile, "\t%s\n", texname);
      }

   }

   if (next != (list *)NULL)
      ((objectlist *)next)->write_tex(outfile);
}


void objectlist::write_notes(FILE *outfile) {

   if (back != (list *)NULL)
      ((objectlist *)back)->write_notes(outfile);

   if (lw != (lw_shade *)NULL)
      if (lw->trans > 0.0) {
         fprintf(outfile, "# %d\n", id);
         lw->write_notes(outfile);

         if (flag_dbl_side) {
            fprintf(outfile, "# %d\n", other_id);
            lw->write_notes(outfile);
         }

      }

   if (next != (list *)NULL)
      ((objectlist *)next)->write_notes(outfile);
}


void objectlist::write_data(FILE *outfile) {

   int i;

   if (back != (list *)NULL)
      back->write_data(outfile);

   fprintf(outfile, "%d ", polynum);

   if (!rnflag)
      for (i=0; i<polynum; i++)
         fprintf(outfile, "%d ", vptr[i].index);
   else
      for (i=polynum-1; i>-1; i--)
         fprintf(outfile, "%d ", vptr[i].index);

   fprintf(outfile, "\n");

   if (next != (list *)NULL)
      next->write_data(outfile);
}


// ********************************************************************


int converter::read_ply(char *filename) {

   int i, j;

   objectlist *otr;

   vector3f point;

   FILE *infile;

   if (!(infile=fopen(filename, "r"))) {
      printf("Cannot open %s.  Aborting...\n", filename);
      exit(1);
   }

   polycount = 0;
   vertexcount = 0;
   edgecount = 0;

   printf("Reading...\n");

   while(1) {
      if (fscanf(infile, "%ld", &i) == EOF)
         break;

      polycount++;
      edgecount += i;

      otr = new objectlist;
      otr->id = polycount;
      otr->rnflag = reverse_normal;

      if (ob == (objectlist *)NULL)
         ob = otr;
      else
         ob->insert_id(otr);

      otr->vptr = new vertexdata[i];
      otr->polynum = i;

      for (j=0; j<i; j++) {

         fscanf(infile, "%f %f %f", &point[0], &point[1], &point[2]);

         if (head == (vertexlist *)NULL) {
            head = new vertexlist;
            copyarray3(head->pt, point);
            head->id = vertexcount = otr->vptr[i-1].index = 1;
         }

         else
            otr->vptr[i-(1+j)].index = head->insert(point, &vertexcount);
      }

   }

   fclose(infile);
   return 1;
}


/* ***********************************************************************

Ok this is the BYU format (well at least this is what I think is the
        BYU format.)

Line 1 has the number of parts, number of vertices, number of polygons and
        number of elements in the connectivity array.

Line 2 has the start polygon number and end polygon number for part 1
Line 3 has the start polygon number and end polygon number for part 2
.
.
.

Line 18 has the x y z coordinates for vertex 1 and vertex 2
Line 19 has the x y z coordinates for vertex 3 and vertex 4
.
.
.
Line 12715 has the connectivity array for the first polygon
        example:
                1 2 3 -4
        vertex 1 to vertex 2 to vertex 3 to vertex 4 and back to vertex 1

        a negative number show the last vertex of a polygon

        source to read in BYU format
        written by Kalvin Quinkert kalvin@uswest.com
        sometime between January 1992 and May 1992
        modified by Alex Harper March 1995
*********************************************************************** */

int converter::read_byu(char *filename) {

   int i, j, k;
   int partcount, temp1, temp2;
   objectlist *otr;
   intlist *ltr, *lltr;
   FILE *infile;
   vector3f point;
   int *index;
   int vcount;

   if (!(infile=fopen(filename, "r"))) {
      printf("Cannot open %s.  Aborting...\n", filename);
      exit(1);
   }

   printf("Reading...\n");

   fscanf(infile, "%ld %ld %ld %ld", &partcount, &vcount, &polycount, &edgecount);

   index = new int[vcount+1];

   for (i=0;i<partcount;i++)
      fscanf(infile, "%ld %ld\n", &temp1, &temp2);

   vertexcount = 0;

   for (i=0; i < vcount; i++) {
      fscanf(infile, "%f %f %f", &point[0], &point[1], &point[2]);

      if (head == (vertexlist *)NULL) {

         head = new vertexlist;
         copyarray3(head->pt, point);
         head->id = vertexcount = index[1] = 1;
      }

      else
         index[i+1] = head->insert(point, &vertexcount);
   }

                                        // reverse vertex list
   ltr = (intlist *)NULL;

   for(i=0; i<polycount; i++) {
      j = 0;

      while (1) {
         j++;
         lltr = new intlist;
         fscanf(infile, "%ld", &lltr->x);
         lltr->next = ltr;
         ltr = lltr;

         if (lltr->x < 0) {
            lltr->x = -lltr->x;
            break;
         }

      }

      otr = new objectlist;
      otr->id = i;
      otr->rnflag = reverse_normal;

      if (ob == (list *)NULL)
         ob = otr;
      else
         ob->insert_id(otr);

      otr->vptr = new vertexdata[j];
      otr->polynum   = j;

      for (k=j-1; k>-1; k--) {
         otr->vptr[k].index = index[ltr->x];
         lltr = ltr;
         ltr = ltr->next;
         delete lltr;
      }

   }

   delete [] index;
   return 1;
}


int converter::read_slide(char *filename) {

   int i, j;
   objectlist *otr, *qtr;
   vector3f *point;
   vector4f vector;
   FILE *infile;

   if (!(infile=fopen(filename, "r"))) {
      printf("Cannot open %s.  Aborting...\n", filename);
      exit(1);
   }

   printf("Reading...\n");

   fscanf(infile, "%f %f %f %f", &vector[0], &vector[1], &vector[2], &vector[3]); // dx dy dz h

   normalize3(vector);
   smultarray3(vector, vector[3]);

   fscanf(infile, "%d", &j);		// read vertex

   if (j < 2) {
      printf("Sliders must have at least 2 vertices... Aborting...\n");
      fclose(infile);
      exit(1);
   }

   polycount = 2 + j;
   edgecount = 4*j + 2*j;

   otr = new objectlist;
   otr->id = 1;
   otr->rnflag = reverse_normal;
   otr->vptr = new vertexdata[j];
   otr->polynum = j;
   ob = otr;

   point = new vector3f[j];

   for (i=0; i<j; i++) {
      fscanf(infile, "%f %f %f", &point[i][0], &point[i][1], &point[i][2]);

      if (head == (vertexlist *)NULL) {
         head = new vertexlist;
         copyarray3(head->pt, point[i]);
         head->id = vertexcount = ob->vptr[i].index = 1;
      }

      else
         ob->vptr[i].index = head->insert(point[i], &vertexcount);
   }

   qtr = new objectlist;
   qtr->id = 2;
   qtr->rnflag = !(reverse_normal);
   qtr->vptr = new vertexdata[j];
   qtr->polynum = j;
   ob->insert_id(qtr);

   for (i=0; i<j; i++) {
      addarray3(point[i], vector);
      qtr->vptr[j-i-1].index = head->insert(point[i], &vertexcount);
   }

   delete [] point;

   for (i=1; i<j; i++) {
      otr = new objectlist;
      otr->id = i+2;
      otr->rnflag = reverse_normal;
      otr->vptr = new vertexdata[4];
      otr->polynum = 4;
      ob->insert_id(otr);

      otr->vptr[0].index = i;
      otr->vptr[1].index = i + j;
      otr->vptr[2].index = i+j+1;
      otr->vptr[3].index = i+1;
   }

   otr = new objectlist;
   otr->id = j+2;
   otr->rnflag = reverse_normal;
   otr->vptr = new vertexdata[4];
   otr->polynum = 4;
   ob->insert_id(otr);

   otr->vptr[0].index = j;
   otr->vptr[1].index = vertexcount;
   otr->vptr[2].index = j+1;
   otr->vptr[3].index = 1;

   fclose(infile);
   return 1;
}


int converter::write_spg(char *filename, int sflag) {

   list *qtr, *ptr;
   FILE *outfile;

   if (!vertexcount || !polycount || !edgecount || head == (list *)NULL || ob == (list *)NULL) {
      printf("NULL Object... Aborting...\n");
      exit(1);
   }

   if (sflag) {
      qtr = head;
      head = (vertexlist *)NULL;
      head = (vertexlist *)qtr->get_node();

      if (head != qtr)			// sort on id not number
         while (qtr != (list *)NULL) {
            ptr = qtr->get_node();
            if (ptr == qtr)
               qtr = (list *)NULL;
            head->insert_id(ptr);
         }

   }

   if (!(outfile=fopen(filename, "w"))) {
      printf("Cannot open %s.  Aborting...\n", filename);
      exit(1);
   }

   printf("Writing .spg ...\n");

   ob->adjust_totals(&polycount, &edgecount);

   fprintf(outfile, "%d %d %d\n", vertexcount, polycount, edgecount);

   head->write_data(outfile);
   ob->write_data(outfile);
   ob->write_dbl_side(outfile);

   fclose(outfile);
   return 1;
}


int converter::write_ilm(char *filename) {

   FILE *outfile;
   base_shade *base, *ptr;

   if (!(outfile=fopen(filename, "w"))) {
      printf("Cannot open %s.  Aborting...\n", filename);
      exit(1);
   }

   printf("Writing .ilm ...\n");

   base = surface_list;

   if (base == (base_shade *)NULL)
      return 0;

   base->post_read();
   for (ptr=surface_list->next; ptr != (base_shade *)NULL; ptr = ptr->next) {
      ptr->post_read();
      if (base->count < ptr->count)
         base = ptr;
   }

   base->write_data(outfile);

   fprintf(outfile, "no_sides %d\n\n", polycount-base->count);
   ob->write_ilm(outfile, base->id);

   fclose(outfile);
   return 1;
}


int converter::write_tex(char *filename) {

   FILE *outfile;
   int count = 0;

   ob->tex_count(&count);

   if (!count)
      return 0;

   if (!(outfile=fopen(filename, "w"))) {
      printf("Cannot open %s.  Aborting...\n", filename);
      exit(1);
   }

   printf("Writing .tex ...\n");

   fprintf(outfile, "MONO %d\n", count);

   ob->write_tex(outfile);

   fclose(outfile);
   return 1;
}


int converter::write_notes(char *filename) {

   FILE *outfile;

   if (!(outfile=fopen(filename, "w"))) {
      printf("Cannot open %s.  Aborting...\n", filename);
      exit(1);
   }

   printf("Writing...\n");

   ob->write_notes(outfile);

   fclose(outfile);
   return 1;
}


/*
int converter::read_symbol2d(FILE *infile, char *buffer) {

   int i, j;

   list *qtr;
   float point[3];
   float3dlist *fhead, *ptr;

   polycount = 0;
   vertexcount = 0;
   edgecount = 0;

   printf("Reading 2D...\n");

   fscanf(infile, "%s", buffer);

   while(strcmp(buffer, "3DFACET") ) {

      fhead = (float3dlist *)NULL;
      ptr = 
      


      polycount++;
      edgecount += i;

      qtr = new list;
      qtr->next = ob;
      ob = qtr;

      ob->vptr = new vertexdata[i];
      ob->polynum   = i;

      for (j=0; j<i; j++) {

         fscanf(infile, "%f %f %f", &point[0], &point[1], &point[2]);

         if (head == (list *)NULL) {

            head = new list;
            copyarray3(head->pt, point);
            head->id = vertexcount = ob->vptr[i-1].index = 1;
         }

         else
            ob->vptr[i-(1+j)].index = head->insert(point, &vertexcount);
      }

   }

   fclose(infile);
   return 1;
}


void extract_symbol(char *filename) {

   converter *x;
   FILE *infile;
   char buffer[1000], buffer2[256], name[256];

   if (!(infile = fopen(argv[1], "r"))) {
      printf("Could not access %s... Aborting...\n", argv[1]);
      exit(0);
   }

   fscanf(infile, "%s", buffer);
   if (strcmp(buffer, "EXECUTE")) {
      printf("%s is not a symbol file... Aborting...\n", argv[1]);
      fclose(infile);
      exit(0);
   }

   do {
      if (fscanf(infile, "%s", buffer) == EOF)
         break;
   } while (strcmp(buffer, "SYMBOL"));

   while (1) {
      if (!strcmp(buffer, "SYMBOL")) {
         fscanf(infile, "%s", name);
         fscanf(infile, "%s", buffer);
      }

      else
         if (!strcmp(buffer2, "END"))
            break;
         else
            if (!strcmp(buffer2, "2DFACET")) {
               x = new converter;
               x->read_symbol2d(infile, buffer);
               sprintf(buffer2, "%s.2d", name);
               x->write_spg(buffer2, 1);
               delete x;
            }

            else
               if (!strcmp(buffer2, "3DFACET")) {
                  x = new converter;
                  x->read_symbol3d(infile, buffer);
                  sprintf(buffer2, "%s.2d", name);
                  x->write_spg(buffer2, 1);
                  delete x;
               }

               else
                  fscanf(infile, "%s", buffer);
   }

   fclose(infile);
}
*/


void main(int argc, char **argv) {

   converter x;
   converter *z;
   char buffer[MAXSTRLEN];

   if (argc < 4) {
      printf("usage: convert <source type> <fromfilename> <tofilename> <-r>\n");
      printf("\t\twhere <source type> is of:\n");
      printf("\t\t{\"lw\", \"obj\", \"dxf\", \"ply\", \"byu\", \"dive\", \"slide\", \"sor\", or \"mirror\"}\n");

      exit(1);
   }

   if (argc == 5)
      reverse_normal = 1;

   if (!strcmp(argv[1], "ply")) {
      x.read_ply(argv[2]);

      strcpy(buffer, argv[3]);
      strcat(buffer, ".spg");
      x.write_spg(buffer, 1);

      return;
   }

   if (!strcmp(argv[1], "byu")) {
      x.read_byu(argv[2]);

      strcpy(buffer, argv[3]);
      strcat(buffer, ".spg");
      x.write_spg(buffer, 1);

      return;
   }

   if (!strcmp(argv[1], "lw")) {
      z = new lightwave;
      z->read_data(argv[2]);

      strcpy(buffer, argv[3]);
      strcat(buffer, ".spg");
      z->write_spg(buffer, 1);

      strcpy(buffer, argv[3]);
      strcat(buffer, ".ilm");
      z->write_ilm(buffer);

      strcpy(buffer, argv[3]);
      strcat(buffer, ".nts");
      z->write_notes(buffer);

      delete z;
      return;
   }

   if (!strcmp(argv[1], "dxf")) {
      z = new dxf;
      z->read_data(argv[2]);

      strcpy(buffer, argv[3]);
      strcat(buffer, ".spg");
      z->write_spg(buffer, 1);

      strcpy(buffer, argv[3]);
      strcat(buffer, ".ilm");
      z->write_ilm(buffer);

      delete z;
      return;
   }

   if (!strcmp(argv[1], "dive")) {

      z = new dive;
      z->read_data(argv[2]);

      strcpy(buffer, argv[3]);
      strcat(buffer, ".spg");
      z->write_spg(buffer, 1);

      strcpy(buffer, argv[3]);
      strcat(buffer, ".ilm");
      z->write_ilm(buffer);

      strcpy(buffer, argv[3]);
      strcat(buffer, ".tex");
      z->write_tex(buffer);

      delete z;
      return;
   }

   if (!strcmp(argv[1], "obj")) {

      z = new wavefront;
      z->read_data(argv[2]);

      strcpy(buffer, argv[3]);
      strcat(buffer, ".spg");
      z->write_spg(buffer, 1);

      strcpy(buffer, argv[3]);
      strcat(buffer, ".ilm");
      z->write_ilm(buffer);

      strcpy(buffer, argv[3]);
      strcat(buffer, ".tex");
      z->write_tex(buffer);

      delete z;
      return;
   }

   if (!strcmp(argv[1], "slide")) {
      x.read_slide(argv[2]);

      strcpy(buffer, argv[3]);
      strcat(buffer, ".spg");
      x.write_spg(buffer, 1);
      return;
   }


   if (!strcmp(argv[1], "mirror")) {

printf("Mirror converter currently not implemented...\n");
      return;
   }

   if (!strcmp(argv[1], "sor")) {

printf("Surface Of Revolution converter currently not implemented...\n");
      return;
   }



/*
  if (!strcmp(argv[1], "symbol")) {
      extract_symbol(argv[2]);
      return;
   }
*/

   printf("usage: convert <source> <type> <fromfilename> <tofilename> <-r>\n");
   printf("\t\twhere <source type> is of:\n");
   printf("\t\t{\"lw\", \"obj\", \"dxf\", \"ply\", \"byu\", \"dive\", \"slide\", \"sor\", or \"mirror\"}\n");

   return;
}



