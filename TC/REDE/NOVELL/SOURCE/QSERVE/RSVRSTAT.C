/*   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nitq.h>
#include <nwlocal.h>
#include <nwmisc.h>



/*___________________ ReadQueueServerCurrentStatus __________________________
|
|  Output:    0 		  --  SUCCESSFUL
|	      network error code  --  UNSUCCESSFUL
|
|  Comments:   This function reads the current status of a queue server.  This
|	       call may be made only by a station that has previously attached
|	       to the specified queue as a queue server.
|____________________________________________________________________________*/


int ReadQueueServerCurrentStatus(queueID, serverID,
				 serverStation, serverStatusRecord)

long  queueID;			/* Object ID of the queue that the queue server
				   is attached to			      */
long  serverID; 		/* Object ID of the queue server	      */
WORD  serverStation;		/* The connection number of the queue server  */
char  serverStatusRecord[64];	/* Receives the status of the specified queue
				   server (64 characters)		      */
{
    BYTE    sendPacket[12], receivePacket[66];
    int     ccode;

    *((int *)sendPacket) = 10;
    sendPacket[2] = 118;
    *((long *)(sendPacket + 3)) = LongSwap(queueID);
    *((long *)(sendPacket + 7)) = LongSwap(serverID);
    sendPacket[11] = (BYTE)serverStation;
    *((int *)receivePacket) = 64;
    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
    if (ccode)
	return(ccode);
    memmove(serverStatusRecord, receivePacket + 2, 64);
    return(0);
}
