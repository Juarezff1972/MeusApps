



/***********************************************************

This file was hacked from xsample.c to perform the basic
X windows functions, and modified for my purposes.

hacked by: Alex Harper
***********************************************************/


#include <string.h>
#include <stdlib.h>

#include "alex.h"


#ifdef DOS
#include <conio.h>

/* **************************************************
                              bressem line
************************************************** */
void ALEXwindow::ALEXline(int x0, int y0, int x1, int y1) {

   int absdeltax, absdeltay;
   int deltax, deltay;
   int i, j, k;                   // loop counter
   int yinc, xinc;
   int run_length, rlplus, rlinc, rlflag;

   if (x0 < x1) {
      if (x0 < 0 || x1 >= WINX)
         return;
   }

   else if (x1 < 0 || x0 >= WINX)
      return;

   if (y0 < y1) {
      if (y0 < 0 || y1 >= WINY)
         return;
   }

   else if (y1 < 0 || y0 >= WINY)
      return;

   y0 = WINY-1-y0;
   y1 = WINY-1-y1;

   if (y0 == y1) {                      // horizontal
      if (x1 < x0) {
         j  = x0;
         x0 = x1;
         x1 = j;
      }

      else if (x1==x0)
         x1++;

      vesa_hline32(y0, x0, x1, 0xff);

      return;
   }

   if (x0 == x1) {                      // vertical
      if (y1 < y0)
         vesa_vline32(y1,  y0, x0, 0xff);
      else
         vesa_vline32(y0,  y1, x0, 0xff);

      return;
   }

   deltax = x1 - x0;
   absdeltax = abs(deltax);
   deltay = y1 - y0;                 // (WINY-y1)-(WINY-y0)
   absdeltay = abs(deltay);

   if (absdeltax > absdeltay) {         // left to right

      if (deltax < 0) {                    // deltax is pos
         j = x1;
         k = y1;
         x1 = x0;
         y1 = y0;
         x0 = j;
         y0 = k;

         yinc = -sgn(deltay);
      }

      else
         yinc = sgn(deltay);

      k = 0;

      run_length = absdeltax / absdeltay;
      rlinc = absdeltax % absdeltay;

      if (run_length < 3) {             // bresseham's

         for (; x0<x1; x0++) {
            vesa_put_pixel32(y0, x0, 0xff);
            k += absdeltay;

            if (k > absdeltax) {
               k -= absdeltax;
               y0 += yinc;
            }

         }

         return;
      }

                                // bresseham's run length
      rlflag = 0;
      rlplus = run_length + 1;

      for (j=run_length; j <= absdeltax;) {
         vesa_hline32(y0, x0, i=x0+run_length, 0xff);

         if (rlflag) {
            vesa_put_pixel32(y0, i, 0xff);
            i++;
            rlflag = 0;
         }

         x0 =  i;
         y0 += yinc;
         k  += rlinc;

         if (k > absdeltay) {
            j += rlplus;
            k -= absdeltay;
            rlflag = 1;
         }

         else
            j += run_length;
      }

      vesa_hline32(y0, x0, x1, 0xff);

      return;
   }

   if (deltay < 0) {                    // deltax is pos
      j = x1;
      x1 = x0;
      x0 = j;

      j = y1;
      y1 = y0;
      y0 = j;

      xinc = sgn(-deltax);
   }

   else
      xinc = sgn(deltax);

   k = 0;
                                // down to up
   run_length = absdeltay / absdeltax;
   rlinc = absdeltay % absdeltax;

   if (run_length < 3) {                // bresseham's

      for (; y0<y1; y0++) {     // down to up
         vesa_put_pixel32(y0, x0, 0xff);

         k += absdeltax;

         if (k > absdeltay) {
            k -= absdeltay;
            x0 += xinc;
         }

      }

       return;
   }
                                        // bresseham's run length
   rlflag = 0;
   rlplus = run_length + 1;

   for (j=run_length; j <= absdeltay;) {
      vesa_vline32(y0,  i=y0+run_length, x0, 0xff);

      if (rlflag) {
         vesa_put_pixel32(i, x0, 0xff);
         i++;
         rlflag = 0;
      }

      y0 = i;
      k += rlinc;
      x0 += xinc;

      if (k > absdeltax) {
         j += rlplus;
         k -= absdeltax;
         rlflag = 1;
      }

      else
         j += run_length;
   }

   vesa_vline32(y0, y1, x0, 0xff);
}


void AOGLwindow::AOGLbitblt(mapul *mcanvas) {

    vesa_bitblt((unsigned char *)mcanvas->data);
    getch();
}


void AOGLwindow::AOGLbitblt3(mapul *mcanvas) {

    vesa_bitblt3((unsigned char *)mcanvas->data);
    getch();
}


void AOGLwindow::AOGLbitblti(mapul *mcanvas) {

    unsigned int *buffer = mcanvas->data+WINX*3;
    int i, j;

    for (i=1, j=WINX*6; i<WINY; i+=2, buffer += j)
       vesa_lineblt3((unsigned char *)buffer, i);

    getch();
}


void AOGLwindow::AOGLbitbltx2(mapul *mcanvas) {

    vesa_bitbltx2((unsigned char *)mcanvas->data);
    getch();
}


