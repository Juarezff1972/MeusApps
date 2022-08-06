/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwdir.h>
#include <nwlocal.h>


/*___________________ DeallocateDirectoryHandle ____________________________
|
| Output:   0		       --  SUCCESSFUL
|	    network error code --  UNSUCCESSFUL
|
| Comments:
|   This routine deallocates a drive handle which was previously allocated
|   by AllocPermanentDirectoryHandle or AllocTemporaryDirectoryHandle.	An
|   End of Job will also deallocate a drive handle which was created by
|   AllocTemporaryDirectoryHandle.
|___________________________________________________________________________*/

int DeallocateDirectoryHandle(directoryHandle)
BYTE directoryHandle;	 /* The drive handle to be deallocated		    */
{
    BYTE sendPacket[4], receivePacket[2];

    sendPacket[2] = 20;
    sendPacket[3] = directoryHandle;

    *((int *)sendPacket) = 2;
    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)226, sendPacket, receivePacket));
}
