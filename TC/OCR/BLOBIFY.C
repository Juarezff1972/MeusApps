
#include "mymacros.h"
#include <stdio.h>
#include "vision.h"
#include "utilities.h"
#include "xshowimage.h"
#include <math.h>
#include "blobify.h"
#include "imageproc.h"

vqueue TheQueue;

image_type *autocrop(image_type *oldimage,int foreground)
{

  int i,j,oldheight,oldwidth;
  int top,bottom,left,right;
  image_type *cropimage;

  top = -1;
  bottom = -1;
  right = -1;
  left = -1;

  oldheight = oldimage->height;
  oldwidth = oldimage->width;

  
  for(i=0;i < oldheight;i++)
  {
    for(j=0;j < oldwidth;j++)
    {
      if (oldimage->image[i][j] == foreground)
      {
	top = i;
	break;
      }
    }
    if (top != -1)
      break;
  } 

  for(i=(oldheight-1);i > 0;i--)
  {
    for(j=0;j < oldwidth ;j++)
    {
      if (oldimage->image[i][j] == foreground)
      {
	bottom = i;
	break;
      }
    }
    if (bottom != -1)
      break;
  } 

  for(j=0;j < oldwidth;j++)
  {
    for(i=0;i < oldheight;i++)
    {
      if (oldimage->image[i][j] == foreground)
      {
	left = j;
	break;
      }
    }
    if (left != -1)
      break;
  } 

  for(j=(oldwidth-1);j > 0;j--)
  {
    for(i=0;i < oldheight;i++)
    {
      if (oldimage->image[i][j] == foreground)
      {
	right = j;
	break;
      }
    }
    if (right != -1)
      break;
  } 
  cropimage = crop_image(oldimage,left,right,top,bottom);
  return (cropimage);

}


void Qpop (vqueue *theQ,queuething *thenode)
{

  queuething *tmp;

  if (theQ->head == NULL)
  {
    fprintf(stderr,"popping from an empty queue\n");
    exit(0);
  }

  thenode->x = theQ->head->x;
  thenode->y = theQ->head->y;
  tmp = theQ->head->next;
  free(theQ->head);
  theQ->head = tmp;
  if (theQ->head == NULL)
    theQ->tail=NULL;

 /*  fprintf(stderr,"Exiting pop\n"); */ 
}

void Qpush(vqueue *theQ,queuething *thenode)
{

  queuething *newnode;

  newnode = (queuething *)malloc(sizeof(queuething));

  newnode->x = thenode->x;
  newnode->y = thenode->y;
  newnode->next = NULL;

  if (theQ->tail == NULL)
  {
    theQ->head = newnode;
    theQ->tail = newnode;
  }
  else
  {
  theQ->tail->next = newnode; 
  theQ->tail = newnode;
  }
   /* fprintf(stderr,"Exiting push\n");  */
}

int Qempty(vqueue *theQ)
{

  if (theQ->head == NULL)
    return TRUE;
  else
    return FALSE;

}


void blobify(image_type *oldimage,int background,int foreground,blob_type **dalist) 
{

  int i,j;
  int choicevar;
  int oldheight,oldwidth;
  image_type *workingimage;
  image_type *blobimage;
  image_type *holdimage;
  queuething tmpnode;
  int newx,newy,totx,toty;
  int l,k;
  int m,n;
  blob_type *mytmpblob,*bloblist;
  int tmpcount=0;
  int xminval,xmaxval,yminval,ymaxval;
  queuething holdingarray[5000];
  int holdcnt = 0;
  int maxheight,maxwidth;

  oldheight = oldimage->height;
  oldwidth = oldimage->width;

  bloblist = NULL;
  TheQueue.tail = NULL;
  TheQueue.head = NULL;
  workingimage = copy_image(oldimage);

  fprintf(stderr,"Now blobifying the image\n");

  for(i=1;i < oldheight;i++)
  {
    for(j=1;j < oldwidth;j++)
    {
       holdcnt=0;
       if(workingimage->image[i][j] == foreground)
       {

         tmpnode.x = j;
	 tmpnode.y = i;
	 xminval = xmaxval = j;
	 yminval = ymaxval = i;

	 tmpnode.next = NULL;

         Qpush(&TheQueue,&tmpnode);

         while (!(Qempty(&TheQueue)))
	 {
	   Qpop(&TheQueue,&tmpnode);

           holdingarray[holdcnt] = tmpnode;
	   holdcnt++;

	   newy = tmpnode.y;
	   newx = tmpnode.x;

	   workingimage->image[newy][newx] = background;
	   newy--;
	   newx--;
	   toty = newy + 3;
           totx = newx + 3;

	   for (k=newy;k < toty;k++)
	   {
	     for(l=newx;l < totx;l++)
	     {
	       if (workingimage->image[k][l] == foreground)
 	       {
		 tmpnode.y = k;
		 tmpnode.x = l;

           if(tmpnode.y < yminval)
	     yminval = tmpnode.y;
           else if(tmpnode.y > ymaxval)
	     ymaxval = tmpnode.y;

           if(tmpnode.x < xminval)
	     xminval = tmpnode.x;
           else if(tmpnode.x > xmaxval)
	     xmaxval = tmpnode.x;

		 Qpush(&TheQueue,&tmpnode);
	         workingimage->image[k][l] = background; 
               }
             }
           }
         } /* end while */

         maxheight = ymaxval - yminval+1;
	 maxwidth = xmaxval - xminval+1;

	 mytmpblob = (blob_type *)malloc(sizeof(blob_type));
	 mytmpblob->next = NULL;
	 mytmpblob->image = new_image(maxheight,maxwidth);
         mytmpblob->color = foreground;
	 mytmpblob->y_loc = (ymaxval + yminval)/2;
	 mytmpblob->x_loc = (xmaxval + xminval)/2;

          for(m=0;m < maxheight;m++)
          {
            for(n=0;n < maxwidth;n++)
            {
              mytmpblob->image->image[m][n] = background;
	    }
          }

         for(m=0;m < holdcnt;m++)
	   mytmpblob->image->image[(holdingarray[m].y - yminval)][(holdingarray[m].x - xminval)] = foreground;

         if (bloblist == NULL)
	   bloblist = mytmpblob;
         else
	 {
           mytmpblob->next = bloblist;
	   bloblist = mytmpblob;
	 }
	 /*
         printf("blobbing %d\n",tmpcount);
	 tmpcount++;
	 */
       }  /* end 1st if */
    }     /* inner for */
  }       /* outer for */
/*
  while(bloblist != NULL)
  {
    x_show_image(bloblist->image,"Tester");
    bloblist = bloblist->next;
  }
  */
 
*dalist = bloblist;

}         /*  function end */