void AOGLwindow::AOGLbitblt(int framenum, linelist *mcanvas) {

   line_type *ptr;

   if (framenum >= mcanvas->count)
      return;

   vesa_clear_screen();

   for (ptr=mcanvas->list[framenum]; ptr; ptr=ptr->next)
      ALEXline(ptr->pt[0] >> 16, WINY-1-(ptr->pt[0] & 0xffff), ptr->pt[1] >> 16, WINY-1-(ptr->pt[1] & 0xffff));

    getch();
}


void AOGLwindow::AOGLbitmap(mapul *mcanvas) {

    vesa_bitblt((unsigned char *)mcanvas->data);
}


void AOGLwindow::AOGLbitmap3(mapul *mcanvas) {

    vesa_bitblt3((unsigned char *)mcanvas->data);
}


void AOGLwindow::AOGLbitmapi(mapul *mcanvas) {

    unsigned int *buffer = mcanvas->data+WINX*3;
    int i, j;

    for (i=1, j=WINX*6; i<WINY; i+=2, buffer += j)
       vesa_lineblt3((unsigned char *)buffer, i);
}


void AOGLwindow::AOGLbitmapx2(mapul *mcanvas) {

    vesa_bitbltx2((unsigned char *)mcanvas->data);
}


void AOGLwindow::AOGLbitmap(int framenum, linelist *mcanvas) {

   line_type *ptr;

   if (framenum >= mcanvas->count)
      return;

   vesa_clear_screen();

   for (ptr=mcanvas->list[framenum]; ptr; ptr=ptr->next)
      ALEXline(ptr->pt[0] >> 16, WINY-1-(ptr->pt[0] & 0xffff), ptr->pt[1] >> 16, WINY-1-(ptr->pt[1] & 0xffff));
}


void AOGLwindow::AOGLreset(int argc, char *argv[], int maxx, int maxy) {

   WINX = maxx;
   WINY = maxy;

   if (!read_vesa_head()) {
      printf("Could not read vesa header...\n");
      exit(1);
   }

    start_mode = get_mode();

    if (!set_mode(WINX,WINY,32))
       if (!set_mode(WINX,WINY,24)) {
          printf("Could not read set mode %dx%dx32/24\n", WINX, WINY);
          exit(1);
       }

}

#else

#define BORDER_WIDTH   0 //4           // The width of the window border

GLint  gattribs[] = {
                GLX_RGBA,
                GLX_RED_SIZE, 3,
                GLX_GREEN_SIZE, 3,
                GLX_BLUE_SIZE, 2,
//              GLX_DEPTH_SIZE, 24,
                GLX_DOUBLEBUFFER,

//              GLX_RED_SIZE, 4,
//              GLX_GREEN_SIZE, 4,
//              GLX_BLUE_SIZE, 4,
//              GLX_ACCUM_RED_SIZE, 4,
//              GLX_ACCUM_GREEN_SIZE, 4,
//              GLX_ACCUM_BLUE_SIZE, 4,
//              GLX_ALPHA_SIZE, 4,

                None
};


/************************************************************************
************************************************************************/
void ALEXwindow::ALEXclear(ALEXimage mcanvas) {

   memset(mcanvas->data, 0, mcanvas->height*mcanvas->width<<2);
}


/************************************************************************
 This function renames the XFillRectangle function
************************************************************************/
void ALEXwindow::ALEXfillrect(int y, int z, int a, int b, ALEXpixmap *x) {

   XFillRectangle(mdisplay, *x, mGC, y, WINY-z, a-y, b-z);
}


/************************************************************************
 This function renames the XFillRectangle function
************************************************************************/
void ALEXwindow::ALEXfillrect(int y, int z, int a, int b) {

   XFillRectangle(mdisplay, mwindow, mGC, y, WINY-z, a-y, b-z);
   XFlush(mdisplay);
}


/************************************************************************
 This function renames the XFillRectangle function
************************************************************************/
void ALEXwindow::ALEXrect(int y, int z, int a, int b, ALEXpixmap *x) {

   XDrawRectangle(mdisplay, *x, mGC, y, WINY-z, a-y, b-z);
}


/************************************************************************
 This function renames the XFillRectangle function
************************************************************************/
void ALEXwindow::ALEXrect(int y, int z, int a, int b) {

   XDrawRectangle(mdisplay, mwindow, mGC, y, WINY-z, a-y, b - z);
   XFlush(mdisplay);
}


/************************************************************************
************************************************************************/
void ALEXwindow::ALEXrect(int x0, int y0, int x1, int y1, unsigned int color, ALEXimage mcanvas) {

   ALEXline(x0, y0, x0, y1, color, mcanvas);
   ALEXline(x0, y1, x1, y1, color, mcanvas);
   ALEXline(x1, y1, x1, y0, color, mcanvas);
   ALEXline(x1, y0, x0, y0, color, mcanvas);
}


// 
/************************************************************************
   This function tests whether a point is w/in the viewplane
************************************************************************/
int ALEXwindow::ALEXon_screen(int pt[]) {

   if (pt[0] >-1 && pt[0] < WINX && pt[1] > -1 && pt[1] < WINY)
      return 1;

   return 0;
}


/************************************************************************
   This function draws a point
************************************************************************/
void ALEXwindow::ALEXpoint(int pt[]) {

   XDrawPoint (mdisplay, mwindow, mGC, pt[0], WINY-pt[1]);
   XFlush(mdisplay);
}


/************************************************************************
   This function draws a point
************************************************************************/
void ALEXwindow::ALEXpoint(int size, int pt[][2]) {

   int i;

   for (i=0; i<size; i++)
      XDrawPoint (mdisplay, mwindow, mGC, pt[i][0], WINY-pt[i][1]);

   XFlush(mdisplay);
}


