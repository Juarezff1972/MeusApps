/*   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nitq.h>
#include <nwlocal.h>
#include <nwmisc.h>



/*_________________________ ReadQueueJobEntry ________________________________
|
|  Output:    0    --  SUCCESSFUL
|	      213  --  NO_Q_JOB
|
|  Comments:  This function retreives information about a job from a job queue.
|	      The job's 256-byte record is returned; all fields are filled in.
|_____________________________________________________________________________*/


int ReadQueueJobEntry(queueID, jobNumber, job)

long	     queueID;	 /* Object ID for the queue to be read		      */
WORD	     jobNumber;  /* Number of the job that information will be
			    returned about				      */
JobStruct   *job;	 /* Receives information about the specified job      */
{
    BYTE    sendPacket[9], receivePacket[258];
    int     ccode;

    *((int *)sendPacket) = 7;
    sendPacket[2] = 108;
    *((long *)(sendPacket + 3)) = LongSwap(queueID);
    *((int *)(sendPacket + 7)) = IntSwap(jobNumber);
    *((int *)receivePacket) = 256;
    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
    if (ccode)
	return(ccode);
    memmove((BYTE *)job, receivePacket + 2, 256);
    job->clientIDNumber = LongSwap(job->clientIDNumber);
    job->targetServerIDNumber = LongSwap(job->targetServerIDNumber);
    job->jobNumber = IntSwap(job->jobNumber);
    job->jobType = IntSwap(job->jobType);
    job->serverIDNumber = LongSwap(job->serverIDNumber);
    return(0);
}
