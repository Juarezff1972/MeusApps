/*   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nitq.h>
#include <nwlocal.h>
#include <nwmisc.h>



/*________________________ GetQueueJobList ___________________________________
|
|  Output:	 0  --	SUCCESSFUL
|	       252  --	NO_SUCH_OBJECT
|
|  Comments:   This call may be made by any station that is security
|	       equivalent to an object listed in the queue's Q_USERS group
|	       property or Q_OPERATORS group property.
|_____________________________________________________________________________*/


int GetQueueJobList(queueID, jobCount, jobNumberList, maxJobNumbers)
long  queueID;	      /* Object ID of the queue for which the job list is
			 obtained					     */
WORD *jobCount;       /* Receives the number of jobs in the queue	     */
WORD *jobNumberList;  /* Receives the job numbers of all the jobs in the queue*/
WORD  maxJobNumbers;  /* Contains the maximum number of job numbers to be
			 returned in jobNumberList			     */

{
BYTE	sendPacket[7], receivePacket[504];
int	ccode, i, index;

    *((int *)sendPacket) = 5;
    sendPacket[2] = 107;
    *((long *)(sendPacket + 3)) = LongSwap(queueID);
    *((int *)receivePacket) = 502;
    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
    if (ccode)
       return(ccode);

    *jobCount = IntSwap(*((int *)(receivePacket + 2)) );
    if (*jobCount > maxJobNumbers)
       *jobCount = maxJobNumbers;
    index = 4;
    for (i = 0; i < *jobCount; i++)
    {
	jobNumberList[i] = IntSwap(*(int *)(receivePacket + index));
	index = index + 2;
    }
    return(ccode);
}