/************************************************************************
   This function draws a point on a pixmap
************************************************************************/
void ALEXwindow::ALEXpoint(int pt[], ALEXpixmap *mcanvas) {

   XDrawPoint (mdisplay, *mcanvas, mGC, pt[0], WINY-pt[1]);
}


/************************************************************************
   This function draws a point on a pixmap
************************************************************************/
void ALEXwindow::ALEXpoint(int size, int pt[][2], ALEXpixmap *mcanvas) {

   int i;

   for (i=0; i<size; i++)
      XDrawPoint (mdisplay, *mcanvas, mGC, pt[i][0], WINY-pt[i][1]);
}


/* **************************************************
************************************************** */
void ALEXwindow::ALEXpoint(int x0, int y0, unsigned int color, ALEXimage mcanvas) {

   ((unsigned int *)mcanvas->data)[(WINY-y0)*mcanvas->width+x0] = color;
}

/*  */
/************************************************************************
   This function draws a line
************************************************************************/
void ALEXwindow::ALEXline(int pt1[], int pt2[]) {

   XDrawLine(mdisplay, mwindow, mGC, pt1[0], WINY-pt1[1], pt2[0], WINY-pt2[1]);
   XFlush(mdisplay);
}


/************************************************************************
   This function draws a line
************************************************************************/
void ALEXwindow::ALEXline(int size, int pt1[][2]) {

   int i;

   if (size > 1) {
      for (i=0; i<size-1; i++)
         XDrawLine(mdisplay, mwindow, mGC, pt1[i][0], WINY-pt1[i][1], pt1[i+1][0], WINY-pt1[i+1][1]);

      XFlush(mdisplay);
   }

}


/************************************************************************
   This function draws a line on a pixmap
************************************************************************/
void ALEXwindow::ALEXline(int pt1[], int pt2[], ALEXpixmap *mcanvas) {

   XDrawLine(mdisplay, *mcanvas, mGC, pt1[0], WINY-pt1[1], pt2[0], WINY-pt2[1]);
}


/************************************************************************
   This function draws a line
************************************************************************/
void ALEXwindow::ALEXline(int size, int pt1[][2], ALEXpixmap *mcanvas) {

   int i;

   if (size > 1)
      for (i=0; i<size-1; i++)
         XDrawLine(mdisplay, *mcanvas, mGC, pt1[i][0], WINY-pt1[i][1], pt1[i+1][0], WINY-pt1[i+1][1]);
}


/* **************************************************
                              bressem line
************************************************** */
void ALEXwindow::ALEXline(int x0, int y0, int x1, int y1, unsigned int color, ALEXimage mcanvas) {

   int         i, x;
   int          absdeltax, absdeltay;
   int          deltax, deltay;
   int          j, k;                   // loop counter
   int          yinc;
   int          run_length, rlplus, rlinc, rlflag;

   if (x0 < x1) {
      if (x0 < 0 || x1 >= WINX)
         return;
   }

   else
      if (x1 < 0 || x0 >= WINX)
         return;

   if (y0 < y1) {
      if (y0 < 0 || y1 >= WINY)
         return;
   }

   else
      if (y1 < 0 || y0 >= WINY)
         return;

   if (y0 == y1) {                      // horizontal
      if (x1 < x0) {
         j  = x0;
         x0 = x1;
         x1 = j;
      }

      x = (WINY-1-y0)*mcanvas->width+x0;

      for (i=x+(x1-x0); x<=i; x++)
         ((unsigned int *)mcanvas->data)[x] = color;

      return;
   }

   if (x0 == x1) {                      // vertical
      if (y1 < y0) {
         j  = y0;
         y0 = y1;
         y1 = j;
      }

      i = (WINY-1-y0)*mcanvas->width+x0;
      x = (WINY-1-y1)*mcanvas->width+x0;

      for (; x<i; x+=mcanvas->width)
         ((unsigned int *)mcanvas->data)[x] = color;

      return;
   }

   deltax = x1 - x0;
   absdeltax = abs(deltax);
   deltay = y0 - y1;                 // (WINY-y1)-(WINY-y0)
   absdeltay = abs(deltay);

   if (deltax < 0) {                    // deltax is pos
      j = x1;
      x1 = x0;
      x0 = j;

      j = y1;
      y1 = y0;
      y0 = j;

      yinc = mcanvas->width*sgn(-deltay);
   }

   else
      yinc = mcanvas->width*sgn(deltay);

   x = (WINY-1-y0)*mcanvas->width+x0;

   k = 0;
   if (absdeltax > absdeltay) {         // left to right

      run_length = absdeltax / absdeltay;
      rlinc = absdeltax % absdeltay;

      if (run_length < 3) {             // bresseham's

         for (j=0; j < absdeltax; j++, x++) {
            ((unsigned int *)mcanvas->data)[x] = color;

            k += absdeltay;

            if (k > absdeltax) {
               k -= absdeltax;
               x += yinc;
            }

         }

         return;
      }
                                // bresseham's run length
      rlflag = 0;
      rlplus = run_length + 1;

      for (j=run_length; j <= absdeltax;) {

         for (i=x+run_length; x<i; x++)
            ((unsigned int *)mcanvas->data)[x] = color;

         k += rlinc;
         x += yinc;

         if (rlflag) {
            ((unsigned int *)mcanvas->data)[x] = color;
            x++;
            rlflag = 0;
         }

         if (k > absdeltax) {
            j += rlplus;
            k -= absdeltax;
            rlflag = 1;
         }

         else
            j += run_length;
      }

      for (j -= (rlflag ? rlplus : run_length), i=x+(absdeltax-j); x < i; x++)
         ((unsigned int *)mcanvas->data)[x] = color;

      return;
   }
                                // down to up
   run_length = absdeltay / absdeltax;
   rlinc = absdeltay % absdeltax;

   if (run_length < 3) {                // bresseham's

      for (j=0; j<absdeltay; j++) {     // down to up
         ((unsigned int *)mcanvas->data)[x] = color;

         x += yinc;
         k += absdeltax;

         if (k > absdeltay) {
            k -= absdeltay;
            x++;
         }

      }

       return;
   }
                                        // bresseham's run length
   rlflag = 0;
   rlplus = run_length + 1;

   for (j=run_length; j <= absdeltay;) {

      for (i=0; i<run_length; i++, x+=yinc)
         ((unsigned int *)mcanvas->data)[x] = color;

      k += rlinc;
      x++;

      if (rlflag) {
         ((unsigned int *)mcanvas->data)[x] = color;
         x += yinc;
         rlflag = 0;
      }

      if (k > absdeltay) {
         j += rlplus;
         k -= absdeltay;
         rlflag = 1;
      }

      else
         j += run_length;
   }

   for (j -= (rlflag ? rlplus : run_length); j < absdeltay; j++, x+=yinc)
      ((unsigned int *)mcanvas->data)[x] = color;
}


