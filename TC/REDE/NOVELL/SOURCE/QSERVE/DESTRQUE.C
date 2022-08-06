/*   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nitq.h>
#include <nwlocal.h>
#include <nwmisc.h>

/*______________________________ DestroyQueue _________________________________
|
|
|	Input:	    long    QueueID;
|
|	Output:     Returns a 0 if successful or a network error code if unsuccessful.
|
|	Comments:   This function destroys the queue specified by the queueID.
|		    *  All active jobs are aborted.
|		    *  All servers are detached from the queue.
|		    *  All jobs in the queue are destroyed, and their associated
|		       files are deleted.
|		    *  The queue object and its associated properties are removed
|		       from the bindery.
|		    *  The queue's subdirectory is deleted.
|		    *  This call can be made only by a station with supervisor
|		       privileges.
|____________________________________________________________________________*/

int DestroyQueue(queueID)
long	queueID;		      /* The ID of the queue to be destroyed */
{
BYTE	sendPacket[7];
BYTE	receivePacket[2];


    *((WORD *)sendPacket) = 5;
    sendPacket[2] = 101;
    *((long *)(sendPacket + 3)) = LongSwap(queueID);
    *((WORD *)receivePacket) = 0;
    return( _ShellRequest((BYTE)227, sendPacket, receivePacket));
}
