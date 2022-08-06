

#include "convert.h"

void lw_shade::post_read() {

   copyarray3(ambient, shade);

   diffuse[0] = shade[0]*diff;
   diffuse[1] = shade[1]*diff;
   diffuse[2] = shade[2]*diff;

   specular[0] = shade[0]*spec;
   specular[1] = shade[1]*spec;
   specular[2] = shade[2]*spec;

   luminate[0] = shade[0]*lum;
   luminate[1] = shade[1]*lum;
   luminate[2] = shade[2]*lum;
}


/* ***********************************************************************
  LightWave 3D objects are stored as IFF files with a FORM type of
  LWOB.  Currently, a FORM LWOB must contain a PNTS chunk, a SRFS
  chunk, and a POLS chunk (in that order).  These may be followed by
  zero or more SURF chunks.  LightWave 2.0 also has the ability to
  save and load surface descriptions as FORM LWOB files that contain
  only a SURF chunk.
*********************************************************************** */

int lightwave::read_data(char *filename) {

   FILE *infile;
   char buffer[5];
   int  bytesize;		// 32 bit int
   int  i;
   int *index;

   if (!(infile=fopen(filename, "r"))) {
      printf("Cannot open %s.  Aborting...\n", filename);
      exit(1);
   }

   printf("Reading...\n");

   for (i=0; i<4; i++)
      buffer[i] = getuchar(infile);
   buffer[4] = 0;

   if (strcmp(buffer, "FORM")) {
      printf("%s is not in Lightwave format... Aborting...\n", filename);
      fclose(infile);
      exit(1);
   }

   bytesize = getuint(infile);

   for (i=0; i<4; i++)
      buffer[i] = getuchar(infile);
   buffer[4] = 0;
   bytesize -= 4;

   if (strcmp(buffer, "LWOB")) {
      printf("%s is not in Lightwave format... Aborting...\n", filename);
      fclose(infile);
      exit(1);
   }

   for (i=0; i<4; i++)
      buffer[i] = getuchar(infile);
   buffer[4] = 0;
   bytesize -= 4;

   if (!strcmp(buffer, "SURF")) {
      printf("Only a SURF chunk...  Aborting...\n");
      fclose(infile);
      exit(1);
   }

   if (!strcmp(buffer, "PNTS")) {

      read_pnts(infile, &index, &bytesize);

      for (i=0; i<4; i++)
         buffer[i] = getuchar(infile);
      buffer[4] = 0;
      bytesize -= 4;

      if (strcmp(buffer, "SRFS")) {
         printf("Invalid SRFS chunk -> %s... Aborting...\n", buffer);
         fclose(infile);
         exit(1);
      }

      read_srfs(infile, &bytesize);

      for (i=0; i<4; i++)
         buffer[i] = getuchar(infile);
      buffer[4] = 0;
      bytesize -= 4;

      if (strcmp(buffer, "POLS")) {
         printf("Invalid POLS chunk -> %s... Aborting...\n", buffer);
         fclose(infile);
         exit(1);
      }

      read_pols(infile, index, &bytesize);

      while (bytesize > 0) {

         for (i=0; i<4; i++)
            buffer[i] = getuchar(infile);
         buffer[4] = 0;
         bytesize -= 4;

         if (strcmp(buffer, "SURF")) {
            printf("Invalid SURF chunk -> %s... Aborting...\n", buffer);
            fclose(infile);
            exit(1);
         }

         read_surf(infile, &bytesize);
      }

      fclose(infile);
      delete [] index;
      return 1;
   }

   printf("%s is not in Lightwave format... Aborting...\n", filename);
   fclose(infile);
   exit(1);
   return 0;
}


