

DIR=`cd lib ; pwd`

all:
	@echo ""
	@echo "	Versions supported:"
	@echo "	make sgi5"
	@echo "	make sgi5_dso"
	@echo "	make sgi64"
	@echo "	make linux"
	@echo "	make linux_dso"
	@echo ""

sgi5:
	cd src/tiff;make \
		"COPTS = -O2 -mips2" \
		"LIBRARY = libtiff.a" \
		"MAKELIB = ar r" \
		"MVLIB = libtiff.a"
	cd src/jpeg;make \
		"CC = CC" \
		"CFLAGS = -O2 -mips2" \
		"LIBRARY = libdecjpeg.a" \
		"MAKELIB = ar r" \
		"MVLIB = libdecjpeg.a"
	cd src/eye;make \
		"CC = CC" \
		"ARCH = -O2 -mips2 -DSGI" \
		"LIBRARY = libeye.a" \
		"MAKELIB = ar r" \
		"MVLIB = libeye.a"
	cd src/math3d;make \
		"CC = CC" \
		"ARCH = -O2 -mips2 -DSGI" \
		"LIBRARY = libmath3d.a" \
		"MAKELIB = ar r" \
		"MVLIB = libmath3d.a"
	cd src/image;make \
		"CC = CC" \
		"ARCH = -O2 -mips2 -DSGI" \
		"LIBRARY = libimage.a" \
		"MAKELIB = ar r" \
		"MVLIB = libimage.a"
	cd src;make \
		"CC = CC" \
		"ARCH = -mips2 -DSGI -O2" \
		"LIBRARY = libgraph.a" \
		"MAKELIB = ar r" \
		"MVLIB = libgraph.a" \
		"LIB = -limage -lgraph -leye -lmath3d -ldecjpeg -ltiff -L../lib -lGL -lX11 -lm"
	cd anitroll;make \
		"CC = CC" \
		"CFLAGS = -O2 -mips2 -DSGI" \
		"LIBRARY = libanitroll.a" \
		"MAKELIB = ar r" \
		"MVLIB = libanitroll.a" \
		"LIB = -L../lib -lanitroll -limage -lgraph -leye -lmath3d -ldecjpeg -ltiff -lGL -lm -lX11"
	cd prog;make \
		"CC = CC" \
		"ARCH = -mips2 -DSGI -O2" \
		"LIB = -L../lib -limage -lgraph -leye -lmath3d -ldecjpeg -ltiff -lGL -lX11"
	cd prog/convert;make \
		"CC = CC" \
		"ARCH = -mips2 -DSGI -O2" \
		"LIB = -L../../lib -limage -lgraph -leye -lmath3d -ldecjpeg -ltiff -lm -lGL -lX11"

sgi5_dso:
	cd src/tiff;make \
		"COPTS = -O2 -mips2" \
		"LIBRARY = libtiff.so" \
		"MAKELIB = ../../mklib.irix5" \
		"MVLIB = libtiff.so"
	cd src/jpeg;make \
		"CC = CC" \
		"CFLAGS = -O2 -mips2" \
		"LIBRARY = libdecjpeg.so" \
		"MAKELIB = ../../mklib.irix5" \
		"MVLIB = libdecjpeg.so"
	cd src/eye;make \
		"CC = CC" \
		"ARCH = -O2 -mips2 -DSGI" \
		"LIBRARY = libeye.so" \
		"MAKELIB = ../../mklib.irix5" \
		"MVLIB = libeye.so"
	cd src/math3d;make \
		"CC = CC" \
		"ARCH = -O2 -mips2 -DSGI" \
		"LIBRARY = libmath3d.so" \
		"MAKELIB = ../../mklib.irix5" \
		"MVLIB = libmath3d.so"
	cd src/image;make \
		"CC = CC" \
		"ARCH = -O2 -mips2 -DSGI" \
		"LIBRARY = libimage.so" \
		"MAKELIB = ../../mklib.irix5" \
		"MVLIB = libimage.so"
	cd src;make \
		"CC = CC" \
		"ARCH = -mips2 -DSGI -O2" \
		"LIBRARY = libgraph.so" \
		"MAKELIB = ../mklib.irix5" \
		"MVLIB = libgraph.so" \
		"LIB = -limage -lgraph -leye -lmath3d -ldecjpeg -ltiff -L../lib -lGL -lX11 -lm"
	cd anitroll;make \
		"CC = CC" \
		"CFLAGS = -O2 -mips2 -DSGI" \
		"LIBRARY = libanitroll.so" \
		"MAKELIB = ../mklib.irix5" \
		"MVLIB = libanitroll.so" \
		"LIB = -L../lib -limage -lgraph -lanitroll -leye -lmath3d -ldecjpeg -ltiff -lGL -lm -lX11"
	cd prog;make \
		"CC = CC" \
		"ARCH = -mips2 -DSGI -O2" \
		"LIB = -L../lib -limage -lgraph -leye -lmath3d -ldecjpeg -ltiff -lGL -lX11"
	cd prog/convert;make \
		"CC = CC" \
		"ARCH = -mips2 -DSGI -O2" \
		"LIB = -L../../lib -limage -lgraph -leye -lmath3d -ldecjpeg -ltiff -lm -lGL -lX11"
	ld_library_path

