



/* *************************************************************
   This file contains some of the file I/O routines
************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "pixcon.h"

#include "particle.h"
#include "spatch.h"
#include "cylinder.h"
#include "cone.h"
#include "sbfast.h"
#include "line.h"
#include "sphere.h"
#include "loader.h"


/* **************************************************
************************************************** */
void init_graph() {

   init_lut();
   init_shader();
   init_loader();
}


/* **************************************************
   this function parses command line to find user defined files,
   or else use the default files.
************************************************** */
void parseinput(int argc, char *argv[]) {

   int i, j;
   float tgamma;

   for (i=1; i<argc; i++)
      if (*(argv[i]) == '-')
         switch (*(argv[i]+1)) {
            case 'a':                   // aliasing

               if (argv[i][2] == 'e')
                  ANTIALIAS = 2;
               else
                  ANTIALIAS = 1;

               if (i+1 < argc && argv[i+1][0] != '-') {
                  i++;
                  strcpy(SANTIALIAS, argv[i]);
               }

               else
                  SANTIALIAS[0] = 0;

               break;

            case 'f':                   // image file
               i++;
               if (i < argc) {
                  RGBFILE = 1;
                  strcpy(SRGBFILE, argv[i]);
               }

               break;

            case 'g':
               i++;
               if (i < argc) {
                  GAMMA = 1;
                  tgamma = getgamma(atof(argv[i]));

                  memset(GAMMALUT, 0, 256);

                  for (j=0; j<256; j++)
                     PGAMMA[j] = BYTE_LUT((int)(255.0*gammacorrect(tgamma, j/255.0)));

                  memset(&GAMMALUT[512], PGAMMA[255], 512);
               }

               break;

            case 'i':                   // interlace
               INTERLACE = 1;
               X2 = 0;
               break;

            case 'l':                   // user defined .lst
               i++;
               if (i < argc) {
                  strcpy(SDEFLST, argv[i]);
                  DEFLST = 1;
               }

               break;

            case 'n':                   // no shadow
               if (strlen(argv[i]) > 2 && argv[i][2] == 'i')
                  NIXMIP = 1;
               else
                  if (strlen(argv[i]) > 4 && argv[i][4] == 'c') {
                     NOSCREEN = 1;
                     PENCILTEST = 0;
                  }

                  else
                     NOSHADOW = 1;

               break;

            case 'p':                   // pencil test
               PENCILTEST = 1;
               NOSCREEN = 0;
               break;

            case 's':                   // silent to file
               SILENT = 1;
               break;

            case 'v':                   // verbose mode
               SILENT = 0;
               break;

            case 'x':

               if (argv[i][2] == '2') {
                  X2 = 1;
                  INTERLACE = 0;
                  break;
               }

               i++;
               WINX = atoi(argv[i]);
               break;

            case 'y':
               i++;
               WINY = atoi(argv[i]);
               break;

            default:
               break;
         }

   if (PENCILTEST) {
      X2 = INTERLACE = 0;
   }

   if (X2) {
      WINX = WINX>>1;
      WINY = WINY>>1;
   }

}


/* *************************************************************
************************************************************* */
unsigned int parse_info(char *instring) {

   unsigned int mcinfo = 0;
   int  i, j;

   for (i=0, j=strlen(instring); i<j; i++) {
      switch (instring[i]) {
         case 'R':
            mcinfo |= CIRAYCAST;
            break;

         case '2':
            mcinfo |= CITEXTURE;
            break;

         case '3':
            mcinfo |= CI3DTEX;
            break;

         case 'L':                      // obsolete
            break;

         case 'T':
            mcinfo |= CITRANSPARENT;
            break;

         case 'B':
            mcinfo |= (CIBITMAP | CITEXTURE);
            break;

         default:
            break;
      }

   }

   return mcinfo;
}