/************************************************************************
   This function prints out a polygon
************************************************************************/
void ALEXwindow::ALEXpolygon(int size, int pt[][2]) {

   int i;
   int prev[2];

   prev[0] = pt[0][0];
   prev[1] = pt[0][1];

   for(i=0; i<size; i++) {
      XDrawLine(mdisplay, mwindow, mGC, prev[0], WINY-prev[1], pt[i][0],
                WINY-pt[i][1]);
      prev[0] = pt[i][0];
      prev[1] = pt[i][1];
   }

   XDrawLine(mdisplay, mwindow, mGC, prev[0], WINY-prev[1], pt[0][0], WINY-pt[0][1]);
   XFlush (mdisplay);
}
/*  */
/************************************************************************
   This function prints out a polygon on a pixmap
************************************************************************/
void ALEXwindow::ALEXpolygon(int size, int pt[][2], ALEXpixmap *mcanvas) {

   int i;
   int prev[2];

   prev[0] = pt[0][0];
   prev[1] = pt[0][1];

   for(i=0; i<size; i++) {
      XDrawLine(mdisplay, *mcanvas, mGC, prev[0], WINY-prev[1], pt[i][0],
                WINY-pt[i][1]);
      prev[0] = pt[i][0];
      prev[1] = pt[i][1];
   }

   XDrawLine(mdisplay, *mcanvas, mGC, prev[0], WINY-prev[1], pt[0][0], WINY-pt[0][1]);
}


/************************************************************************
   This function flips through an array of pixmaps, and displays them on
   the screen.  It is terminated by a mouse click on the window
************************************************************************/
void ALEXwindow::ALEXanimate(int numframes, ALEXpixmap mcanvas[]) {

   int     i;
   float  offset;
   clock_t otime, ntime;
   XEvent mevent;

   offset = CLOCKS_PER_SEC/((float)FRAMESPERSEC);

   while (1) {
      for (i=0; i<numframes; i++) {
         XCheckMaskEvent (mdisplay,KeyPressMask | ButtonPressMask, &mevent);
         if (mevent.type == KeyPress || mevent.type == ButtonPress)
            return;

         otime = clock();
         XCopyArea(mdisplay, mcanvas[i], mwindow, mGC, 0, 0, WINX, WINY, 0, 0);
         XFlush (mdisplay);
         while ( (ntime=clock()) - otime < offset && ntime >= otime);
      }

      otime = clock();
      while ( (ntime=clock()) - otime < CLOCKS_PER_SEC && ntime >= otime);
   }

}


/************************************************************************
   This function flips through an array of pixmaps, and displays them on
   the screen.  It is terminated by a mouse click on the window
************************************************************************/
void ALEXwindow::ALEXanimate(int numframes, ALEXimage mcanvas[]) {

   int     i;
   float  offset;
   clock_t otime, ntime;
   XEvent mevent;

   offset = CLOCKS_PER_SEC/((float)FRAMESPERSEC);

   while (1) {
      for (i=0; i<numframes; i++) {
         XCheckMaskEvent (mdisplay, KeyPressMask | ButtonPressMask, &mevent);
         if (mevent.type == KeyPress || mevent.type == ButtonPress)
            return;

         otime = clock();
         XPutImage(mdisplay, mwindow, mGC, mcanvas[i], 0, 0, 0, 0, WINX, WINY);
         XFlush (mdisplay);
         while ( (ntime=clock()) - otime < offset && ntime >= otime);
      }

      otime = clock();
      while ( (ntime=clock()) - otime < CLOCKS_PER_SEC && ntime >= otime);
   }

}


/************************************************************************
************************************************************************/
void ALEXwindow::ALEXfreezeframe(ALEXpixmap *mcanvas) {

   XEvent mevent;

   do {
      XCopyArea(mdisplay, *mcanvas, mwindow, mGC, 0, 0, WINX, WINY, 0, 0);
      XFlush (mdisplay);
      XNextEvent (mdisplay, &mevent);
   } while (mevent.type != KeyPress && mevent.type != ButtonPress);

}


