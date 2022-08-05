
#include "mymacros.h"
#include <stdio.h>
#include "vision.h"
#include "utilities.h"
#include "xshowimage.h"
#include "imageproc.h"
#include <math.h>
#include "rotate.h"
#include "histogram.h"
#include "hough.h"
#include "binarize.h"
#include "fixit.h"
#include "blobify.h"
#include "split.h"
#include "net.h"
#include "pattern.h"
#include "sortblob.h"

#define SPACE_RANGE 30 

main(argc, argv)
int argc;
char **argv;
{
   image_type *temporaryimage,*originalimage;
   image_type *image;
   blob_type *mybloblist;
   pattern_t *outpattern,*letterpat;
   net_t *danet;
   FILE *fp;
   int counter;
   int letindex;
   val_t *hold;
   val_t outvalue;
   int i;
   char character;
   char oldcharacter;
   int mycount=0;
   int spacevalue;
   int dotflag=0;

   if (argc != 4)
     printf("%s netfile activatefile file-to-rectify\n",argv[0]);
   else
   { 
	 temporaryimage = read_pgm_image(argv[3]);
	 originalimage = copy_image(temporaryimage);
	 temporaryimage = split_image(temporaryimage);
	 temporaryimage = autocrop(temporaryimage,0);
	 temporaryimage = hough_transform(temporaryimage,64,4,255);
	 temporaryimage = binarize(temporaryimage,245);
	 temporaryimage = hough_transform(temporaryimage,1,1,255);
	 image = fix_image(temporaryimage,originalimage);
	 image = split_image(image);
	 blobify(image,255,0,&(mybloblist));
	 mybloblist = blob_sort(mybloblist); 
	 danet = init_net(argv[1],argv[2]);
	 fp = fopen("asciioutput","w");

	 if (fp == NULL)
	 {
	   fprintf(stderr,"You cant write a file in this directory!\n");
	   exit(0);
         }

	 while (mybloblist != NULL)
	 {

	  if (mybloblist->id == 42)
	     character = '\n';
          else
	  {
	   outpattern = make_pattern(mybloblist,8,10);
           feed_forward(danet,outpattern);
	   letterpat = get_output(danet);
           counter = letterpat->count;
	   hold = letterpat->data;
	   letindex = -1;
	   outvalue = -1.0;

	   for(i=0;i < counter;i++)
	   {
	     if ( *hold > outvalue)
	     {
	       outvalue = *hold;
	       letindex = i;
             }
             hold++;
           }

	   if (letindex == 26)
	     character = '.';
           else
	     character = 'a' + letindex;     
          }
          
	  if (mycount == 0)
	  {
	     oldcharacter = character;
	     mycount++;
	     spacevalue = mybloblist->x_loc;
          }
          else 
	  {
	    /*
	    printf("xloc: %d\n",mybloblist->x_loc);
	    */

	    if ((oldcharacter == '.') && ((character == 'i') ||
	       (character == 'j')))
	    {
	    }
            else if (((oldcharacter == 'j') || (oldcharacter == 'i')) && (character == '.'))
	    {
	      fprintf(fp,"%c",oldcharacter);
              dotflag=1;  
            }
            else if (dotflag == 1)
	    {
	      dotflag=0;
            }
            else
	      fprintf(fp,"%c",oldcharacter);
         
	    if ((mybloblist->x_loc - spacevalue) > SPACE_RANGE)
	       fprintf(fp," ");
            spacevalue = mybloblist->x_loc;

	    oldcharacter = character;
          }

          mybloblist = mybloblist->next;
         }
	 fclose(fp);
  }
}
