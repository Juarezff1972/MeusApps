/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>


/*__________________________________________________________________________
  |
  |  Output:			 0  --	SUCCESSFUL
  |		Network Error code  --	UNSUCCESSFUL
  |
  |  Comments:	This function returns miscellaneous information about the
  |		file server whose connection ID is specified.  The calling
  |		workstation can specify the amount of information to receive
  |		in structSize, if all information is not needed.
  |________________________________________________________________________*/


#define MIN(a,b)	    ((a) < (b) ? (a) : (b))

int GetFileServerMiscInformation( connectionID, structSize, miscInformation )

WORD	connectionID;	   /* Workstation's connection ID of server to get
			      information on				    */
int	structSize;	   /* Number of bytes to return in connectionUsage  */
SERVER_MISC_INFO *miscInformation;  /* Receives miscellaneous information   */

{
int	ccode;
int	loopCount;
WORD	oldConnectionID;
BYTE	sendPacket[3];
BYTE	replyPacket[58];


    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );
    *((WORD *)sendPacket) = 1;
    sendPacket[2] = 0xE8;
    *((WORD *)replyPacket) = 56;

    ccode = _ShellRequest((BYTE)0xE3, sendPacket, replyPacket);

    if (ccode == 0)
     {
	*((long *)(replyPacket+2)) = LongSwap(*((long *)(replyPacket+2)));
	for ( loopCount = 5; loopCount < 11; loopCount++ )
	    ((int *)(replyPacket))[ loopCount ] =
		    IntSwap(((int *)(replyPacket))[ loopCount ]);
	for ( loopCount = 0; loopCount < 9; loopCount++ )
	    ((long *)(replyPacket + 22))[loopCount] =
		    LongSwap(((long *)(replyPacket + 22))[loopCount]);
	memmove( miscInformation, (replyPacket + 2), MIN( structSize, 56 ) );
     }

    SetPreferredConnectionID( oldConnectionID );
    return( ccode );
}
