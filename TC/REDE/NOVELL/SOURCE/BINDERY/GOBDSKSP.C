/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwbindry.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>
#include <nwconsol.h>


int GetBinderyObjectDiskSpaceLeft( connectionID, binderyObjectID,
                                   systemElapsedTime, unusedDiskBlocks, 
                                   restrictionEnforced)

WORD	connectionID;	       /* the conneciton ID of the server for which
             			          to get information                           */
long	binderyObjectID;      /* the ID of the bindery object to return
			                      information on                               */
long   *systemElapsedTime;  
long   *unusedDiskBlocks;   /* the number of disk blocks the object has left*/
int    *restrictionEnforced;/* TRUE if maximum disk space is enforced for the
                			       bindery object by the file server.           */

{
int	ccode;
WORD	oldConnectionID;
BYTE	sendPacket[7];
BYTE	replyPacket[16];



    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );
    *((WORD *)sendPacket) = 5;
    sendPacket[2] = 0xE6;
    *((long *)(sendPacket + 3)) = LongSwap( binderyObjectID );
    *((WORD *)replyPacket) = 14;

    ccode = _ShellRequest((BYTE)0xE3, sendPacket, replyPacket);

    if (ccode == 0)
     {
	   if (systemElapsedTime)
	      *systemElapsedTime = LongSwap(*((long *)(replyPacket + 2)));
	   if (unusedDiskBlocks)
	      *unusedDiskBlocks = LongSwap(*((long *)(replyPacket + 10)));
	   if (restrictionEnforced)
	      *restrictionEnforced =  (WORD)replyPacket[14];
     }
    SetPreferredConnectionID( oldConnectionID );
    return( ccode );
}
