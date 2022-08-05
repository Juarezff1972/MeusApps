
#include "blobify.h"
#include "vision.h"
#include "imageproc.h"
#include "net.h"
#include "pattern.h"
#include <stdio.h>
#define ALPHA_NUM 27

pattern_t *make_pattern(blob_type *dablob,int x,int y)
{

  double tmp1,tmp2,scaling_value;
  int i,j;
  int holdheight,holdwidth;
  image_type *scaledimage;
  pattern_t *tmppat;
  val_t *tmpptr,*holdptr;

  holdheight = dablob->image->height;
  holdwidth = dablob->image->width;

  tmppat = (pattern_t *)(malloc(sizeof(pattern_t)));
  tmppat->count = 0;

  tmp1 = (float)(x)/(float)(holdwidth);
  tmp2 = (float)(y)/(float)(holdheight);

  if (tmp1 < tmp2)
    scaling_value = tmp1;
  else
    scaling_value = tmp2;

  scaledimage = scale_image(dablob->image,scaling_value);
  holdheight = scaledimage->height;
  holdwidth = scaledimage->width;
  tmppat->data = (val_t *)malloc(((x*y+1)*(sizeof(val_t))));
  tmpptr = tmppat->data;
  
  tmppat->count = (x * y + 1);
  holdptr = tmpptr;
  for(i=0;i < tmppat->count;i++)
  {
    *holdptr = 1.0; /* 256 after scaling to 1 -1 range */
    holdptr++;

  }

  for(i=0;i < holdheight;i++)
  {
    for(j=0;j < holdwidth;j++)
    {
      *tmpptr = (val_t)(((float)(2*(scaledimage->image[i][j]))/255.0) - 1.0); 
       tmpptr++;
    }
  }
  *tmpptr = (val_t)(scaling_value);

  burn_image(scaledimage);
  return tmppat;
}

void make_train_file(blob_type *bloblist)
{
  pattern_t *output_pattern;
  FILE *fp;
  int i,j,k,count;
  blob_type *temporary_list;
  val_t *holdptr;
  val_t alphaarray[ALPHA_NUM];
  blob_type *tmphold;
  int ind;
  myoutput holdarray[580];
  
  temporary_list = bloblist;
  for(i=0;i < ALPHA_NUM;i++)
    alphaarray[i] = -1.0;

  for(j=28;j >= 0;j--)
  {
       printf("Loop: %d\n",j);
       count=j;
       if ((j == 8) || (j == 10))
	 ind = 26; 
       else if(j == 9)
	 ind = 8 ;
       else if (j > 10)
	 ind = j - 2;
       else
	 ind = j;

     for(k=0;k < 20;k++)
     {
       holdarray[count].dapattern = make_pattern(temporary_list,8,10);
       holdarray[count].letterval = ind;
       count += 29;
       temporary_list = temporary_list->next;
     }
  }

  fp = fopen("train.file","w");
  if (fp != NULL)
  {
    fprintf(fp,"0.01 .1 5 580\n");
    for(i=0;i < 580;i++)
    {
       output_pattern = holdarray[i].dapattern;
       alphaarray[(holdarray[i].letterval)] = 1.0;
       holdptr = output_pattern->data;
       count = output_pattern->count;
	
       for(j=0;j < count;j++)
       {
	  fprintf(fp,"%5.5f ",*holdptr);
	  /*
	  if (j == (count/2))
	    fprintf(fp,"\n");
	    */
          holdptr++;
       }

       fprintf(fp,"\n");

       for(j=0;j < ALPHA_NUM;j++)
	 fprintf(fp,"%5.5f",alphaarray[j]);

       fprintf(fp,"\n\n");
       alphaarray[(holdarray[i].letterval)] = -1.0;
     }
      fclose(fp);
    }
    else
      printf("Wrong file name!\n");
 }
