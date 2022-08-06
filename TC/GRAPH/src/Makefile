
INC = -Itiff -Ijpeg -Iinclude

#	Macro to sacrifice accuracy/detail for speed
#GMACRO = -DREAL_FAST
GMACRO = 


CCFLAGS = $(ARCH) $(INC) $(GMACRO)


OBJ =	shading.o linetype.o pstring.o zbuffer.o io.o memman.o \
        global.o datatype.o alex.o \
	shaders.o shader.o texobj.o superclass.o base.o geo.o cone.o \
        cylinder.o sphere.o polyclet.o polyfile.o polyform.o polyshad.o \
        polyscan.o spatfile.o spatscan.o shadscan.o partscan.o shadowpt.o \
     	radiant.o line.o shadline.o sbfast.o sbrender.o sbbeam.o

H =	include/alex.h include/eye.h include/line.h include/particle.h include/shader.h \
	include/spatch.h include/zbuffer.h include/base.h include/geo.h include/linetype.h \
	include/pixcon.h include/shaders.h include/sphere.h include/camera.h include/global.h \
	include/loader.h include/polygon.h include/shading.h include/superclass.h \
	include/cone.h include/io.h include/map.h include/pstring.h include/shadline.h \
	include/texobj.h include/cylinder.h include/matrix.h include/radiant.h \
	include/shadow.h include/texture.h include/datatype.h include/light.h include/memman.h \
	include/sbfast.h include/shadowpt.h

render: $(LIBRARY) loader.o stuff.o engine.o render.c
	$(CC) $(CCFLAGS) -o render render.c stuff.o loader.o engine.o $(LIB)
	mv render ../bin

pixcon: $(LIBRARY) loader.o stuff.o engine.o pixcon.c
	$(CC) $(CCFLAGS) -o pixcon pixcon.c stuff.o loader.o engine.o $(LIB)
	mv pixcon ../bin

stuff.o: $(H) stuff.c
	$(CC) -c $(CCFLAGS) stuff.c

loader.o: $(H) loader.c
	$(CC) -c $(CCFLAGS) loader.c

engine.o: $(H) engine.c
	$(CC) -c $(CCFLAGS) engine.c

# ################### LIBGRAPH #######################

alex.o: include/linetype.h include/alex.h include/map.h include/pstring.h alex.c
	$(CC) -c $(CCFLAGS) alex.c

base.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/io.h \
	include/linetype.h include/datatype.h include/global.h base.c
	$(CC) -c $(CCFLAGS) base.c

cone.o:	include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/global.h \
	include/linetype.h include/polygon.h include/shadow.h include/geo.h \
	include/datatype.h include/texobj.h include/io.h include/cone.h cone.c
	$(CC) -c $(CCFLAGS) cone.c

cylinder.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/global.h \
	include/linetype.h include/polygon.h include/shadow.h include/geo.h \
	include/datatype.h include/texobj.h include/io.h include/cylinder.h \
	cylinder.c
	$(CC) -c $(CCFLAGS) cylinder.c

datatype.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/io.h \
	include/linetype.h include/datatype.h datatype.c
	$(CC) -c $(CCFLAGS) datatype.c

geo.o:	include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/io.h \
	include/linetype.h include/polygon.h include/shadow.h include/geo.h \
	include/datatype.h include/texobj.h include/global.h geo.c 
	$(CC) -c $(CCFLAGS) geo.c

global.o: include/global.h include/linetype.h include/shading.h \
	include/matrix.h include/pstring.h global.c
	$(CC) -c $(CCFLAGS) global.c

io.o: include/io.h include/pstring.h io.c
	$(CC) -c $(CCFLAGS) io.c

line.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/io.h \
	include/linetype.h include/global.h include/line.h include/shadline.h \
	line.c
	$(CC) -c $(CCFLAGS) line.c

linetype.o: include/linetype.h linetype.c
	$(CC) -c $(CCFLAGS) linetype.c

memman.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/io.h \
	include/linetype.h include/polygon.h include/shadow.h \
	include/datatype.h include/texobj.h include/memman.h memman.c
	$(CC) -c $(CCFLAGS) memman.c

partscan.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/io.h \
	include/linetype.h include/particle.h include/shadowpt.h \
	include/global.h partscan.c
	$(CC) -c $(CCFLAGS) partscan.c

polyclet.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h include/memman.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/io.h \
	include/linetype.h include/polygon.h include/shadow.h \
	include/datatype.h include/texobj.h polyclet.c
	$(CC) -c $(CCFLAGS) polyclet.c

polyfile.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/io.h \
	include/linetype.h include/polygon.h include/shadow.h \
	include/datatype.h include/texobj.h polyfile.c
	$(CC) -c $(CCFLAGS) polyfile.c

