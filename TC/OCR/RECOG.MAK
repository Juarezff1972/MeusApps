
PROJ    = recog  
SOURCE  = sortblob.o fixit.o hough.o rotate.o binarize.o split.o myutilities.o xshowimage.o imageproc.o pattern.o blobify.o recog.o net.o utilities.o

CC      = cc
CFLAGS  = -Aa -c -g  

$(PROJ) : $(SOURCE) 
	$(CC) $(SOURCE) -g -o $(PROJ) -lm -lX11  

$(SOURCE): 
	$(CC) $(CFLAGS) $*.c  



