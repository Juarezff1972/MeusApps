/* *************************************************************
************************************************************* */
#include <stdlib.h>
#include <string.h>

#include "alex.h"
#include "texture.h"

/* **************************************************
   main: program controller
************************************************** */
void main (int argc, char *argv[]) {

   texture    *tex;
   int WINX, WINY;
   AOGLwindow gwindow;

#ifdef DOS
   mapul mcanvas;
   int i;
#endif

   if (argc < 2) {
      printf("usage: viewer <rgb>\n");
      exit(0);
   }

   tex = new texture;

   if (!tex->read_texture(argv[1], 1))
      exit(0);

   WINX = tex->maxx;
   WINY = tex->maxy;

#ifdef DOS
   gwindow.AOGLreset(argc, argv, 640, 480);

   mcanvas.init_map(640, 480);
   memset(mcanvas.data, 0, 640*480*4);

   if (WINX > 640)
      WINX = 640;

   if (WINY > 480)
      WINY = 480;

   for (i=0; i<WINY; i++)
      memcpy(mcanvas.pdata[WINY-1-i], tex->tob.pdata[i], WINX*4);

   gwindow.AOGLbitblt(&mcanvas);
#else
   gwindow.AOGLreset(argc, argv, WINX, WINY, "Viewer");
   gwindow.AOGLbitblt(&tex->tob);
#endif

   delete tex;

   gwindow.ALEXcancel();
}