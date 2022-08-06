/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwdir.h>
#include <nwlocal.h>
#include <nwmisc.h>


/*_____________________ AddTrusteeToDirectory _______________________________
|
| Output:   0		             --  SUCCESSFUL
|  	      network error code --  UNSUCCESSFUL
|
| Comments:
|   This routine adds a trustee to a directory.  If the object ID is already
|   a trustee of the directory, the access mask  for the trustee is replaced
|   by the trusteeRightsMask.  Otherwise, the object is added as a trustee to
|   directory with its rights mask equal to the trusteeRightsMask.
|____________________________________________________________________________*/

int AddTrusteeToDirectory( directoryHandle, directoryPath,
			                  trusteeObjectID, trusteeRightsMask )

BYTE  directoryHandle;	  /* directoryPath is relative to this directory    */
char *directoryPath;	     /* Path specification of directory to add trustee to
			                    (max 255 characters INCLUDING null terminator) */
long  trusteeObjectID;	  /* Unique bindery object ID of trustee	          */
BYTE  trusteeRightsMask;  /* Requesting workstation's trustee rights        */
{
    BYTE sendPacket[265], receivePacket[2];
    int len;

    sendPacket[2] = 13;
    sendPacket[3] = directoryHandle;
    *((long *)(sendPacket + 4)) = LongSwap(trusteeObjectID);
    sendPacket[8] = trusteeRightsMask;
    len = strlen(directoryPath);
    sendPacket[9] = (BYTE)len;
    memmove(sendPacket + 10, directoryPath, len);

    *((int *)sendPacket) = len + 8;
    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)226, sendPacket, receivePacket));
}
