/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */

#ifndef _NTT_H
#define _NTT_H

#ifndef _PROLOG_H
   #include <c:\tc\rede\novell\inc\prolog.h>
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef WORD
#define WORD unsigned int
#endif

#ifndef DWORD
#define DWORD unsigned long
#endif

#ifndef LONG
#define LONG unsigned long
#endif

#ifndef NULL
#if (!defined(M_I86CM) && !defined(M_I86LM))
#define  NULL    0
#else
#define  NULL    0L
#endif
#endif

#ifndef  VOID
#define  VOID   void
#endif

#ifdef __WATCOMC__
#include <stddef.h>
#include <stdarg.h>
#endif


/* NetWare specific size constants */
#define  NWMAX_NCP_PATH          255
#define  NWMAX_NS_NAME           12      /* max length of a name space name */
#define  NWMAX_VOL_NAME          48
#define  NCP_MAX_BUFFER          512
#define  MAX_DIR_ENTRY           128
#define  NWMAX_DIR_RESTRICTIONS  33

#define NO_MORE_RESTRICTIONS 0xFE


/* MODIFY BITS TO BE USED WITH SETDIRECTORYENTRY */

#define  M_MODIFY_NAME             0x0001L
#define  M_FILE_ATTRIBUTES         0x0002L
#define  M_CREATE_DATE             0x0004L
#define  M_CREATE_TIME             0x0008L
#define  M_OWNER_ID                0x0010L
#define  M_LAST_ARCHIVED_DATE      0x0020L
#define  M_LAST_ARCHIVED_TIME      0x0040L
#define  M_LAST_ARCHIVED_ID        0x0080L
#define  M_LAST_UPDATED_DATE       0x0100L
#define  M_LAST_UPDATED_TIME       0x0200L
#define  M_LAST_UPDATED_ID         0x0400L
#define  M_LAST_ACCESSED_DATE      0x0800L
#define  M_INHERITED_RIGHTS_MASK   0x1000L
#define  M_MAXIMUM_SPACE           0x2000L

#define   M_MAC_MODIFY_NAME         1
#define   M_MAC_FINDER_INFO         2
#define   M_MAC_PRODOS_INFO         4

/* define the NameSpace values   */

#define DOS_NAME_SPACE         0
#define MAC_NAME_SPACE         1

/* 386 TRUSTEE RIGHTS */

#define TR_NONE                 0x0000
#define TR_READ                 0x0001
#define TR_WRITE                0x0002
#define TR_CREATE               0x0008
#define TR_ERASE                0x0010
#define TR_ACCESS               0x0020
#define TR_FILE                 0x0040
#define TR_MODIFY               0x0080
#define TR_SUPERVISOR           0x0100
#define TR_NORMAL               0x00FF
#define TR_ALL                  0x01FF

/* ADDITIONAL EXTENDED FILE ATTRIBUTES */

#ifndef TF_READ_ONLY

#define TF_NORMAL                  0x0000
#define TF_READ_ONLY               0x0001L
#define TF_HIDDEN                  0x0002L
#define TF_SYSTEM                  0x0004L
#define TF_EXECUTE_ONLY            0x0008L

#define TF_DIRECTORY               0x0010L
#define TF_NEEDS_ARCHIVED          0x0020L
#define TF_EXECUTE_CONFIRM         0X0040L
#define TF_SHAREABLE               0x0080L

#define TF_LOW_SEARCH              0x0100L
#define TF_MID_SEARCH              0x0200L
#define TF_HI_SEARCH               0x0400L
#define TF_PRIVATE                 0x0800L

#define TF_TRANSACTIONAL           0x1000L
#define TF_INDEXED                 0x2000L
#define TF_READ_AUDIT              0x4000L
#define TF_WRITE_AUDIT             0x8000L

#define TF_PURGE                   0x10000L
#define TF_RENAME_INHIBIT          0x20000L
#define TF_DELETE_INHIBIT          0x40000L
#define TF_COPY_INHIBIT            0x80000L

#endif

/* DIRECTORY ATTRIBUTES */

