/*   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nitq.h>
#include <nwlocal.h>
#include <nwmisc.h>



/*____________________ SetQueueServerCurrentStatus ___________________________
|
|  Output:    0 		  --  SUCCESSFUL
|	      network error code  --  UNSUCCESSFUL
|
|  Comments:  This function updates the queue management software's copy
|	      of a server's status record.  It may be called only by a station
|	      that has previously attached to the specified queue as a queue
|	      server.
|____________________________________________________________________________*/

int SetQueueServerCurrentStatus(queueID, serverStatusRecord)
long queueID;		       /* Object ID of the queue		    */
BYTE serverStatusRecord[64];   /* Contains the new status record of the
				  server				    */
{
BYTE sendPacket[71], receivePacket[2];

    *((WORD *)sendPacket) = 69;
    sendPacket[2] = 119;
    *((long *)(sendPacket + 3)) = LongSwap(queueID);
    memmove(sendPacket + 7, serverStatusRecord, 64);
    *((WORD *)receivePacket) = 0;
    return( _ShellRequest((BYTE)227, sendPacket, receivePacket));
}
