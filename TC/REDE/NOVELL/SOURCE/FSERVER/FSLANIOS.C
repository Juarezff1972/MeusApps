/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>


#define MIN(a,b)	    ((a) < (b) ? (a) : (b))

int GetFileServerLANIOStats(connectionID, structSize, serverLANIOStats)

WORD	connectionID;	 /* Workstation's connection ID on the server whose
			    statistics are to be returned		    */
int	structSize;	 /* Number of bytes to return in serverLANIOStats   */

SERVER_LAN_IO *serverLANIOStats;  /* Receives the LAN i\o statistics	    */

{
int	      ccode;
register int  loopCount;
WORD	      oldConnectionID;
BYTE	      sendPacket[3];
BYTE	      replyPacket[sizeof(SERVER_LAN_IO)+2];
int	      bufferSize = sizeof(SERVER_LAN_IO);

    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );
    *((WORD *)sendPacket) = 1;
    sendPacket[2] = 0xE7;
    *((WORD *)replyPacket) = 66;
    ccode = _ShellRequest((BYTE)0xE3, sendPacket, replyPacket);
    if (ccode)
       return(ccode);

    *((long *)(replyPacket+2)) =  LongSwap(*((long *)(replyPacket+2)));
    *((int *)(replyPacket+6)) =   IntSwap(*((int *)(replyPacket+6)));
    *((int *)(replyPacket+8)) =   IntSwap(*((int *)(replyPacket+8)));
    *((int *)(replyPacket+10)) =  IntSwap(*((int *)(replyPacket+10)));
    *((long *)(replyPacket+12)) = LongSwap(*((long *)(replyPacket+12)));

    for ( loopCount = 16; loopCount < 56; loopCount+=2 )
	*((int *)(replyPacket+loopCount)) =
			     IntSwap(*((int *)(replyPacket+loopCount)));

    *((long *)(replyPacket+56)) = LongSwap(*((long *)(replyPacket+56)));
    *((long *)(replyPacket+60)) = LongSwap(*((long *)(replyPacket+60)));
    *((long *)(replyPacket+64)) = LongSwap(*((long *)(replyPacket+64)));
    memmove( serverLANIOStats, (replyPacket + 2), MIN( structSize, bufferSize));

    SetPreferredConnectionID( oldConnectionID );
    return( ccode );
}
