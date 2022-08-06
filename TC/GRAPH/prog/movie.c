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

   rgb          tex;
   sfile        *file;
   AOGLwindow   gwindow;
   int          WINX, WINY;
   int  i, j, k;
   unsigned int timer;
   float fps;
   float zoom = 1;
   int start;

#ifdef DOS
   mapul mcanvas;

   if (argc < 3) {
      printf("usage: movie frames/sec frame1 frame2 ...\n");
      exit(0);
   }

   j = argc - 2;
   start = 2;

#else
   XEvent mevent;

   if (argc < 3) {
      printf("usage: movie frames/sec <-z #> frame1 frame2 ...\n");
      exit(0);
   }

   if (argv[2][0] == '-' && (argv[2][1] == 'z')) {
      zoom = atof(argv[3]);
      j = argc - 4;
      start = 4;
   }

   else {
      j = argc - 2;
      start = 2;
   }

#endif

   fps = atof(argv[1]) * CLOCKS_PER_SEC;

   if (j<=0)
      exit(0);

   file = new sfile[j];
   for (i=0; i<j; i++)
      file[i].scan_data(argv[start+i]);

   tex.scan_data(&file[0], 1);

#ifdef DOS

   WINX = tex.query_x();
   WINY = tex.query_y();

   if (WINX > 640)
      WINX = 640;

   if (WINY > 480)
      WINY = 480;

   gwindow.AOGLreset(argc, argv, 640, 480);
   mcanvas.init_map(640, 480);

   i=0;

   while(1) {
      timer = clock();

      memset(mcanvas.data, 0, 640*480*4);

      for (k=0; k<WINY; k++)
         memcpy(mcanvas.pdata[WINY-1-k], tex.pdata[k], WINX*4);

      gwindow.AOGLbitmap(&mcanvas);

      i++;
      if (i==j) {
         i=0;

         if (kbhit())
            break;
      }

      file[i].sseek(0);
      tex.scan_data(&file[i], 1);

      while(clock() >= timer && clock()-timer < fps);
   }


#else
   WINX = (int)(tex.query_x()*zoom);
   WINY = (int)(tex.query_y()*zoom);

   gwindow.AOGLreset(argc, argv, WINX, WINY, "RGB Movie");
   glPixelZoom(zoom, zoom);

   i=0;
   while(1) {
      timer = clock();

      gwindow.AOGLbitmap(&tex);

      i++;
      if (i==j) {
         i=0;

         XCheckMaskEvent (gwindow.mdisplay, KeyPressMask | ButtonPressMask, &mevent);
         if (mevent.type == KeyPress || mevent.type == ButtonPress)
            break;
      }

      file[i].sseek(0);
      tex.scan_data(&file[i], 1);

      while(clock() >= timer && clock()-timer < fps);
   }

#endif

   delete [] file;

   gwindow.ALEXcancel();
}
