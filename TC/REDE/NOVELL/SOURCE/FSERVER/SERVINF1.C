/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>
#include <nwmisc.h>


/*________________________ GetFileServerInformation __________________________
|
| Output:   Returns a 0 if successful or a network error code if unsuccessful.
|
| Comments:
|   This function returns information about the current preferred file server.
|   No information needs to be passed to this function.  The server's name,
|   major version number (1 - 255), minor version number (0 - 99), maximum
|   connections supported, current connections in use, and maximum volumes
|   supported are returned.  Also returned is the revision level, System Fault
|   Tolerance level, Transaction Tracking System level, and the peak connections
|   used.
|____________________________________________________________________________*/

int GetFileServerInformation(serverName, netwareVersion, netwareSubVersion,
			     maximumConnectionsSupported, connectionsInUse,
			     maximumVolumesSupported, revisionLevel, SFTLevel,
			     TTSLevel, peakConnectionsUsed)

char *serverName;
BYTE *netwareVersion, *netwareSubVersion, *revisionLevel, *SFTLevel;
BYTE *TTSLevel;
WORD *maximumConnectionsSupported, *connectionsInUse;
WORD *maximumVolumesSupported, *peakConnectionsUsed;
{
BYTE sendPacket[3], receivePacket[130];
int ccode;



    sendPacket[2] = 17;
    *((int *)sendPacket) = 1;

    *((int *)receivePacket) = 128;
    memset(receivePacket + 2, 0, 128);

    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
    if (!ccode)
     {
	if (serverName != (char *)0)
	    strcpy(serverName, (char *)(receivePacket + 2));

	if (netwareVersion != 0)
	    *netwareVersion = receivePacket[50];

	if (netwareSubVersion != 0)
	    *netwareSubVersion = receivePacket[51];

	if (maximumConnectionsSupported != 0)
	    *maximumConnectionsSupported = IntSwap(*((int *)(receivePacket + 52)));

	if (connectionsInUse != 0)
	    *connectionsInUse = IntSwap(*((int *)(receivePacket + 54)));

	if (maximumVolumesSupported != 0)
	    *maximumVolumesSupported = IntSwap(*((int *)(receivePacket + 56)));

	if (revisionLevel != 0)
	    *revisionLevel = receivePacket[58];

	if (SFTLevel != 0)
	    *SFTLevel = receivePacket[59];

	if (TTSLevel != 0)
	    *TTSLevel = receivePacket[60];

	if (peakConnectionsUsed != (WORD *)NULL)
	    *peakConnectionsUsed = IntSwap(*((int *)(receivePacket + 61)));
     }
    return (ccode);
}
