/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>


#define MIN(a,b)    ((a)<(b)?(a):(b))

int GetConnectionsUsageStats( connectionID, connectionNumber, structSize,
			      connectionUsage )
WORD	connectionID;
WORD	connectionNumber;
int	structSize;
CONN_USAGE *connectionUsage;

{
int	ccode;
WORD	oldConnectionID;
BYTE	sendPacket[5];
BYTE	replyPacket[22];


    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID(connectionID);
    *((WORD *)sendPacket) = 3;
    sendPacket[2] = 0xE5;
    *((WORD *)(sendPacket + 3)) = IntSwap(connectionNumber);
    *((WORD *)replyPacket) = 20;
    ccode = _ShellRequest((BYTE)0xE3, sendPacket, replyPacket);
    if (!ccode)
     {
	*((long *)(replyPacket + 2)) = LongSwap(*((long *)(replyPacket + 2)));
	*((long *)(replyPacket + 18)) = LongSwap(*((long *)(replyPacket + 18)));
	memmove(connectionUsage, (replyPacket+2), MIN(structSize,20));
     }
    SetPreferredConnectionID(oldConnectionID);
    return(ccode);
}
