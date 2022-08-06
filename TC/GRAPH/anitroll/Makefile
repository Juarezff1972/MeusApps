
SRC =	buckey.c helix.c neutron.c chain.c tachyon.c \
	atom.c electron.c ion.c photon.c tube.c \
	boid.c funnel.c molecule.c quark.c flight.c wwdigo.c lock.c

OBJ =	buckey.o helix.o neutron.o chain.o tachyon.o \
	atom.o electron.o ion.o photon.o tube.o \
	boid.o funnel.o molecule.o quark.o flight.o wwdigo.o lock.o

_H =	boidbait.h chain.h tube.h helix.h molecule.h anitroll.h \
	buckey.h electron.h funnel.h ion.h neutron.h quark.h \
	boid.h atom.h tachyon.h flight.h photon.h shell.h wwdigo.h lock.h

INC = -I../src/include


anitroll:	anitroll.c $(LIBRARY)
	$(CC) -o anitroll  anitroll.c $(INC) $(CFLAGS) $(LIB)
	mv anitroll ../bin

anitroll.o: $(_H) anitroll.c
	$(CC) -c $(INC) $(CFLAGS) anitroll.c

atom.o: $(_H) atom.c
	$(CC) -c $(INC) $(CFLAGS) atom.c

buckey.o: $(_H) buckey.c
	$(CC) -c $(INC) $(CFLAGS) buckey.c

chain.o: $(_H) chain.c
	$(CC) -c $(INC) $(CFLAGS) chain.c

electron.o: $(_H) electron.c
	$(CC) -c $(INC) $(CFLAGS) electron.c

flight.o: $(_H) flight.c
	$(CC) -c $(INC) $(CFLAGS) flight.c

wwdigo.o: $(_H) wwdigo.c
	$(CC) -c $(INC) $(CFLAGS) wwdigo.c

lock.o: $(_H) lock.c
	$(CC) -c $(INC) $(CFLAGS) lock.c

funnel.o: $(_H) funnel.c
	$(CC) -c $(INC) $(CFLAGS) funnel.c

helix.o: $(_H) helix.c
	$(CC) -c $(INC) $(CFLAGS) helix.c

ion.o: $(_H) ion.c
	$(CC) -c $(INC) $(CFLAGS) ion.c

molecule.o: $(_H) molecule.c
	$(CC) -c $(INC) $(CFLAGS) molecule.c

neutron.o: $(_H) neutron.c
	$(CC) -c $(INC) $(CFLAGS) neutron.c

photon.o: $(_H) photon.c
	$(CC) -c $(INC) $(CFLAGS) photon.c

quark.o: $(_H) quark.c
	$(CC) -c $(INC) $(CFLAGS) quark.c

tachyon.o: $(_H) tachyon.c
	$(CC) -c $(INC) $(CFLAGS) tachyon.c

tube.o: $(_H) tube.c
	$(CC) -c $(INC) $(CFLAGS) tube.c

boid.o:	$(_H) boid.c
	$(CC) -c $(INC) $(CFLAGS) boid.c

$(LIBRARY): $(OBJ)
	$(MAKELIB) $(LIBRARY) $(OBJ)
	mv $(MVLIB) ../lib

clean:
	rm -f $(OBJ)

