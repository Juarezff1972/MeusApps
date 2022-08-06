



/* *************************************************************
   This program is a keyframe-hiearchical script interpretor
   designed to create an animation datafile in the PIXCON
   data ascii file format

   by: Alex Harper              12/20/93
************************************************************* */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "anitroll.h"

/* **************************************************
   this function parses command line to find user defined files,
   or else use the default files.
************************************************** */
void parseinput(int argc, char **argv, int *parselist) {

   int i;

   for (i=1; i<argc; i++)
      if (*(argv[i]) == '-')
         switch (*(argv[i]+1)) {
            case 't':                   // test mode
               parselist[WF] = 1;
               break;

            case 'f':                   // partial animation
               parselist[FRAME] = 1;
               i++;
               parselist[FRAME+1] = atoi(argv[i]);
               i++;
               parselist[FRAME+2] = atoi(argv[i]);
               break;

            case 'c':			// continue numbering (must have -f)
               parselist[CONTINUE] = 1;
               break;

            default:
               break;
         }

}


/* *************************************************************
   This function reads in the data for the animation
************************************************************* */
atom *read_data(char *filename, int *framecount,
                float floor[]) {

   FILE       *infile;                          // input file
   int        count;				// # of objects
   atom       *header = (atom *)NULL;           // list of objects
   atom       *ptr;                             // temp ptr
   int        i;                                // looping var
   eventtype  *rtr, *qtr, *fptr;                // temp pointers
   char       errorflag;
   char token[MAXSTRLEN];

   infile = fopen(filename, "r");

   get_token(infile, token);
   lower_case(token);

   if (strcmp(token, "anitroll")) {
      printf("ERROR: Input file not an Anitroll script... Aborting...\n");
      fclose(infile);
      exit(0);
   }

   get_token(infile, token);		// frame count
   get_token(infile, token);
   *framecount = atoi(token);

   get_token(infile, token);		// shadow plane
   get_token(infile, token);
   floor[0] = atof(token);
   get_token(infile, token);
   floor[1] = atof(token);
   get_token(infile, token);
   floor[2] = atof(token);
   get_token(infile, token);
   floor[3] = atof(token);

   get_token(infile, token);		// atom count
   get_token(infile, token);
   count = atoi(token);

   for (i=0; i<count; i++) {

      ptr = new atom;

      get_token(infile, token);		// atm filename
      strcpy(ptr->name, token);

      ptr->read_data(token);

      get_token(infile, token);

      do {
         if ((errorflag = !get_token(infile, token)) || token[0] == '}')
            break;

         lower_case(token);

         if (!strcmp(token, "name")) {
            get_token(infile, ptr->name);	// atm name
         }

         else {
            rtr = new eventtype;

            get_token(infile, token);
            rtr->event = atoi(token);

            get_token(infile, token);
            get_token(infile, token);
            rtr->timefactor = atof(token);

            get_token(infile, token);
            get_token(infile, rtr->efilename);

            if (ptr->event == (eventtype *)NULL || rtr->event <= ptr->event->event) {
               if (ptr->event == (eventtype *)NULL)
                  fptr = rtr;
               rtr->next  = ptr->event;
               ptr->event = rtr;
            }

            else if (fptr->event <= rtr->event) {
               rtr->next  = (eventtype *)NULL;
               fptr->next = rtr;
               fptr       = rtr;
            }

            else {
               qtr = ptr->event;

               while (qtr->next->event < rtr->event)
                  qtr = qtr->next;

               rtr->next = qtr->next;
               qtr->next = rtr;
            }

         }

      } while (1);

      if (errorflag) {
         printf("ERROR: EOF reached before token \"}\"... Aborting...\n");
         fclose(infile);
         exit(1);
      }

      ptr->next = header;
      header = ptr;
   }

   fclose(infile);
   return header;
}


/* ****************************************************************
   This is the animation controller - main function
**************************************************************** */
void main(int argc, char **argv) {

   atom   *aheader, *atr;               // object pointers
   int    framemax;                     // # of frames
   int    obcount = 0;                  // # of objects
   float  floor[4];                     // floor plane for shadows
   int    i, j;                         // looping var
   FILE   *outfile;                     // output file
   int    tflag[MAXOPTION] = {0,0,0,0,0,0,0,0,0,0}; // flags for input parameters
   molecule *ob, *ptr;                   // list to be printed to a file

   if (argc < 3) {
      printf("usage : anitroll <script file> <output file> [-test] [-frame # #] [-c]\n");
      exit(0);
   }

   init_lut(MASK_SIN_COS | MASK_ASIN_ACOS);

   aheader = read_data(argv[1], &framemax, floor);
   outfile = fopen(argv[2], "w");       // print frames

   parseinput(argc, argv, tflag);

   if (!tflag[1]) {
      tflag[2] = 1;
      tflag[3] = framemax;
   }

   else {
      if (tflag[2] < 1)
         tflag[2] = 1;

      if (tflag[3] < tflag[2]) {
         printf("Invalid frame numbers...\n");
         exit(0);
      }

   }

   ob = (molecule *)NULL;
                                        // update animation to start frame
   for (i=1; i<tflag[2]; i++)
      for (atr=aheader; atr!=(atom *)NULL; atr=atr->next) {
         atr->new_action(i, &aheader);
         atr->whereami(i, &ob, (int *)NULL, 0, aheader);
         atr->update((quark *)NULL);
      }

   if (tflag[CONTINUE])
      j = i;
   else
      j = 1;
                                         // calculate animation
   for (; i<=tflag[3]; i++, j++)
      for (atr=aheader; atr!=(atom *)NULL; atr=atr->next) {
         atr->new_action(i, &aheader);
         atr->whereami(j, &ob, &obcount, 1, aheader);
         atr->update((quark *)NULL);
      }

                                        // print # of frames & objects
   fprintf(outfile, "%d %d\n", tflag[3]-tflag[2]+1, obcount);
                                        // print object data
   for (ptr=ob; ptr!=(molecule *)NULL; ptr=ob) {
      ptr->print_data(outfile, tflag[0], floor);
      ob = ptr->next;
      delete ptr;
   }

   fclose(outfile);

   if (aheader != (atom *)NULL)
      delete aheader;
}