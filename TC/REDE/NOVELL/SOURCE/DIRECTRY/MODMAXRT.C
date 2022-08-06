/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwdir.h>
#include <nwlocal.h>


/*_____________________ ModifyMaximumRightsMask ____________________________
|
| Output:   0		       --  SUCCESSFUL
|	    network error code --  UNSUCCESSFUL
|
| Comments:
|   This function modifies the maximum rights mask of a directory. The
|   rights in the revokeRightsMask are first deleted from the directory's
|   maximum rights mask, and then the rights in the grantRightsMask are added.
|   The maximumRightsMask can be reset by setting the revokeRightsMask to FFh
|   and then setting the grantRightsMask to the desired maximum rights mask.
|____________________________________________________________________________*/

int ModifyMaximumRightsMask( directoryHandle, directoryPath,
                             revokeRightsMask, grantRightsMask )

BYTE  directoryHandle;   /* directoryPath is relative to this directory     */
char *directoryPath;     /* Path specification of the directory to be
                            modified (max 255 characters INCLUDING null     */
BYTE  revokeRightsMask;  /* The rights mask of rights to be revoked	       */
BYTE  grantRightsMask;   /* The rights mask of rights to be granted	       */
{
    BYTE sendPacket[262], receivePacket[2];
    int  len;

    sendPacket[2] = 4;
    sendPacket[3] = directoryHandle;
    sendPacket[4] = grantRightsMask;
    sendPacket[5] = revokeRightsMask;
    len = strlen(directoryPath);
    sendPacket[6] = (BYTE)len;
    memmove(sendPacket + 7, directoryPath, len);

    *((int *)sendPacket) = len + 5;
    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)226, sendPacket, receivePacket));
}
