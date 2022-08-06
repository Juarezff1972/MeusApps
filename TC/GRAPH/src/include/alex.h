



/*   X include files      */

#if !defined (__ALEX_H)
#define __ALEX_H

#include <stdio.h>
#include <time.h>
#include <math.h>

#include "map.h"
#include "pstring.h"
#include "linetype.h"

#ifndef sgn
#define sgn(x)   ( ((x)<0) ? -1 : ((x) ? 1:0) )
#endif

#define DEFWINX 640
#define DEFWINY 480


#ifdef DOS

#include "vesa.h"

//class ALEXwindow : public vesa1_2 {
class ALEXwindow : public vesa2_0 {

   public:
      int           WINX, WINY;

      ALEXwindow() {
         WINX = DEFWINX;
         WINY = DEFWINY;
      }

      void ALEXline(int x0, int y0, int x1, int y1);
      void ALEXcancel() { vesa_api::set_mode(start_mode); }
};


class AOGLwindow : public ALEXwindow {

   public:
      void          AOGLbitblt(mapul *mcanvas);
      void          AOGLbitblt3(mapul *mcanvas);
      void          AOGLbitblti(mapul *mcanvas);
      void          AOGLbitbltx2(mapul *mcanvas);
      void          AOGLbitblt(int framenum, linelist *mcanvas);
      void          AOGLbitmap(mapul *mcanvas);
      void          AOGLbitmap3(mapul *mcanvas);
      void          AOGLbitmapi(mapul *mcanvas);
      void          AOGLbitmapx2(mapul *mcanvas);
      void          AOGLbitmap(int framenum, linelist *mcanvas);
      void          AOGLreset(int argc, char *argv[], int maxx, int maxy);
};


#else


#include <sys/types.h>

#ifdef SGI
#include <sys/prctl.h>
#endif

#include <signal.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysymdef.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <termio.h>
#include <unistd.h>
#include <GL/glx.h>

#define FRAMESPERSEC      30
#define FRAMERATE         72

typedef Pixmap   ALEXpixmap;
typedef XImage * ALEXimage;
typedef GC       ALEXGC;
typedef Drawable ALEXdrawable;



/* X window variables */

class ALEXwindow {

   public:
      int           WINX;
      int           WINY;
      Display       *mdisplay;
      Window        mwindow;
      int           mdepth;
      XVisualInfo   *vid;
      GC            mGC;
      int           mscreen;

      ALEXwindow() {
         WINX = DEFWINX;
         WINY = DEFWINY;
      }

      ALEXpixmap ALEXcreatemap(unsigned int x, unsigned int y) { return XCreatePixmap(mdisplay, mwindow, x, y, mdepth); }
      void   ALEXdestroymap(ALEXpixmap *x) { XFreePixmap(mdisplay, *x); }
      GC     ALEXcreateGC() { return XCreateGC(mdisplay, mwindow, 0, NULL); }
      void   ALEXdestroyGC(ALEXGC *x) { XFreeGC(mdisplay, *x); }
      void   ALEXsetforeground(unsigned int x) { XSetForeground(mdisplay, mGC, x); }
      void   ALEXdestroyimage(ALEXimage x) { XDestroyImage(x); }
      void   ALEXpasteimage(ALEXimage src, ALEXdrawable dest) { XPutImage(mdisplay, dest, mGC, src, 0, 0, 0, 0, WINX, WINY); }
      ALEXimage ALEXgetimage() { return XGetImage(mdisplay, mwindow, 0, 0, WINX, WINY, AllPlanes, ZPixmap); }
      unsigned int ALEXpackchar(unsigned char *x) { return x[0] | (((x[1]<<8) | x[2])<<8); }
      void   ALEXiputpixel(ALEXimage mcanvas, int *pt, int pv) { XPutPixel(mcanvas, pt[0], WINY-pt[1], pv); }

      void   ALEXclear(ALEXimage mcanvas);

