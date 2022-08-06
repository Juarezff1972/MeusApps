



/* *************************************************************
************************************************************* */

#include <stdlib.h>
#ifdef DOS
#include <conio.h>
#endif

#include <string.h>
#include "pixcon.h"


/* **************************************************
   main: program controller
************************************************** */
void main (int argc, char *argv[]) {

   int          i;
   windowtype   gwindow;
   listtype     mainlist;               // ptr to objects
   int          mapcount;               // # of pixmaps
   mapul        mapbuffer;
   int          flagscreengl;
   texbase      *tptr;
   clock_t      oldtime, newtime;


#ifndef DOS
   GLint attribs[] = {
                GLX_RGBA,
//                GLX_DEPTH_SIZE, 24,
                GLX_DOUBLEBUFFER,
                None
   };
#endif

   if (!SILENT)
      printf("Preprocessing...\n");

   init_graph();

   parseinput(argc, argv);                      // parse command line

   if (!SILENT)
      printf("Parsing script file...\n");

   if (!DEFLST)
      strcpy(SDEFLST, "object.lst");

   if (!parse_list(SDEFLST, &mainlist, &mapcount))
      if (!read_list(SDEFLST, &mainlist, &mapcount))
         exit(0);

   if (!SILENT)
      printf("Processing mipmaps...\n");

   if (!NIXMIP)
      for (tptr=mainlist.tdtr; tptr != (texbase *)NULL; tptr=tptr->next)
         if (tptr->query_whatami() == TEXTURE)
            ((texture *)tptr)->build_mipmaps();

   if (!NOSCREEN)
#ifdef DOS
      if (X2)
         gwindow.win.AOGLreset(argc, argv, WINX+WINX, WINY+WINY);
      else
         gwindow.win.AOGLreset(argc, argv, WINX, WINY);
#else
      if (X2)
         gwindow.win.AOGLreset(argc, argv, WINX+WINX, WINY+WINY, "Pixcon", attribs);
      else
         gwindow.win.AOGLreset(argc, argv, WINX, WINY, "Pixcon", attribs);
#endif

   flagscreengl = (!(NOSCREEN || PENCILTEST));

   if (PENCILTEST)
      gpline.build(mapcount);

   if (X2)
      mapbuffer.init_map(WINX, WINY*4);
   else
      mapbuffer.init_map(WINX, WINY*3);

   if (X2) {
      mapbuffer.maxx = WINX+WINX;
      mapbuffer.maxy = WINY+WINY;
   }

   else
      mapbuffer.maxy = WINY;

   for (i=0; i<mapcount; i++) {
      if (mainlist.camdtr[i] == (camera *)NULL) {
         mainlist.camdtr[i] = new camera(WINX, WINY);
         mainlist.camdtr[i]->preprocess(NULL);
      }

      if (mainlist.lightdtr[i] == (light *)NULL && mainlist.beamdtr[i] == (light *)NULL) {
         mainlist.lightdtr[i] = new far_light;
         mainlist.lightdtr[i]->preprocess(NULL);
      }

   }

#ifdef DOS
   oldtime = clock();
   while ((newtime = clock()) >= oldtime && newtime-oldtime < CLOCKS_PER_SEC);
#endif

   if (!SILENT)
      printf("Start Rendering...\n");

   oldtime = clock();

   for (i=0; i<mapcount; i++) {
      spawn(mainlist.camdtr[i], mainlist.lightdtr[i], mainlist.beamdtr[i], mainlist.header[i], mainlist.raddtr[i], i, &mapbuffer, &gwindow.zbuff);

      if (mapcount > 1)
         if (flagscreengl) {
#ifdef DOS
            if (gwindow.zbuff.compress_flag)
               gwindow.win.AOGLbitmap(&mapbuffer);
            else if (INTERLACE)
               gwindow.win.AOGLbitmapi(&mapbuffer);
            else if (X2)
               gwindow.win.AOGLbitmapx2(&mapbuffer);
            else
               gwindow.win.AOGLbitmap3(&mapbuffer);
#else
            gwindow.win.AOGLbitmap(&mapbuffer);
#endif
         }

         else if (PENCILTEST)
            gwindow.win.AOGLbitmap(i, &gpline);

      if (!SILENT)
         printf("FINISHED %d :)\n", i);
   }

   newtime = clock();

   if (!SILENT) {
      printf("######  ####### #     # #######\n");
      printf("#     # #     # ##    # #\n");
      printf("#     # #     # # #   # #\n");
      printf("#     # #     # #  #  # #####\n");
      printf("#     # #     # #   # # #\n");
      printf("#     # #     # #    ## #\n");
      printf("######  ####### #     # #######\n");

      printf("Average rendering time = %f\n", (newtime - oldtime)/(float)(mapcount*CLOCKS_PER_SEC));
      printf("Have a nice day :)\n");
   }

   if (mapcount == 1)
      if (PENCILTEST)
         gwindow.win.AOGLbitblt(0, &gpline);
      else if (flagscreengl) {
#ifdef DOS
         if (gwindow.zbuff.compress_flag)
            gwindow.win.AOGLbitblt(&mapbuffer);
         else if (INTERLACE)
            gwindow.win.AOGLbitblti(&mapbuffer);
         else if (X2)
            gwindow.win.AOGLbitbltx2(&mapbuffer);
         else
            gwindow.win.AOGLbitblt3(&mapbuffer);
#else
         gwindow.win.AOGLbitblt(&mapbuffer);
#endif
   }

   if (!NOSCREEN)
      gwindow.win.ALEXcancel();         // independent of OGL/X
}