void parse_color(char *ctype, colortype *mctype, unsigned int *mcinfo) {

   switch (ctype[0]) {
      case 'A':
         *mctype = ALEX;
         *mcinfo = parse_info(ctype+4);
         break;

      case 'B':
         *mctype = BW;
         *mcinfo = parse_info(ctype+2);
         break;

      case 'C':
         *mctype = CONSTANT;
         *mcinfo = parse_info(ctype+8);
         break;

      case 'D':
         *mctype = DOT;
         *mcinfo = parse_info(ctype+3);
         break;

      case 'G':
         *mctype = GOURAUD;
         *mcinfo = parse_info(ctype+7);
         break;

      case 'P':
         switch (ctype[1]) {
            case 'H':
               *mctype = PHONG;
               *mcinfo = parse_info(ctype+5);
               break;

            case 'A':
               *mctype = PALEX;
               *mcinfo = parse_info(ctype+5);
               break;

            case 'B':
               *mctype = PBW;
               *mcinfo = parse_info(ctype+3);
               break;

            case 'C':
               *mctype = PCONSTANT;
               *mcinfo = parse_info(ctype+9);
               break;

            case 'D':
               *mctype = PDOT;
               *mcinfo = parse_info(ctype+4);
               break;

            case 'G':
               *mctype = PGOURAUD;
               *mcinfo = parse_info(ctype+8);
               break;

            case 'P':
               *mctype = PPHONG;
               *mcinfo = parse_info(ctype+6);
               break;

            default:
               *mctype = PWFBW;
         }

         break;

      default:
         *mctype = WFBW;
   }

}