polyform.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/io.h \
	include/linetype.h include/polygon.h include/shadow.h \
	include/datatype.h include/texobj.h polyform.c
	$(CC) -c $(CCFLAGS) polyform.c

polyscan.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/global.h \
	include/linetype.h include/polygon.h include/shadow.h include/io.h \
	include/datatype.h include/texobj.h include/memman.h polyscan.c
	$(CC) -c $(CCFLAGS) polyscan.c

polyshad.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/io.h \
	include/linetype.h include/polygon.h include/shadow.h include/global.h \
	include/datatype.h include/texobj.h include/memman.h polyshad.c \
	polywfbw.c polybw.c polyct.c polygd.c polyal.c polypg.c polylt.c polyplate.c
	$(CC) -c $(CCFLAGS) polyshad.c

pstring.o: pstring.c
	$(CC) -c $(CCFLAGS) pstring.c

radiant.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/global.h \
	include/linetype.h include/polygon.h include/shadow.h include/geo.h \
	include/datatype.h include/texobj.h include/io.h include/sphere.h \
	include/radiant.h include/global.h radiant.c
	$(CC) -c $(CCFLAGS) radiant.c

sbfast.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/io.h \
	include/linetype.h include/datatype.h include/global.h include/texobj.h \
	include/sbfast.h sbfast.c
	$(CC) -c $(CCFLAGS) sbfast.c

sbbeam.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/io.h \
	include/linetype.h include/datatype.h include/global.h include/texobj.h \
	include/sbfast.h sbbeam.c sblt.c
	$(CC) -c $(CCFLAGS) sbbeam.c

sbrender.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/io.h \
	include/linetype.h include/datatype.h include/global.h include/texobj.h \
	include/sbfast.h sbrender.c sbshade.c
	$(CC) -c $(CCFLAGS) sbrender.c

shader.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/io.h \
	include/linetype.h include/shaders.h shader.c
	$(CC) -c $(CCFLAGS) shader.c

shaders.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/linetype.h include/sbfast.h include/texobj.h include/io.h \
	include/alex.h include/map.h include/pstring.h include/datatype.h shaders.c
	$(CC) -c $(CCFLAGS) shaders.c

shading.o: include/shading.h include/matrix.h include/pstring.h shading.c
	$(CC) -c $(CCFLAGS) shading.c

shadline.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/io.h \
	include/linetype.h include/line.h include/shadline.h \
	shadline.c
	$(CC) -c $(CCFLAGS) shadline.c

shadowpt.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/io.h \
	include/linetype.h include/particle.h include/shadowpt.h \
	shadowpt.c
	$(CC) -c $(CCFLAGS) shadowpt.c

shadscan.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/global.h \
	include/linetype.h include/polygon.h include/shadow.h include/io.h \
	include/datatype.h include/texobj.h include/memman.h polyplate.c shadct.c shadscan.c
	$(CC) -c $(CCFLAGS) shadscan.c

spatfile.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/io.h \
	include/linetype.h include/polygon.h include/shadow.h include/spatch.h \
	include/datatype.h include/texobj.h spatfile.c
	$(CC) -c $(CCFLAGS) spatfile.c

spatscan.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/io.h \
	include/linetype.h include/polygon.h include/shadow.h include/spatch.h \
	include/datatype.h include/texobj.h include/global.h spatscan.c
	$(CC) -c $(CCFLAGS) spatscan.c

sphere.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/global.h \
	include/linetype.h include/polygon.h include/shadow.h include/geo.h \
	include/datatype.h include/texobj.h include/io.h include/sphere.h \
	include/radiant.h sphere.c
	$(CC) -c $(CCFLAGS) sphere.c

superclass.o: include/superclass.h include/pstring.h superclass.c
	$(CC) -c $(CCFLAGS) superclass.c

texobj.o: include/camera.h include/light.h include/zbuffer.h include/shader.h \
	include/eye.h include/linetype.h include/superclass.h \
	include/base.h include/texture.h include/shading.h include/matrix.h \
	include/alex.h include/map.h include/pstring.h include/io.h \
	include/linetype.h include/texobj.h include/datatype.h texobj.c
	$(CC) -c $(CCFLAGS) texobj.c

zbuffer.o: include/linetype.h include/alex.h include/texture.h \
	include/zbuffer.h include/io.h include/map.h include/matrix.h \
	include/pstring.h zbuffer.c
	$(CC) -c $(CCFLAGS) zbuffer.c

$(LIBRARY): $(OBJ)
	$(MAKELIB) $(LIBRARY) $(OBJ)
	mv $(MVLIB) ../lib

clean:
	rm -f $(OBJ) pixcon.o stuff.o loader.o engine.o
