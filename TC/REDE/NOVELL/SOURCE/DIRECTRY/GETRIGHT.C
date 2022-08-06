/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwdir.h>
#include <nwlocal.h>



/*___________________ GetEffectiveDirectoryRights __________________________
|
| Output:   0		       --  SUCCESSFUL
|	    network error code --  UNSUCCESSFUL
|
| Comments:
|   This routine returns the requesting workstation's effective rights to
|   a directory.
|___________________________________________________________________________*/

int GetEffectiveDirectoryRights( directoryHandle, directoryPath,
				                     effectiveRightsMask )

BYTE  directoryHandle;      /* directoryPath is relative to this directory  */
char *directoryPath;	       /* Path specification (max 255 characters 
                               INCLUDING the null terminator)			       */
BYTE *effectiveRightsMask;  /* Receives the requesting workstation's rights
			                      to the specified directory.		             */
{
    BYTE sendPacket[260], receivePacket[3];
    int  ccode, len;

    sendPacket[2] = 3;
    sendPacket[3] = directoryHandle;
    len = strlen(directoryPath);
    sendPacket[4] = (BYTE)len;
    memmove(sendPacket + 5, directoryPath, len);

    *((int *)sendPacket) = len + 3;
    *((int *)receivePacket) = 1;

    ccode = _ShellRequest((BYTE)226, sendPacket, receivePacket);
    if (ccode)
	return (ccode);

    *effectiveRightsMask = receivePacket[2];

    return (0);
}
