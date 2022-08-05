#include "mymacros.h"
#include <stdio.h>
#include "vision.h"
#include "utilities.h"
#include "xshowimage.h"
#include <math.h>
#include "rotate.h"
#include "fixit.h"
#include "blobify.h"


image_type *fix_image(image_type *oldimage,image_type *orgimage) 
{

  double slope_hold;
  double return_rad;
  image_type *fixedimage;
  int oldheight,oldwidth;
  int holdval,realval;
  int i,zeroval;
  int otherval;

  holdval = 0;
  oldheight = oldimage->height;
  oldwidth = oldimage->width;

  zeroval = oldheight/2;

  fprintf(stderr,"w: %d h: %d\n",oldwidth,oldheight);
  for(i=0;i < oldwidth;i++)
  {
    if (oldimage->image[zeroval][i] > holdval)
    {
      holdval = oldimage->image[zeroval][i];
      realval = i; 
      fprintf(stderr,"I: %d\n",i);
    }
  }

  otherval = realval - oldwidth/2;
  slope_hold = (float)otherval/(float)(oldwidth/2);

  return_rad = atan(slope_hold);
  return_rad = (return_rad)*((180.0 / 3.142));

  fprintf(stderr,"hold: %d xloc: %d, slope: %lf, rad %lf \n",otherval,realval,slope_hold,return_rad);
  
  fixedimage = rotate_image(orgimage,-return_rad,255); 

  return (fixedimage);

}
      
