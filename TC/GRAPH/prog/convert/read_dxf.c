



#include "convert.h"

    /* AutoCAD color table */
static float DXFcolors[][3] =    {
       {0.0f,  0.0f,  0.0f},	/* 0 == black   */
       {1.0f,  0.0f,  0.0f},	/* 1 == red     */
       {1.0f,  1.0f,  0.0f},	/* 2 == yellow  */
       {0.0f,  1.0f,  0.0f},	/* 3 == green   */
       {0.0f,  1.0f,  1.0f},	/* 4 == cyan    */
       {0.0f,  0.0f,  1.0f},	/* 5 == blue    */
       {1.0f,  0.0f,  1.0f},	/* 6 == magenta */
       {1.0f,  1.0f,  1.0f},	/* 7 == white   */
       {0.5f,  0.5f,  0.5f},	/* 8 == dk grey */
       {0.75f, 0.75f, 0.75f} 	/* 9 == lt grey */
};


dxf::dxf() {

   int i;
   base_shade **ptr, *qtr;;

   color = 7;
   obname[0] = 0;
   ptr = &surface_list;

   for (i=0; i < 10; i++) {
      floats[i] = angles[i] = 0.0;
      ints[i] = 0;
      vertex[i][0] = vertex[i][1] = vertex[i][2] = 0;

      qtr = new base_shade;

      copyarray3(qtr->ambient, DXFcolors[i]);

      copyarray3(qtr->diffuse, DXFcolors[i]);
      smultarray3(qtr->diffuse, 0.7);

      copyarray3(qtr->specular, DXFcolors[i]);
      smultarray3(qtr->specular, 0.3);

      *ptr = qtr;
      ptr = &qtr->next;
   }

   vstack = (vertexlist *)NULL;
   vcount = 0;
}


dxf::~dxf() {

   if (vstack != (vertexlist *)NULL)
      delete vstack;
}


int dxf::query_degenerate(int a, int b, int c) {

   return (similar3(vertex[a], vertex[b]) ||
           similar3(vertex[a], vertex[c]) ||
           similar3(vertex[b], vertex[c]));
}


void dxf::store_poly(int a, int b, int c) {

   objectlist *otr;
   int i;

   polycount++;
   edgecount += 3;

   otr = new objectlist;
   otr->id = polycount;
   otr->rnflag = reverse_normal;

   if (ob == (objectlist *)NULL)
      ob = otr;
   else
      ob->insert_id(otr);

   otr->vptr = new vertexdata[3];
   otr->polynum = 3;

   if (head == (vertexlist *)NULL) {
      head = new vertexlist;
      copyarray3(head->pt, vertex[a]);
      head->id = vertexcount = otr->vptr[0].index = 1;
   }

   else
      otr->vptr[0].index = head->insert(vertex[a], &vertexcount);

   otr->vptr[1].index = head->insert(vertex[b], &vertexcount);
   otr->vptr[2].index = head->insert(vertex[c], &vertexcount);

   if (color < 0 || color > 9)
      color = 7;

   for (i=0, otr->lw = surface_list; i < color; i++, otr->lw=otr->lw->next);

   otr->lw->count++;
}


