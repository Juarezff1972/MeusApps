/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>



#define MIN(a,b)	    ((a) < (b) ? (a) : (b))

int GetPhysicalDiskStats( connectionID, physicalDiskNumber, structSize,
			                 physicalDiskStats, driveDefinition )

WORD	connectionID;		   /* Workstation's connection ID of the
				                  server for which to get information       */
BYTE	physicalDiskNumber;  /* Physical disk for which to return info    */
int	structSize;		      /* the number of bytes to return in
				                  connectionUsage			                  */
PHYS_DISK_STATS *physicalDiskStats;
char   *driveDefinition;	/* drive definition string		               */

{
register int ccode;		         /* error number */
register WORD oldConnectionID;	/* the preferred connneciton ID to restore */
BYTE	sendPacket[4];
BYTE	replyPacket[96];

   oldConnectionID = GetPreferredConnectionID();
   SetPreferredConnectionID( connectionID );
   *((WORD *)sendPacket) = 2;
   sendPacket[2] = 0xD8;
   sendPacket[3] = physicalDiskNumber;
   *((WORD *)replyPacket) = 94;
   if( (ccode = _ShellRequest((BYTE)0xE3, sendPacket, replyPacket)) == 0 )
      {
      *((long *)(replyPacket+2)) = LongSwap(*((long *)(replyPacket+2)));
      *((long *)(replyPacket+12)) = LongSwap(*((long *)(replyPacket+12)));
      *((int *)(replyPacket+16)) = IntSwap(*((int *)(replyPacket+16)));
      if (driveDefinition)
         {
	      memmove( driveDefinition, (char *)(replyPacket+20), 64 );
	      *(driveDefinition + 63) = '\0';
         }
      memmove( (replyPacket+20), (replyPacket+84), 11 );
      *((int *)(replyPacket+20)) = IntSwap(*((int *)(replyPacket+20)));
      *((long *)(replyPacket+22)) = LongSwap(*((long *)(replyPacket+22)));
      *((int *)(replyPacket+26)) = IntSwap(*((int *)(replyPacket+26)));
      *((int *)(replyPacket+28)) = IntSwap(*((int *)(replyPacket+28)));
      memmove( physicalDiskStats, (replyPacket + 2), MIN( structSize, 29) );
      }

   SetPreferredConnectionID( oldConnectionID );
   return( ccode );
}
