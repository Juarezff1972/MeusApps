/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef _NWFILE_H
#define _NWFILE_H
/*______________________________________________________________

       Definitions for the Netware API File Services logic
  ______________________________________________________________*/

#ifndef SetFileInformation
   #define SetFileInformation _SetFileInformation
   #define ScanFileInformation _ScanFileInformation
#endif

#ifndef _PROLOG_H
   #include <prolog.h>
#endif

/* Drive flags */

#ifndef DRIVE_LOCAL
   #define DRIVE_LOCAL         ((BYTE)'\x80')
   #define DRIVE_PERMANENT     ((BYTE)'\x01')
   #define DRIVE_TEMPORARY     ((BYTE)'\x02')
   #define DRIVE_NETWORK       ((BYTE)'\x03')
#endif


/* File Attributes */

#ifndef FA_READ_ONLY
   #define FA_NORMAL           ((BYTE)0x00)
   #define FA_READ_ONLY        ((BYTE)0x01)
   #ifndef FA_HIDDEN
      #define FA_HIDDEN           ((BYTE)0x02)
   #endif
   #ifndef FA_SYSTEM
      #define FA_SYSTEM           ((BYTE)0x04)
   #endif
   #define FA_EXECUTE_ONLY     ((BYTE)0x08)
   #define FA_DIRECTORY        ((BYTE)0x10)
   #define FA_NEEDS_ARCHIVED   ((BYTE)0x20)
   #define FA_SHAREABLE        ((BYTE)0x80)

/* Extended file attributes */

   #define FA_TRANSACTIONAL    ((BYTE)0x10)
   #define FA_INDEXED          ((BYTE)0x20)
   #define FA_READ_AUDIT       ((BYTE)0x40)
   #define FA_WRITE_AUDIT      ((BYTE)0x80)
#endif

/* Trustee Access Rights in a network directory */

#ifndef TA_NONE
   #define TA_NONE             ((BYTE)0x00)
   #define TA_READ             ((BYTE)0x01)
   #define TA_WRITE            ((BYTE)0x02)
   #define TA_OPEN             ((BYTE)0x04)
   #define TA_CREATE           ((BYTE)0x08)
   #define TA_DELETE           ((BYTE)0x10)
   #define TA_OWNERSHIP        ((BYTE)0x20)
   #define TA_SEARCH           ((BYTE)0x40)
   #define TA_MODIFY           ((BYTE)0x80)
   #define TA_ALL              ((BYTE)0xFF)
#endif

/* Drive Constants */

#ifndef NO_BASE_DRIVE
   #define NO_BASE_DRIVE   ((BYTE)'\xff')
   #define TEMPORARY_DRIVE ((BYTE)0)
   #define PERMANENT_DRIVE ((BYTE)1)
   #define TEMP_DRIVE      ((BYTE)26)
   #define TEMP_DRIVE1     ((BYTE)27)
   #define TEMP_DRIVE2     ((BYTE)28)
   #define TEMP_DRIVE3     ((BYTE)29)
#endif

#ifdef PROTOTYPE

extern int    EraseFiles(BYTE, char *, BYTE);
extern int    cdecl  FileServerFileCopy(int, int, long, long, long, long *);
extern int    cdecl  GetExtendedFileAttributes(char *, BYTE *);
extern int    PurgeAllErasedFiles(void);
extern int    PurgeErasedFiles(void);
extern int    RestoreErasedFile(BYTE, char *, char *, char *);
extern int    ScanFileInformation(BYTE, char *, BYTE, int *, char *, BYTE *, BYTE *, long *, char *, char *, char *, char *, long *);
extern int    cdecl  SetExtendedFileAttributes(char *, BYTE);
extern int    SetFileInformation(BYTE, char *, BYTE, BYTE, BYTE, char *, char *, char *, char *, long);

#else

extern int    EraseFiles();
extern int    cdecl  FileServerFileCopy();
extern int    cdecl  GetExtendedFileAttributes();
extern int    FileServerFileCopy();
extern int    GetExtendedFileAttributes();
extern int    PurgeAllErasedFiles();
extern int    PurgeErasedFiles();
extern int    RestoreErasedFile();
extern int    ScanFileInformation();
extern int    SetExtendedFileAttributes();
extern int    cdecl  SetExtendedFileAttributes();
extern int    SetFileInformation();

#endif

#endif
