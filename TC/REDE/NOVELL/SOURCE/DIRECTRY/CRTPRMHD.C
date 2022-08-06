/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwlocal.h>
#include <nwdir.h>



/*________________ AllocPermanentDirectoryHandle ___________________________
|
| Output:   0		             --  SUCCESSFUL
|   	      network error code --  UNSUCCESSFUL
|
| Comments:
|   This routine creates a permanent drive handle.  Each permanent drive
|   handle must be assigned a unique drive name, if the given drive name
|   is a permanant drive handle which already exists, the network will
|   simply reuse the already existing drive handle.
|___________________________________________________________________________*/

int AllocPermanentDirectoryHandle(directoryHandle, directoryPath, driveLetter,
                                  newDirectoryHandle, effectiveRightsMask)

BYTE  directoryHandle;       /* directoryPath is relative to this directory */
char *directoryPath;         /* Path specification of the directory that will
                                be assigned the driveLetter (max 255 chars) */
char  driveLetter;           /* Drive letter to be printed when referring
                                to the newDirectoryHandle (A..Z)	          */
BYTE *newDirectoryHandle;    /* Receives the directory handle for the
                                directory assigned to driveLetter	          */
BYTE *effectiveRightsMask;   /* Receives the requesting workstation's
                                effective rights to the reassigned directory*/
{
    BYTE sendPacket[261], receivePacket[4];
    int ccode, len;

    sendPacket[2] = 18;
    sendPacket[3] = directoryHandle;
    sendPacket[4] = driveLetter;
    len = strlen(directoryPath);
    sendPacket[5] = (BYTE)len;
    memmove(sendPacket + 6, directoryPath, len);

    *((int *)sendPacket) = len + 4;
    *((int *)receivePacket) = 2;

    ccode = _ShellRequest((BYTE)226, sendPacket, receivePacket);
    if (ccode)
       return (ccode);

    if (newDirectoryHandle != (BYTE *)0)
	*newDirectoryHandle = receivePacket[2];
    if (effectiveRightsMask != (BYTE *)0)
	*effectiveRightsMask = receivePacket[3];

    return (0);
}
