/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>
#include <nat.h>

/**** AFPSetFileInformation -- Returns information for the AFP side of a
        directory.
* Input:    int serverNum;              Number of the server where the
*                                           transaction is to be performed.
*           BYTE volumeNum;             Volume number where the directory
*                                           entry is.
*           long AFPBaseID;             AFP base ID.
*           WORD requestBitMap;         Information requested Bit Map.
*           char *AFPPathString;        AFP style directory path, relative to
*                                           AFPbaseID.
*           WORD strucLength;           Length of the AFPSETINFO structure.
*           AFPSETINFO AFPSetInfo;      Structure containing values
*                                           for setting files information.
*
* Output:   Returns a 0 is successful or a bindery error code if unsuccessful.
*
****/

int AFPSetFileInformation(serverNum, volumeNum, AFPBaseID, requestBitMap,
                          AFPPathString, structSize, AFPSetInfo)
WORD serverNum;
BYTE volumeNum;
long AFPBaseID;
WORD requestBitMap;
char *AFPPathString;
WORD structSize;          /* This parameter is not used, but is kept in
                             the code for backward compatibility          */
AFPSETINFO *AFPSetInfo;
{
BYTE sendPacket[313];
int  ccode;
int  packetLength;
WORD currentServerNum;

   currentServerNum = GetPreferredConnectionID();
   SetPreferredConnectionID( serverNum );

   packetLength = 3 + (54 + (int)AFPPathString[0]); /* header+(parameters) */
   *((int *)sendPacket) = IntSwap(packetLength - 2);
   sendPacket[2] = 9;                               /* request number      */
   sendPacket[3] = volumeNum;
   *((long *)(sendPacket + 4)) = AFPBaseID;
   *((WORD *)(sendPacket + 8)) = requestBitMap;
   *((WORD *)(sendPacket + 10)) = AFPSetInfo->attributes;
   *((WORD *)(sendPacket + 12)) = IntSwap(AFPSetInfo->creationDate);
   *((WORD *)(sendPacket + 14)) = IntSwap(AFPSetInfo->accessDate);
   *((WORD *)(sendPacket + 16)) = IntSwap(AFPSetInfo->modifyDate);
   *((WORD *)(sendPacket + 18)) = IntSwap(AFPSetInfo->modifyTime);
   *((WORD *)(sendPacket + 20)) = IntSwap(AFPSetInfo->backupDate);
   *((WORD *)(sendPacket + 22)) = IntSwap(AFPSetInfo->backupTime);
   memmove(sendPacket + 24, AFPSetInfo->finderInfo, (WORD)32);
   memmove(sendPacket + 56, AFPPathString, (WORD)(AFPPathString[0] + 1));

   ccode = _FileServiceRequest( (BYTE)35, sendPacket, packetLength, 
                                (BYTE *)NULL, (WORD)NULL );

   SetPreferredConnectionID( currentServerNum );
   return( ccode );
}
