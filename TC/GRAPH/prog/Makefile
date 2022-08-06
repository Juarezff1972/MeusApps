

INC = -I../src/tiff -I../src/jpeg -I../src/include

CCFLAGS = $(ARCH) $(INC)

OBJ =   flip.o mflip.o viewer.o movie.o morph.o 

all:	flip mflip viewer movie morph

flip: flip.c
	$(CC) $(CCFLAGS) -o flip flip.c $(LIB)
	mv flip ../bin

mflip: mflip.c
	$(CC) $(CCFLAGS) -o mflip mflip.c $(LIB)
	mv mflip ../bin

movie: movie.c
	$(CC) $(CCFLAGS) -o movie movie.c $(LIB)
	mv movie ../bin

morph:	morph.c
	$(CC) $(CCFLAGS) -o morph morph.c $(LIB)
	mv morph ../bin

viewer: viewer.c
	$(CC) $(CCFLAGS) -o viewer viewer.c $(LIB)
	mv viewer ../bin

clean:
	rm -f $(OBJ)

