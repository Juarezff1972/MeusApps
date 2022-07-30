
#include "mymacros.h"
#include <stdio.h>
#include "vision.h"
#include "utilities.h"
#include "xshowimage.h"
#include <math.h>
#include "scale.h"

image_type *resize_image(image_type *oldimage, float scale,int x,int y,int xex,int yex) 
{

   image_type *newimage;
   int i,j;
   int xscale,yscale;

   xscale = scale*xex;
   yscale = scale*yex;

   newimage = new_image(yscale,xscale);

   fprintf(stderr,"Scaling Image to %f times size\n",scale);
   for(i=0;i < yscale;i++)
   {
     for(j=0;j < xscale;j++)
     {
       newimage->image[i][j] = oldimage->image[y + (int)(i/scale)][x + (int)(j/scale)];
     }
   }

     return (newimage);

}
