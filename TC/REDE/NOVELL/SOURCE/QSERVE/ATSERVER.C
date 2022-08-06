/*   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwlocal.h>
#include <nwmisc.h>
#include <nitq.h>


/*_______________________ AttachQueueServerToQueue ___________________________
|
|  Output:     0		  --  SUCCESSFUL
|	       network error code --  UNSUCCESSFUL
|
|  Comments:   This routine attaches the requesting workstation to the specified
|	       queue as a queue server.  A station must attach itself to a
|	       queue as a server before it can service jobs from that queue.  A
|	       queue may have as many as 25 servers attached to it at one time.
|	       The station making this call must be security equivalent to one
|	       of the objects listed in the queue's Q_SERVERS group property.
|____________________________________________________________________________*/

int AttachQueueServerToQueue(queueID)

long queueID;	/* Object ID of the queue that the requesting workstation
		   wants to be attached to				     */
{
    BYTE sendPacket[7], receivePacket[2];

    *((WORD *)sendPacket) = 5;
    sendPacket[2] = 111;
    *((long *)(sendPacket + 3)) = LongSwap(queueID);
    *((WORD *)receivePacket) = 0;
    return( _ShellRequest((BYTE)227, sendPacket, receivePacket));
}
