


#if !defined (__ANITROLL_H)

#define __ANITROLL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRLEN 256
#define MAXKNOT   100
#define MAXOPTION  10 

#define WF       0
#define FRAME    1
#define CONTINUE 4

class quark;

typedef struct linkstruct {                     // list of children

   quark             *link;                     // child data
   struct linkstruct *next;                     // next child
} linktype;


typedef struct eventstruct {                    // event structure

   int                event;                    // event time
   float              timefactor;               // time multiplier
   char               efilename[MAXSTRLEN];     // event file
   struct eventstruct *next;
} eventtype;


class atom;
class chain;
class electron;
class photon;
class neutron;
class ion;
class tachyon;
class helix;
class molecule;
class buckey;
class tube;
class funnel;
class flight;
class shell;
class wwdigo;
class lock;

#include "quark.h"
#include "atom.h"
#include "chain.h"
#include "electron.h"
#include "photon.h"
#include "neutron.h"
#include "ion.h"
#include "tachyon.h"
#include "helix.h"
#include "molecule.h"
#include "buckey.h"
#include "tube.h"
#include "funnel.h"
#include "wwdigo.h"
#include "lock.h"
#include "boidbait.h"
#include "boid.h"
#include "flight.h"
#include "shell.h"

#endif
