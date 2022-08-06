/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwmsg.h>
#include <nwlocal.h>



/*___________________________________________________________________________
|
|     Output:  0 --  SUCCESSFUL
|__________________________________________________________________________*/

int SendPersonalMessage(message, connectionList, resultList, connectionCount)

char *message;         /* Null terminated string containing the message to
                          send (maximum 127 characters INCLUDING the NULL. */
WORD *connectionList;  /* Contains a list of logical connection number to
                          send the message to.                             */
WORD  connectionCount; /* Number of logical connections in connectionList. */
BYTE *resultList;      /* Receives a result code for each corresponding
                          target connection in the connectionList.         */
{
int     ccode, offset;
char    len, sendBuf[231], replyBuf[103];

    len = (char)strlen(message);
    if (len > 126)
        len = 126;
    *((int *)sendBuf) = connectionCount + len + 3;
    sendBuf[2] = (char)4;
    sendBuf[3] = (BYTE)connectionCount;
    for( offset = -1; ++offset < connectionCount;
         sendBuf[4+offset] = ((BYTE)connectionList[offset]) );
    sendBuf[connectionCount + 4] = len;
    memmove((sendBuf + connectionCount + 5), message, (int)len);
    *((int *)replyBuf) = connectionCount + 1;
    replyBuf[2] = (char)connectionCount;
    ccode = _ShellRequest((BYTE)0xE1, (BYTE *)sendBuf, (BYTE *)replyBuf);
    if (ccode)
        return (ccode);
    memmove(resultList, replyBuf + 3, connectionCount);
    resultList[connectionCount] = 0;
    return (0);
}

 /***************************************************************************/

int GetPersonalMessage(messageBuffer, sourceConnection)

char *messageBuffer;    /* Receives oldest message from pipe message queue */
WORD *sourceConnection; /* Receives the connection number of the client that
                           originated the message (1..100)                 */
{
    char sendBuf[3], replyBuf[132], len;
    int ccode;

    *((int *)sendBuf) = 1;
    sendBuf[2] = (char)5;
    *((int *)replyBuf) = 130;
    ccode = _ShellRequest((BYTE)0xE1, (BYTE *)sendBuf, (BYTE *)replyBuf);
    if (ccode)
        return (ccode);
    *sourceConnection = (WORD)replyBuf[2];
    len = replyBuf[3];
    memmove(messageBuffer, replyBuf + 4, len);
    messageBuffer[len] = 0;
    return (0);
}
