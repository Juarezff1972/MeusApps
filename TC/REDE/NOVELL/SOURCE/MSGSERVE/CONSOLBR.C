/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwlocal.h>
#include <nwconsol.h>



/*__________________________________________________________________________
|
|   Output:     0  --  SUCCESSFUL
|             198  --  NO_CONSOLE_RIGHTS
|__________________________________________________________________________*/

int SendConsoleBroadcast(message, connectionCount, connectionList)

char *message;          /* Null terminated string containing the message
                           to send (maximum 60 characters INCLUDING null)  */
WORD connectionCount;   /* Number of logical connection in the connectionList
                        (0 = broadcast to all workstations)                */
WORD *connectionList;   /* Contains a list of logical connection numbers to
                        receive a message.                                 */
{
    BYTE requestPacket[164], replyPacket[2], messageLength;
    int offset;

    requestPacket[2] = (BYTE)209;
    requestPacket[3] = (BYTE)connectionCount;
    if (connectionCount > (BYTE)0)
        for( offset = -1; ++offset < connectionCount;
             requestPacket[4+offset] = (BYTE)(connectionList[offset]) );
    messageLength = (BYTE)strlen(message);
    requestPacket[4 + connectionCount] = messageLength;
    if (messageLength > 0)
        memmove((requestPacket+5+connectionCount), message, (WORD)messageLength);
    *((int *)requestPacket) = 3 + connectionCount + messageLength;
    *((int *)replyPacket) = 0;

    return (_ShellRequest((BYTE)227, requestPacket, replyPacket));
}
