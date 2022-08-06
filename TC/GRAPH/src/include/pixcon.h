



#if !defined __PIXCON_H
#define __PIXCON_H

#include "base.h"
#include "datatype.h"
#include "global.h"

class windowtype {

   public:
      AOGLwindow win;
      zbuffer zbuff;
};


void parseinput(int argc, char *argv[]);
int  read_list(char *filename, listtype *mainlist, int *mapcount);
int  parse_list(char *filename, listtype *mainlist, int *mapcount);
void init_graph();
void spawn(camera *cparm, light *lparm, light *bparm, pc *object_list, pc *rad_list, int frame, mapul *mapbuffer, zbuffer *zbuff);

#endif
