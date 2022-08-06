/*   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nitq.h>
#include <nwlocal.h>
#include <nwmisc.h>




/*________________________  RemoveJobFromQueue ______________________________
|
|  Output:    0 		  --  SUCCESSFUL
|	      network error code  --  UNSUCCESSFUL
|
|
|  Comments:   This function removes a job from a job queue.  It may only be
|	       called by the user that created the job or by an operator.
|___________________________________________________________________________*/

int RemoveJobFromQueue(queueID, jobNumber)
long queueID;		    /* Object ID of the queue			      */
WORD jobNumber; 	    /* Number of the job to be removed from the queue */
{
    BYTE sendPacket[9], receivePacket[2];

    *((WORD *)sendPacket) = 7;
    sendPacket[2] = 106;
    *((long *)(sendPacket + 3)) = LongSwap(queueID);
    *((WORD *)(sendPacket + 7)) = IntSwap(jobNumber);
    *((WORD *)receivePacket) = 0;
    return( _ShellRequest((BYTE)227, sendPacket, receivePacket));
}