/************************************************************************
************************************************************************/
void ALEXwindow::ALEXfreezeframe(ALEXimage mcanvas) {

   XEvent mevent;

   do {
      XPutImage(mdisplay, mwindow, mGC, mcanvas, 0, 0, 0, 0, WINX, WINY);
      XFlush (mdisplay);
      XNextEvent (mdisplay, &mevent);
   } while (mevent.type != KeyPress && mevent.type != ButtonPress);

}


/************************************************************************
************************************************************************/
void ALEXwindow::ALEXpause() {

   XEvent mevent;

   do {
      XNextEvent (mdisplay, &mevent);
   } while (mevent.type != KeyPress && mevent.type != ButtonPress);

}
/*  */
/************************************************************************
   This function displays a pixmap
************************************************************************/
void ALEXwindow::ALEXbitblt(ALEXpixmap *mcanvas) {

   XEvent mevent;

   XCopyArea(mdisplay, *mcanvas, mwindow, mGC, 0, 0, WINX, WINY, 0, 0);
   XFlush(mdisplay);

   while (1) {
      XNextEvent (mdisplay, &mevent);
      if (mevent.type == KeyPress || mevent.type == ButtonPress)
         return;

      XCopyArea(mdisplay, *mcanvas, mwindow, mGC, 0, 0, WINX, WINY, 0, 0);

      XFlush (mdisplay);
   }

}


/************************************************************************
************************************************************************/
void ALEXwindow::ALEXbitblt(ALEXimage mcanvas) {

   XEvent mevent;

   XPutImage(mdisplay, mwindow, mGC, mcanvas, 0, 0, 0, 0, WINX, WINY);
   XFlush(mdisplay);

   while (1) {
      XNextEvent (mdisplay, &mevent);
      if (mevent.type == KeyPress || mevent.type == ButtonPress)
         return;

      XPutImage(mdisplay, mwindow, mGC, mcanvas, 0, 0, 0, 0, WINX, WINY);

      XFlush (mdisplay);
   }

}


/************************************************************************
   This function displays a pixmap
************************************************************************/
void ALEXwindow::ALEXbitmap(ALEXpixmap *mcanvas) {

   XCopyArea(mdisplay, *mcanvas, mwindow, mGC, 0, 0, WINX, WINY, 0, 0);
   XFlush(mdisplay);
}


/************************************************************************
************************************************************************/
void ALEXwindow::ALEXbitmap(ALEXimage mcanvas) {

   XPutImage(mdisplay, mwindow, mGC, mcanvas, 0, 0, 0, 0, WINX, WINY);
   XFlush(mdisplay);
}


/************************************************************************
************************************************************************/
void ALEXwindow::ALEXmapcpy(ALEXpixmap *mcanvas, int x, int y, mapul *tob) {

   int    xx, yy;
   int    i, j;
   int    mpt[2][2];
   unsigned int *base;
   unsigned int temp, temp2;
   int      diff;

   if (tob->maxx > x) {
      xx = x;
      diff = tob->maxx - x;
   }

   else {
      xx = tob->maxx;
      diff = 0;
   }

   yy = (tob->maxy > y) ? y : tob->maxy;

   base = tob->data;
   for (i=0; i<yy; i++) {
      temp = *base;
      mpt[1][0] = 0;
      mpt[0][1] = mpt[1][1] = i;
      base++;

      for (j=1; j<xx; j++, base++)
         if (temp != (temp2 = *base)) {
            ALEXsetforeground(temp);
            mpt[0][0] = mpt[1][0];
            mpt[1][0] = j-1;
            ALEXline(mpt[0], mpt[1], mcanvas);
            mpt[1][0]++;
            temp = temp2;
         }

      ALEXsetforeground(temp);
      mpt[0][0] = mpt[1][0];
      mpt[1][0] = xx-1;
      ALEXline(mpt[0], mpt[1], mcanvas);
      base += diff;
   }

}


void ALEXwindow::ALEXmapcpy(ALEXimage mcanvas, mapul *tob) {

   int  xx, yy;
   int ydiff, yxdiff;
   int i, j;
   int mapsize, xmapsize;
   int yinc, yinc2, yxinc;
   unsigned int *base, *dbase;

   if (tob->maxx > mcanvas->width) {
      xx   = mcanvas->width;
      yxdiff = 0;
      ydiff  = tob->maxx-xx;
   }

   else {
      xx   = (int)tob->maxx;
      yxdiff = mcanvas->width-xx;
      ydiff  = 0;
   }

   yy = (tob->maxy > mcanvas->height) ? mcanvas->height : tob->maxy;

   yxinc  = mcanvas->width;
   yxdiff -= (yxinc<<1);
   yinc   = tob->maxx;
   yinc2 = yinc - ydiff;

   xmapsize  = mcanvas->width*mcanvas->height;
   mapsize   = yy*yinc;

   base  = tob->data;
   dbase = (unsigned int *)mcanvas->data;

   yxdiff += yinc2;
   ydiff  += yinc2;
   yinc2 = yinc2<<2;

   for (i=0, j=xmapsize-yxinc; i<mapsize; j+=yxdiff, i+=ydiff)
      memcpy(&base[j], &dbase[i], yinc2);
}


