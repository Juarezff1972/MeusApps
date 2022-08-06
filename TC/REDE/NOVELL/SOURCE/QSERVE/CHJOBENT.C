/*   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nitq.h>

/*___________________________ ChangeQueueJobEntry _____________________________
|
|   Output:			 0  --	SUCCESSFUL
|		Network Error Code  --	UNSUCCESSFUL
|
|   Comments:	This function changes the information in a job's queue record.
|		This call may be made by the user that originally created
|		the job, or by an operator.  If the target entry is already
|		being serviced then a Q_SERVICING error is returned and no
|		changes are made to the job's record.  The caller has to supply
|		the targetServerIDNumber, targetExecutionTime, jobType,
|		userHoldFlag, textJobDescription, clientRecordArea.  If the
|		caller is an operator, then the operatorHoldFlag is also reset
|		to the value supplied by the caller.
|___________________________________________________________________________*/

int ChangeQueueJobEntry(queueID,job)
long	    queueID;	      /* Queue ID of the job to be changed	     */
JobStruct  *job;	      /* Structure containing information on the job */
{
BYTE sendPacket[263], receivePacket[2];


    *((WORD *)sendPacket) = 261;
    sendPacket[2] = 109;
    *((WORD *)receivePacket) = 0;
    *((long *)(sendPacket + 3)) = LongSwap(queueID);
    memmove(sendPacket + 7, (char *)job, (WORD)256);

    job = (JobStruct *)(sendPacket + 7);
    job->targetServerIDNumber = LongSwap(job->targetServerIDNumber);
    job->jobNumber = IntSwap(job->jobNumber);
    job->jobType = IntSwap(job->jobType);
    return( _ShellRequest((BYTE)227, sendPacket, receivePacket));
}
