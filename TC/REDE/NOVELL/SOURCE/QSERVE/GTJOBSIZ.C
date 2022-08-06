/*   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nitq.h>
#include <nwlocal.h>
#include <nwmisc.h>



/*_________________________ GetQueueJobsFileSize ___________________________
|
|  Output:    0                   --  SUCCESSFUL
|             network error code  --  UNSUCCESSFUL
|
|  Comments:  This function finds the size of a file in a job queue.
|____________________________________________________________________________*/


int GetQueueJobsFileSize(queueID, jobNumber, fileSize)
long  queueID;                           /* Object ID of the queue           */
WORD  jobNumber;                         /* Number of the job in the queue   */
long *fileSize;                          /* Receives the file size           */
{
int     ccode;
BYTE    sendPacket[9], receivePacket[12];

    *((int *)sendPacket) = 7;
    sendPacket[2] = 120;
    *((long *)(sendPacket + 3)) = LongSwap(queueID);
    *((int *)(sendPacket + 7)) = IntSwap(jobNumber);
    *((int *)receivePacket) = 10;
    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
    if (ccode)
        return(ccode);
    *fileSize = LongSwap( *((long *)(receivePacket + 8)));
    return(0);
}
