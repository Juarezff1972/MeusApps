/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwdir.h>
#include <nwlocal.h>

/*_______________________ CreateDirectory ___________________________________
|
| Output:   0		       --  SUCCESSFUL
|	    network error code --  UNSUCCESSFUL
|
| Comments:
|   This routine creates a subdirectory under the directory which is pointed
|   to by directoryHandle.  The subdirectory to be created must be included as
|   the last name in the directoryPath.  It cannot include wildcard characters
|   or be longer than 14 characters (15 INCLUDING the null).  Any names longer
|   than 14 characters will be truncated.
|____________________________________________________________________________*/

int CreateDirectory( directoryHandle, directoryPath, maximumRightsMask )

BYTE  directoryHandle;	 /* directoryPath is relative to this directory     */
char *directoryPath;	    /* Path specification of new directory to be created
			                   (max 255 characters INCLUDING null terminator   */
BYTE  maximumRightsMask; /* Maximum rights mask for directory to be created */
{
    BYTE sendPacket[261], receivePacket[2];
    int  len;

    sendPacket[2] = 10;
    sendPacket[3] = directoryHandle;
    sendPacket[4] = maximumRightsMask;
    len = strlen(directoryPath);
    sendPacket[5] = (BYTE)len;
    memmove(sendPacket + 6, directoryPath, len);

    *((int *)sendPacket) = len + 4;
    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)226, sendPacket, receivePacket));
}
