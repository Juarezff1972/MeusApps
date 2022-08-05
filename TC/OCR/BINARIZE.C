
#include "mymacros.h"
#include <stdio.h>
#include "vision.h"
#include "utilities.h"
#include "xshowimage.h"
#include <math.h>

image_type *binarize(image_type *oldimage,int splitval)
{

  int i,j;
  int oldheight,oldwidth;
  int splitconst;
  image_type *newimage;

  newimage = new_image(oldimage->height,oldimage->width);

  splitconst = splitval;

  oldheight = oldimage->height;
  oldwidth = oldimage->width;

  fprintf(stderr,"Binarizing on value : %d\n",splitconst);

  for(i=0;i < oldheight;i++)
  {
    for(j=0;j < oldwidth;j++)
    {
       if (oldimage->image[i][j] < splitconst)
	 newimage->image[i][j] = 0;
       else
	  newimage->image[i][j] = 255;
    }
  }

  return (newimage);

}

