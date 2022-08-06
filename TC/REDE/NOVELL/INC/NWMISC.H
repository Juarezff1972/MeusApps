/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef _NWMISC_H
#define _NWMISC_H
/*______________________________________________________________

	Prototypes for the Netware API Utilities Logic
  ______________________________________________________________*/


#ifndef _PROLOG_H
   #include <c:\tc\rede\novell\inc\prolog.h>
#endif

extern BYTE DOSError;	/* Used to return DOS Errors from INT 21 calls */

#ifdef PROTOTYPE
   extern char	      *AllocNITBuffer(void);
   extern int	cdecl ASCIIZToLenStr(char *, char *);
   extern int	      ConvertNameToFullPath(char *, char *);
   extern int	      ConvertNameToVolumePath(char *, char *);
   extern int	      ConvertToUpperCase(char *);
   extern void       Dummy2(void);
   extern int	      GetFullPath(char, char *);
   extern int	      GetMaxSearchOrder(char *);
   extern int	cdecl IntSwap(int);
   extern char* cdecl LenStrCat(char *, char *);
   extern int	cdecl LenStrCmp(char *, char *);
   extern char* cdecl LenStrCpy(char *, char *);
   extern int	cdecl LenToASCIIZStr(char *, char *);
   extern long	cdecl LongSwap(long);
   extern int	      ParsePath(char *, char *, char *, char *);
   extern char	      *StripFileServerFromPath(char *, char *);
   extern int	      IsWhiteSpace( char );
   #ifdef LATTICE
      extern int	memicmp( char *, char *, WORD );
   #endif
#else
   extern char	      *AllocNITBuffer();
   extern int	cdecl ASCIIZToLenStr();
   extern int	      ConvertNameToFullPath();
   extern int	      ConvertNameToVolumePath();
   extern int	      ConvertToUpperCase();
   extern void       Dummy2();
   extern int	      GetFullPath();
   extern int	      GetMaxSearchOrder();
   extern int	cdecl IntSwap();
   extern char* cdecl LenStrCat();
   extern int	cdecl LenStrCmp();
   extern char* cdecl LenStrCpy();
   extern int	cdecl LenToASCIIZStr();
   extern long	cdecl LongSwap();
   extern int	      ParsePath();
   extern char	      *StripFileServerFromPath();
   extern int	      IsWhiteSpace();
   #ifdef LATTICE
      extern int	memicmp();
   #endif
#endif

#endif
