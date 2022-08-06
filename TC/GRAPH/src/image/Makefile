
INC = -I../include -I../jpeg -I../tiff

#	Macro to sacrifice accuracy/detail for speed
#GMACRO = -DREAL_FAST
GMACRO = 


CCFLAGS = $(ARCH) $(INC) $(GMACRO)


OBJ =  gif.o ilbm.o jpeg.o map.o ppm.o rgb.o texture.o tiff.o

all: $(LIBRARY)

map.o: ../include/map.h map.c
	$(CC) -c $(CCFLAGS) map.c

rgb.o: ../include/map.h ../include/io.h ../include/texture.h rgb.c
	$(CC) -c $(CCFLAGS) rgb.c

ilbm.o: ../include/map.h ../include/io.h ../include/texture.h ilbm.c
	$(CC) -c $(CCFLAGS) ilbm.c

jpeg.o: ../include/map.h ../include/io.h ../include/texture.h jpeg.c
	$(CC) -c $(CCFLAGS) jpeg.c

gif.o: ../include/map.h ../include/io.h ../include/texture.h gif.c
	$(CC) -c $(CCFLAGS) gif.c

ppm.o: ../include/map.h ../include/io.h ../include/texture.h ppm.c
	$(CC) -c $(CCFLAGS) ppm.c

texture.o: ../include/map.h ../include/io.h ../include/texture.h ../include/matrix.h texture.c
	$(CC) -c $(CCFLAGS) texture.c

tiff.o: ../include/map.h ../include/io.h ../include/texture.h tiff.c
	$(CC) -c $(CCFLAGS) tiff.c

$(LIBRARY): $(OBJ)
	$(MAKELIB) $(LIBRARY) $(OBJ)
	mv $(MVLIB) ../../lib

clean:
	rm -f $(OBJ)
