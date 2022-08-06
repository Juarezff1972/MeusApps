/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwdir.h>
#include <nwlocal.h>
#include <nwmisc.h>


/*____________________ DeleteTrusteeFromDirectory __________________________
|
| Output:   Returns a 0 if successful or a network error code if unsuccessful.
|
| Comments:
|   This routine removes a trustee from a directory's trustee list.  This
|   function revokes all of the rights that a trustee has been granted.  The
|   requesting workstation must have parental rights in the directory, or to
|   a parent of the directory, in order to delete a trustee.
|____________________________________________________________________________*/

int DeleteTrusteeFromDirectory( directoryHandle, directoryPath, 
                                trusteeObjectID )

BYTE  directoryHandle;	/* directoryPath is relative to this directory	    */
char *directoryPath;	   /* Path specification that the trustee should be 
                           removed from (max 255 characters INCLUDING null 
                           terminator                                       */
long  trusteeObjectID;	/* Unique bindery object ID of trustee		          */
{
BYTE  sendPacket[265], receivePacket[2];
int   len;

    sendPacket[2] = 14;
    sendPacket[3] = directoryHandle;
    *((long *)(sendPacket + 4)) = LongSwap(trusteeObjectID);
    len = strlen(directoryPath);
    sendPacket[9] = (BYTE)len;
    memmove(sendPacket + 10, directoryPath, len);

    *((int *)sendPacket) = len + 8;

    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)226, sendPacket, receivePacket));
}
