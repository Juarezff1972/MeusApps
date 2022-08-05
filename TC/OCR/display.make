
PROJ    = display  
SOURCE  = myutilities.o split.o blobify.o fixit.o binarize.o hough.o imageproc.o scale.o histogram.o average.o rotate.o display.o xshowimage.o utilities.o

CC      = cc
CFLAGS  = -Aa -c -g  

$(PROJ) : $(SOURCE) 
	$(CC) $(SOURCE) -g -o $(PROJ) -lm -lX11 

$(SOURCE): 
	$(CC) $(CFLAGS) $*.c  

display.o     : hough.h binarize.h vision.h utilities.h scale.h average.h
binarize.o    : binarize.h
hough.o       : hough.h
imageproc.o   : imageproc.h
scale.o       : scale.h
histogram.o   : histogram.h
average.o     : average.h
rotate.o      : rotate.h 
xshowimage.o  : vision.h xshowimage.h 
utilities.o   : utilities.h 


