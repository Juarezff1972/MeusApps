/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwdir.h>
#include <nwlocal.h>
#include <nwmisc.h>

/*_____________________ SetDirectoryInformation ______________________________
|
| Output:   0			--  SUCCESSFUL
|	    network error code	--  UNSUCCESSFUL
|
| Comments:
|   This function changes a directory's information.  The directory may be
|   given as a directory path relative to a drive handle, or as an absolute
|   directory path.  If the directory path is given as an absolute directory
|   path the drive handle may be 0.  The user must have parental rights to the
|   directory's father in order to change the directory's information.  The
|   supervisor is the only one who can give a directory a new owner object ID.
|_____________________________________________________________________________*/

int SetDirectoryInformation( directoryHandle, directoryPath,
                			     newCreationDateAndTime, newOwnerObjectID, 
                             newMaximumRightsMask )

BYTE directoryHandle;	       /* directoryPath is relative to this directory*/
char *directoryPath;	          /* Path specification of the directory whose
				                      information is changed		                */
BYTE *newCreationDateAndTime;  /* New date and time that the file was created */
long newOwnerObjectID;	       /* Unique bindery object ID of the new owner
				                      of the directory			                   */
BYTE newMaximumRightsMask;     /* New maximum rights mask of the directory  */
{
    BYTE sendPacket[270], receivePacket[2];
    int  len;

    sendPacket[2] = 25;
    sendPacket[3] = directoryHandle;

    *((long *)(sendPacket + 4)) = *((long *)newCreationDateAndTime);
    *((long *)(sendPacket + 8)) = LongSwap(newOwnerObjectID);

    sendPacket[12] = newMaximumRightsMask;
    len = strlen(directoryPath);
    sendPacket[13] = (BYTE)len;
    memmove(sendPacket + 14, directoryPath, len);

    *((int *)sendPacket) = len + 12;
    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)226, sendPacket, receivePacket));
}
