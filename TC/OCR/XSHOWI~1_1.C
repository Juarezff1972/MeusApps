#include <stdio.h>
#include "vision.h"
#include "xshowimage.h"
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#define COLORS 256

static char *dither(image_type *image)
{
   char *bit_map, *malloc();
   int row, col, bit_mask_size, bit, mask;
   
   bit_mask_size = ((image->width+7)*image->height >> 3) + 1;
   bit_map = malloc(bit_mask_size);
   bzero(bit_map, bit_mask_size);

   bit = 0;
   for (row = 0; row < image->height; row++) {
      for (col = 0; col < image->width; col++) {
         if (image->image[row][col] < (random() % 255) + 1)
            bit_map[bit>>3] |= 1 << (7 -(bit & 07));
         bit++;
      }
      bit = (bit + 7) & ~07;
   }
   return bit_map;
}

static char *no_bound(image_type *image)
{
   char *image_map, *malloc();
   int row, col, image_size, map_index;
   
   image_size = (image->width*image->height);
   image_map = malloc(image_size);
   bzero(image_map, image_size);

   map_index = 0;
   for (row = 0; row < image->height; row++)
      for (col = 0; col < image->width; col++) 
         image_map[map_index++] = image->image[row][col];
   return image_map;
}

static Window mainwin;
static Display *display = NULL;

void x_burn_image()
{
   XDestroyWindow(display, mainwin);
}

void x_show_image(image_type *picture, char *label)
{
   GC gc;
   Colormap colormap;
   XImage *image;
   Window rootwin;
   Visual *visual;
   int screen;
    
   unsigned long pixel[COLORS];
 
   XColor color[COLORS];
   XSetWindowAttributes attrs;
   XGCValues xgcv;
   unsigned int attrsmask, xgcvmask, pixmask;
   XEvent evt;
   XSizeHints sizes;
   char *array;
   int  i, depth;
 
   if (display == NULL && !(display = XOpenDisplay(getenv("DISPLAY"))))
   {
      printf("Unable to open display.\n");
      exit(1);
   }

   rootwin = DefaultRootWindow(display);
   screen = DefaultScreen(display);
   visual = DefaultVisual(display, screen);
   depth = DefaultDepth(display, screen);
 
   attrs.background_pixel = 0;
   attrs.border_pixel = COLORS-1;
   attrs.override_redirect = False;
   attrs.backing_store = 1; /* When Mapped */
   attrsmask = CWBackingStore | CWBorderPixel | CWOverrideRedirect | 
    CWBackPixel;
   mainwin = XCreateWindow(display, rootwin, 0, 0, picture->width, 
    picture->height, 3, CopyFromParent, InputOutput, visual, attrsmask,&attrs);
 
   XSelectInput(display, mainwin,
                 ExposureMask | StructureNotifyMask);
   XStoreName(display, mainwin, label);
 
   xgcv.foreground = 0;
   xgcv.background = COLORS-1;
   xgcvmask = GCForeground | GCBackground;
   gc = XCreateGC(display, mainwin, xgcvmask, &xgcv);
 
   sizes.min_width = picture->width;
   sizes.max_width = picture->width;
   sizes.min_height = picture->height;
   sizes.max_height = picture->height;
   sizes.flags = PMinSize | PMaxSize;
   XSetNormalHints(display, mainwin, &sizes);
 
   XMapWindow(display, mainwin);
   do {
      XNextEvent(display, &evt);
   } while (evt.type != Expose);
 
   XFlush(display);
 
   if (depth == 8) {
      colormap = XCreateColormap(display, mainwin, visual, AllocNone);
      XAllocColorCells(display, colormap, False, &pixmask, 0, pixel, COLORS);
      for(i = 0; i < COLORS; i++) {
         color[i].pixel = pixel[i];
         color[i].red = i * 256;
         color[i].green = i * 256;
         color[i].blue = i * 256;
         color[i].flags = DoRed | DoGreen | DoBlue;	/* black */
      }
      XStoreColors(display, colormap, color, COLORS);
      XSetWindowColormap(display, mainwin, colormap);
      array = no_bound(picture);
   } else
      array = dither(picture);
   image = XCreateImage(display, visual, depth, ZPixmap, 0, array, 
    picture->width, picture->height, 8, 0);
   XPutImage(display, mainwin, gc, image, 0, 0, 0, 0, picture->width, 
    picture->height);
   XFlush(display);
}
