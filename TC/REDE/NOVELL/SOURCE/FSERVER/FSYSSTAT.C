/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>



#define MIN(a,b)	    ((a) < (b) ? (a) : (b))

int GetFileSystemStats( connectionID, structSize, fileSysStats )

WORD	connectionID;	 /* Workstation's connection ID on the server whose
			    statistics are to be returned		     */
int	structSize;	 /* Number of bytes to return in fileSysStats	     */

FILE_SYS_STATS *fileSysStats;  /* Receives the statistics on the file system */
{
int	      bufferSize = sizeof(FILE_SYS_STATS);
int	      ccode;
register int  loopCount;
WORD	      oldConnectionID;	  /* the preferred connneciton ID to restore */
BYTE	      sendPacket[3];
BYTE	      replyPacket[sizeof(FILE_SYS_STATS) + 2];



    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );

    *((WORD *)sendPacket) = 1;
    sendPacket[2] = 0xD4;
    *((WORD *)replyPacket) = bufferSize;

    ccode = _ShellRequest((BYTE)0xE3, sendPacket, replyPacket);
    if (ccode)
       return(ccode);

    *((long *)(replyPacket+2)) =  LongSwap(*((long *)(replyPacket+2)));
    *((int *)(replyPacket+6)) =   IntSwap(*((int *)(replyPacket+6)));
    *((int *)(replyPacket+8)) =   IntSwap(*((int *)(replyPacket+8)));
    *((int *)(replyPacket+10)) =  IntSwap(*((int *)(replyPacket+10)));
    *((long *)(replyPacket+12)) = LongSwap(*((long *)(replyPacket+12)));
    *((long *)(replyPacket+16)) = LongSwap(*((long *)(replyPacket+16)));
    *((long *)(replyPacket+20)) = LongSwap(*((long *)(replyPacket+20)));
    *((int *)(replyPacket+24)) =  IntSwap(*((int *)(replyPacket+24)));
    *((long *)(replyPacket+26)) = LongSwap(*((long *)(replyPacket+26)));

    for ( loopCount = 30; loopCount < 38; loopCount+=2 )
       *((int *)(replyPacket + loopCount)) =
			    IntSwap(*((int *)(replyPacket + loopCount)));

    memmove(fileSysStats, &replyPacket[2], MIN(structSize, bufferSize));

    SetPreferredConnectionID( oldConnectionID );
    return( ccode );
}
