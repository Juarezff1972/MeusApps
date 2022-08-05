#include "mymacros.h"
#include <stdio.h>
#include "vision.h"
#include "utilities.h"
#include "xshowimage.h"
#include <math.h>
#include "rotate.h"


image_type *rotate_image(image_type *oldimage,double angle,int background)
{
  image_type *rotimage;
  int nx,ny,newheight,newwidth,oldheight,oldwidth,i,j,halfnewheight,halfnewwidth;
  int halfoldheight,halfoldwidth;
  double radians; 
  double cosval,sinval;

  fprintf(stderr,"Rotating Image %lf Degrees\n",angle);
  radians =  -(angle) / ((180 / 3.142));
  cosval = cos(radians);
  sinval = sin(radians);
  oldheight = oldimage->height;
  oldwidth = oldimage->width; 

  newwidth = (int)abs((int)(oldwidth*cosval)) + (int)abs((int)(oldheight*sinval));
  newheight = (int)abs((int)(-oldwidth*sinval)) + (int)abs((int)(oldheight*cosval));

  halfnewheight = newheight / 2;
  halfnewwidth = newwidth / 2;
  halfoldwidth = oldwidth /2;
  halfoldheight = oldheight /2 ;


  rotimage = new_image(newheight,newwidth);
  
  for(i=0;i < newheight;i++)
  {
    for(j=0;j < newwidth;j++)
    {
       rotimage->image[i][j] = (pix_type)background;
       nx =(int)( (j - halfnewwidth)*cosval + (i-halfnewheight)*sinval);
       ny =(int)( (0-(j - halfnewwidth))*sinval + (i - halfnewheight)*cosval);
       nx = nx + halfoldwidth;
       ny = ny + halfoldheight;
       if ((nx < oldwidth) && (ny < oldheight) && (nx > 0) && (ny > 0))
         rotimage->image[i][j] = oldimage->image[ny][nx];
    }
  }
  return (rotimage);
}

