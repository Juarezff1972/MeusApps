/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>


#define MIN(a,b)	    ((a) < (b) ? (a) : (b))

int GetDriveMappingTable( connectionID, structSize, driveMappingTable )

WORD	connectionID;	/* Connection ID of the server to get information for */
int	structSize;	/* Number of bytes to return in connectionUsage       */

DRIVE_MAP_TABLE   *driveMappingTable;

{
int	ccode;
WORD	oldConnectionID;
BYTE	replyPacket[238];
BYTE	sendPacket[3];
register int tableCount;


    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );
    *((WORD *)sendPacket) = 1;
    sendPacket[2] = 0xD7;
    *((WORD *)replyPacket) = 236;
    ccode = _ShellRequest((BYTE)0xE3, sendPacket, replyPacket);

    if (ccode == 0)
     {
	*((long *)(replyPacket+2)) = LongSwap(*((long *)(replyPacket+2)));
	*((int *)(replyPacket+14)) = IntSwap(*((int *)(replyPacket+14)));
	*((long *)(replyPacket+114)) = LongSwap(*((long *)(replyPacket+114)));

	for ( tableCount = 118; tableCount < 238; tableCount++ )
	     *((int *)(replyPacket + tableCount))  =
		    IntSwap(*((int *)(replyPacket + tableCount)));

	memmove(driveMappingTable, &replyPacket[2], MIN(structSize, 236));
      }

    SetPreferredConnectionID( oldConnectionID );
    return( ccode );
}
