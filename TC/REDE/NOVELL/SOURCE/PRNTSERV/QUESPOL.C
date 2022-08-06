/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwprint.h>
#include <nwlocal.h>

/*____________________________ QueueCaptureFile ____________________________
 |
 | Input:    None.
 |
 | Output:   Returns a 0 if successful or a network error code if unsuccessful.
 |
 | Comments:
 |   This routine will close the current station's spool capture file and place
 |   it in the network print queue.
 |_________________________________________________________________________*/

int QueueCaptureFile()
{
    char sendPacket[4], receivePacket[2];

    sendPacket[2] = 1;
    sendPacket[3] = 0;
    *((int *)sendPacket) = 2;

    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)224, (BYTE *)sendPacket, (BYTE *)receivePacket));
}
