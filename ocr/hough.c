
#include "mymacros.h"
#include <stdio.h>
#include "vision.h"
#include "utilities.h"
#include "xshowimage.h"
#include <math.h>
#include "imageproc.h"

image_type *hough_transform(image_type *oldimage,int slopeval,int cval,pix_type pixelval)
{

  image_type *transformed;
  int i,j;
  int ycoord,xcoord,maxnum,holdnum;
  int ycoord1,xcoord1;
  int yint;
  int halfheight,halfwidth;
  int fullwidth;
  int fullheight;
  int halfimageheight,halfimagewidth;
  float slope;

  fullwidth = oldimage->height/cval;
  fullheight = slopeval*2;
  halfheight = slopeval;
  halfwidth = fullwidth/2;
  halfimagewidth = oldimage->width/2;
  halfimageheight = oldimage->height/2;
  transformed = new_image(fullheight,fullwidth);

  fprintf(stderr,"Now performing hough transform on value %d\n",(int)pixelval);

  for(i=0;i < fullwidth;i++)
  {
    fprintf(stderr,"Width Value: %d \n",i);
    for(j=0;j < fullheight;j++)
    {
      yint=cval*(-i + halfwidth);
      xcoord=0;
      xcoord1=0;
      ycoord1=0;
      slope=(float)(j-halfheight)/(float)slopeval;
      maxnum=0;
      holdnum=0;
      ycoord=0;

      while ((xcoord <= halfimagewidth))
      {
	ycoord=slope*(xcoord) + yint;
	ycoord1=slope*(xcoord1) + yint;

	if ((ycoord <= halfimageheight) && (ycoord >= -halfimageheight)) 
	{
          if (oldimage->image[ycoord+halfimageheight][xcoord + halfimagewidth] == pixelval)
	    holdnum++;
          maxnum++;
        }

	if ((ycoord1 <= halfimageheight) && (ycoord1 >= -halfimageheight)) 
	{
          if (oldimage->image[ycoord1+halfimageheight][xcoord1+halfimagewidth] == pixelval)
            holdnum++;
          maxnum++;
        }

	xcoord++;
	xcoord1--;
      }
      maxnum++;
      transformed->image[j][i] = (pix_type)(((float)holdnum / (float)maxnum) * 255.0);
    }
  }
  
  return (transformed);
}

