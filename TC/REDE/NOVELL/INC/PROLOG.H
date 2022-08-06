/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef _PROLOG_H
   #define _PROLOG_H
/* _______________________________________________________________________
	   File including definitions needed for LATTICE C,
	      MICROSOFT C, TURBO C, and WATCOM C
  ________________________________________________________________________*/

/*-- If prototype checking is NOT disabled, then the default will be
     to check prototypes.  To disable prototype checking, you should
     define PROTO_OFF on the compile command line --*/

#ifndef PROTO_OFF
   #define PROTOTYPE
#endif
    
#ifndef BYTE
   #define BYTE  unsigned char
#endif

#ifndef WORD
   #define WORD  unsigned short
#endif

#ifndef DWORD
   #define DWORD  unsigned long
#endif

#ifndef LONG
   #define LONG  unsigned long
#endif

/*********************************** LC60 ***********************************/

#ifdef LC60
  #define _ELIPSIS_

#ifndef NULL
   #ifdef SPTR
      #define NULL 0
   #else
      #define NULL 0L
   #endif
#endif

#define cdecl

#endif

/******************************** WAT70 *************************************/

#ifdef __WATCOMC__
   #include <stdarg.h>
   #include <stddef.h>
   #define _ELIPSIS_ ...

   #ifndef O_RAW
       #define O_RAW O_BINARY
   #endif

   #ifndef NPrintF
      #define NPrintF printf
   #endif

   #ifndef NULL
      #if defined(M_I86LM) || defined(M_I86CM) || defined(M_I86HM)
	      #define NULL 0L
      #else
	      #define NULL 0
      #endif
   #endif

   #ifndef Free
       #define Free(p) (free(p), 0)
   #endif

   #ifndef ANSI
       #define ANSI
   #endif
#endif

/******************************** TURBO C ***********************************/

/*
    if running TURBO 'C' pretend to be Microsoft with prototypes.
*/

#ifdef __TURBOC__
    #define MSC 000
    #define LINT_ARGS
    #ifndef ANSI
	   #define ANSI
    #endif
    #if defined(__TINY__)
	   #define M_I86TM __TINY__
    #elif defined(__SMALL__)
	   #define M_I86SM __SMALL__
    #elif defined(__MEDIUM__)
	   #define M_I86MM __MEDIUM__
    #elif defined(__COMPACT__)
	   #define M_I86CM __COMPACT__
    #elif defined(__LARGE__)
	   #define M_I86LM __LARGE__
    #elif defined(__HUGE__)
	   #define M_I86HM __HUGE__
    #endif
#endif

/******************************** MSC510 ************************************/

#ifdef MSC
    #define _ELIPSIS_ ...

    #if MSC == 400
	   #ifndef memmove
	      #define memmove(dest,src,n) memcpy((char *)(dest),(char *)(src),n)
	   #endif
    #endif

    #ifndef O_RAW
	   #define O_RAW O_BINARY
    #endif

    #ifndef NPrintF
	   #define NPrintF printf
    #endif

    #ifndef NULL
	   #if defined(M_I86LM) || defined(M_I86CM) || defined(M_I86HM)
	      #define NULL 0L
	   #else
	      #define NULL 0
	   #endif
    #endif

    #ifndef Free
	   #define Free(p) (free(p), 0)
    #endif
#endif

#ifdef MSC
   #if MSC == 510
	   #ifndef ANSI
	      #define ANSI
	   #endif
   #endif
#endif

/****************************************************************************/

#ifndef NOPROC
   #define NOPROC ((int (*)())0)
#endif

#ifndef TRUE
   #define TRUE	1
   #define FALSE	0
#endif


#ifdef LC310
    #define FARDATAPTR(type,var) WORD	var[2]
    #define FARCODEPTR(type,var) WORD	var[2]
#else
    #define FARDATAPTR(type,var) type far *var
    #define FARCODEPTR(type,var) type (far *var)()
#endif


/*-------------------------------------------------------------------------*/

#ifndef PROLOG_STDARG
   #include <stdarg.h>
   #define va_Special_start(ap,v,bytes) ap = (va_list)((char *)(&v) + \
      sizeof(v) + bytes)
   #define va_Special_arg(ap,t) (*((t*)(ap = (va_list)((char *)ap - \
      sizeof(t)))))
   #define va_Special_end(ap,v,n)
   #define PROLOG_STDARG
#endif




/******************************* DOS ERRORS *********************************/

#ifndef DOS_INVALID_FUNCTION_NUMBER

   /* DOS errors */
   #define DOS_INVALID_FUNCTION_NUMBER	    1
   #define DOS_FILE_NOT_FOUND		          2
   #define DOS_PATH_NOT_FOUND		          3
   #define DOS_TOO_MANY_OPEN_FILES 	       4
   #define DOS_ACCESS_DENIED		          5
   #define DOS_INVALID_FILE_HANDLE         6
   #define DOS_MEMORY_BLOCKS_DESTROYED	    7
   #define DOS_INSUFFICIENT_MEMORY 	       8
   #define DOS_INVALID_MEMORY_BLOCK_ADDR	 9
   #define DOS_INVALID_ENVIRONMENT 	      10
   #define DOS_INVALID_FORMAT		         11
   #define DOS_INVALID_ACCESS_CODE 	      12
   #define DOS_INVALID_DATA		         13
   #define DOS_INVALID_DRIVE_SPECIFIED	   15
   #define DOS_ATTEMPT_TO_DEL_CURRENT_DIR	16
   #define DOS_NOT_SAVE_DEVICE		      17
   #define DOS_NO_MORE_FILES		         18
   #define DOS_SHARING_VIOLATION		      32
   #define DOS_LOCK_VIOLATION		         33

#endif

#endif  /* End #ifdef _PROLOG_H */
