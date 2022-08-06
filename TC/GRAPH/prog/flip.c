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

   texture      *tex;
   AOGLwindow   gwindow;
   int          WINX, WINY;
   int  i, j, k;
   unsigned int timer;
   float fps;

#ifdef DOS
   mapul mcanvas;
#else
   int *maxx, *maxy;
   XEvent mevent;
#endif

   if (argc < 3) {
      printf("usage: flip frames/sec frame1 frame2 ...\n");
      exit(0);
   }

   j = argc-2;
   fps = atof(argv[1]) * CLOCKS_PER_SEC;

   if (j<=0)
      exit(0);

   tex = new texture[j];
   for (i=0; i<j; i++)
      tex[i].read_texture(argv[i+2], 1);

#ifdef DOS

   gwindow.AOGLreset(argc, argv, 640, 480);
   mcanvas.init_map(640, 480);

   i=0;

   while(1) {
      timer = clock();

      memset(mcanvas.data, 0, 640*480*4);

      WINX = tex[i].maxx;
      WINY = tex[i].maxy;
      if (WINX > 640)
         WINX = 640;

      if (WINY > 480)
         WINY = 480;

      for (k=0; k<WINY; k++)
         memcpy(mcanvas.pdata[WINY-1-k], tex[i].tob.pdata[k], WINX*4);

      gwindow.AOGLbitmap(&mcanvas);

      i++;
      if (i==j) {
         i=0;

         if (kbhit())
            break;
      }

      while(clock() >= timer && clock()-timer < fps);
   }

#else
   maxx = new int[j];
   maxy = new int[j];

   WINX = WINY = 0;
   for (i=0; i<j; i++) {
      k = tex[i].maxx;
      if (k > WINX)
         WINX = k;

      k = tex[i].maxy;
      if (k > WINY)
         WINY = k;
   }

   for (i=0; i<j; i++) {
      k = tex[i].maxx;

      if (WINX > k)
         maxx[i] = (WINX-k)>>1;
      else
         maxx[i] = 0;

      k = tex[i].maxy;

      if (WINY > k)
         maxy[i] = (WINY-k)>>1;
      else
         maxy[i] = 0;
   }

   gwindow.AOGLreset(argc, argv, WINX, WINY, "Flip");

   i=0;

   while(1) {
      timer = clock();

      glRasterPos2i(maxx[i], maxy[i]);
      glClear(GL_COLOR_BUFFER_BIT);
      gwindow.AOGLbitmap(&tex[i].tob);

      while(clock() >= timer && clock()-timer < fps);
      i++;
      if (i==j) {
         i=0;

         XCheckMaskEvent (gwindow.mdisplay, KeyPressMask | ButtonPressMask, &mevent);
         if (mevent.type == KeyPress || mevent.type == ButtonPress)
            break;
      }

   }

   delete [] maxx;
   delete [] maxy;
#endif

   delete [] tex;

   gwindow.ALEXcancel();
}