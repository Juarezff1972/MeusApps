#include "mymacros.h"
#include <stdio.h>
#include "vision.h"
#include "utilities.h"
#include "xshowimage.h"
#include <math.h>
#include "average.h"


image_type *average(image_type *oldimage,int iterations)
{
   image_type *holdimage;
   int i,j,k,l,m,sumval,col,row,nrow,ncol;
   int oldheight,oldwidth;

   holdimage = new_image(oldimage->height,oldimage->width);
   holdimage = copy_image(oldimage);

   oldheight = oldimage->height;
   oldwidth = oldimage->width;

   for(m=0;m < iterations;m++)
   {
    fprintf(stderr,"Averaging Loop#: %d \n",m);

    for(k=1;k < oldheight;k++)
    {
     for(l=1;l < oldwidth;l++)
     {
       row=k-1;
       col=l-1;
       sumval = 0;
       nrow = row + 3;
       ncol = col + 3;

       for(i=row;i < nrow;i++)
       {
         for(j=col;j < ncol;j++)
         {
	   sumval = sumval + oldimage->image[i][j]; 
         }
       }

       holdimage->image[k][l] = ((pix_type)(sumval / 9));

     }
    }
     oldimage = copy_image(holdimage); 
   }
   return (holdimage);
}