sgi64:
	cd src/tiff;make \
		"COPTS = -O3 -mips4" \
		"LIBRARY = libtiff.a" \
		"MAKELIB = ar r" \
		"MVLIB = libtiff.a"
	cd src/jpeg;make \
		"CC = CC" \
		"CFLAGS = -O3 -mips4" \
		"LIBRARY = libdecjpeg.a" \
		"MAKELIB = ar r" \
		"MVLIB = libdecjpeg.a"
	cd src/eye;make \
		"CC = CC" \
		"ARCH = -O3 -mips4 -DSGI" \
		"LIBRARY = libeye.a" \
		"MAKELIB = ar r" \
		"MVLIB = libeye.a"
	cd src/math3d;make \
		"CC = CC" \
		"ARCH = -O3 -mips4 -DSGI" \
		"LIBRARY = libmath3d.a" \
		"MAKELIB = ar r" \
		"MVLIB = libmath3d.a"
	cd src/image;make \
		"CC = CC" \
		"ARCH = -O3 -mips4 -DSGI" \
		"LIBRARY = libimage.a" \
		"MAKELIB = ar r" \
		"MVLIB = libimage.a"
	cd src;make \
		"CC = CC" \
		"ARCH = -mips4 -DSGI -O3" \
		"LIBRARY = libgraph.a" \
		"MAKELIB = ar r" \
		"MVLIB = libgraph.a" \
		"LIB = -limage -lgraph -leye -lmath3d -ldecjpeg -ltiff -L../lib -lGL -lX11 -lm"
	cd anitroll;make \
		"CC = CC" \
		"CFLAGS = -O3 -mips4 -DSGI" \
		"LIBRARY = libanitroll.a" \
		"MAKELIB = ar r" \
		"MVLIB = libanitroll.a" \
		"LIB = -L../lib -lanitroll -limage -lgraph -leye -lmath3d -ldecjpeg -ltiff -lGL -lm -lX11"
	cd prog;make \
		"CC = CC" \
		"ARCH = -mips4 -DSGI -O3" \
		"LIB = -L../lib -limage -lgraph -leye -lmath3d -ldecjpeg -ltiff -lGL -lm -lX11"
	cd prog/convert;make \
		"CC = CC" \
		"ARCH = -mips4 -DSGI -O3" \
		"LIB = -L../../lib -limage -lgraph -leye -lmath3d -ldecjpeg -ltiff -lGL -lm -lX11"

linux:
	cd src/tiff;make \
		"COPTS = -O2" \
		"LIBRARY = libtiff.a" \
		"MAKELIB = ar r" \
		"MVLIB = libtiff.a"
	cd src/jpeg;make \
		"CC = g++" \
		"CFLAGS = -O2" \
		"LIBRARY = libdecjpeg.a" \
		"MAKELIB = ar r" \
		"MVLIB = libdecjpeg.a"
	cd src/eye;make \
		"CC = g++" \
		"ARCH = -O2 -DLINUX" \
		"LIBRARY = libeye.a" \
		"MAKELIB = ar r" \
		"MVLIB = libeye.a"
	cd src/math3d;make \
		"CC = g++" \
		"ARCH = -O2 -DLINUX" \
		"LIBRARY = libmath3d.a" \
		"MAKELIB = ar r" \
		"MVLIB = libmath3d.a"
	cd src/image;make \
		"CC = g++" \
		"ARCH = -O2 -DLINUX" \
		"LIBRARY = libimage.a" \
		"MAKELIB = ar r" \
		"MVLIB = libimage.a"
	cd src;make \
		"CC = g++" \
		"ARCH = -DLINUX -O2" \
		"LIBRARY = libgraph.a" \
		"MAKELIB = ar r" \
		"MVLIB = libgraph.a" \
		"LIB = -limage -lgraph -leye -lmath3d -ldecjpeg -ltiff -L../lib -L/usr/local/lib -L/usr/X11R6/lib -lMesaGL -lX11 -lXext -lm -lieee"
	cd anitroll;make \
		"CC = g++" \
		"CFLAGS = -O2 -DLINUX" \
		"LIBRARY = libanitroll.a" \
		"MAKELIB = ar r" \
		"MVLIB = libanitroll.a" \
		"LIB = -L../lib -L/usr/X11R6/lib -L/usr/local/lib -lanitroll -limage -lgraph -leye -lmath3d -ldecjpeg -ltiff -lMesaGL -lm -lX11 -lXext"
	cd prog;make \
		"CC = g++" \
		"ARCH = -DLINUX -O2" \
		"LIB = -L../lib -L/usr/X11R6/lib -L/usr/local/lib -limage -lgraph -leye -lmath3d -ldecjpeg -ltiff -lMesaGL -lm -lX11 -lXext"
	cd prog/convert;make \
		"CC = g++" \
		"ARCH = -DLINUX -O2" \
		"LIB = -L../../lib -L/usr/X11R6/lib -L/usr/local/lib -limage -lgraph -leye -lmath3d -ldecjpeg -ltiff -lMesaGL -lm -lX11 -lXext"