#define TD_HIDDEN                 TF_HIDDEN
#define TD_SYSTEM                 TF_SYSTEM
#define TD_PURGE                  TF_PURGE
#define TD_PRIVATE                TF_PRIVATE
#define TD_VISIBLE                TF_PRIVATE
#define TD_RENAME_INHIBIT         TF_RENAME_INHIBIT
#define TD_DELETE_INHIBIT         TF_DELETE_INHIBIT

#define SALVAGE_UNAVAILABLE       251
#define NO_MORE_FILES             255

/* ERROR CODES */
#define INVALID_NAME_SPACE_NUMBER   0x9001
#define INVALID_DATA_STREAM_NUMBER  0x9002

/* NIT BUFFER IDs */
#define RI_GETNAMESPACEANDDATASTREAMINF     0x1111



/* STRUCTURE DEFINITIONS --------------------------------------------------- */

typedef struct
{
   LONG   entryID;
   LONG   attributes;
   BYTE   nameSpaceType;
   BYTE   nameLength;
   BYTE   name[12];
   LONG   lastArchiveDateAndTime;
   LONG   lastArchiverID;
   LONG   modifyDateAndTime;
   LONG   lastModifierID;
   LONG   dataForkSize;
   WORD   lastAccessedDate;
   LONG   deletedFileTime;
   LONG   deletedDateAndTime;
   LONG   deletorID;
}  NWSALV_ENTRY;


typedef struct
{
   LONG   sequence;
   LONG   reserved1;
   LONG   attributes;
   BYTE   reserved2[2];
   BYTE   NSType;
   BYTE   nameLength;
   BYTE   name[12];
   WORD   creationDate;
   WORD   creationTime;
   LONG   ownerID;
   WORD   lastArchivedDate;
   WORD   lastArchivedTime;
   LONG   lastArchiverID;
   WORD   modifyDate;
   WORD   modifyTime;
   LONG   lastModifierID;
   LONG   dataForkSize;
   BYTE   reserved3[44];
   WORD   inheritedRightsMask;
   WORD   lastAccessDate;
   BYTE   reserved4[28];
}  NWFILE_ENTRY;



typedef struct
{
   LONG   sequence;
   LONG   reserved1;
   LONG   attributes;
   BYTE   reserved2[2];
   BYTE   NSType;
   BYTE   nameLength;
   BYTE   name[12];
   WORD   creationDate;
   WORD   creationTime;
   LONG   ownerID;
   WORD   lastArchivedDate;
   WORD   lastArchivedTime;
   LONG   lastArchiverID;
   WORD   modifyDate;
   WORD   modifyTime;
   BYTE   reserved3[56];
   WORD   inheritedRightsMask;
   BYTE   reserved4[26];
}  NWDIR_ENTRY;



typedef struct
{
   LONG   sequence;
   LONG   reserved1;
   LONG   attributes;
   BYTE   reserved2[2];
   BYTE   NSType;
   BYTE   nameLength;
   BYTE   name[12];
   WORD   creationDate;
   WORD   creationTime;
   LONG   ownerID;
   WORD   lastArchivedDate;
   WORD   lastArchivedTime;
   LONG   lastArchiverID;
   WORD   modifyDate;
   WORD   modifyTime;
   LONG   lastModifierID;
   LONG   dataForkSize;
   BYTE   reserved3[44];
   WORD   inheritedRightsMask;
   WORD   lastAccessDate;
   BYTE   reserved4[28];
   LONG   forkSize[10];
}  NWPHYS_ENTRY;






typedef struct
{
   LONG   sequence;
   LONG   reserved1;
   LONG   attributes;
   BYTE   reserved2[2];
   BYTE   NSType;
   BYTE   nameLength;
   BYTE   name[32];
   LONG   resourceFork;
   LONG   resourceForkSize;
   BYTE   finderInfo[32];
   BYTE   proDosInfo[6];
   BYTE   reserved3[38];
}  NWMAC_ENTRY;


typedef struct
{
   LONG   totalBlocks;
   LONG   freeBlocks;
   LONG   totalDirEntries;
   LONG   availDirEntries;
   LONG   flags;            /* currently unused */
   BYTE   sectorsPerBlock;
   char   volumeName[NWMAX_VOL_NAME];
}   NWDIR_INFO;

