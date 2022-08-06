/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef _NDT_H
#define _NDT_H
/*______________________________________________________________

    Definitions and structures for Netware API DOS Utilities logic

  ______________________________________________________________*/

#ifndef _PROLOG_H
   #include <prolog.h>
#endif


#ifndef LC310
   #ifdef PROTOTYPE
      #define _MAIN_PROTOTYPE_		  void main(int, char **);
   #else
      #define _MAIN_PROTOTYPE_		  void main();
   #endif
   #define _MAIN_WITH_COMMAND_LINE_    void main(argc, argv) int argc; char **argv;
   #define _COMMAND_LINE_DECLARATIONS_
   #define _PARSECOMMANDLINE_
#else
   #ifdef PROTOTYPE
      #define _MAIN_PROTOTYPE_		  void main(void);
   #else
      #define _MAIN_PROTOTYPE_		  void main();
   #endif
   #define _MAIN_WITH_COMMAND_LINE_    void main()
   #define _COMMAND_LINE_DECLARATIONS_ int argc; char **argv; char argBuffer[256];
   #define _PARSECOMMANDLINE_	       argc = ParseCommandLine(argBuffer,&argv);
#endif


/*_____________________________________________________________________________

   -- This defines the data structure returned by FindFirst/FindNext calls --
      The first 21 bytes are officially undefined, but the following definition
      is that used if the search is done on a network drive
  ____________________________________________________________________________*/

/* File Attributes */

#ifndef FA_READ_ONLY
   #define FA_NORMAL		   ((BYTE)0x00)
   #define FA_READ_ONLY 	   ((BYTE)0x01)

   #ifndef FA_HIDDEN
      #define FA_HIDDEN 	   ((BYTE)0x02)
   #endif

   #ifndef FA_SYSTEM
      #define FA_SYSTEM 	   ((BYTE)0x04)
   #endif

   #define FA_EXECUTE_ONLY	   ((BYTE)0x08)
   #define FA_DIRECTORY 	   ((BYTE)0x10)
   #define FA_NEEDS_ARCHIVED	   ((BYTE)0x20)
   #define FA_SHAREABLE 	   ((BYTE)0x80)

/* Extended file attributes */
   #define FA_TRANSACTIONAL	   ((BYTE)0x10)
   #define FA_INDEXING		   ((BYTE)0x20)
   #define FA_AUDIT_READ	   ((BYTE)0x40)
   #define FA_AUDIT_WRITE	   ((BYTE)0x80)
#endif



/* File Open Modes */
/* Inheritance flag */

#define IF_INHERITED		    (BYTE)0x00
#define IF_PRIVATE		    (BYTE)0x80

    /* Sharing modes */
#define SM_COMPATIBILITY	    (BYTE)0x00
#define SM_DENY_READ_WRITE	    (BYTE)0x10
#define SM_DENY_WRITE		    (BYTE)0x20
#define SM_DENY_READ		    (BYTE)0x30
#define SM_DENY_NONE		    (BYTE)0x40

    /* Access modes */
#define AM_READ 		    (BYTE)0x00
#define AM_WRITE		    (BYTE)0x01
#define AM_READ_WRITE		    (BYTE)0x02



extern BYTE DOSError;	/* Used to return DOS Errors from INT 21 calls */

   #ifdef PROTOTYPE
      extern int  cdecl  ChangeDirectory(char *);
      extern int  cdecl  DOSClose(int);
      extern int  cdecl  DOSOpen(char *, BYTE);
      extern int  Dummy(void);
      extern int  cdecl  GetCurrentDirectory(BYTE, char *);
      extern WORD cdecl  GetDosVersion(void);
      extern void cdecl  MemFetch(WORD, WORD, BYTE *, WORD);
      extern void cdecl  MemStore(WORD, WORD, BYTE *, WORD);
   #else
      extern int  cdecl  ChangeDirectory();
      extern int  cdecl  DOSClose();
      extern int  cdecl  DOSOpen();
      extern int  Dummy();
      extern int  cdecl  GetCurrentDirectory();
      extern WORD cdecl  GetDosVersion();
      extern void cdecl  MemFetch();
      extern void cdecl  MemStore();
   #endif

#endif
