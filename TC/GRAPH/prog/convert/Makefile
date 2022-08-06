

INC = -I../../src/include

CCFLAGS = $(ARCH) $(INC)

OBJ =   convert.o read_lw.o read_obj.o read_dxf.o read_dive.o

all:	convert

convert: $(OBJ) convert.h
	$(CC) $(CCFLAGS) -o convert $(OBJ) $(LIB)
	mv convert ../../bin

convert.o: convert.c convert.h
	$(CC) $(CCFLAGS) -c convert.c

read_lw.o: read_lw.c convert.h
	$(CC) $(CCFLAGS) -c read_lw.c

read_obj.o: read_obj.c convert.h
	$(CC) $(CCFLAGS) -c read_obj.c

read_dxf.o: read_dxf.c convert.h
	$(CC) $(CCFLAGS) -c read_dxf.c

read_dive.o: read_dive.c convert.h
	$(CC) $(CCFLAGS) -c read_dive.c

clean:
	rm -f $(OBJ)

