/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwprint.h>
#include <nwlocal.h>

/*__________________________ QueueExistingFile ______________________________

  Input:    BYTE driveHandle;       The drive handle that is pointing
                                    to the directory from which the
                                    filePath is relative.
            char *filePath;         An absolute path or a path relative
                                    to the drive handle that specifies the
                                    file to be printed.

  Output:   Returns a 0 if successful or a network error code if unsuccessful.

  Comments:
    This routine will cause the file given by the drive handle and file path
    to be placed in the print queue.  If the file is on a local drive, it will
    be spooled to the network.
    NOTE: According to the documentation edits, this call should be eliminated

  ___________________________________________________________________________*/

int QueueExistingFile(driveHandle, filePath)
BYTE driveHandle;
char *filePath;
{
    char sendPacket[261], receivePacket[2];

    sendPacket[2] = 3;
    sendPacket[3] = driveHandle;
    sendPacket[4] = (char)((BYTE)strlen(filePath));
    if (sendPacket[4] > (char)0)
        memmove(sendPacket + 5, filePath, (WORD)sendPacket[4]);
    *((int *)sendPacket) = 3 + sendPacket[4];

    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)224, (BYTE *)sendPacket, (BYTE *)receivePacket));
}