void ALEXwindow::ALEXmapcpy(mapul *mcanvas, mapul *tob) {

   unsigned int i, j, k, ptr;
   unsigned int start, inc;

   i = mcanvas->query_y();
   j = mcanvas->query_x();

   start = (i > tob->maxy) ? (((i-tob->maxy)>>1)*j) : 0;

   if (j > tob->maxx) {
      inc = j - tob->maxx;
      start += inc >> 1;
   }

   else
      inc = 0;

   inc += tob->maxx;
   j = tob->maxx<<2;
   for (i=k=0, ptr=start; i<tob->maxy; i++, ptr+=inc, k+=tob->maxx)
      memcpy(&mcanvas->data[ptr], &tob->data[k], j);
}


/************************************************************************
************************************************************************/
int ALEXwindow::ALEXqtest(unsigned int x) {

   XEvent mevent;

   if (XCheckMaskEvent (mdisplay, x, &mevent)) {
      XPutBackEvent(mdisplay, &mevent);
      return 1;
   }

   return 0;
}


/************************************************************************
************************************************************************/
int ALEXwindow::ALEXpollkeyboard(char *x) {

   return XQueryKeymap(mdisplay, x);
}


/************************************************************************
************************************************************************/
int ALEXwindow::ALEXcode2sym(unsigned int x) {

   return XKeycodeToKeysym(mdisplay, x, 0);
}


/************************************************************************
************************************************************************/
XEvent ALEXwindow::ALEXqread(unsigned int *x) {

   XEvent mevent;

   XNextEvent (mdisplay, &mevent);

   if (mevent.type == KeyPress)
      *x = XKeycodeToKeysym( mdisplay, mevent.xkey.keycode, 0 );

   return mevent;
}


/************************************************************************
************************************************************************/
void ALEXwindow::ALEXqenter(XEvent *mevent) {

   XPutBackEvent(mdisplay, mevent);
}


/************************************************************************
   This is the main function that initializes a window
************************************************************************/
void ALEXwindow::ALEXreset(int argc, char *argv[], int maxx, int maxy, char *winname) {

   XVisualInfo y;
   XSetWindowAttributes mattrib;
   XTextProperty wname;
   char *wn = "gemstone";
   XClassHint *cl;
   XEvent mevent;
   Window mroot;
   int TWENTYFOUR = 0;

   if ((mdisplay = XOpenDisplay(NULL)) == (Display *)NULL) {
      fprintf (stderr,
               "ERROR: Could not open a connection to X on display %s.\n",
               XDisplayName ("V R US"));
      exit (0);
   }

   WINX = maxx;
   WINY = maxy;

   mscreen = XDefaultScreen(mdisplay);
   mroot = XRootWindow (mdisplay, mscreen);

   y.screen = mscreen;
   y.depth = 24;
   y.c_class = TrueColor;

   if (!(vid = XGetVisualInfo(mdisplay, VisualScreenMask | VisualDepthMask | VisualClassMask, &y, &TWENTYFOUR))) {
      printf("Could not create 24 bit visual... Aborting...\n");
      exit(0);
   }

   mdepth = y.depth;
   mattrib.colormap = XCreateColormap(mdisplay, mroot, vid->visual, AllocNone);
   mattrib.event_mask =  ButtonPressMask | KeyPressMask |  ExposureMask | StructureNotifyMask;

   mwindow = XCreateWindow(mdisplay, mroot, 0, 0, WINX, WINY, BORDER_WIDTH,
                           mdepth, InputOutput, vid->visual,
                           CWBackPixel | CWBorderPixel | CWColormap | CWEventMask,
                           &mattrib);

   mGC = XCreateGC(mdisplay, mwindow, 0, NULL);

   XStringListToTextProperty(&winname, 1, &wname);
   XSetWMName(mdisplay, mwindow, &wname);
   XSetWMIconName( mdisplay, mwindow, &wname );

   cl = XAllocClassHint();
   XGetClassHint(mdisplay, mwindow, cl);
   XFree(cl->res_class);
   cl->res_class = new char[MAXSTRLEN];
   strcpy(cl->res_class, wn);
   XSetClassHint(mdisplay, mwindow, cl);
   delete [] cl->res_class;
   cl->res_class = NULL;
   XFree(cl->res_name);
   XFree(cl);

   XMapWindow(mdisplay, mwindow);

   do {
      XNextEvent (mdisplay, &mevent);
   } while (mevent.type != Expose);

   XSetForeground(mdisplay, mGC, 0);
   XFillRectangle(mdisplay, mwindow, mGC, 0, 0, WINX, WINY);
   XFlush(mdisplay);
}


/************************************************************************
************************************************************************/
void ALEXwindow::ALEXcancel() {

   if (mGC != NULL)
      XFreeGC(mdisplay, mGC);

   XCloseDisplay(mdisplay);
   exit(2);
}


void AOGLwindow::AOGLfreezeframe(mapul *mcanvas) {

   XEvent mevent;

   do {
      glDrawPixels(mcanvas->query_x(), mcanvas->query_y(), GL_RGBA, GL_UNSIGNED_BYTE, mcanvas->data);
      glXSwapBuffers(mdisplay, mwindow);
      XNextEvent (mdisplay, &mevent);
   } while (mevent.type != KeyPress && mevent.type != ButtonPress);

}


void AOGLwindow::AOGLbitmap(mapul *mcanvas) {

   glDrawPixels(mcanvas->query_x(), mcanvas->query_y(), GL_RGBA, GL_UNSIGNED_BYTE, mcanvas->data);
   glXSwapBuffers(mdisplay, mwindow);
}


