/*   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nitq.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>


/*_________________________________________________________________________
 |
 |  Output:		      0  --  SUCCESSFUL
 |	     Network Error Code  --  UNSUCCESSFUL
 |
 |  Comments:  This function gets the file size of a job entry in the queue.
 |
 |________________________________________________________________________*/


int GetQueueJobEntrysFileSize( connectionID, queueID, jobNumber, fileSize )

WORD	connectionID;	    /* Conneciton ID of the server to get information
			       on					  */
long	queueID;	    /* ID of the bindery object to return information
			       on					  */
WORD	jobNumber;	    /* Queue job number to get file size of	  */
long   *fileSize;	    /* Receives the size of the job file	  */

{
int	ccode;
WORD	oldConnectionID;
BYTE	sendPacket[9];
BYTE	replyPacket[12];

    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );
    *((WORD *)sendPacket) = 7;
    sendPacket[2] = 0x78;
    *((long *)(sendPacket + 3)) = LongSwap( queueID );
    *((WORD *)(sendPacket + 7)) = IntSwap( jobNumber );
    *((WORD *)replyPacket) = 10;
    ccode = _ShellRequest((BYTE)0xE3, sendPacket, replyPacket);
    if (ccode == 0)
	*fileSize = LongSwap( *((long *)(replyPacket + 8)));
    SetPreferredConnectionID( oldConnectionID );
    return( ccode );
}