void dxf::store_data() {

   if (strstr(obname, "TRACE") ||	// 2 back-to-back triangles
       strstr(obname, "SOLID") ||	// 1 or 2 triangles_  
       strstr(obname, "3DFACE")) {	// 1 or 2 triangles_  

      if (!query_degenerate(0, 1, 2))
         store_poly(2, 1, 0);

      if (!query_degenerate(0, 2, 3))
         store_poly(3, 2, 0);

      return;
   }

/*
   else if ( (strstr(obname, "LINE")) && (!strstr(curobj_,"POLYLINE")) ) {
		glBegin(GL_LINES);
			glVertex3f(xcoords_[0], ycoords_[0], zcoords_[0]);
			glVertex3f(xcoords_[1], ycoords_[1], zcoords_[1]);
		glEnd();
		return;
	}
	else if (strstr(curobj_, "POINT"))	
	{
		glVertex3f(xcoords_[0], ycoords_[0], zcoords_[0]);
		return;
	}
	else if (strstr(curobj_, "CIRCLE"))
	{
		return;
	}
	else if (strstr(curobj_, "ARC"))		// not implemented for now
	{
		return;
	}

   if (strstr(curobj_, "TEXT"))	// not implemented for now  
   if (strstr(curobj_, "SHAPE"))	// these look very hard  
   if (strstr(curobj_, "BLOCK"))	// these look very hard  
   if (strstr(curobj_, "ENDBLK"))	// these look very hard  
   if (strstr(curobj_, "INSERT"))	// these look very hard  
   if (strstr(curobj_, "ATTDEF"))	// not implemented for now  
   if (strstr(curobj_, "ATTRIB"))	// not implemented for now  

	else if (strstr(curobj_, "POLYLINE"))	// these look fairly hard  
	{
		if (pLineState_ & FLAG_SET)
		{
		    // groupcode 70 flags  
		    if (ints_[0] & 0x01) 	// 0x01 = is closed ?  
			glBegin(GL_POLYGON);
		    else
			glBegin(GL_LINE_LOOP);

		    pLineState_ &= ~FLAG_SET; // unset FLAG_SET  
		}
		else
			glBegin(GL_POLYGON);

		return;
	}
	else if (strstr(curobj_, "VERTEX"))	
	{
		glVertex3f(xcoords_[0], ycoords_[0], zcoords_[0]);
		return;
	}
	else if (strstr(curobj_, "SEQEND"))	// Polyline end 
	{
		withinPolyLine_ = FALSE;
		glEnd();
		return;
	}

	else if (strstr(curobj_, "3DLINE"))	
	{
		if (xcoords_[0] == xcoords_[1] && ycoords_[0] == ycoords_[1] && 
							zcoords_[0] == zcoords_[1])
		{
			degenerates_++;
			return;
		}
		glBegin(GL_LINES);
			glVertex3f(xcoords_[0], ycoords_[0], zcoords_[0]);
			glVertex3f(xcoords_[1], ycoords_[1], zcoords_[1]);
		glEnd();

		return;
	}


*/


}


int dxf::getline(FILE *infile, char *buffer, int *code) {	/* read a group code and the next line from FpIn_ */

   if (fgets(buffer, MAXSTRLEN, infile) == NULL)	/* get a line from .DXF */
      return 1;

   sscanf(buffer, "%3d", code);			// scan out group code

   if (fgets(buffer, MAXSTRLEN, infile) == NULL)	/* get a line from .DXF */
      return 1;

   return 0;
}