/* *************************************************************
   This procedure reads in a list of objects, their attributes,
   then reads in the object data.
************************************************************* */
pc *read_object(FILE *infile, char *otype, char *ctype) {

   pc   *ptr = (pc *)NULL;
   char stype[MAXSTRLEN];
   int  j, k, l;

   fscanf(infile, "%s", ctype);        // object coloring

   switch (otype[0]) {

      case 'G':

         switch (otype[4]) {

            case 'P':                   // SPHERE
               ptr = new sphere;
               break;

            case 'Y':                   // CYLINDER
               ptr = new cylinder;
               break;

            case 'O':                   // CONE
               ptr = new cone;
               break;

            default:
               break;
         }

         parse_color(ctype, &ptr->mctype, &ptr->mcinfo);

         if ((ptr->mctype > BW && ptr->mctype <=PHONG) || ptr->mctype > PBW) {
            fscanf(infile, "%s", ptr->colorname);                      // color file

            if (ptr->mcinfo & CITEXTURE)
               fscanf(infile, "%s", ptr->texname2);                   // texture filename

            if (ptr->mcinfo & CI3DTEX)
               fscanf(infile, "%s", ptr->texname3);                 // 3D texture file
         }

         fscanf(infile, "%s", stype);                      // shadow flag

         if (ptr->sflag = !strcmp(stype, "SHADOW"))
            fscanf(infile, "%f %f %f %f", &ptr->splane[0], &ptr->splane[1],
                                          &ptr->splane[2], &ptr->splane[3]);

         fscanf(infile, "%f %f %f", &ptr->center[0], &ptr->center[1],
                                        &ptr->center[2]);       // object location

         fscanf(infile, "%f", &ptr->size);          // object size

         for (j=0; j<4; j++)                         // object rotation
            fscanf(infile, "%f %f %f %f", &ptr->rotate[j][0],
                   &ptr->rotate[j][1], &ptr->rotate[j][2],
                   &ptr->rotate[j][3]);

         ptr->read_data(infile);

         break;

      case 'L':                 // line
         ptr = new line;

         if (ctype[0] == 'G')
            ptr->mctype = GOURAUD;
         else
            ptr->mctype = WFBW;

         fscanf(infile, "%s", stype);                      // shadow flag

         if (ptr->sflag = !strcmp(stype, "SHADOW"))
            fscanf(infile, "%f %f %f %f", &ptr->splane[0], &ptr->splane[1],
                                          &ptr->splane[2], &ptr->splane[3]);
         ptr->read_data(infile);

         break;

      case 'P':
         if (!strcmp(otype, "PARTICLE")) {

            ptr = new particle;

            if (ctype[0] == 'G')
               ptr->mctype = GOURAUD;
            else
               ptr->mctype = WFBW;

            fscanf(infile, "%s", stype);                      // shadow flag

            if (ptr->sflag = !strcmp(stype, "SHADOW"))
               fscanf(infile, "%f %f %f %f", &ptr->splane[0], &ptr->splane[1],
                                             &ptr->splane[2], &ptr->splane[3]);

            ptr->read_data(infile);
         }

         else {
            ptr = new polygon;

            parse_color(ctype, &ptr->mctype, &ptr->mcinfo);

            fscanf(infile, "%s", ptr->filename);                      // object file

            if (ptr->mctype > BW) {
               fscanf(infile, "%s", ptr->colorname);                      // color file

               if (ptr->mcinfo & CITEXTURE) {
                  fscanf(infile, "%s", ptr->texname2);                   // texture filename
                  if ((ptr->mcinfo & CIBITMAP)) {
                     fscanf(infile, "%d %d %d", &j, &k, &l);
                     ((polygon *)ptr)->inviso = (l | (((j<<8) | k)<<8)<<8);
                     ptr->mcinfo |= CIINVISO;
                  }

               }

               if (ptr->mcinfo & CI3DTEX)
                  fscanf(infile, "%s", ptr->texname3);                 // 3D texture file
            }

            fscanf(infile, "%s", stype);                      // shadow flag

            if (ptr->sflag = !strcmp(stype, "SHADOW"))
               fscanf(infile, "%f %f %f %f", &ptr->splane[0], &ptr->splane[1],
                                             &ptr->splane[2], &ptr->splane[3]);

            fscanf(infile, "%f %f %f", &ptr->center[0], &ptr->center[1],
                                        &ptr->center[2]);

            fscanf(infile, "%f", &ptr->size);          // object size

            for (j=0; j<4; j++)                         // object rotation
               fscanf(infile, "%f %f %f %f", &ptr->rotate[j][0],
                      &ptr->rotate[j][1], &ptr->rotate[j][2], &ptr->rotate[j][3]);
         }

         break;

      case 'R':
         ptr = new radiant;

         if (ctype[1] == 'S') {
            ptr->mctype = RSOLID;
            for (j=6, k=strlen(ctype); j<k; j++)
               if (ctype[j] == 'H')                     // flag for hollow - black hole
                  ((radiant *)ptr)->flagh = 1;
         }

         else
            if (ctype[0] == 'I') {
               ptr->mctype = IRRADIANT;
               for (j=6, k=strlen(ctype); j<k; j++)
                  if (ctype[j] == 'H')
                     ((radiant *)ptr)->flagh = 1;
            }

            else
               ptr->mctype = WFBW;

         if (ptr->mctype != WFBW)
            fscanf(infile, "%s", ptr->colorname);        // object coloring

         ptr->read_data(infile);

         break;

      case 'S':
         if (otype[1] == 'B') {                 // SBFAST

            ptr = new sbfast;

            parse_color(ctype, &ptr->mctype, &ptr->mcinfo);

            if (ptr->mctype != PALEX)
               ptr->mctype = ALEX;

            fscanf(infile, "%s", ptr->colorname);                      // color file

            if (ptr->mcinfo & CITEXTURE)
               fscanf(infile, "%s", ptr->texname2);                   // texture filename

            if (ptr->mcinfo & CI3DTEX)
               fscanf(infile, "%s", ptr->texname3);                 // 3D texture file

            fscanf(infile, "%f %f %f", &ptr->center[0], // object location
                                          &ptr->center[1], &ptr->center[2]);
            fscanf(infile, "%f", &ptr->size);          // object size

            for (j=0; j<4; j++)                         // object rotation
               fscanf(infile, "%f %f %f %f", &ptr->rotate[j][0],
                      &ptr->rotate[j][1], &ptr->rotate[j][2],
                      &ptr->rotate[j][3]);

            ptr->read_data(infile);
         }

         else {                                         // SPATCH
            ptr = new spatch;

            parse_color(ctype, &ptr->mctype, &ptr->mcinfo);

            fscanf(infile, "%s", ptr->filename);                      // object file

            if ((ptr->mctype > BW && ptr->mctype <= PHONG) || ptr->mctype > PBW) {
               fscanf(infile, "%s", ptr->colorname);                      // color file

               if (ptr->mcinfo & CITEXTURE)
                  fscanf(infile, "%s", ptr->texname2);                   // texture filename

               if (ptr->mcinfo & CI3DTEX)
                  fscanf(infile, "%s", ptr->texname3);                 // 3D texture file
            }

            fscanf(infile, "%s", stype);                      // shadow flag

            if (ptr->sflag = !strcmp(stype, "SHADOW"))
               fscanf(infile, "%f %f %f %f", &ptr->splane[0], &ptr->splane[1],
                                             &ptr->splane[2], &ptr->splane[3]);

            fscanf(infile, "%f %f %f", &ptr->center[0], // object location
                                       &ptr->center[1], &ptr->center[2]);
            fscanf(infile, "%f", &ptr->size);          // object size

            for (j=0; j<4; j++)                         // object rotation
               fscanf(infile, "%f %f %f %f", &ptr->rotate[j][0],
                      &ptr->rotate[j][1], &ptr->rotate[j][2],
                      &ptr->rotate[j][3]);
         }

         break;

      default:
         break;
   }

   return ptr;
}



