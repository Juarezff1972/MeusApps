/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>

#define MIN(a,b)	    ((a) < (b) ? (a) : (b))

int GetDiskCacheStats( connectionID, structSize, cacheStats )

WORD	connectionID;		/* the workstation's connection ID of the
				   server for which to get information */
int	structSize;		/* the number of bytes to return in
				   connectionUsage */
DISK_CACHE_STATS  *cacheStats;	/* disk cache statistics */

{
int	ccode;		    /* error number */
register int loopCount;
WORD	oldConnectionID;    /* the preferred connneciton ID to restore */
BYTE	sendPacket[3];
BYTE	replyPacket[sizeof(DISK_CACHE_STATS)+2];

    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );
    *((WORD *)sendPacket) = 1;
    sendPacket[2] = 0xD6;
    *((WORD *)replyPacket) = sizeof(DISK_CACHE_STATS);
    if ((ccode = _ShellRequest((BYTE)0xE3, sendPacket, replyPacket)) == 0) {
	*((long *)(replyPacket+2)) = LongSwap(*((long *)(replyPacket+2)));
	*((int *)(replyPacket+6)) = IntSwap(*((int *)(replyPacket+6)));
	*((int *)(replyPacket+8)) = IntSwap(*((int *)(replyPacket+8)));
	*((int *)(replyPacket+10)) = IntSwap(*((int *)(replyPacket+10)));
	for ( loopCount = 3; loopCount < 9; loopCount++ )
	    ((long *)(replyPacket))[loopCount] =
		    LongSwap(((long *)(replyPacket))[loopCount]);
	*((int *)(replyPacket+36)) = IntSwap(*((int *)(replyPacket+36)));
	*((int *)(replyPacket+38)) = IntSwap(*((int *)(replyPacket+38)));
	for ( loopCount = 10; loopCount < 17; loopCount++ )
	    ((long *)(replyPacket))[loopCount] =
		    LongSwap(((long *)(replyPacket))[loopCount]);
	for ( loopCount = 34; loopCount < 40; loopCount++ )
	    ((int *)(replyPacket))[loopCount] =
		    IntSwap(((int *)(replyPacket))[loopCount]);
	memmove( cacheStats, (replyPacket + 2),
		 MIN( structSize, sizeof(DISK_CACHE_STATS) ) );
    }
    SetPreferredConnectionID( oldConnectionID );
    return( ccode );
}