typedef struct
{
   LONG   totalBlocks;
   LONG   freeBlocks;
   LONG   purgableBlocks;
   LONG   notYetPurgableBlocks;
   LONG   totalDirEntries;
   LONG   availDirEntries;
   LONG   maxDirEntriesUsed;
   BYTE   unused[4];
   WORD   blockSize;
   BYTE   isHashing;
   BYTE   isRemovable;
   char   volName[NWMAX_VOL_NAME];
}   NWVOL_USAGE;

typedef BYTE  NWBUFFER[NCP_MAX_BUFFER];
typedef BYTE  DIRENTRYBUFFER[MAX_DIR_ENTRY];


#define InlineIntSwap(ptr) (((int)*(unsigned char *)(ptr) << 8) \
      | (int)*((unsigned char *)(ptr) + 1))


/*-------------------------  FUNCTION PROTOTYPES -------------------------*/

#ifdef PROTOTYPE

extern WORD ConvertDirectoryHandle( WORD connectionID, BYTE oldHandle,
                                    char *oldDirPath, BYTE *newHandle,
                                    char *newDirPath );

extern int _NWMapLenPrecPath( char *path, char *mappedPath,
                              int *totalPathLength );

extern int _NWMapWildPath( char *path, char *mappedPath );

extern WORD _NCPConnRequest( WORD serverConnID, BYTE NCPFunctionCode,
                             WORD requestLength, VOID *requestBody,
                             WORD replyLength, VOID *returnBody );

extern int cdecl _NCPRequest( BYTE NCPFunctionCode, WORD requestLength, 
                              VOID *requestBody, WORD replyLength, 
                              VOID *returnBody );

extern long _NW286DateAdjust( int date, int time );

extern WORD FillNameSpaceBuffer( WORD serverConnID,BYTE volumeNum,
                                 NWBUFFER NWBuffer );

extern WORD GetDataStreamInfo( NWBUFFER NWBuffer,BYTE dataStreamType,
                               char *dataStreanName,int *volSupports );

extern WORD GetNameSpaceInfo( NWBUFFER NWBuffer,BYTE nameSpaceType,
                              char *nameSpaceName,int *OSReady,
                              int *volSupports );

extern WORD GetNumNameSpaceAndDataStreams( NWBUFFER NWBuffer,
                                           BYTE *numNameSpaces,
                                           BYTE *numDataStreams );

extern WORD GetNameSpaceEntry( WORD serverConnID, BYTE volumeNum,
                               LONG sequence, BYTE nameSpaceType,
                               void *entryInfo );

extern WORD OpenDataStream( WORD serverConnID, BYTE dataStreamType,
                            BYTE dirHandle, BYTE fileAttributes,
                            BYTE openRights, char *filePath,
                            BYTE *netwareHandle, int *fileHandle );

extern WORD GetDirEntry( WORD serverConnID, BYTE dirHandle,
                         NWDIR_ENTRY *dirEntry );

extern WORD ScanDirEntry ( WORD serverConnID, BYTE dirHandle,
                           char *searchPath, BYTE searchAttributes,
                           LONG *sequence, NWDIR_ENTRY *dirEntry );

extern WORD ScanFileEntry ( WORD serverConnID, BYTE dirHandle,
                            char *searchPath, BYTE searchAttributes,
                            LONG *sequence, NWFILE_ENTRY *fileEntry );

extern WORD ScanFilePhysical( WORD serverConnID, BYTE dirHandle,
                              char *searchPath, BYTE searchAttributes,
                              LONG *sequence, NWPHYS_ENTRY *dirEntry );

extern WORD GetNameSpaceEntry( WORD serverConnID, BYTE volumeNum,
                               LONG sequence, BYTE NSType,
                               void *entryInfo );

extern WORD GetVolUsage( WORD serverConnID, WORD volNumber,
                         NWVOL_USAGE *volUsage );


extern WORD  ScanDirRestrictions( WORD  serverConnID, BYTE  dirHandle,
                                  BYTE *buffer, 
                                  BYTE  numberOfStructuresToReturn );