/* *************************************************************
   This procedure reads in a list of objects, their attributes,
   then reads in the object data.
************************************************************* */
int read_list(char *filename, listtype *mainlist, int *mapcount) {

   FILE    *infile;                // input file name
   pc      *ptr;                   // pointers to objects
   camera  *vtr;
   light   *ltr;

   char    otype[MAXSTRLEN], ctype[MAXSTRLEN];
   char    buffer[MAXSTRLEN];

   int     objectcount;            // # of objects
   int     i, j;

   if (!(infile = fopen(filename, "r"))) {
      printf("Cannot access .lst file %s...\n", filename);
      return 0;
   }

   fscanf(infile, "%d %d", mapcount, &objectcount);     // # of bitmaps / objects

   mainlist->init(*mapcount);

   for (i=0; i<objectcount; i++) {
      fscanf(infile, "%d", &j);                 // map # to place object
      j--;

      fscanf(infile, "%s", otype);              // object type

      switch (otype[0]) {

         case 'C':                              // camera
            vtr = new camera(WINX, WINY);
            vtr->frame = j;

            vtr->read_data(infile);

            if (j < *mapcount) {
               vtr->next = mainlist->camdtr[j];
               mainlist->camdtr[j] = vtr;
            }

            else
               delete vtr;

            break;

         case 'L':                              // light

            if (otype[2] == 'G') {
               fscanf(infile, "%s", buffer);

               if (!strcmp(buffer, "BEAM_POINT"))
                  ltr = new beam_point;
               else if (!strcmp(buffer, "BEAM_FAR"))
                  ltr = new beam_far;
               else if (!strcmp(buffer, "POINT"))
                  ltr = new point_light;
               else if (!strcmp(buffer, "FAR"))
                  ltr = new far_light;
               else if (!strcmp(buffer, "SIMPLE_BEAM_POINT"))
                  ltr = new simple_beam_point;
               else if (!strcmp(buffer, "SIMPLE_BEAM_FAR"))
                  ltr = new simple_beam_far;
               else if (!strcmp(buffer, "SIMPLE_POINT"))
                  ltr = new simple_point_light;
               else if (!strcmp(buffer, "SIMPLE_FAR"))
                  ltr = new simple_far_light;
               else {
                  printf ("Error: bad light type : %s\n", buffer);
                  fclose(infile);
                  exit(1);
               }

               ltr->frame = j;

               ltr->read_data(infile, &mainlist->tdtr);

               if (j < *mapcount) {
                  if (ltr->query_class() == SPOT_LIGHT) {
                     ltr->next = mainlist->beamdtr[j];
                     mainlist->beamdtr[j] = ltr;
                  }

                  else {
                     ltr->next = mainlist->lightdtr[j];
                     mainlist->lightdtr[j] = ltr;
                  }

               }

               else
                  delete ltr;
            }

            break;

         default:
            ptr = read_object(infile, otype, ctype);

            if (ptr == (pc *)NULL)
               break;

            if (j >= *mapcount) {
               delete ptr;
               break;
            }

            ptr->preprocess(mainlist);
            ptr->frame = j;
            ptr->id = i+1;

            if (ptr->mcinfo & CITRANSPARENT) {
               ptr->next = mainlist->raddtr[ptr->frame];
               mainlist->raddtr[ptr->frame] = ptr;
            }

            else {
               ptr->next = mainlist->header[ptr->frame];
               mainlist->header[ptr->frame] = ptr;
            }

            break;
      }

   }

   fclose(infile);
   return 1;
}


