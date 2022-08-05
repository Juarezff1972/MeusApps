
#include "mymacros.h"
#include <stdio.h>
#include "vision.h"
#include "utilities.h"
#include "xshowimage.h"
#include <math.h>
#include "imageproc.h"


image_type *histo_norm(image_type *oldimage)
{
  image_type *newimage;
  int i,j,lastend,range;
  int normalized[256];
  int divisor,sumval;
  int oldheight,oldwidth;
  int *histogram;
  int max_value;
 
  divisor = ((oldimage->height * oldimage->width)/256)+1;
  fprintf(stderr,"Divi: %d\n",divisor);
  histogram = get_histogram(oldimage);
  fprintf(stderr,"Divisor for Histogram: %d\n",divisor);

  oldheight = oldimage->height; 
  oldwidth = oldimage->width;

  newimage = new_image(oldimage->height, oldimage->width);

  lastend = 0;
  sumval = 0;

  for(i=0;i < 256;i++)
  {
    normalized[i] = 0;
  }

  for(i=0;i < 256;i++)
  {
     sumval = sumval + histogram[i]; 
     max_value = (float)(sumval)/((float)(divisor) +.5);
     range = (int)(((float)(histogram[i]) / (float)divisor) + .5); 
     normalized[i] = (max_value - range/2);
  }  

  for(i=0;i < oldheight ;i++)
  {
    for (j=0; j < oldwidth;j++)
    {
       newimage->image[i][j] = (pix_type)(normalized[(int)(oldimage->image[i][j])]);
    }
  }


  return (newimage);
}