// negate z to get "pixcon" coord system
int lightwave::read_pnts(FILE *infile, int **index, int *bytesize) {

    int i, j;
    vector3f point;

   printf("PNTS...\n");

   j = getuint(infile);
   (*bytesize) -= 4 + j;

   if (*bytesize < 0) {
      printf("Invalid bytesize... Aborting...\n");
      fclose(infile);
      exit(1);
   }

   j /= 12;

   *index = new int[j];

   for (i=0; i<j; i++) {
      *((unsigned int *)(&point[0])) = getuint(infile);
      *((unsigned int *)(&point[1])) = getuint(infile);
      *((unsigned int *)(&point[2])) = getuint(infile);

      if (head == (vertexlist *)NULL) {
         head = new vertexlist;
         copyarray3(head->pt, point);
         head->id = (*index)[0] = vertexcount = 1;
      }

      else
         (*index)[i] = head->insert(point, &vertexcount);
   }

   return 1;
}


int lightwave::read_srfs(FILE *infile, int *bytesize) {

   int j;
   int len;
   lw_shade *ptr;

   printf("SRFS...\n");

   j = getuint(infile);
   (*bytesize) -= 4 + j;

   if (*bytesize < 0) {
      printf("Invalid SRFS - bad chunk size... Aborting...\n");
      fclose(infile);
      exit(1);
   }

   for (scount=0; j > 0; scount++) {

      ptr = new lw_shade;
      ptr->id = scount;

      len = getstring(infile, ptr->name, MAXSTRLEN);

      if (len < 1) {
         printf("Invalid string size (%d:%d)... Aborting...\n", polycount, len);
         fclose(infile);
         exit(1);
      }

      j -= len;

      if ((len & 0x01)) {		// if (odd # of bytes, read 1 more)
         getuchar(infile);
         j--;
      }

      ptr->next = surface_list;
      surface_list = ptr;
   }

   if (j < 0) {
      printf("SRFS - chunk size != listed chunk size... Aborting...\n");
      fclose(infile);
      exit(1);
   }

   return 1;
}


int lightwave::read_pols(FILE *infile, int *index, int *bytesize) {

   int chunk_size;
   int i, j, k;
   objectlist *otr;

   printf("POLS...\n");

   chunk_size = getuint(infile);
   (*bytesize) -= 4 + chunk_size;

   if (*bytesize < 0) {
      printf("POLS: Invalid bytesize... Aborting...\n");
      fclose(infile);
      exit(1);
   }

   edgecount = 0;
   polycount = 0;

   while (chunk_size > 0) {

      otr = new objectlist;
      otr->rnflag = reverse_normal;

      polycount++;
      otr->id = polycount;
      otr->polynum   = getushort(infile);
      otr->vptr = new vertexdata[otr->polynum];
      edgecount += otr->polynum;

      chunk_size -= (1 + otr->polynum + 1) << 1;

      for (i=0; i<otr->polynum; i++)
         otr->vptr[i].index = index[getushort(infile)];

      k = (short)getushort(infile);

      if (otr->polynum > 2) {
         otr->lw = surface_list->find(abs(k)-1);
         otr->lw->count++;

         if (ob == (objectlist *)NULL)
            ob = otr;
         else
            ob->insert_id(otr);
      }

      else {
         polycount--;
         edgecount -= otr->polynum;
         delete otr;
      }

      if (k < 0) {		// detail polygons...

         k = getushort(infile);
         chunk_size -= 2;

         for (j=0; j<k; j++) {
            otr = new objectlist;
            otr->rnflag = reverse_normal;

            polycount++;
            otr->id = polycount;
            otr->polynum = getushort(infile);
            otr->vptr = new vertexdata[otr->polynum];
            edgecount += otr->polynum;

            chunk_size -= (1 + otr->polynum + 1) << 1;

            for (i=0; i<otr->polynum; i++)
               otr->vptr[i].index = index[getushort(infile)];
     
            otr->lw = surface_list->find(getushort(infile));

            if (otr->polynum > 2) {
               otr->lw->count++;
               ob->insert_id(otr);
            }

            else {
               polycount--;
               edgecount -= otr->polynum;
               delete otr;
            }

         }

      }

   }

   if (chunk_size != 0) {
      printf("Data read != chunksize (%d)...  Aborting...\n", chunk_size);
      fclose(infile);
      exit(1);
   }

   return 1;
}


