/*   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nitq.h>
#include <nwlocal.h>
#include <nwmisc.h>



/*____________________ DetachQueueServerFromQueue ___________________________
|
|  Output:    0 		  --  SUCCESSFUL
|	      network error code  --  UNSUCCESSFUL
|
|  Comments:  This function removes the calling station from the queue's list
|	      of active servers.  It may be called only by a station that had
|	      previously attached itself to the queue as a server.
|____________________________________________________________________________*/

int DetachQueueServerFromQueue(queueID)
long  queueID;	/* Object ID of the queue    */
{
BYTE  sendPacket[7], receivePacket[2];


    *((WORD *)sendPacket) = 5;
    sendPacket[2] = 112;
    *((WORD *)receivePacket) = 0;
    *((long *)(sendPacket + 3)) = LongSwap(queueID);
    return( _ShellRequest((BYTE)227, sendPacket, receivePacket));
}
