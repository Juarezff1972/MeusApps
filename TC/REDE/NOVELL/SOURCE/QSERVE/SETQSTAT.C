/*   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nitq.h>
#include <nwlocal.h>
#include <nwmisc.h>



/*____________________________ SetQueueCurrentStatus _________________________
|
|  Output:    0 		  --  SUCCESSFUL
|	      network error code  --  UNSUCCESSFUL
|
|  Comments:  This function controls the addition of jobs and servers to a
|	      queue.  This call may be made only by a station with operator
|	      privileges.
|_____________________________________________________________________________*/


int SetQueueCurrentStatus(queueID, queueStatus)
long queueID;	    /* Object ID of the queue to be set 		      */
BYTE queueStatus;   /* The control byte that determines the new status	      */
{
BYTE sendPacket[8], receivePacket[2];

    *((WORD *)sendPacket) = 6;
    sendPacket[2] = 103;
    *((long *)(sendPacket + 3)) = LongSwap(queueID);
    sendPacket[7] = queueStatus;
    *((WORD *)receivePacket) = 0;
    return( _ShellRequest((BYTE)227, sendPacket, receivePacket));
}