int lightwave::read_surf(FILE *infile, int *bytesize) {

   int chunk_size, sub_chunk_size;
   char buffer[MAXSTRLEN];
   int i;
   int temp;
   lw_shade *ptr;

   int texflag = MASK_NULL;

   printf("SURF...\n");

   chunk_size = getuint(infile);
   (*bytesize) -= 4 + chunk_size;

   if (*bytesize < 0) {
      printf("Invalid bytesize... Aborting...\n");
      fclose(infile);
      exit(1);
   }

   temp = getstring(infile, buffer, MAXSTRLEN);

   if (temp < 1) {
      printf("Underestimated string size (%d:%d)... Aborting...\n", temp, MAXSTRLEN);
      fclose(infile);
      exit(1);
   }

   chunk_size -= temp;

   if ((temp & 0x01)) {		// if (odd # of bytes, read 1 more)
      getuchar(infile);
      chunk_size--;
   }

   for (ptr=(lw_shade *)surface_list; ptr!= (lw_shade *)NULL; ptr=(lw_shade *)ptr->next)
      if (!strcmp(ptr->name, buffer))
         break;

   if (ptr == (lw_shade *)NULL) {
      printf("Invalid polygon reference... Aborting...\n");
      fclose(infile);
      exit(1);
   }

   while (chunk_size > 0) {

      for (i=0; i<4; i++)
         buffer[i] = getuchar(infile);
      buffer[4] = 0;
      sub_chunk_size = getushort(infile);
      chunk_size -= 6 + sub_chunk_size;

	// read 4 bytes - rgb {extra}  surface color
      if (!strcmp(buffer, "COLR")) {

         printf("COLR...\n");

         if (sub_chunk_size != 4) {
            printf("Invalid COLR chunk size... Aborting...\n");
            fclose(infile);
            exit(1);
         }

         ptr->shade[0] = getuchar(infile)/255.0;
         ptr->shade[1] = getuchar(infile)/255.0;
         ptr->shade[2] = getuchar(infile)/255.0;

         getuchar(infile);
      }

      else if (!strcmp(buffer, "FLAG")) {

         printf("FLAG...\n");

         if (sub_chunk_size != 2) {
            printf("Invalid FLAG chunk size... Aborting...\n");
            fclose(infile);
            exit(1);
         }

         temp = getushort(infile);

         if ((temp & 0x01))
            ptr->lum = 1.0;

         ptr->flag_dbl_side = (unsigned char)((temp >> 8) & 0x01);
      }

      else if (!strcmp(buffer, "LUMI")) {
         
         printf("LUMI...\n");

         if (sub_chunk_size != 2) {
            printf("Invalid LUMI chunk size... Aborting...\n");
            fclose(infile);
            exit(1);
         }

         ptr->lum = getushort(infile)/256.0;
      }

      else if (!strcmp(buffer, "DIFF")) {
         
         printf("DIFF...\n");

         if (sub_chunk_size != 2) {
            printf("Invalid DIFF chunk size... Aborting...\n");
            fclose(infile);
            exit(1);
         }

         ptr->diff = getushort(infile)/256.0;
      }

      else if (!strcmp(buffer, "SPEC")) {

         printf("SPEC...\n");

         if (sub_chunk_size != 2) {
            printf("Invalid SPEC chunk size... Aborting...\n");
            fclose(infile);
            exit(1);
         }

         ptr->spec = getushort(infile)/256.0;
      }

      else if (!strcmp(buffer, "GLOS")) {

         printf("GLOS...\n");

         if (sub_chunk_size != 2) {
            printf("Invalid GLOS chunk size... Aborting...\n");
            fclose(infile);
            exit(1);
         }

         i = getushort(infile);

         switch(i) {
            case 1024:
               ptr->specn = 7.0;
               break;
            case 256:
               ptr->specn = 5.0;
               break;
           case 64:
               ptr->specn = 3.0;
               break;
           case 16:
               ptr->specn = 1.0;
               break;
           default:
               ptr->specn = 1.0;
         }

      }

      else if (!strcmp(buffer, "TRAN")) {
         printf("TRAN...\n");

         if (sub_chunk_size != 2) {
            printf("Invalid TRAN chunk size... Aborting...\n");
            fclose(infile);
            exit(1);
         }

         i = getushort(infile);

         if (!i)
            ptr->trans = -1.0;
         else
            ptr->trans = i/256.0;
      }

      else if (!strcmp(buffer, "DTEX")) {
         printf("DTEX...\n");

         texflag = MASK_DTEX;
         ptr->tob.init(MASK_DTEX);
         
         i = getstring(infile, buffer, MAXSTRLEN);

         if (i < 1) {
            printf("Underestimated string size (%d:%d)... Aborting...\n", i, MAXSTRLEN);
            fclose(infile);
            exit(1);
         }
 
         if ((i & 0x01)) 		// if (odd # of bytes, read 1 more)
            getuchar(infile);
      }

      else if (!strcmp(buffer, "STEX")) {
         printf("STEX...\n");

         texflag = MASK_STEX;
         if (ptr->tob.id != MASK_DTEX)
            ptr->tob.init(MASK_STEX);

         i = getstring(infile, buffer, MAXSTRLEN);

         if (i < 1) {
            printf("Underestimated string size (%d:%d)... Aborting...\n", i, MAXSTRLEN);
            fclose(infile);
            exit(1);
         }
 
         if ((i & 0x01)) 		// if (odd # of bytes, read 1 more)
            getuchar(infile);
      }

      else if (!strcmp(buffer, "CTEX")) {
         printf("CTEX...\n");

         texflag = MASK_CTEX;

         i = getstring(infile, buffer, MAXSTRLEN);

         if (i < 1) {
            printf("Underestimated string size (%d:%d)... Aborting...\n", i, MAXSTRLEN);
            fclose(infile);
            exit(1);
         }
 
         if ((i & 0x01)) 		// if (odd # of bytes, read 1 more)
            getuchar(infile);
      }

      else if (!strcmp(buffer, "RTEX")) {
         printf("RTEX...\n");

         texflag = MASK_RTEX;

         i = getstring(infile, buffer, MAXSTRLEN);

         if (i < 1) {
            printf("Underestimated string size (%d:%d)... Aborting...\n", i, MAXSTRLEN);
            fclose(infile);
            exit(1);
         }
 
         if ((i & 0x01)) 		// if (odd # of bytes, read 1 more)
            getuchar(infile);
      }

      else if (!strcmp(buffer, "TTEX")) {
         printf("TTEX...\n");

         texflag = MASK_TTEX;

         i = getstring(infile, buffer, MAXSTRLEN);

         if (i < 1) {
            printf("Underestimated string size (%d:%d)... Aborting...\n", i, MAXSTRLEN);
            fclose(infile);
            exit(1);
         }
 
         if ((i & 0x01)) 		// if (odd # of bytes, read 1 more)
            getuchar(infile);
      }

      else if (!strcmp(buffer, "BTEX")) {
         printf("BTEX...\n");

         texflag = MASK_BTEX;

         i = getstring(infile, buffer, MAXSTRLEN);

         if (i < 1) {
            printf("Underestimated string size (%d:%d)... Aborting...\n", i, MAXSTRLEN);
            fclose(infile);
            exit(1);
         }
 
         if ((i & 0x01)) 		// if (odd # of bytes, read 1 more)
            getuchar(infile);
      }

      else if (!strcmp(buffer, "LTEX")) {
         printf("LTEX...\n");

         texflag = MASK_LTEX;

         i = getstring(infile, buffer, MAXSTRLEN);

         if (i < 1) {
            printf("Underestimated string size (%d:%d)... Aborting...\n", i, MAXSTRLEN);
            fclose(infile);
            exit(1);
         }
 
         if ((i & 0x01)) 		// if (odd # of bytes, read 1 more)
            getuchar(infile);
      }

      else if (!strcmp(buffer, "TIMG")) {
         printf("TIMG...\n");

         if (texflag == ptr->tob.id) {
            i = ptr->tob.read_texname(infile);

            if (i < 1) {
               printf("Underestimated string size (%d:%d)... Aborting...\n", i, MAXSTRLEN);
               printf("Invalid bytesize (%d:%d)... Aborting...\n", i, sub_chunk_size);
               fclose(infile);
               exit(1);
            }

            if ((i & 0x01)) 		// if (odd # of bytes, read 1 more)
               getuchar(infile);
         }

         else {
            i = getstring(infile, buffer, MAXSTRLEN);

            if (i < 1) {
               printf("Underestimated string size (%d:%d)... Aborting...\n", i, MAXSTRLEN);
               printf("Invalid bytesize (%d:%d)... Aborting...\n", i, sub_chunk_size);
               fclose(infile);
               exit(1);
            }

            if ((i & 0x01)) 		// if (odd # of bytes, read 1 more)
               getuchar(infile);
         }

      }


/*		unknown chunks... or dont know what to do w/ them...

      else if (!strcmp(buffer, "TCTR")) {
         printf("TCTR...\n");

         if (texflag == ptr->tob.id) {
         }

         else {
         }

getfloat(infile); getfloat(infile); getfloat(infile);         
//         printf("%f %f %f\n", getfloat(infile), getfloat(infile), getfloat(infile));
//3 4-byte floats - x y z of texture center
      }

      else if (!strcmp(buffer, "TSIZ")) {
         printf("TSIZ...\n");
getfloat(infile); getfloat(infile); getfloat(infile);         
//         printf("%f %f %f\n", getfloat(infile), getfloat(infile), getfloat(infile));
//3 4-byte floats - x y z of texture size
      }

     else if (!strcmp(buffer, "TVAL")) {
//short - texture value for current texture
     }

        else if (!strcmp(sub_chunk, "TFLG")) {
short - 7 bits


xaxis
yaxis
z axis
world coord
negative image
pixel blending
antialiasing

        }


      else if (!strcmp(sub_chunk, "REFL")) {
short
0 - 256
0 - 100 percent reflection
IGNORE!!!!
if not set -> default to zero

        }

        else if (!strcmp(sub_chunk, "RIMG")) {
filename even bytes
image to be used as reflection map...
if reflect flag set, and no RIMG, then use sky/ground or
	raytracing as reflect map....
IGNORE
        }

        else if (!strcmp(sub_chunk, "RSAN")) {
4 byte float - angle of reflection map seam (deg)
IGRNORE
        }

        else if (!strcmp(sub_chunk, "RIND")) {
4 byte float - refractive index - ratio of speed of light in a vacum
		to speed of light in material
IGGNORE

        }

        else if (!strcmp(sub_chunk, "EDGE")) {
4 byte float - edge transparency
IGNORE
        }

        else if (!strcmp(sub_chunk, "SMAN")) {
4 byte float - max angle (deg) between 2 adj polygons that can
be smooth shaded
IGNORE
        }


        else if (!strcmp(sub_chunk, "TFAL")) {
3 4-byte floats - x y z of texture falloff
        }

        else if (!strcmp(sub_chunk, "TVEL")) {
3 4-byte floats - x y z of texture velocity
        }

        else if (!strcmp(sub_chunk, "TCLR")) {
4 bytes - rgb-null - color for texture - color modifier
        }



        else if (!strcmp(sub_chunk, "TAMP")) {
4-byte float - amplitude of current bump texture
        }

        else if (!strcmp(sub_chunk, "TFRQ")) {
short - number of noise freq/wave sources for current texture
        }

        else if (!strcmp(sub_chunk, "TSP0")) {
4-byte float - represent button 0 that controls special params 
		(Contrast/Turbulence/Wavelength/etc)
        }

        else if (!strcmp(sub_chunk, "TSP1")) {
4-byte float - represent button 0 that controls special params 
		(Contrast/Turbulence/Wavelength/etc)
        }

        else if (!strcmp(sub_chunk, "TSP2")) {
4-byte float - represent button 0 that controls special params 
		(Contrast/Turbulence/Wavelength/etc)
        }
*/


      else {
         printf("Skipping %s chunk...\n", buffer);
         for (i=0; i<sub_chunk_size; i++)
            getuchar(infile);
      }

   }

   return 1;
}



