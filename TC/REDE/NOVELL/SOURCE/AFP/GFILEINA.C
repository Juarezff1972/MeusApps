/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwwrkenv.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nat.h>

/*__________________________ AFPGetEntryIDFromName _________________________
  
   Description:  Returns a unique AFP baseID from a AFP baseID and 
                 modifying path
   Input:        int connectionID;       Number of the server where the
                                         transaction is to be performed.
                 BYTE volumeNumber;      volume to return information for
                 long AFPEntryID;        Unique AFP baseID.
                 char *AFPPathString;    path string modifying AFPEntryID
                 long *newAFPEntryID;    AFP entry id of path given.

   Output:       0 is successful or a NetWare error code if unsuccessful.

____________________________________________________________________________*/

int AFPGetEntryIDFromName( connectionID, volumeNum, AFPEntryID, AFPPathString,
                           newAFPEntryID )
WORD    connectionID;
BYTE    volumeNum;
long    AFPEntryID;
char   *AFPPathString;
long   *newAFPEntryID;
{
BYTE sendPacket[320];
BYTE receivePacket[6];
int  ccode;
int  packetLength;
int  pathLength;
WORD oldConnectionID;

    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID(connectionID);

    /*                                                   */
    /* build the packet for getting the base ID          */
    /*                                                   */
    pathLength = AFPPathString[0];
    packetLength = 9 + pathLength;
    *((int *)sendPacket) = IntSwap(packetLength - 2);
    sendPacket[2] = (BYTE)4;                 /* request number        */
    sendPacket[3] = volumeNum;
    *((long *)(sendPacket + 4)) = AFPEntryID;
    memmove( sendPacket + 8, AFPPathString, pathLength + 1 );

    /*                                                   */
    /* Send the request for the base ID                  */
    /*                                                   */
    if ((ccode = _FileServiceRequest((BYTE)35, sendPacket, packetLength,
       receivePacket, 4)) == 0)
        *newAFPEntryID = *((long *)receivePacket);

    SetPreferredConnectionID(oldConnectionID);
    return( ccode );
}
