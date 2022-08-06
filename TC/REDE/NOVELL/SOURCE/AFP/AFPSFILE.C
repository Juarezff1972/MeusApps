/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>
#include <nat.h>

#define MIN(val1,val2)  ((val1)<(val2)?(val1):(val2))

/**** AFPGetFileInformation -- Returns information for the AFP side of a
*       directory.
* Input:    int connectionID;           Number of the server where the
*                                       transaction is to be performed.
*           BYTE volumeNum;             Volume number where the directory
*                                       entry is.
*           long AFPEntryID;            AFP base ID.
*           WORD searchBitMap;
*           WORD requestBitMap;         Information requested Bit Map.
*           char *AFPPathString;        AFP style directory path, relative to
*                                       AFPbaseID.
*           WORD strucSize;             Size of the AFPFILEINFO structure.
*           AFPFILEINFO AFPFileInfo;    Structure for returning AFP file
*                                       information.
*
* Output:   Returns a 0 is successful or a bindery error code if unsuccessful.
*
****/
/*  first 2 bytes are how many instances are returned. */
#define p_EntryID               (long *)(receivePacket+2)
#define p_ParentID              (long *)(receivePacket+6)
#define p_Attributes            (WORD *)(receivePacket+10)
#define p_DataForkLength        (long *)(receivePacket+12)
#define p_ResourceForkLength    (long *)(receivePacket+16)
#define p_NumOffspring          (WORD *)(receivePacket+20)
#define p_CreationDate          (WORD *)(receivePacket+22)
#define p_AccessDate            (WORD *)(receivePacket+24)
#define p_ModifyDate            (WORD *)(receivePacket+26)
#define p_ModifyTime            (WORD *)(receivePacket+28)
#define p_BackupDate            (WORD *)(receivePacket+30)
#define p_BackupTime            (WORD *)(receivePacket+32)
#define p_FinderInfo            (BYTE *)(receivePacket+34)
#define p_LongName              (char *)(receivePacket+66)
#define p_OwnerID               (long *)(receivePacket+98)
#define p_ShortName             (char *)(receivePacket+102)
#define p_AccessPrivileges      (WORD *)(receivePacket+116)


int AFPScanFileInformation( connectionID, volumeNum, AFPEntryID, AFPLastSeenID,
                            searchBitMap, requestBitMap, AFPPathString, 
                            structSize, AFPFileInfo )
WORD            connectionID;       /* indicates file server to search      */
BYTE            volumeNum;          /* volume to search                     */
long            AFPEntryID;         /* directory to search                  */
long           *AFPLastSeenID;      /* last item seen (initialize to -1)    */
WORD            searchBitMap;       /* indicates type of files to search for*/
WORD            requestBitMap;      /* indicates which items to return      */
char           *AFPPathString;      /* directory to search                  */
WORD            structSize;         /* maximum number of bytes to return    */
AFPFILEINFO    *AFPFileInfo;        /* information on found file            */
{

BYTE    sendPacket[320];
BYTE    receivePacket[sizeof(AFPFILEINFO)];
int     ccode;
int     packetLength;
WORD    oldConnectionID;
BYTE    tempPacket[sizeof(AFPFILEINFO)];

    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID(connectionID);

    packetLength = 3 + (16 + (int)AFPPathString[0]);/* header + (parameters) */
    *((int *)sendPacket) = IntSwap(packetLength - 2);
    sendPacket[2] = 10;                            /* request number        */
    sendPacket[3] = (BYTE)volumeNum;
    *((long *)(sendPacket + 4)) = AFPEntryID;
    *((long *)(sendPacket + 8)) = *AFPLastSeenID;
    *((WORD *)(sendPacket + 12)) = 0x0100;               /* one at a time */
    *((WORD *)(sendPacket + 14)) = searchBitMap;
    *((WORD *)(sendPacket + 16)) = requestBitMap;
    memmove( sendPacket + 18, AFPPathString, AFPPathString[0] + 1 );

    if ((ccode = _FileServiceRequest((BYTE)35, sendPacket, packetLength,
       receivePacket, sizeof(AFPFILEINFO))) == 0) {
        *p_Attributes = IntSwap(*p_Attributes);
        *p_DataForkLength = LongSwap(*p_DataForkLength);
        *p_ResourceForkLength = LongSwap(*p_ResourceForkLength);
        *p_NumOffspring = IntSwap(*p_NumOffspring);
        *p_CreationDate = IntSwap(*p_CreationDate);
        *p_AccessDate = IntSwap(*p_AccessDate);
        *p_ModifyDate = IntSwap(*p_ModifyDate);
        *p_ModifyTime = IntSwap(*p_ModifyTime);
        *p_BackupDate = IntSwap(*p_BackupDate);
        *p_BackupTime = IntSwap(*p_BackupTime);
        *p_OwnerID = LongSwap(*p_OwnerID);
        *p_AccessPrivileges = IntSwap(*p_AccessPrivileges);

                /* force null terminate long name */
#ifdef LC310
        memmove( tempPacket, receivePacket + 2,
                 (int)( (long)p_OwnerID - (long)(receivePacket + 2) ) );
#else
        memmove( tempPacket, receivePacket + 2,
                 (int)( (long)(long far *)p_OwnerID - (long)(long far *)(receivePacket + 2) ) );
#endif
        ((AFPFILEINFO *)tempPacket)->longName[32] = '\0';
        ((AFPFILEINFO *)tempPacket)->ownerID = *p_OwnerID;

                /* force null terminate short name */
        memmove( ((AFPFILEINFO *)tempPacket)->shortName, p_ShortName, 12 );
        ((AFPFILEINFO *)tempPacket)->shortName[12] = '\0';
        ((AFPFILEINFO *)tempPacket)->accessPrivileges = *p_AccessPrivileges;

        memmove( (char *)AFPFileInfo, tempPacket,
                 MIN( sizeof(AFPFILEINFO), structSize ) );
        *AFPLastSeenID = AFPFileInfo->entryID;
    } else
        *AFPLastSeenID = -1;

    SetPreferredConnectionID(oldConnectionID);
    return( ccode );
}
