/*   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwlocal.h>
#include <nwmisc.h>
#include <nitq.h>



/*_________________________  ChangeQueueJobPosition ___________________________
|
|  Output:     0		  --  SUCCESSFUL
|	       network error code --  UNSUCCESSFUL
|
|
|   Comments:	This function changes a job's position in a queue.  The value
|		of the new position may be from 1 to 250.  This function may
|		be performed only by an operator.
|___________________________________________________________________________*/

int ChangeQueueJobPosition(queueID, jobNumber, newPosition)
long queueID;	   /* Object ID of the queue				    */
WORD jobNumber;    /* Number of the job that needs to be repositioned	    */
BYTE newPosition;  /* New position of the job				    */
{
    BYTE sendPacket[10], receivePacket[2];

    *((WORD *)sendPacket) = 8;
    sendPacket[2] = 110;
    *((long *)(sendPacket + 3)) = LongSwap(queueID);
    *((WORD *)(sendPacket + 7)) = IntSwap(jobNumber);
    sendPacket[9] = newPosition;
    *((WORD *)receivePacket) = 0;
    return( _ShellRequest((BYTE)227, sendPacket, receivePacket));
}
