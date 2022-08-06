/*   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nitq.h>
#include <nwlocal.h>
#include <nwmisc.h>



/*_________________________ CreateQueue ____________________________________
|
|  Output:    0 		  --  SUCCESSFUL
|	      network error code  --  UNSUCCESSFUL
|
|   Comments:
|	The group properties Q_SERVERS, Q_OPERATORS,  and Q_USERS will also be
|	created by the queue management software.  This call can be made only
|	by a station with supervisor privileges.
|___________________________________________________________________________*/


int CreateQueue(queueName, queueType, directoryHandle, pathName, queueID)

char *queueName;	/* String containing the name of the queue to be
			   created (max 48 characters INCLUDING the null     */
int   queueType;	/* Type of the queue to be created		     */
char  directoryHandle;	/* Directory handle that points to the directory
			   where the queue's directory is to be created      */
char *pathName; 	/* Path relative to the directoryHandle that specifies
			   the directory where the queue's directory is to
			   be created (max 119 characters INCLUDING the null */
long *queueID;		/* Receives the object ID number of the newly
			   created queue				     */
{
    BYTE sendPacket[173], receivePacket[6];
    int position, ccode, len, pathlen;

    sendPacket[2] = 100;
    *((int *)(sendPacket + 3)) = IntSwap(queueType);
    len = strlen(queueName);
    sendPacket[5] = len;
    memmove((sendPacket + 6), queueName, (WORD)sendPacket[5]);
    position = 6 + sendPacket[5];
    sendPacket[position++] = directoryHandle;
    pathlen = strlen(pathName);
    sendPacket[position++] = pathlen;
    memmove((sendPacket + position), pathName, pathlen);
    *((int *)sendPacket) = len + pathlen + 6;
    *((int *)receivePacket) = 4;
    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
    if (ccode)
	return(ccode);
    *queueID = LongSwap(*((long *)(receivePacket + 2)));
    return(0);
}
