/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef _NWDIR_H
#define _NWDIR_H
/*______________________________________________________________

        Definitions for the netware api DIRECTORY logic
  ______________________________________________________________*/

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
   #define NO_BASE_DRIVE   ((BYTE)0xff)
   #define TEMPORARY_DRIVE ((BYTE)0)
   #define PERMANENT_DRIVE ((BYTE)1)
   #define TEMP_DRIVE      ((BYTE)26)
   #define TEMP_DRIVE1     ((BYTE)27)
   #define TEMP_DRIVE2     ((BYTE)28)
   #define TEMP_DRIVE3     ((BYTE)29)
#endif


/* search drive flag values */

#ifndef DRIVE_ADD
   #define DRIVE_ADD                           1
   #define DRIVE_INSERT                        2
   #define DRIVE_DELETE                        3
   #define MAX_NETWARE_SEARCH_DRIVES          16
#endif


typedef struct
 {
    long    systemElapsedTime;
    BYTE    volumeNumber;
    BYTE    logicalDriveNumber;
    WORD    sectorsPerBlock;
    WORD    startingBlock;
    WORD    totalBlocks;
    WORD    availableBlocks;
    WORD    totalDirectorySlots;
    WORD    availableDirectorySlots;
    WORD    maxDirectorySlotsUsed;
    BYTE    isHashing;
    BYTE    isCaching;
    BYTE    isRemovable;
    BYTE    isMounted;
    char    volumeName[16];
 }VOLUME_STATS;


#ifdef PROTOTYPE

extern int    AddTrusteeToDirectory(BYTE, char *, long, BYTE);
extern void   AllignDriveVectorToPath(char *);
extern int    AllocPermanentDirectoryHandle(BYTE, char *, char, BYTE *, BYTE *);
extern int    AllocTemporaryDirectoryHandle(BYTE, char *, char, BYTE *, BYTE *);
extern int    CreateDirectory(BYTE, char *, BYTE);
extern int    DeallocateDirectoryHandle(BYTE);
extern int    DeleteDirectory(BYTE, char *);
extern int    DeleteTrusteeFromDirectory(BYTE, char *, long);
extern int   cdecl GetCurrentDirectory(BYTE, char *);
extern BYTE  cdecl GetDirectoryHandle(BYTE);
extern int    GetDirectoryPath(BYTE, char *);
extern BYTE cdecl  GetDriveInformation(BYTE, WORD *, BYTE *);
extern int    GetEffectiveDirectoryRights(BYTE, char *, BYTE *);
extern int    GetPathFromDirectoryEntry(WORD, BYTE, WORD, char *);
extern void cdecl  GetSearchDriveVector(BYTE *);
extern int    GetVolumeInformation(WORD, BYTE, int, VOLUME_STATS *);
extern int    GetVolumeInfoWithHandle(BYTE, char *, WORD *, WORD *, WORD *, WORD *, WORD *, WORD *);
extern int  cdecl  GetVolumeInfoWithNumber(BYTE, char *, WORD *, WORD *, WORD *, WORD *, WORD *, WORD *);
extern int    GetVolumeName(int, char *);
extern int    GetVolumeNumber(char *, int *);
extern int    IsSearchDrive(char);

extern int    MapDriveUsingString( char *, char *, char * );

extern int    MapDrive(WORD, BYTE, char *, BYTE, int, char *);


extern int    ModifyMaximumRightsMask(BYTE, char *, BYTE, BYTE);
extern int    RelativeToFullPath(char *, char *);
extern int    RenameDirectory(BYTE, char *, char *);
extern int    RestoreDirectoryHandle(char *, BYTE *, BYTE *);
extern int    SaveDirectoryHandle(BYTE, char *);
extern int    ScanDirectoryForTrustees(BYTE, char *, int *, char *, BYTE *, long *, long *, BYTE *);
extern int    ScanDirectoryInformation(BYTE, char *, int *, char *, BYTE *, long *, BYTE *);
extern int    SetDirectoryHandle(BYTE, char *, BYTE);
extern int    SetDirectoryInformation(BYTE, char *, BYTE *, long, BYTE);
extern int    SetDrivePath(BYTE, BYTE, char *, BYTE);
extern void cdecl  SetSearchDriveVector(BYTE *);
extern char  *StripFileServerFromPath( char *, char *);

#else

extern int    AddTrusteeToDirectory();
extern int    AllocPermanentDirectoryHandle();
extern int    AllocTemporaryDirectoryHandle();
extern void   AllignDriveVectorToPath();
extern int    CreateDirectory();
extern int    DeallocateDirectoryHandle();
extern int    DeleteDirectory();
extern int    DeleteTrusteeFromDirectory();
extern int  cdecl GetCurrentDirectory();
extern BYTE cdecl GetDirectoryHandle();
extern int    GetDirectoryPath();
extern BYTE cdecl  GetDriveInformation();
extern int    GetEffectiveDirectoryRights();
extern int    GetPathFromDirectoryEntry();
extern void cdecl  GetSearchDriveVector();
extern int    GetVolumeInformation();
extern int    GetVolumeInfoWithHandle();
extern int  cdecl  GetVolumeInfoWithNumber();
extern int    GetVolumeName();
extern int    GetVolumeNumber();
extern int    IsSearchDrive();

extern int    MapDrive();
extern int    MapDriveUsingString();

extern int    ModifyMaximumRightsMask();
extern int    RelativeToFullPath();
extern int    RenameDirectory();
extern int    RestoreDirectoryHandle();
extern int    SaveDirectoryHandle();
extern int    ScanDirectoryForTrustees();
extern int    ScanDirectoryInformation();
extern int    SetDirectoryHandle();
extern int    SetDirectoryInformation();
extern int    SetDrivePath();
extern void cdecl  SetSearchDriveVector();
extern char  *StripFileServerFromPath();
#endif

#endif
