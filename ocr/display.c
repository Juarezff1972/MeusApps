#include "mymacros.h"
#include <stdio.h>
#include "vision.h"
#include "utilities.h"
#include "xshowimage.h"
#include "imageproc.h"
#include <math.h>
#include "rotate.h"
#include "average.h"
#include "histogram.h"
#include "scale.h"
#include "hough.h"
#include "binarize.h"
#include "fixit.h"
#include "blobify.h"
#include "split.h"

main(argc, argv)
int argc;
char **argv;
{
   image_type *image;
   image_type *newimage; 
   image_type *fixedimage;
   image_type *originalimage;
   image_type *temporaryimage;
   int pixval;
   int checksplit;
   int forecolor,backcolor;
   float scale;
   char filename1[100];
   int checker;
   int numit,xval,yval,xex,yex;
   int splitbin,cext,mext;
   int checkervar;
   int fixval;
   double rads,comprads;
   blob_type **bloblist;
   double angle;

   while (1)
   {
      fprintf(stderr,"1:File 2:hough 3:histo 4:scale 5:bin 6:avg 7:rot 8:pri 9:disp 10:fix 11:blob     12:crop 13:AUTO-FIX 14:end\n");
      scanf("%d",&checkervar);
      switch (checkervar)
      {
	case 1: fprintf(stderr,"Enter filename:\n");
		scanf("%s",filename1);
		image = read_pgm_image(filename1);
                originalimage = copy_image(image);
		break;
        case 2:    fprintf(stderr,"Please input the c and m extent values and pixval for hough\n");
	   scanf("%d%d%d",&cext,&mext,&pixval);
	   image = hough_transform(image,mext,cext,((pix_type)pixval));
           break;
        case 3: image = histo_norm(image);
		break;
        case 4:    fprintf(stderr,"Input scale,x val,yval,x extent,y extent for sizing\n");
	   scanf("%f%d%d%d%d",&scale,&xval,&yval,&xex,&yex);
           image = resize_image(image,scale,xval,yval,xex,yex);
	   break;
	case 5: fprintf(stderr,"Do you wish to (1) use split function or (2) input your own value?\n");
		scanf("%d",&checksplit);
		if (checksplit == 1)
		  image = split_image(image);
		else
		{
                  fprintf(stderr,"Please input the split value fo binarization\n");
	          scanf("%d",&splitbin);
	   
                  image = binarize(image,splitbin);
	        } 
	        break;
        case 6:    fprintf(stderr,"Input number of iterations for averaging\n");
	   scanf("%d",&numit);
           image = average(image,numit);
	   break;
        case 7:    fprintf(stderr,"Input angle of rotation and background color\n");
	   scanf("%lf%d",&angle,&backcolor);
	   image = rotate_image(image,angle,backcolor);
	   break;
        case 8: fprintf(stderr,"Input output filename:\n");
		scanf("%s",filename1);
		print_pgm_image(image,filename1);
		break;
        case 9: x_show_image(image,"Teste_image");
		break;
        case 10:
		 fprintf(stderr,"Fix which image? \n");
		 scanf("%s",filename1);
		 originalimage = read_pgm_image(filename1);
                 image = fix_image(image,originalimage);
		 break;
	case 11: 
		 fprintf(stderr,"Please enter backgroun and foreground color\n");
		 scanf("%d%d",&backcolor,&forecolor);
		 blobify(image,backcolor,forecolor,bloblist);
		 break;
        case 12:
		 fprintf(stderr,"Enter background color\n");
                 scanf("%d",&backcolor);
		 image = autocrop(image,backcolor);
		 break;
	case 13:
		 fprintf(stderr,"Enter the image to fix\n");
		 scanf("%s",filename1);
		 temporaryimage = read_pgm_image(filename1);
		 originalimage = copy_image(temporaryimage);
		 temporaryimage = split_image(temporaryimage);
		 temporaryimage = autocrop(temporaryimage,0);
		 temporaryimage = hough_transform(temporaryimage,64,4,255);
		 temporaryimage = binarize(temporaryimage,245);
		 temporaryimage = hough_transform(temporaryimage,1,1,255);
		 image = fix_image(temporaryimage,originalimage);
		 burn_image(temporaryimage);
		 burn_image(originalimage);
		 break;
	case 14: fprintf(stderr,"Do you really want to exit? (1 == yes) \n");
		 scanf("%d",&checker);
		 if (checker == 1)
		   exit (0);
		break;
        default: fprintf(stderr,"Sorry, try again. \n");
		 
       }
    }
}

