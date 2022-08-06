/*   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nitq.h>
#include <nwlocal.h>
#include <nwmisc.h>



/*______________________  ReadQueueCurrentStatus _____________________________
|
|   Output:	 0 -- SUCCESSFUL		 211 -- NO_Q_RIGHTS
|	       150 -- SERVER_OUT_OF_MEMORY	 241 -- INVALID_BINDERY_SECURITY
|	       156 -- INVALID_PATH		 252 -- NO_SUCH_OBJECT
|	       209 -- NO_QUEUE			 254 -- SERVER_BINDERY_LOCKED
|	       210 -- NO_Q_SERVER		 255 -- BINDERY_FAILURE
|
|   Comments:  This function returns the current status of the queue.  It may
|	       be made by any workstation who has security equivalent to one of
|	       the objects listed in the queue's Q_USERS group property or
|	       Q_OPERATORS group property.
|_____________________________________________________________________________*/


int ReadQueueCurrentStatus(queueID, queueStatus, currentEntries,
	currentServers, serverIDList, serverStationList, maxConnections)

long  queueID;
BYTE *queueStatus;		/* Receives the queue's current status      */
BYTE *currentEntries;		/* Receives the number of jobs in the queue */
BYTE *currentServers;		/* Receives the number of servers attached
				   to service the queue 		    */
long  serverIDList[25]; 	/* Receives a list of server object IDs that
				   are servicing the queue		    */
WORD  serverStationList[25];	/* Receives a list of servers connection
				   numbers				    */
WORD  maxConnections;		/* Receives the maximum number of servers to
				   be returned in serverIDList and
				   serverStationList			   */
{
BYTE   sendPacket[7], receivePacket[134], index;
int    ccode, offset;
char  *ptr;


    *((WORD *)sendPacket) = 5;
    sendPacket[2] = 102;
    *((long *)(sendPacket + 3)) = LongSwap(queueID);
    *((WORD *)receivePacket) = 132;
    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
    if (!ccode)
     {
	if (queueStatus != (BYTE *)NULL)
	    *queueStatus = receivePacket[6];
	if (currentEntries != (BYTE *)NULL)
	    *currentEntries = (BYTE)receivePacket[7];
	if (currentServers != (BYTE *)NULL)
	    *currentServers = (BYTE)receivePacket[8];
	if ((WORD)receivePacket[8] > maxConnections)
	    receivePacket[8] = (BYTE)maxConnections;
	if (serverIDList != (long *)NULL)
	 {
	   for (index = 0; index < receivePacket[8]; index++)
	       serverIDList[index] =
			 LongSwap(*(long *)(receivePacket + 9 + index * 4));
	 }
	if (serverStationList != (WORD *)NULL)
	 {
	    ptr = (char *)(receivePacket + 9 + receivePacket[8] * 4);
	    for (offset = -1; ++offset < ((int)receivePacket[8]);
		 serverStationList[ offset ] = ((WORD)ptr[ offset ]) );
	 }
     }
    return(ccode);
}
