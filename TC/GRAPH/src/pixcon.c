



/* *************************************************************
************************************************************* */

#include <string.h>
#include "pixcon.h"
#include "laser.h"


/* **************************************************
   main: program controller
************************************************** */
void main (int argc, char *argv[]) {

   int          i;
   windowtype	gwindow;
   listtype     mainlist;               // ptr to objects
   int          mapcount;               // # of pixmaps
   mapul        *mapbuffer = (mapul *)NULL;
   int          flagscreengl;
   texbase	*tptr;
   clock_t	oldtime, newtime;

   GLint attribs[] = {
                GLX_RGBA,
//                GLX_DEPTH_SIZE, 24,
                GLX_DOUBLEBUFFER,
                None
   };

   printf("Parsing command-line arguments...\n");
   parseinput(argc, argv);			// parse command line

   if (!SILENT)
      printf("Preprocessing...\n");

   init_graph();

   if (!SILENT)
      printf("Parsing script file...\n");

   if (!DEFLST)
      strcpy(SDEFLST, "object.lst");

   if (!parse_list(SDEFLST, &mainlist, &mapcount, shades))
      read_list(SDEFLST, &mainlist, &mapcount, shades);

   if (!SILENT)
      printf("Processing mipmaps...\n");

   if (!NIXMIP)
      for (tptr=mainlist.tdtr; tptr != (texbase *)NULL; tptr=tptr->next)
         if (tptr->query_whatami() == TEXTURE)
            ((texture *)tptr)->build_mipmaps();

   if (!NOSCREEN)
      gwindow.win.AOGLreset(argc, argv, WINX, WINY, "Pixcon", attribs);

   flagscreengl = (!(NOSCREEN || PENCILTEST));

   if (flagscreengl)
      mapbuffer = new mapul[mapcount];
   else if (PENCILTEST)
      gpline.build(mapcount);

   if (RENDER) {
      if (LQ4000_init("/dev/ttyd2")) {
         printf("Could not open laser disk...\n");
         exit(-1);
      }

      if (LQ4000_rec_standby(RENDER_START, RENDER_END)) {
         printf("Bad start/stop frames...\n");
         exit(-1);
      }

   }

   for (i=0; i<mapcount; i++) {
      if (flagscreengl)
         mapbuffer[i].init_map(WINX, WINY);

      if (mainlist.camdtr[i] == (camera *)NULL) {
         mainlist.camdtr[i] = new camera(WINX, WINY);
         mainlist.camdtr[i]->preprocess(NULL);
      }

      if (mainlist.lightdtr[i] == (light *)NULL) {
         mainlist.lightdtr[i] = new far_light;
         mainlist.lightdtr[i]->preprocess(NULL);
      }

   }

   if (!SILENT)
      printf("Start Rendering...\n");

   oldtime = clock();

   for (i=0; i<mapcount; i++) {
      if (flagscreengl) {

         spawn(mainlist.camdtr[i], mainlist.lightdtr[i], mainlist.header[i], mainlist.raddtr[i], i, &mapbuffer[i], &gwindow.zbuff);

         if (mapcount > 1)
            gwindow.win.AOGLbitmap(&mapbuffer[i]);
      }

      else {
         spawn(mainlist.camdtr[i], mainlist.lightdtr[i], mainlist.header[i], mainlist.raddtr[i], i, (mapul *)NULL, &gwindow.zbuff);

         if (mapcount > 1 && PENCILTEST)
            gwindow.win.AOGLbitmap(i, &gpline);
      }

      if (RENDER)
         if (LQ4000_rec()) {
            printf("Laser disk error...\n");
            exit(-1);
         }

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

   if (PENCILTEST)
      if (mapcount == 1)
         gwindow.win.AOGLbitblt(0, &gpline);
      else
         gwindow.win.AOGLanimate(mapcount, &gpline);
   else
      if (flagscreengl)
         if (mapcount == 1)
            gwindow.win.AOGLbitblt(mapbuffer);                       // pixcon
         else
            gwindow.win.AOGLanimate(mapcount, mapbuffer);            // animaron && reannie

   if (RENDER)
      LQ4000_close();

   if (!NOSCREEN)
      gwindow.win.ALEXcancel();		// independent of OGL/X

   if (mapbuffer != (mapul *)NULL)
      delete [] mapbuffer;
}
