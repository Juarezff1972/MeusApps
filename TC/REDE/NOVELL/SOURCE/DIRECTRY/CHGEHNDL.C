/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwdir.h>
#include <nwlocal.h>


/*_________________________ SetDirectoryHandle ____________________________
|
| Output:   0	                --  SUCCESSFUL
|	         network error code --  UNSUCCESSFUL
|
| Comments:
|   This routine changes the directory that is being pointed to by the target
|   directory handle.  The targetDirectoryHandle must already exist.  If this
|   call fails, the targetDirectoryHandle remains unchanged.  When multiple
|   file servers are being used, the sourceDirectoryHandle and targetDirectory-
|   Handle must exist on the same file server.
|____________________________________________________________________________*/

int SetDirectoryHandle( sourceDirectoryHandle, sourceDirectoryPath,
                        targetDirectoryHandle )

BYTE  sourceDirectoryHandle; /* sourceDirectoryPath is relative to this
                                directory                                   */
char *sourceDirectoryPath;   /* Path specification (maximum 255 characters
                                INCLUDING the null terminator               */
BYTE  targetDirectoryHandle; /* Directory handle to be assigned to the dir-
                                ectory specified by sourceDirectoryHandle and
                                sourceDirectoryPath                         */
{
    BYTE sendPacket[261], receivePacket[2];
    int  len;

    sendPacket[2] = 0;
    sendPacket[3] = targetDirectoryHandle;
    sendPacket[4] = sourceDirectoryHandle;
    len = strlen(sourceDirectoryPath);
    sendPacket[5] = (BYTE)len;
    memmove(sendPacket + 6, sourceDirectoryPath, len);

    *((int *)sendPacket) = len + 4;
    *((int *)receivePacket) = 0;

    return ( _ShellRequest((BYTE)226, sendPacket, receivePacket));
}
