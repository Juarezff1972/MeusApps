/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwconsol.h>
#include <nwwrkenv.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <ntt.h>


/*________________________ GetDiskUtilization _____________________________
|
| Output:   0		       --  SUCCESSFUL
|	    network error code --  UNSUCCESSFUL
|
| Comments:
|   This function returns the disk usage of a bindery object on a volume.
|   To determine the total disk space used, this call should be made
|   repetitively for all mounted volumes.  To determine the number of bytes
|   of disk space used, the usedBlocks should be multiplied by the number of
|   sectors and the bytes per sector. Currently network implementations
|   allocate the disk in 8 512-byte sectors per block, which is 4K per block.
|__________________________________________________________________________*/


int GetDiskUtilization( objectID, volumeNumber, usedDirectories,
		                  usedFiles, usedBlocks )

long  objectID; 	      /* Unique ID of the bindery object		             */
BYTE  volumeNumber;	   /* Volume for which statistics are requested (0..31)*/
LONG *usedDirectories;	/* Receives the directories owned by the object     */
LONG *usedFiles;	      /* Receives the files created by the object	       */
LONG *usedBlocks;	      /* Receives the disk blocks used by the object      */
{
BYTE	sendPacket[8], receivePacket[13];
int	isV3, ccode;
WORD  connectionID;

    *((WORD *)sendPacket) = 6;
    sendPacket[2] = 14;
    sendPacket[3] = volumeNumber;
    *((long *)(sendPacket + 4)) = LongSwap( objectID );

    *((WORD *)receivePacket) = 11;

    ccode = _ShellRequest( (BYTE)227, sendPacket, receivePacket );

    if( ccode )
	   return( ccode );

    if (usedDirectories != 0)
	   *usedDirectories = (LONG)( IntSwap(*((int *)(receivePacket + 7))) );
    if (usedFiles != 0)
	   *usedFiles = (LONG)( IntSwap(*((int *)(receivePacket + 9))) );
    if (usedBlocks != 0)
	   *usedBlocks = (LONG)( IntSwap(*((int *)(receivePacket + 11))) );

    connectionID = GetPreferredConnectionID();
    isV3 = IsV3Supported( connectionID );
    if( isV3 )
      {
      *(WORD *)&sendPacket[0]    = 6;
      sendPacket[2]              = 41;
      sendPacket[3]              = volumeNumber;
      *(LONG *)&sendPacket[4]    = LongSwap( objectID );
      *(WORD *)&receivePacket[0] = 8;

      ccode = _NCPConnRequest( connectionID, 22, 8, sendPacket,
                               10, &receivePacket[2] );
      if( ccode )
         return( ccode );

      *usedBlocks = *(LONG *)&receivePacket[4];
      }

    return( 0 );
}