extern WORD ScanVolForRestrictions( WORD serverConnID, BYTE volNumber,
                                         LONG sequence, NWBUFFER NWBuffer );

extern WORD IsV3Supported( WORD serverConnID );

extern BYTE MapV3TrusteeRightsToV2( LONG trusteeRightsV3 );

extern LONG MapV2TrusteeRightsToV3( BYTE trusteeRightsV2 );

extern WORD MoveEntry( WORD serverConnID, BYTE searchAttribute,
                       BYTE sourceDirHandle, char *sourcePath,
                       BYTE destDirHandle, char *destPath );


extern WORD ScanEntryForTrustees( WORD     serverConnID,
                                  BYTE     directoryHandle,
                                  char     *directoryPath,
                                  BYTE     *trusteeBuffer,
                                  BYTE     trusteeSetNumber,
                                  BYTE     trusteeGroupsToReturn );

extern WORD SetTrustee( WORD serverConnID, BYTE dirHandle, char *path,
                        LONG objectID, LONG trusteeRights );

extern WORD ClearVolRestrictions( WORD serverConnID, BYTE volNumber,
                                  LONG objectID );

extern WORD DeleteTrustee( WORD serverConnID, BYTE dirHandle, char* path,
                           LONG objectID );

extern WORD GetDirInfo( WORD serverConnID, BYTE dirHandle, NWDIR_INFO*
                             dirInfo );

extern WORD GetObjectDiskRestrictions ( WORD serverConnID, BYTE volNumber,
                                        LONG objectID, LONG* restriction,
                                        LONG* inUse );

extern WORD GetEffectiveRights ( WORD serverConnID, BYTE dirHandle,
                                 char* path, WORD* rights );

extern WORD PurgeSalvagableFile( WORD serverConnID, BYTE dirHandle,
                                 LONG entryID, char* fileName );

extern WORD RecoverSalvagableFile( WORD serverConnID, BYTE dirhandle,
                                   LONG entryID, char* filename,
                                   char* newname );

extern WORD ScanSalvagableFiles( WORD serverConnID, BYTE dirHandle,
                                 LONG* entryID, NWSALV_ENTRY* salvEntry );

extern WORD SetDirRestriction( WORD serverConnID, BYTE dirHandle,
                               LONG restriction );

extern WORD SetEntry( WORD serverConnID, BYTE dirHandle, BYTE searchAttribute,
                      LONG entryID, LONG changeBits, void *entry );

extern WORD SetVolRestriction( WORD serverConnID, BYTE volNumber,
                               LONG objectID, LONG restriction );

#else

extern WORD ConvertDirectoryHandle();
extern int _NWMapLenPrecPath();
extern int _NWMapWildPath();
extern WORD _NCPConnRequest();
extern int cdecl _NCPRequest();
extern long _NW286DateAdjust();
extern WORD FillNameSpaceBuffer();
extern WORD GetDataStreamInfo();
extern WORD GetNameSpaceInfo();
extern WORD GetNumNameSpaceAndDataStreams();
extern WORD GetNameSpaceEntry();
extern WORD OpenDataStream();
extern WORD GetDirEntry();
extern WORD ScanDirEntry ();
extern WORD ScanFileEntry ();
extern WORD ScanFilePhysical();
extern WORD GetNameSpaceEntry();
extern WORD GetVolUsage();
extern WORD  ScanDirRestrictions();
extern WORD ScanVolForRestrictions();
extern WORD IsV3Supported();
extern BYTE MapV3TrusteeRightsToV2();
extern LONG MapV2TrusteeRightsToV3();
extern WORD MoveEntry();
extern WORD ScanEntryForTrustees();
extern WORD SetTrustee();
extern WORD ClearVolRestrictions();
extern WORD DeleteTrustee();
extern WORD GetDirInfo();
extern WORD GetObjectDiskRestrictions ();
extern WORD GetEffectiveRights ();
extern WORD PurgeSalvagableFile();
extern WORD RecoverSalvagableFile();
extern WORD ScanSalvagableFiles();
extern WORD SetDirRestriction();
extern WORD SetEntry();
extern WORD SetVolRestriction();

#endif

#endif
