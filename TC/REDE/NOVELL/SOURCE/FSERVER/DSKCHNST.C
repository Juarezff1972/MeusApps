/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>

#define MIN(a,b)	    ((a) < (b) ? (a) : (b))



int GetDiskChannelStats( connectionID, channelNumber, structSize,
			 diskChannelStats )

WORD	connectionID;	 /* Workstation's connection ID of the server to get
			    information on				     */
int	channelNumber;	 /* Channel number to return information about	     */
int	structSize;	 /* Number of bytes to return in connectionUsage     */

DISK_CHANNEL_STATS  *diskChannelStats;	/* disk channel statistics */

{
int	ccode;
WORD	oldConnectionID;
BYTE	sendPacket[4];
BYTE	replyPacket[sizeof(DISK_CHANNEL_STATS)+2];
int	bufferSize = sizeof(DISK_CHANNEL_STATS);


    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );
    *((WORD *)sendPacket) = 2;
    sendPacket[2] = 0xD9;
    sendPacket[3] = (BYTE)channelNumber;
    *((WORD *)replyPacket) = sizeof(DISK_CHANNEL_STATS);

    ccode = _ShellRequest((BYTE)0xE3, sendPacket, replyPacket);
    if (ccode == 0)
     {
       *((long *)(replyPacket+2)) = LongSwap(*((long *)(replyPacket+2)));
       *((int *)(replyPacket+6)) = IntSwap(*((int *)(replyPacket+6)));
       *((int *)(replyPacket+8)) = IntSwap(*((int *)(replyPacket+8)));
       *((int *)(replyPacket+78)) = IntSwap(*((int *)(replyPacket+78)));
       *((int *)(replyPacket+80)) = IntSwap(*((int *)(replyPacket+80)));
       *((int *)(replyPacket+82)) = IntSwap(*((int *)(replyPacket+82)));
       *((int *)(replyPacket+84)) = IntSwap(*((int *)(replyPacket+84)));
       *((int *)(replyPacket+89)) = IntSwap(*((int *)(replyPacket+89)));
       *((int *)(replyPacket+94)) = IntSwap(*((int *)(replyPacket+94)));
       replyPacket[bufferSize + 1] = '\0';
       memmove(diskChannelStats, &replyPacket[2], MIN( structSize, bufferSize));
     }

    SetPreferredConnectionID( oldConnectionID );
    return( ccode );
}
