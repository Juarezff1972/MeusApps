
INC = -I../include

#	Macro to sacrifice accuracy/detail for speed
#GMACRO = -DREAL_FAST
GMACRO = 


CCFLAGS = $(ARCH) $(INC) $(GMACRO)


OBJ =  perlin.o matrix.o lookup.o collision.o geomath.o orient.o spline.o

all: $(LIBRARY)

perlin.o: ../include/matrix.h perlin.c
	$(CC) -c $(CCFLAGS) perlin.c

matrix.o: ../include/matrix.h matrix.c
	$(CC) -c $(CCFLAGS) matrix.c

lookup.o: ../include/matrix.h lookup.c
	$(CC) -c $(CCFLAGS) lookup.c

collision.o: ../include/matrix.h collision.c
	$(CC) -c $(CCFLAGS) collision.c

geomath.o: ../include/matrix.h geomath.c
	$(CC) -c $(CCFLAGS) geomath.c

orient.o: ../include/matrix.h orient.c
	$(CC) -c $(CCFLAGS) orient.c

spline.o: ../include/matrix.h spline.c
	$(CC) -c $(CCFLAGS) spline.c

$(LIBRARY): $(OBJ)
	$(MAKELIB) $(LIBRARY) $(OBJ)
	mv $(MVLIB) ../../lib

clean:
	rm -f $(OBJ)
