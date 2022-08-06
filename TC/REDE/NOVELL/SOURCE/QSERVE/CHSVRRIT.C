/*   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwlocal.h>
#include <nwmisc.h>
#include <nitq.h>



/*_______________________  ChangeToClientRights ______________________________
|
|  Output:    0 		  --  SUCCESSFUL
|	      network error code  --  UNSUCCESSFUL
|
|  Comments:   Changes a server's current login identity to match the identity
|	       of the client for whom the server is acting.  This call can be
|	       made only by a queue server that has previously accepted a job
|	       for service.
|____________________________________________________________________________*/

int ChangeToClientRights(queueID, jobNumber)

long queueID;	  /* Object ID of the queue			      */
WORD jobNumber;   /* Number of the job				      */

{
    BYTE sendPacket[9], receivePacket[2];

    *((WORD *)sendPacket) = 7;
    sendPacket[2] = 116;
    *((long *)(sendPacket + 3)) = LongSwap(queueID);
    *((WORD *)(sendPacket + 7)) = IntSwap(jobNumber);
    *((WORD *)receivePacket) = 0;
    return( _ShellRequest((BYTE)227, sendPacket, receivePacket));
}
