
INC = -I../include

#	Macro to sacrifice accuracy/detail for speed
#GMACRO = -DREAL_FAST
GMACRO = 


CCFLAGS = $(ARCH) $(INC) $(GMACRO)


OBJ =  eye.o camera.o light.o

all: $(LIBRARY)

camera.o: ../include/pstring.h ../include/matrix.h ../include/camera.h \
	../include/eye.h ../include/superclass.h camera.c
	$(CC) -c $(CCFLAGS) camera.c

eye.o: ../include/matrix.h ../include/eye.h ../include/pstring.h \
	../include/superclass.h eye.c
	$(CC) -c $(CCFLAGS) eye.c

light.o: ../include/camera.h ../include/light.h ../include/zbuffer.h ../include/eye.h \
	../include/shader.h ../include/pstring.h ../include/io.h \
	../include/superclass.h ../include/matrix.h ../include/linetype.h \
        ../include/base.h ../include/texture.h ../include/shading.h ../include/alex.h \
	../include/map.h ../include/memman.h light.c
	$(CC) -c $(CCFLAGS) light.c

$(LIBRARY): $(OBJ)
	$(MAKELIB) $(LIBRARY) $(OBJ)
	mv $(MVLIB) ../../lib

clean:
	rm -f $(OBJ)
