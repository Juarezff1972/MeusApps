/* *************************************************************
************************************************************* */

#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef DOS
#include <conio.h>
#endif

#include "alex.h"
#include "texture.h"

/* **************************************************
   main: program controller
************************************************** */
void main (int argc, char *argv[]) {

   texture      tex;
   AOGLwindow	gwindow;
   int		WINX, WINY;
   int  i, j, k;
   unsigned int timer;
   float fps;

#ifdef DOS
   mapul mcanvas;
#else
   XEvent mevent;
#endif

   if (argc < 3) {
      printf("usage: mviewer frames/sec frame1 frame2 ...\n");
      exit(0);
   }

   j = argc-2;
   fps = atof(argv[1]) * CLOCKS_PER_SEC;

   if (j<=0)
      exit(0);

   tex.read_texture(argv[2], 1);

#ifdef DOS

   gwindow.AOGLreset(argc, argv, 640, 480);
   mcanvas.init_map(640, 480);

   i=0;

   while(1) {
      timer = clock();

      memset(mcanvas.data, 0, 640*480*4);

      WINX = tex.maxx;
      WINY = tex.maxy;
      if (WINX > 640)
         WINX = 640;

      if (WINY > 480)
         WINY = 480;

      for (k=0; k<WINY; k++)
         memcpy(mcanvas.pdata[WINY-1-k], tex.tob.pdata[k], WINX*4);

      gwindow.AOGLbitmap(&mcanvas);

      i++;
      if (i==j) {
         i=0;

         if (kbhit())
            break;
      }

      tex.read_texture(argv[i+2], 1);

      while(clock() >= timer && clock()-timer < fps);
   }

#else

   WINX = tex.maxx;
   WINY = tex.maxy;

   gwindow.AOGLreset(argc, argv, WINX, WINY, "Movie Viewer");

   i=0;
   while(1) {
      timer = clock();

      glClear(GL_COLOR_BUFFER_BIT);
      gwindow.AOGLbitmap(&tex.tob);

      i++;
      if (i==j) {
         i=0;

         XCheckMaskEvent (gwindow.mdisplay, KeyPressMask | ButtonPressMask, &mevent);
         if (mevent.type == KeyPress || mevent.type == ButtonPress)
            break;
      }

      tex.read_texture(argv[i+2], 1);

      while(clock() >= timer && clock()-timer < fps);
   }

#endif

   gwindow.ALEXcancel();
}