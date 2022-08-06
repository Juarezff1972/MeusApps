/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwdir.h>
#include <nwlocal.h>


/*___________________________ GetVolumeName __________________________________
|
| Output:   0		       --  SUCCESSFUL
|	    network error code --  UNSUCCESSFUL
|
| Comments:
|   This routine will look up a volume name given the volume number.  The
|   returned volume name will be of length 0 if no volume is mounted in
|   that volume number slot.  If a volume is mounted, its name will be returned
|   in all capital letters, and there will be no colon (:) after the name.
|____________________________________________________________________________*/

int GetVolumeName(volumeNumber, volumeName)

int   volumeNumber;  /* Volume number of the volume whose name that is being
			               searched for					                         */
char *volumeName;    /* Receives a null terminated string containing the
			               volume name for the specified volume number	       */
{
    BYTE sendPacket[4], receivePacket[18];
    int ccode;

    sendPacket[2] = 6;
    sendPacket[3] = (BYTE)volumeNumber;
    *((int *)sendPacket) = 2;

    /* Note that the maximum receive size should be 17, but because */
    /* the server is returning an extra byte, we changed to 18 */

    *((int *)receivePacket) = 18;

    ccode = _ShellRequest((BYTE)226, sendPacket, receivePacket);
    if (ccode)
	return (ccode);

    /* The returned volume name is no longer NULL terminated, we
       must NULL terminate it ourselves 			       */

    if (receivePacket[2] > 0)
	memmove(volumeName, receivePacket + 3, receivePacket[2]);
    volumeName[receivePacket[2]] = '\0';

    return (0);
}
