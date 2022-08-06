/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwwrkenv.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nat.h>

/*________________________ AFPCreateDirectory ____________________________

  Description:  Creates a directory using an AFP name.
  Input:        int connectionID;        Number of the server where the
                                         transaction is to be performed.
                BYTE volumeNum;          Volume number where the directory
                                         entry is.
                long AFPEntryID;         AFP base ID.
                BYTE *finderInfo;        Finder information for new directory.
                char *AFPPathString;     AFP style directory path, relative to
                                         AFPbaseID.
                long *newAFPEntryID;     ID of the newly created directory.
 
  Output:       0 is successful or a bindery error code if unsuccessful.
 
 __________________________________________________________________________*/

int AFPCreateDirectory( connectionID, volumeNum, AFPEntryID, finderInfo,
                        AFPPathString, newAFPEntryID )
WORD connectionID;
BYTE volumeNum;
long AFPEntryID;
BYTE *finderInfo;
char *AFPPathString;
long *newAFPEntryID;
{
BYTE sendPacket[298];
BYTE receivePacket[4];
int  ccode;
int  packetLength;
WORD oldConnectionID;

    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID(connectionID);

    /*                                                   */
    /* build the packet for creating the directory entry */
    /*                                                   */
    packetLength = 3 + (39 + (int)AFPPathString[0]); /* header+(parameters) */
    *((int *)sendPacket) = IntSwap(packetLength - 2);
    sendPacket[2] = 1;  /* create directory request number */
    sendPacket[3] = (BYTE)volumeNum;
    *((long *)(sendPacket + 4)) = AFPEntryID;
    /* sendPacket[8] is unused */
    memmove(sendPacket + 9, (char *)finderInfo, (WORD)32);
    memmove(sendPacket + 41, AFPPathString, (WORD)(AFPPathString[0] + 1));
    /*                                                     */
    /* Request that the directory be created               */
    /*                                                     */
    if ((ccode = _FileServiceRequest((BYTE)35, sendPacket, packetLength,
           receivePacket, (WORD)4)) == 0)
        *newAFPEntryID = *((long *)receivePacket);

    SetPreferredConnectionID(oldConnectionID);
    return( ccode );
}
