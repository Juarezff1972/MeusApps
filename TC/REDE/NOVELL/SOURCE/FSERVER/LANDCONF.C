/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>
#include <nwwrkenv.h>


#define MIN(a,b)	    ((a) < (b) ? (a) : (b))

int GetLANDriverConfigInfo( connectionID, LANBoardNumber, structSize,
			    LANConfiguration )

WORD	connectionID;	      /* Workstation's connection ID of the server
				 to get information on			      */
int	structSize;	      /* Number of bytes to return in connectionUsage */
BYTE	LANBoardNumber;       /* LAN board number to return info on (0-3)     */
LAN_CONFIG *LANConfiguration; /* Receives the LANDriverConfiguration info     */

{
int	ccode;
int	firstStrLen;
WORD	oldConnectionID;
BYTE	replyPacket[174];
int	secondStrLen;
BYTE	sendPacket[4];
char	tempString[80];

    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );
    *((WORD *)sendPacket) = 2;
    sendPacket[2] = 0xE3;
    sendPacket[3] = LANBoardNumber;
    *((WORD *)replyPacket) = 172;
    if ((ccode = _ShellRequest((BYTE)0xE3, sendPacket, replyPacket)) == 0) {
	firstStrLen = strlen((char *)(replyPacket + 14));
	secondStrLen = MIN(strlen( (char *)(replyPacket + 15 + firstStrLen) ),
			    79);
	memmove( tempString, (char *)(replyPacket + 15 + firstStrLen ),
		 secondStrLen );
	tempString[ secondStrLen ] = '\0';
	*(replyPacket + 14 + MIN(firstStrLen,79) ) = '\0';
	strcpy( (char *)(replyPacket + 94), tempString );
	*(replyPacket + MIN( structSize, sizeof(LAN_CONFIG) ) - 1 ) = '\0';
	memmove( (char *)LANConfiguration, (char *)(replyPacket + 2),
		    MIN( structSize, sizeof(LAN_CONFIG) ) );
    }
    SetPreferredConnectionID( oldConnectionID );
    return( ccode );
}
