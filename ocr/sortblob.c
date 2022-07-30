
#include "mymacros.h"
#include "vision.h"
#include "net.h"
#include "imageproc.h"
#include <stdio.h>
#include <math.h>
#include "utilities.h"
#include "xshowimage.h"
#include "sortblob.h"


blob_type *full_sort_blob(blob_type *bloblist,int thexext)
{
    blob_type *tmpblob,*headblob,*lastblob;
    blob_type *newlineblob;
    blob_type **ptrarray;
    int i;
    int starty;
    int kludgeflag=0;
    int kludgecount=0;

  
  ptrarray = (blob_type **)(malloc((thexext * (sizeof(blob_type*)))));
  while(bloblist != NULL)
  {
    kludgeflag=0;    
    newlineblob = (blob_type *)malloc(sizeof(blob_type));
    newlineblob->id = 42;

    for(i=0;i < thexext;i++)
       ptrarray[i] = NULL;
    
    starty = bloblist->y_loc;

    while((bloblist != NULL) && (bloblist->y_loc < starty + 25) && (bloblist->y_loc > starty -25))
    {
      ptrarray[(bloblist->x_loc)] = bloblist;
      bloblist = bloblist->next;
    }

    for(i=0;i < thexext;i++)
    {
      if(ptrarray[i] != NULL)
      {
	if(kludgeflag == 0)
	{
	  tmpblob = ptrarray[i];
	  if (kludgecount == 0)
	  {
	    headblob = tmpblob;
	    kludgecount++;
          }
	  else
	    lastblob->next = tmpblob;

	  kludgeflag++;
        }
	else
	{
	  tmpblob->next = ptrarray[i];
	  tmpblob = tmpblob->next;
        }
      }
    }

    newlineblob->x_loc = tmpblob->x_loc;
    tmpblob->next = newlineblob;
    lastblob = newlineblob;
  }
  return headblob;
}

blob_type *blob_sort(blob_type *bloblist)
{

  blob_type *tmpblob,*holdblob,*returnblob;
  int count = 0;
  int xval = 0;

  tmpblob = bloblist->next;
  bloblist->next = NULL;

  while(tmpblob != NULL)
  {
    count++;
    if (bloblist->x_loc > xval)
      xval = bloblist->x_loc;
    holdblob = tmpblob->next;
    tmpblob->next = bloblist;
    bloblist = tmpblob;
    tmpblob = holdblob;
  }

  returnblob = full_sort_blob(bloblist,xval);

 return returnblob;

}


