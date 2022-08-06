/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef _NAT_H
#define _NAT_H

/*____________________________________________________________________

    Definitions and structures for the Netware API AppleTalk logic
  ___________________________________________________________________*/

#ifndef _PROLOG_H
   #include <prolog.h>
#endif

#define AFP_SFI_SETATTRIBUTES   0x0001
#define AFP_SFI_CREATIONDATE    0X0002
#define AFP_SFI_ACCESSDATE      0X0004
#define AFP_SFI_MODIFYDATETIME  0X0008
#define AFP_SFI_BACKUPDATETIME  0X0010
#define AFP_SFI_FINDERINFO      0X0020
#define AFP_GFI_ATTRIBUTES      0X0001
#define AFP_GFI_PARENTDIRID     0X0002
#define AFP_GFI_CREATIONDATE    0X0004
#define AFP_GFI_ACCESSDATE      0X0008
#define AFP_GFI_MODIFYDATETIME  0X0010
#define AFP_GFI_BACKUPDATETIME  0X0020
#define AFP_GFI_FINDERINFO      0X0040
#define AFP_GFI_LONGNAME        0X0080
#define AFP_GFI_ENTRYID         0X0100
#define AFP_GFI_DATAFORKLEN     0X0200
#define AFP_GFI_RESRCFORKLEN    0X0400
#define AFP_GFI_NUMOFFSPRING    0X0800
#define AFP_GFI_OWNERID         0X1000
#define AFP_GFI_SHORTNAME       0X2000

#ifndef  AFPFINFO
   #define AFPFINFO 1

   typedef struct
   {
           long entryID;
           long parentID;
           WORD attributes;
           long dataForkLength;
           long resourceForkLength;
           WORD numOffspring;
           WORD creationDate;
           WORD accessDate;
           WORD modifyDate;
           WORD modifyTime;
           WORD backupDate;
           WORD backupTime;
           BYTE finderInfo[32];
           char longName[33];
           char pad1;
           long ownerID;
           char shortName[13];
           char pad2;
           WORD accessPrivileges;
   } AFPFILEINFO;
#endif

typedef struct
{
        WORD attributes;
        WORD creationDate;
        WORD accessDate;
        WORD modifyDate;
        WORD modifyTime;
        WORD backupDate;
        WORD backupTime;
        BYTE finderInfo[32];
} AFPSETINFO;


#ifdef PROTOTYPE

  extern  int   AFPAllocTemporaryDirHandle(WORD, BYTE, long, char *,
                                           BYTE *, BYTE *);
  extern  int   AFPCreateDirectory(WORD, BYTE, long, BYTE *, char *, long *);
  extern  int   AFPCreateFile(WORD, BYTE, long, BYTE, BYTE *, char *, long *);
  extern  int   AFPDelete(WORD, BYTE, long, char *);
  extern  int   AFPGetEntryIDFromName(WORD, BYTE, long, char *, long *);
  extern  int   AFPGetEntryIDFromNetWareHandle(BYTE *, BYTE *, long *, BYTE *);
  extern  int   AFPGetEntryIDFromPathName(WORD, BYTE, char *, long *);
  extern  int   AFPGetFileInformation(WORD, BYTE, long, WORD, char *,
                                      WORD, AFPFILEINFO *);
  extern  int   AFPDirectoryEntry(WORD, BYTE, char *);
  extern  int   AFPOpenFileFork(WORD, BYTE, long, BYTE, BYTE, char *,
                                long *, long *, BYTE *, int *);
  extern  int   AFPRename(WORD, BYTE, long, long, char *, char *);
  extern  int   AFPScanFileInformation(WORD, BYTE, long, long *, WORD,
                                       WORD, char *, WORD, AFPFILEINFO *);
  extern  int   AFPSetFileInformation(WORD, BYTE, long, WORD, char *,
                                      WORD, AFPSETINFO *);
  extern  int   AFPSupported(WORD);
  extern  int   cdecl _ConvertHandle(WORD, BYTE, BYTE *, long, int *);

#else

  extern  int     AFPAllocTemporaryDirHandle();
  extern  int     AFPCreateDirectory();
  extern  int     AFPCreateFile();
  extern  int     AFPDelete();
  extern  int     AFPGetBaseID();
  extern  int     AFPGetFileIDFromHandle();
  extern  int     AFPGetEntryIDFromPathName();
  extern  int     AFPGetFileInformation();
  extern  int     AFPDirectoryEntry();
  extern  int     AFPOpenFileFork();
  extern  int     AFPRename();
  extern  int     AFPScanFileInformation();
  extern  int     AFPSetFileInformation();
  extern  int     AFPSupported();
  extern  int     cdecl _ConvertHandle();
  extern  int     NetWareHandleServerFileCopy();

#endif

#endif
