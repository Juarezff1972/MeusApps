/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwmsg.h>
#include <nwlocal.h>



/*__________________________________________________________________________
|                                                                          |
|  connectionList   --     Contains a list of logical connection numbers   |
|                          to connect a pipe to.                           |
|  resultList       --     Receives a result code for each corresponding   |
|                          target connection in the connectionList.        |
|  connectionCount  --     Number of logical connections in connectionList |
|                          (maximum of 100 connections).                   |
|                                                                          |
|  Output:  0 --  SUCCESSFUL                                              |
|_________________________________________________________________________*/

int OpenMessagePipe(connectionList, resultList, connectionCount)
WORD *connectionList, connectionCount;
BYTE *resultList;
{
    char sendBuf[104], replyBuf[103];
    int ccode;
    int offset;

    *((int *)sendBuf) = connectionCount + 2;
    sendBuf[2] = (char)6;
    sendBuf[3] = (BYTE)connectionCount;
    for (offset = -1; ++offset < connectionCount;
         sendBuf[4+offset] = (BYTE)(connectionList[offset]) );
    *((int *)replyBuf) = connectionCount + 1;
    ccode = _ShellRequest((BYTE)0xE1, (BYTE *)sendBuf, (BYTE *)replyBuf);
    if (ccode)
        return (ccode);
    memmove(resultList, replyBuf + 3, (int)connectionCount);
    resultList[connectionCount] = 0;
    return (0);
}






int CloseMessagePipe(connectionList, resultList, connectionCount)
WORD *connectionList, connectionCount;
BYTE *resultList;
{
    char sendBuf[104], replyBuf[103];
    int ccode, offset;

    *((int *)sendBuf) = connectionCount + 2;
    sendBuf[2] = (char)7;
    sendBuf[3] = (char)connectionCount;
    for (offset = -1; ++offset < connectionCount;
         sendBuf[4+offset] = (BYTE)(connectionList[offset]) );
    *((int *)replyBuf) = connectionCount + 1;
    ccode = _ShellRequest((BYTE)0xE1, (BYTE *)sendBuf, (BYTE *)replyBuf);
    if (ccode)
        return (ccode);
    memmove(resultList, replyBuf + 3, (int)connectionCount);
    resultList[connectionCount] = 0;
    return (0);
}








int CheckPipeStatus(connectionList, resultList, connectionCount)
WORD *connectionList, connectionCount;
BYTE *resultList;
{
    char sendBuf[104], replyBuf[103];
    int ccode, offset;

    *((int *)sendBuf) = connectionCount + 2;
    sendBuf[2] = (char)8;
    sendBuf[3] = (char)connectionCount;
    for (offset = -1; ++offset < connectionCount;
         sendBuf[4+offset] = (BYTE)(connectionList[offset]) );
    *((int *)replyBuf) = connectionCount + 1;
    ccode = _ShellRequest((BYTE)0xE1, (BYTE *)sendBuf, (BYTE *)replyBuf);
    if (ccode)
        return (ccode);
    memmove(resultList, replyBuf + 3, (int)connectionCount);
    resultList[connectionCount] = 0;
    return (0);
}
