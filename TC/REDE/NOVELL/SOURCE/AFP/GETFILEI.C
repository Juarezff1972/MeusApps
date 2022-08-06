/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>
#include <nat.h>


#define MIN(val1,val2)  ((val1)<(val2)?(val1):(val2))

/*__________________________ AFPGetFileInformation _________________________

   Description:  Returns information for the AFP side of a directory
   Returns       0 is successful or a bindery error code if unsuccessful.
___________________________________________________________________________*/

#define p_EntryID               (long *)(receivePacket)
#define p_ParentID              (long *)(receivePacket+4)
#define p_Attributes            (WORD *)(receivePacket+8)
#define p_DataForkLength        (long *)(receivePacket+10)
#define p_ResourceForkLength    (long *)(receivePacket+14)
#define p_NumOffspring          (WORD *)(receivePacket+18)
#define p_CreationDate          (WORD *)(receivePacket+20)
#define p_AccessDate            (WORD *)(receivePacket+22)
#define p_ModifyDate            (WORD *)(receivePacket+24)
#define p_ModifyTime            (WORD *)(receivePacket+26)
#define p_BackupDate            (WORD *)(receivePacket+28)
#define p_BackupTime            (WORD *)(receivePacket+30)
#define p_FinderInfo            (BYTE *)(receivePacket+32)
#define p_LongName              (char *)(receivePacket+64)
#define p_OwnerID               (long *)(receivePacket+96)
#define p_ShortName             (char *)(receivePacket+100)
#define p_AccessPrivileges      (WORD *)(receivePacket+112)


int AFPGetFileInformation(connectionID, volumeNum, AFPEntryID, requestBitMap,
                          AFPPathString, structSize, AFPFileInfo)
WORD connectionID;         /* Number of the server                           */
BYTE volumeNum;            /* Volume number for directory entry              */
long AFPEntryID;           /* AFP base ID.                                   */
WORD requestBitMap;        /* Information requested Bit Map.                 */
char *AFPPathString;       /* AFP style directory path, relative to AFPbaseID*/
WORD structSize;           /* Size of the AFPFILEINFO structure.             */
AFPFILEINFO *AFPFileInfo;  /* Structure for returning AFP file info          */
{
BYTE sendPacket[269];
BYTE receivePacket[sizeof(AFPFILEINFO)];
int  ccode;
int  packetLength;
WORD oldConnectionID;
BYTE tempPacket[sizeof(AFPFILEINFO)];

    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID(connectionID);

    packetLength = 11 + (int)AFPPathString[0];
    *((int *)sendPacket) = IntSwap(packetLength - 2);
    sendPacket[2] = 5;                            /* request number        */
    sendPacket[3] = (BYTE)volumeNum;
    *((long *)(sendPacket + 4)) = AFPEntryID;
    *((WORD *)(sendPacket + 8)) = requestBitMap;
    memmove(sendPacket + 10, AFPPathString, (WORD)(AFPPathString[0] + 1));

    if ((ccode = _FileServiceRequest((BYTE)0x23, sendPacket, packetLength,
       receivePacket, sizeof(AFPFILEINFO))) == 0) {


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
        memmove( tempPacket, receivePacket,
                 (int)( (long)p_OwnerID - (long)receivePacket ) );
#else
        memmove( tempPacket, receivePacket,
                 (int)( (long)(long far *)p_OwnerID - (long)(long far *)receivePacket ) );
#endif
        ((AFPFILEINFO *)tempPacket)->longName[32] = '\0';
        ((AFPFILEINFO *)tempPacket)->ownerID = *p_OwnerID;

                /* force null terminate short name */
        memmove( ((AFPFILEINFO *)tempPacket)->shortName, p_ShortName, 12 );
        ((AFPFILEINFO *)tempPacket)->shortName[12] = '\0';
        ((AFPFILEINFO *)tempPacket)->accessPrivileges = *p_AccessPrivileges;

        memmove( (char *)AFPFileInfo, tempPacket,
                 MIN( sizeof(AFPFILEINFO), structSize ) );
    }

    SetPreferredConnectionID(oldConnectionID);
    return( ccode );
}
