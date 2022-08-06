

SRC =	jcomapi.c jdapimin.c jdapistd.c jdatasrc.c jdcoefct.c jdcolor.c \
	jddctmgr.c jdhuff.c jdinput.c jdmainct.c jdmarker.c jdmaster.c \
        jdmerge.c jdphuff.c jdpostct.c jdsample.c jdtrans.c jerror.c \
        jidctflt.c jidctfst.c jidctint.c jidctred.c jmemmgr.c jmemnobs.c \
	jquant1.c jquant2.c jutils.c

OBJ =	jcomapi.o jdapimin.o jdapistd.o jdatasrc.o jdcoefct.o jdcolor.o \
	jddctmgr.o jdhuff.o jdinput.o jdmainct.o jdmarker.o jdmaster.o \
        jdmerge.o jdphuff.o jdpostct.o jdsample.o jdtrans.o jerror.o \
        jidctflt.o jidctfst.o jidctint.o jidctred.o jmemmgr.o jmemnobs.o \
	jquant1.o jquant2.o jutils.o

#libdecjpeg.a: $(OBJ)
#	ar r libdecjpeg.a $(OBJ)
#	mv libdecjpeg.a ../../lib

$(LIBRARY): $(OBJ)
	$(MAKELIB) $(LIBRARY) $(OBJ)
	mv $(MVLIB) ../../lib

$(OBJ):

clean:
	rm -f $(OBJ)