/* *************************************************************
************************************************************* */
int parse_list(char *filename, listtype *mainlist, int *mapcount) {

   FILE    *infile;                // input file name
   superclass *ptr;                // pointers to objects
   char    otype[MAXSTRLEN], token[MAXSTRLEN];

   int     i;
   int     objectcount;            // # of objects

   if (!(infile = fopen(filename, "r"))) {
      printf("Cannot access .list file %s...\n", filename);
      return 0;
   }

   get_token(infile, token);
   lower_case(token);

   if (strcmp(token, "list")) {
      printf("%s is not a .list file...\n", filename);
      fclose(infile);
      return 0;
   }

   get_token(infile, token);
   get_token(infile, token);
   *mapcount = atoi(token);

   get_token(infile, token);
   get_token(infile, token);
   objectcount = atoi(token);

   mainlist->init(*mapcount);

   for (i=0; i<objectcount; i++) {
      get_token(infile, token);
      lower_case(token);
      get_token(infile, otype);

      if (!(ptr = gloader->parse(infile, token))) {
         printf("Invalid object type \"%s\"\n", token);
         fclose(infile);
         exit(1);
      }

      switch (ptr->query_category()) {

         case CAMERA_CLASS:

            if (ptr->frame < *mapcount) {
               ptr->preprocess(NULL);
               ptr->next = mainlist->camdtr[ptr->frame];
               mainlist->camdtr[ptr->frame] = (camera *)ptr;
            }

            else
               delete ptr;

            break;

         case LIGHT_CLASS:
            if (ptr->frame < *mapcount) {
               ptr->preprocess(&mainlist->tdtr);

               if (((light *)ptr)->query_class() == SPOT_LIGHT) {
                  ptr->next = mainlist->beamdtr[ptr->frame];
                  mainlist->beamdtr[ptr->frame] = (light *)ptr;
               }

               else {
                  ptr->next = mainlist->lightdtr[ptr->frame];
                  mainlist->lightdtr[ptr->frame] = (light *)ptr;
               }

            }

            else
               delete ptr;

            break;

         default:

            if (ptr->frame >= *mapcount) {
               delete ptr;
               break;
            }

            ptr->preprocess(mainlist);
            ((pc *)ptr)->id = i+1;

            if (((pc *)ptr)->mcinfo & CITRANSPARENT) {
               ptr->next = mainlist->raddtr[ptr->frame];
               mainlist->raddtr[ptr->frame] = (pc *)ptr;
            }

            else {
               ptr->next = mainlist->header[ptr->frame];
               mainlist->header[ptr->frame] = (pc *)ptr;
            }

            break;
      }

   }

   fclose(infile);
   return 1;
}

