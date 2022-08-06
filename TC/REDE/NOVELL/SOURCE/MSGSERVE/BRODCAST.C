/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwmsg.h>
#include <nwlocal.h>



/*___________________________________________________________________________
|
|   Output:  0	--  SUCCESSFUL
|___________________________________________________________________________*/

int SendBroadcastMessage(message, connectionList, resultList, connectionCount)

char *message;	      /* Null terminated string containg the message to
			 send. (maximum 56 characters INCLUDING null).	     */
BYTE *resultList;     /* Receives a list of result codes for each
			 connection the message was sent		     */
WORD  connectionCount;/* Number of logical connections in the connectionList */
WORD *connectionList; /* Contains a list of logical connection numbers that
			 the message is sent to 			     */
{
    BYTE sendBuf[160], replyBuf[103], len;
    int ccode, offset;

    len = (BYTE)strlen(message);
    *((WORD *)sendBuf) = connectionCount + len + 3;
    sendBuf[2] = (BYTE)0;
    sendBuf[3] = (BYTE)connectionCount;
    for( offset = -1; ++offset < connectionCount;
	 sendBuf[4+offset] = (BYTE)connectionList[offset] );
    sendBuf[connectionCount + 4] = len;
    memmove((sendBuf + connectionCount + 5), message, len);
    *((WORD *)replyBuf) = connectionCount + 1;
    replyBuf[2] = (BYTE)connectionCount;
    ccode = _ShellRequest((BYTE)0xE1, sendBuf, replyBuf);
    if (ccode)
	return (ccode);
    memmove(resultList, replyBuf + 3, connectionCount);
    return (0);
}

/****************************************************************************/

int GetBroadcastMessage(messageBuffer)
char *messageBuffer;
{
    char sendBuf[3], replyBuf[58];
    int len, ccode;

    *((int *)sendBuf) = 1;
    sendBuf[2] = (char) 1;
    *((int *)replyBuf) = 56;
    replyBuf[2] = (char)55;
    ccode = _ShellRequest((BYTE)0xE1, (BYTE *)sendBuf, (BYTE *)replyBuf);
    if (ccode)
	return (ccode);
    len = (int)replyBuf[2];
    memmove(messageBuffer, replyBuf + 3, len);
    messageBuffer[len] = 0;
    return (0);
}

/****************************************************************************/

int DisableBroadcasts()
{
    char sendBuf[4], replyBuf[3];

    *((int *)sendBuf) = 1;
    sendBuf[2] = (char) 2;
    *((int *)replyBuf) = 2;
    return(_ShellRequest((BYTE)0xE1, (BYTE *)sendBuf, (BYTE *)replyBuf));
}

/****************************************************************************/

int EnableBroadcasts()
{
    char sendBuf[4], replyBuf[3];

    *((int *)sendBuf) = 1;
    sendBuf[2] = (char) 3;
    *((int *)replyBuf) = 2;
    return (_ShellRequest((BYTE)0xE1, (BYTE *)sendBuf, (BYTE *)replyBuf));
}

/****************************************************************************/

int BroadcastToConsole(message)
char *message;
{
    char sendBuf[64], replyBuf[3], len;

    len = (char)strlen(message);
    if (len > 60)
	len = 60;
    *((int *)sendBuf) = len + 2;
    sendBuf[2] = (char)9;
    sendBuf[3] = len;
    memmove(sendBuf + 4, message, (int) len);
    *((int *)replyBuf) = 0;
    return (_ShellRequest((BYTE)0xE1, (BYTE *)sendBuf, (BYTE *)replyBuf));
}
