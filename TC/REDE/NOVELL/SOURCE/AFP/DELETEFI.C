/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwwrkenv.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nat.h>

/*____________________________ AFPDelete __________________________________

  Description:  Deletes a file using an AFP name.
  Input:        int connectionID;          Number of the server where the
                                           transaction is to be performed.
                BYTE volumeNum;            Volume number where the directory
                                           entry is.
                long AFPEntryID;           AFP base ID.
                char *AFPPathString;       AFP style directory path, relative 
                                           to AFPbaseID.
  Output:       0 is successful or a bindery error code if unsuccessful.
___________________________________________________________________________*/

int AFPDelete(connectionID, volumeNum, AFPEntryID, AFPPathString)
WORD connectionID;
BYTE volumeNum;
long AFPEntryID;
char *AFPPathString;
{
BYTE sendPacket[265];
int  ccode;
int  packetLength;
WORD oldConnectionID;

    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID(connectionID);

    /*                                                   */
    /* build the packet for deleting the file            */
    /*                                                   */
    packetLength = 3 + (6 + (int)AFPPathString[0]); /* header+(parameters) */
    *((int *)sendPacket) = IntSwap(packetLength - 2);
    sendPacket[2] = 3; /* create file request number */
    sendPacket[3] = volumeNum;
    *((long *)(sendPacket + 4)) = AFPEntryID;
    memmove(sendPacket + 8, AFPPathString, (WORD)(AFPPathString[0] + 1));


    /*                                                     */
    /* Request that the file be deleted                    */
    /*                                                     */
    if ((ccode = _FileServiceRequest((BYTE)35, sendPacket, packetLength,
                (BYTE *)NULL, (WORD)0 )) == 0)
        SetPreferredConnectionID(oldConnectionID);
    return( ccode );
}
