
#include "mymacros.h"
#include <stdio.h>
#include "vision.h"
#include "utilities.h"
#include "xshowimage.h"
#include "imageproc.h"
#include <math.h>
#include "binarize.h"
#include "split.h"

#define SPLIT_CONST .60

image_type *split_image(image_type *oldimage)
{
  int  *histogram;
  image_type *doneimage;
  long holdval;
  int i,sumval;
  
  histogram = get_histogram(oldimage);

  holdval = 0;

  for (i=0;i < 256;i++)
    holdval = holdval + i*histogram[i];

  sumval = (holdval/(oldimage->height*oldimage->width)) * SPLIT_CONST;

  fprintf(stderr,"Splitting on value: %d\n",sumval);
  doneimage = binarize(oldimage,sumval);

  return (doneimage);

}