int dxf::read_data(char *filename) {

   char	buffer[MAXSTRLEN];
   FILE *infile;
   int errorflag = 0;
   int code;
   vertexlist *vtr;

   if (!(infile=fopen(filename, "r"))) {
        printf("Cannot open %s.  Aborting...\n", filename);
        exit(1);
   }

   vertexcount = edgecount = polycount = 0;

   printf("Reading DXF ...\n");

   while (!errorflag) {
      while (!errorflag) {	/* run file up to the "ENTITIES" section */

          if ((errorflag = getline(infile, buffer, &code)))		/* get a group code and a line */
             break;

          if (!code) {	/* file section mark */
             if ((errorflag = (strstr(buffer, "EOF") != NULL)))
                break;

             if (strstr(buffer, "SECTION")) {
                if ((errorflag = getline(infile, buffer, &code)))	/* get a group code and a line */
                   break;

                if (code == 2 && (strstr(buffer, "ENTITIES") || strstr(buffer, "OBJECTS")))
                   break;
             }

          }

      }

      while (!errorflag) {		/* scan ENTITIES section */
         if ((errorflag = getline(infile, buffer, &code)))	/* get a group code and a line */
            break;

         if (code <100) {

            if (code < 10)	/* cardinal group codes */

               switch(code) {
                  case 0: /* start of entity, table, file sep */
                     if (obname[0])
                        store_data();
                        

                     if ((errorflag = (strstr(buffer, "EOF") != NULL)))
                        break;

                     if (strstr(buffer, "ENDSEC"))
                        break;

                     obname[0] = 0; /* reset object */
                     color = 7;
                     strcpy(obname, buffer);	/* get new */

                     break;

/*
                  case 1:	// primary text value for entity (?)
                     break;

                  case 2:	// block name, attribute tag, etc 
                     break;

                  case 3:	// other names 
					strcpy(string_[5], linbuf_);	

					if (strstr(curobj_, "DICTIONARY"))
					{
						if (objState_ & NAME_SET)
						{}
						else
						{
							// clear list first
						}
						objState_ |= NAME_SET;
						strfix(tmpString, string_[5]);
					}
					break;
                  case 4:
                     break;

                  case 5:	// entity or OBJECT handle (hex string) 
					strcpy(string_[2], linbuf_);	
					if (strstr(curobj_, "POLYLINE"))
						pLineState_ |= HANDLE_SET;
					if (strstr(curobj_, "DICTIONARY"))
						objState_ |= HANDLE_SET;
					break;

                  case 6:	// line type name 
					strcpy(string_[0], linbuf_);	
					if (strstr(curobj_, "POLYLINE"))
						pLineState_ |= LTYPE_SET;

					break;

                  case 7:	// text style name 
                  case 8:	// layer name 
					strcpy(string_[1], linbuf_);
					if (strstr(curobj_, "POLYLINE"))
						pLineState_ |= LAYER_SET;
					break;
                  case 9:	// variable name ID (only in header)
					break;
*/

                  default:
                     break;
               }

            else if (code < 20) /* Some X coord */
               sscanf(buffer, "%f", &vertex[code-10][0]);
            else if (code < 30) /* Some Y coord */
               sscanf(buffer, "%f", &vertex[code-20][1]); 
            else if (code < 38) /* Some Z coord */
               sscanf(buffer, "%f", &vertex[code-30][2]);

            else if (code == 38) {	// entity elevation if nonzero
            }

            else if (code == 39) {	// entity thickness if nonzero
/*
			if (strstr(curobj_, "POLYLINE"))
			{
				sscanf(linbuf_, "%f", &(thick_));
				pLineState_ |= THICKNESS_SET;
			}
*/
            }

            else if (code < 49) /* misc floats_ */
               sscanf(buffer, "%f", &floats[code-40]);

            else if (code == 49) { /* repeated value groups */
            }

            else if (code < 60) /* misc angles_ */
               sscanf(buffer, "%f", &angles[code-50]);
            else if (code == 62) /* Color number */
               sscanf(buffer, "%6d", &color);

            else if (code < 66) { // ???
            }

            else if (code == 66) { /* "entities follow" flag */
            }

            else if (code < 70) { 
            }

            else if (code < 80) { /* misc ints_ */

               sscanf(buffer, "%d", &ints[code-70]);

		// the following is guesswork... AWH

               switch (ints[code-70]) {

                  case 128:		// read triangle indices into vertex list
                    if ((errorflag = getline(infile, buffer, &code)))
                       break;

                    sscanf(buffer, "%d", &ints[code-70]);

                    vtr = (vertexlist *)vstack->find_id(abs(ints[code-70]));
                    copyarray3(vertex[0], vtr->pt);

                    if ((errorflag = getline(infile, buffer, &code)))
                       break;

                    sscanf(buffer, "%d", &ints[code-70]);

                    vtr = (vertexlist *)vstack->find_id(abs(ints[code-70]));
                    copyarray3(vertex[1], vtr->pt);

                    if ((errorflag = getline(infile, buffer, &code)))
                       break;

                    sscanf(buffer, "%d", &ints[code-70]);

                    vtr = (vertexlist *)vstack->find_id(abs(ints[code-70]));
                    copyarray3(vertex[2], vtr->pt);

                    if (!query_degenerate(0, 1, 2))
                       store_poly(2, 1, 0);

                    break;

                  case 192:		// declare next vertex on vertex stack

                     if (vstack == (vertexlist *)NULL) { 
                        vstack = new vertexlist;
                        copyarray3(vstack->pt, vertex[0]);
                        vcount = vstack->id = 1;
                        break;
                     }

                     vstack->insert(vertex[0], &vcount);
                     break;

                  default:
                     break;
               }

            }

         }

/*
         else if (code == 210 || code == 220 || code == 230) {
		// X, Y, Z components of extrusion direction
         }

         else if (code == 300) {	// Group Description
            strcpy(string_[3], linbuf_);	
            if (strstr(curobj_, "GROUP"))
               objState_ |= DESC_SET;
         }

	else if (groupcode_ == 330) { // Soft Pointer to owner Dictionary
			strcpy(string_[4], linbuf_);	
			if (strstr(curobj_, "GROUP"))
				objState_ |= OWNER_ID_SET;
	}

	else if (groupcode_ == 340) {	// Handles of entities  related to group
			strcpy(string_[6], linbuf_);	
			if (strstr(curobj_, "GROUP")) {
				objState_ |= POLYS_ASSIGNED;
				strfix(tmpString, string_[6]);
			}
	}
*/

      }

   }

   fclose(infile);
   return 1;
}
