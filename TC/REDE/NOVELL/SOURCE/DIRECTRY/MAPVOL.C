/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwdir.h>
#include <nwlocal.h>

/*________________________ GetVolumeNumber ____________________________________
|
| Output:   Returns a 0 if successful or a network error code if unsuccessful.
|
| Comments:
|   This routine will return the volume number of the named volume.  Wild card
|   characters are allowed in the volume name.
|____________________________________________________________________________*/

int GetVolumeNumber(volumeName, volumeNumber)
char *volumeName;    /* Name of volume that volume number will be returned 
                        for                                                */
int  *volumeNumber;  /* Receives the volume number (0..31)		            */
{
BYTE	sendPacket[19], receivePacket[3];
int	ccode, len;

    sendPacket[2] = 5;
    len = strlen(volumeName);
    sendPacket[3] = len;
    memmove(sendPacket + 4, volumeName, len);

    *((int *)sendPacket) = len + 2;
    *((int *)receivePacket) = 1;

    ccode = _ShellRequest((BYTE)226, sendPacket, receivePacket);
    if (ccode)
	return (ccode);

    *volumeNumber = receivePacket[2];
    return (0);
}