linux_dso:
	cd src/tiff;make \
		"COPTS = -O2" \
		"LIBRARY = libtiff.so" \
		"MAKELIB = ../../mklib.linux" \
		"MVLIB = libtiff.a libtiff.so libtiff.so.1 libtiff.so.1.0.4"
	cd src/jpeg;make \
		"CC = g++" \
		"CFLAGS = -O2" \
		"LIBRARY = libdecjpeg.so" \
		"MAKELIB = ../../mklib.linux" \
		"MVLIB = libdecjpeg.a libdecjpeg.so libdecjpeg.so.1 libdecjpeg.so.1.0.4"
	cd src/eye;make \
		"CC = g++" \
		"ARCH = -DLINUX -O2" \
		"LIBRARY = libeye.so" \
		"MAKELIB = ../../mklib.linux" \
		"MVLIB = libeye.a libeye.so libeye.so.1 libeye.so.1.0.4"
	cd src/math3d;make \
		"CC = g++" \
		"ARCH = -DLINUX -O2" \
		"LIBRARY = libmath3d.so" \
		"MAKELIB = ../../mklib.linux" \
		"MVLIB = libmath3d.a libmath3d.so libmath3d.so.1 libmath3d.so.1.0.4"
	cd src/image;make \
		"CC = g++" \
		"ARCH = -DLINUX -O2" \
		"LIBRARY = libimage.so" \
		"MAKELIB = ../../mklib.linux" \
		"MVLIB = libimage.a libimage.so libimage.so.1 libimage.so.1.0.4"
	cd src;make \
		"CC = g++" \
		"ARCH = -DLINUX -O2" \
		"LIBRARY = libgraph.so" \
		"MAKELIB = ../mklib.linux" \
		"MVLIB = libgraph.a libgraph.so libgraph.so.1 libgraph.so.1.0.4" \
		"LIB = -limage -lgraph -leye -lmath3d -ldecjpeg -ltiff -L../lib -L/usr/local/lib -L/usr/X11R6/lib -lMesaGL -lX11 -lXext -lm -lieee"
	cd anitroll;make \
		"CC = g++" \
		"CFLAGS = -O2 -DLINUX" \
		"LIBRARY = libanitroll.so" \
		"MAKELIB = ../mklib.linux" \
		"MVLIB = libanitroll.a libanitroll.so libanitroll.so.1 libanitroll.so.1.0.4" \
		"LIB = -L../lib -L/usr/X11R6/lib -L/usr/local/lib -lanitroll -limage -lgraph -leye -lmath3d -ldecjpeg -ltiff -lMesaGL -lm -lX11 -lXext"
	cd prog;make \
		"CC = g++" \
		"ARCH = -DLINUX -O2" \
		"LIB = -L../lib -L/usr/X11R6/lib -L/usr/local/lib -limage -lgraph -leye -lmath3d -ldecjpeg -ltiff -lMesaGL -lm -lX11 -lXext"
	cd prog/convert;make \
		"CC = g++" \
		"ARCH = -DLINUX -O2" \
		"LIB = -L../../lib -L/usr/X11R6/lib -L/usr/local/lib -limage -lgraph -leye -lmath3d -ldecjpeg -ltiff -lMesaGL -lm -lX11 -lXext"
	ld_library_path

clean:
	cd src/tiff;make clean "LIBRARY = NA"
	cd src/jpeg;make clean "LIBRARY = NA"
	cd src/eye;make clean "LIBRARY = NA"
	cd src/math3d;make clean "LIBRARY = NA"
	cd src/image;make clean "LIBRARY = NA"
	cd src;make clean "LIBRARY = NA"
	cd anitroll;make clean "LIBRARY = NA"
	cd prog;make clean
	cd prog/convert;make clean