      int           ALEXon_screen(int pt[]);

      void          ALEXfillrect(int y, int z, int a, int b, ALEXpixmap *x);
      void          ALEXfillrect(int y, int z, int a, int b);

      void          ALEXrect(int y, int z, int a, int b, ALEXpixmap *x);
      void          ALEXrect(int y, int z, int a, int b);
      void          ALEXrect(int x0, int y0, int x1, int y1, unsigned int color, ALEXimage mcanvas);

      void          ALEXpoint(int pt[]);
      void          ALEXpoint(int size, int pt[][2]);
      void          ALEXpoint(int pt[], ALEXpixmap *mcanvas);
      void          ALEXpoint(int size, int pt[][2], ALEXpixmap *mcanvas);
      void          ALEXpoint(int x0, int y0, unsigned int color, ALEXimage mcanvas);

      void          ALEXline(int pt1[], int pt2[]);
      void          ALEXline(int size, int pt1[][2]);
      void          ALEXline(int pt1[], int pt2[], ALEXpixmap *mcanvas);
      void          ALEXline(int size, int pt1[][2], ALEXpixmap *mcanvas);
      void          ALEXline(int x0, int y0, int x1, int y1, unsigned int color, ALEXimage mcanvas);

      void          ALEXpolygon(int size, int pt[][2]);
      void          ALEXpolygon(int size, int pt[][2], ALEXpixmap *mcanvas);

      void          ALEXanimate(int numframes, ALEXpixmap mcanvas[]);
      void          ALEXanimate(int numframes, ALEXimage mcanvas[]);

      void          ALEXfreezeframe(ALEXpixmap *mcanvas);
      void          ALEXfreezeframe(ALEXimage mcanvas);

      void          ALEXpause();

      void          ALEXbitblt(ALEXpixmap *mcanvas);
      void          ALEXbitblt(ALEXimage mcanvas);

      void          ALEXbitmap(ALEXpixmap *mcanvas);
      void          ALEXbitmap(ALEXimage mcanvas);

      void          ALEXmapcpy(ALEXpixmap *mcanvas, int x, int y, mapul *tob);
      void          ALEXmapcpy(ALEXimage mcanvas, mapul *tob);
      void          ALEXmapcpy(mapul *mcanvas, mapul *tob);

      int           ALEXqtest(unsigned int x);
      int           ALEXpollkeyboard(char *x);
      int           ALEXcode2sym(unsigned int x);
      XEvent        ALEXqread(unsigned int *x);
      void          ALEXqenter(XEvent *mevent);
      void          ALEXreset(int argc, char *argv[], int maxx, int maxy, char *winname);
      void          ALEXcancel();

      void      ALEXgetdim(int *w, int *h) {
         XWindowAttributes wa;
         XGetWindowAttributes(mdisplay, mwindow, &wa);
         *w = wa.width;
         *h = wa.height;
      }

};


class AOGLwindow : public ALEXwindow {

   public:
      GLXContext context;
      GLuint     fontid;

      void          AOGLfreezeframe(mapul *mcanvas);
      void          AOGLbitmap(mapul *mcanvas);
      void          AOGLbitmap(int framenum, linelist *mcanvas);
      void          AOGLswapbuffers();
      void          AOGLanimate(int numframes, mapul *mcanvas);
      void          AOGLanimate(int numframes, linelist *mcanvas);
      void          AOGLbitblt(mapul *mcanvas);
      void          AOGLbitblt(int framenum, linelist *mcanvas);
      void          AOGLreset(int argc, char *argv[], int maxx, int maxy, char *winname, GLint *uattribs = (GLint *)NULL);
      void          AOGLreset3D(int argc, char *argv[], int maxx, int maxy, float w, float h, char *winname, GLint *uattribs = (GLint *)NULL);
      void          AOGLwinset();
      void          AOGLcharstr(char *s);
      void          AOGLinit_font(void);
};

#endif

#endif
