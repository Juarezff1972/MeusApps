/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwdir.h>
#include <nwlocal.h>
#include <nwmisc.h>

/*_____________________ GetVolumeInfoWithHandle _____________________________
|
| Output:   0		       --  SUCCESSFUL
|	    network error code --  UNSUCCESSFUL
|
| Comments:
|   This routine will return information about the volume that is being
|   pointed to by a drive handle.
|___________________________________________________________________________*/

int GetVolumeInfoWithHandle( directoryHandle, volumeName, totalBlocks,
		                       sectorsPerBlock, availableBlocks, 
                             totalDirectorySlots, availableDirectorySlots, 
                             volumeIsRemovable )

BYTE directoryHandle;	       /* Directory to get information from	       */
char *volumeName;	             /* Receives the volume name (max 16 char)    */
WORD *totalBlocks;	          /* Receives the number of blocks on the volume*/
WORD *sectorsPerBlock;	       /* Receives the number of sectors in a block */
WORD *availableBlocks;	       /* Receives the number of unused blocks on  
                                  the volume				                      */
WORD *totalDirectorySlots;     /* Receives the number of directory slots on
				                      the volume				                      */
WORD *availableDirectorySlots; /* Receives the number of unused directory
				                      slots on the volume			                */
WORD *volumeIsRemovable;       /* Receives a flag indicating if the volume
				                      is removable				                   */
{
    BYTE sendPacket[4], receivePacket[30];
    int ccode;

    sendPacket[2] = 21;
    sendPacket[3] = directoryHandle;

    *((int *)sendPacket) = 2;
    *((int *)receivePacket) = 28;

    ccode = _ShellRequest((BYTE)226, sendPacket, receivePacket);
    if (ccode)
       return(ccode);

    if (sectorsPerBlock != (WORD *)0)
	*sectorsPerBlock = IntSwap(*((int *)(receivePacket + 2)));
    if (totalBlocks != (WORD *)0)
	*totalBlocks = IntSwap(*((int *)(receivePacket + 4)));
    if (availableBlocks != (WORD *)0)
	*availableBlocks = IntSwap(*((int *)(receivePacket + 6)));
    if (totalDirectorySlots != (WORD *)0)
	*totalDirectorySlots = IntSwap(*((int *)(receivePacket + 8)));
    if (availableDirectorySlots != (WORD *)0)
	*availableDirectorySlots = IntSwap(*((int *)(receivePacket + 10)));
    if (volumeName != (char *)0)
	strcpy(volumeName, (char *)(receivePacket + 12));
    if (volumeIsRemovable != (WORD *)0)
	*volumeIsRemovable = IntSwap(*((int *)(receivePacket + 28)));
    return (ccode);
}
