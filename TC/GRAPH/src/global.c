

#include <string.h>
#include "global.h"

int PENCILTEST = 0;
int NOSHADOW = 0;
int NOSCREEN = 0;

#ifdef DOS
int SILENT = 1;
#else
int SILENT = 0;
#endif

int NIXMIP = 0;

int  GAMMA = 0;
unsigned char GAMMALUT[1024];
unsigned char *PGAMMA = &GAMMALUT[256];

int ANTIALIAS = 0;
int DEFLST = 0;
int RGBFILE = 0;

int INTERLACE = 0;
int X2 = 0;

char SANTIALIAS[MAXSTRLEN];
char SDEFLST[MAXSTRLEN];
char SRGBFILE[MAXSTRLEN];

shadetype  gshade;
linelist   gpline;

unsigned int WINX = 640;
unsigned int WINY = 480;
