



/* *************************************************************
   This file contains all the global data types for my programs
************************************************************* */

#if !defined (__GLOBAL_H)
#define __GLOBAL_H

#include "linetype.h"
#include "shading.h"

extern int PENCILTEST;
extern int NOSHADOW;
extern int NOSCREEN;
extern int SILENT;
extern int NIXMIP;

extern int GAMMA;
extern unsigned char GAMMALUT[1024];
extern unsigned char *PGAMMA;

extern int ANTIALIAS;
extern int DEFLST;
extern int RGBFILE;

extern int INTERLACE;
extern int X2;
extern char SANTIALIAS[MAXSTRLEN];
extern char SDEFLST[MAXSTRLEN];
extern char SRGBFILE[MAXSTRLEN];

extern shadetype  gshade;
extern linelist   gpline;

extern unsigned int WINX, WINY;

#endif
