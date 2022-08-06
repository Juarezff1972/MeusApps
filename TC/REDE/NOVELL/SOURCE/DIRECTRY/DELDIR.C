/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwdir.h>
#include <nwlocal.h>


/*________________________ DeleteDirectory _________________________________
|
| Output:   0		       --  SUCCESSFUL
|	    network error code --  UNSUCCESSFUL
|
| Comments:
|   This routine deletes a directory.  If the directory to be deleted is not
|   empty, does not exist, or the requesting workstation doesn't have the
|   proper rights, the directory is not removed.  The standard directories on
|   the SYS volume may not be deleted (i.e. SYSTEM, PUBLIC, MAIL, LOGIN).
|   Root directories may not be deleted.
|___________________________________________________________________________*/

int DeleteDirectory(driveHandle, directoryPath)
BYTE  driveHandle;   /* directoryPath is relative to this directory  	    */
char *directoryPath; /* Path specification of the directory to be deleted
			               (maximum 255 character INCLUDING null terminator    */
{
    BYTE sendPacket[261], receivePacket[2];
    int  len;

    sendPacket[2] = 11;
    sendPacket[3] = driveHandle;
    sendPacket[4] = 0;
    len = strlen(directoryPath);
    sendPacket[5] = (BYTE)len;
    memmove(sendPacket + 6, directoryPath, len);

    *((int *)sendPacket) = len + 4;
    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)226, sendPacket, receivePacket));
}
