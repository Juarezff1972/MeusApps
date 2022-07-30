
#ifndef BLOB_H
#define BLOB_H

#include "vision.h"

#define TRUE 1
#define FALSE 0

typedef struct queuething {
	int x,y;
	struct queuething *next;
		      } queuething;

typedef struct vqueue {
	queuething *head,*tail;
		      } vqueue;

void blobify(image_type *oldimage,int background,int foreground,blob_type **dalist); 
image_type *autocrop(image_type *oldimage,int foreground);

#endif