void AOGLwindow::AOGLbitmap(int framenum, linelist *mcanvas) {

   glClear(GL_COLOR_BUFFER_BIT);
   mcanvas->bitblt(framenum);
   glXSwapBuffers(mdisplay, mwindow);
}


void AOGLwindow::AOGLswapbuffers() {

   glXSwapBuffers(mdisplay, mwindow);
}


void AOGLwindow::AOGLbitblt(mapul *mcanvas) {

   XEvent mevent;

   glDrawPixels(mcanvas->query_x(), mcanvas->query_y(), GL_RGBA, GL_UNSIGNED_BYTE, mcanvas->data);
   glXSwapBuffers(mdisplay, mwindow);

   while (1) {
      XNextEvent (mdisplay, &mevent);
      if (mevent.type == KeyPress || mevent.type == ButtonPress)
         return;

      glDrawPixels(mcanvas->query_x(), mcanvas->query_y(), GL_RGBA, GL_UNSIGNED_BYTE, mcanvas->data);
      glXSwapBuffers(mdisplay, mwindow);
   }

}

void AOGLwindow::AOGLbitblt(int framenum, linelist *mcanvas) {

   XEvent mevent;

   glClear(GL_COLOR_BUFFER_BIT);
   mcanvas->bitblt(framenum);
   glXSwapBuffers(mdisplay, mwindow);

   while (1) {
      XNextEvent (mdisplay, &mevent);
      if (mevent.type == KeyPress || mevent.type == ButtonPress)
         return;

      glClear(GL_COLOR_BUFFER_BIT);
      mcanvas->bitblt(framenum);
      glXSwapBuffers(mdisplay, mwindow);
   }

}


void AOGLwindow::AOGLanimate(int numframes, mapul *mcanvas) {

   int     i;
   float  offset;
   clock_t otime, ntime;
   XEvent mevent;

   offset = CLOCKS_PER_SEC/((float)FRAMESPERSEC);

   while (1) {
      for (i=0; i<numframes; i++) {
         XCheckMaskEvent (mdisplay,KeyPressMask | ButtonPressMask, &mevent);
         if (mevent.type == KeyPress || mevent.type == ButtonPress)
            return;

         otime = clock();
         glDrawPixels(mcanvas->query_x(), mcanvas->query_y(), GL_RGBA, GL_UNSIGNED_BYTE, mcanvas[i].data);
         glXSwapBuffers(mdisplay, mwindow);
         while ( (ntime=clock()) - otime < offset && ntime >= otime);
      }

      otime = clock();
      while ( (ntime=clock()) - otime < CLOCKS_PER_SEC && ntime >= otime);
   }

}


void AOGLwindow::AOGLanimate(int numframes, linelist *mcanvas) {

   int     i;
   float  offset;
   clock_t otime, ntime;
   XEvent mevent;

   offset = CLOCKS_PER_SEC/((float)FRAMESPERSEC);

   while (1) {
      for (i=0; i<numframes; i++) {
         XCheckMaskEvent (mdisplay,KeyPressMask | ButtonPressMask, &mevent);
         if (mevent.type == KeyPress || mevent.type == ButtonPress)
            return;

         otime = clock();
         glClear(GL_COLOR_BUFFER_BIT);
         mcanvas->bitblt(i);
         glXSwapBuffers(mdisplay, mwindow);
         while ( (ntime=clock()) - otime < offset && ntime >= otime);
      }

      otime = clock();
      while ( (ntime=clock()) - otime < CLOCKS_PER_SEC && ntime >= otime);
   }

}


void AOGLwindow::AOGLreset(int argc, char *argv[], int maxx, int maxy, char *winname, GLint *uattribs) {

   XSetWindowAttributes mattrib;
   XTextProperty wname;
   char *wn = "gemstone";
   XClassHint *cl;
   XEvent mevent;
   Window mroot;
   GLint *attribs;
   int i;

   if ((mdisplay = XOpenDisplay(NULL)) == (Display *)NULL) {
      fprintf (stderr,
               "ERROR: Could not open a connection to X on display %s.\n",
               XDisplayName ("V R US"));
      exit (0);
   }

   WINX = maxx;
   WINY = maxy;

   if (uattribs != (GLint *)NULL)
      attribs = uattribs;
   else
      attribs = gattribs;

   mscreen = XDefaultScreen(mdisplay);
   if (!(vid = glXChooseVisual(mdisplay, mscreen, attribs))) {
      printf("Could not create 24 bit visual... Aborting...\n");
      exit(0);
   }

   mroot = RootWindow( mdisplay, vid->screen );

   mattrib.colormap = XCreateColormap(mdisplay, mroot, vid->visual, AllocNone);
   mattrib.border_pixel = 0x0;
   mattrib.event_mask =  ButtonPressMask | KeyPressMask |  ExposureMask;

   mdepth = vid->depth;

   mwindow = XCreateWindow(mdisplay, mroot, 0, 0, WINX, WINY, BORDER_WIDTH,
                           mdepth, InputOutput, vid->visual,
                           CWColormap | CWBorderPixel | CWEventMask | CWBackPixel,
                           &mattrib );

   mGC = NULL;

   XStringListToTextProperty( &winname, 1, &wname );
   XSetWMName( mdisplay, mwindow, &wname );
   XSetWMIconName( mdisplay, mwindow, &wname );

   cl = XAllocClassHint();
   XGetClassHint(mdisplay, mwindow, cl);
   XFree(cl->res_class);
   cl->res_class = new char[MAXSTRLEN];
   strcpy(cl->res_class, wn);
   XSetClassHint(mdisplay, mwindow, cl);
   delete [] cl->res_class;
   cl->res_class = NULL;
   XFree(cl->res_name);
   XFree(cl);

   XMapWindow( mdisplay, mwindow );
   context = glXCreateContext( mdisplay, vid, NULL, GL_TRUE );
   glXMakeCurrent( mdisplay, mwindow, context );

//   glPushMatrix();
   glOrtho(0,WINX,0,WINY, -1, 1);
   do {
      XNextEvent (mdisplay, &mevent);
   } while (mevent.type != Expose);

   glClearColor(0.0,0.0,0.0,0.0);
   glClear(GL_COLOR_BUFFER_BIT);

   if (uattribs != (GLint *)NULL)
      for (i=0; uattribs[i] != None;) {
         if (uattribs[i] == GLX_DOUBLEBUFFER) {
            glXSwapBuffers(mdisplay, mwindow);
            glClear(GL_COLOR_BUFFER_BIT);
            break;
         }

         if (uattribs[i] == GLX_RGBA)
            i++;
         else
            i+=2;
      }

   AOGLinit_font();
   glEnable(GL_LINE_STIPPLE);
}


void AOGLwindow::AOGLreset3D(int argc, char *argv[], int maxx, int maxy, float w, float h, char *winname, GLint *uattribs) {

   XSetWindowAttributes mattrib;
   XTextProperty wname;
   char *wn = "gemstone";
   XClassHint *cl;
   XEvent mevent;
   Window mroot;
   GLint *attribs;

   if ((mdisplay = XOpenDisplay(NULL)) == (Display *)NULL) {
      fprintf (stderr,
               "ERROR: Could not open a connection to X on display %s.\n",
               XDisplayName ("V R US"));
      exit (0);
   }

   WINX = maxx;
   WINY = maxy;

   if (uattribs != (GLint *)NULL)
      attribs = uattribs;
   else
      attribs = gattribs;

   mscreen = XDefaultScreen(mdisplay);
   if (!(vid = glXChooseVisual(mdisplay, mscreen, attribs))) {
      printf("Could not create 24 bit visual... Aborting...\n");
      exit(0);
   }

   mroot = RootWindow( mdisplay, vid->screen );

   mattrib.colormap = XCreateColormap(mdisplay, mroot, vid->visual, AllocNone);
   mattrib.border_pixel = 0x0;
   mattrib.event_mask =  ButtonPressMask | KeyPressMask |  ExposureMask;

   mdepth = vid->depth;

   mwindow = XCreateWindow(mdisplay, mroot, 0, 0, WINX, WINY, BORDER_WIDTH,
                           mdepth, InputOutput, vid->visual,
                           CWColormap | CWBorderPixel | CWEventMask | CWBackPixel,
                           &mattrib );

   mGC = NULL;

   XStringListToTextProperty( &winname, 1, &wname );
   XSetWMName( mdisplay, mwindow, &wname );
   XSetWMIconName( mdisplay, mwindow, &wname );

   cl = XAllocClassHint();
   XGetClassHint(mdisplay, mwindow, cl);
   XFree(cl->res_class);
   cl->res_class = new char[MAXSTRLEN];
   strcpy(cl->res_class, wn);
   XSetClassHint(mdisplay, mwindow, cl);
   delete [] cl->res_class;
   cl->res_class = NULL;
   XFree(cl->res_name);
   XFree(cl);

   XMapWindow( mdisplay, mwindow );
   context = glXCreateContext( mdisplay, vid, NULL, GL_TRUE );
   glXMakeCurrent( mdisplay, mwindow, context );
   glFrustum(-0.5*w, 0.5*w, -0.5*h, 0.5*h, 1, 1000000);

   do {
      XNextEvent (mdisplay, &mevent);
   } while (mevent.type != Expose);

   glClearColor(0.0,0.0,0.0,0.0);
   glClear(GL_COLOR_BUFFER_BIT);
   glXSwapBuffers(mdisplay, mwindow);
   glClear(GL_COLOR_BUFFER_BIT);

   AOGLinit_font();
   glEnable(GL_LINE_STIPPLE);
}


void AOGLwindow::AOGLwinset() {

   glXMakeCurrent( mdisplay, mwindow, context );
}


void AOGLwindow::AOGLinit_font(void) {

   XFontStruct *fontInfo;
   Font id;
   unsigned int first, last;

//   fontInfo = XLoadQueryFont(mdisplay, "-adobe-helvetica-medium-r-normal--14-140-75-75-p-77-iso8859-1");
   fontInfo = XLoadQueryFont(mdisplay, "-adobe-helvetica-medium-r-normal--17-120-100-100-p-88-iso8859-1");

   if (fontInfo == NULL) {
      printf ("no font found\n");
      exit (0);
   }

   last = fontInfo->max_char_or_byte2;
   fontid = glGenLists(last+1);

   if (!fontid) {
      printf ("out of display lists\n");
      exit (0);
   }

   id = fontInfo->fid;
   first = fontInfo->min_char_or_byte2;

   glXUseXFont(id, first, last-first+1, fontid+first);
}

void AOGLwindow::AOGLcharstr(char *s) {

   glPushAttrib (GL_LIST_BIT);
   glListBase(fontid);
   glCallLists(strlen(s), GL_UNSIGNED_BYTE, (unsigned char *)s);
   glPopAttrib ();
}

#endif
