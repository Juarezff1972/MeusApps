/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>
#include <nwmisc.h>


/*_____________________ GetFileServerExtendedInfo __________________________
|
| Output:   0			--  SUCCESSFUL
|	    network error code	--  UNSUCCESSFUL
|
| Comments:
|   This function gets extended information about the current preferred
|   file server.  No information needs to be passed to this function.  The
|   Accounting, VAP, Queing, Print Server, and Virtual Console versions and
|   the Security Restriction Level and Internet Bridge Support are returned.
|____________________________________________________________________________*/

int GetFileServerExtendedInfo(accountingVersion, VAPVersion, queuingVersion,
			      printServerVersion, virtualConsoleVersion,
			      securityRestrictionsLevel, internetBridgeSupport)

BYTE *accountingVersion, *VAPVersion, *queuingVersion;
BYTE *printServerVersion, *virtualConsoleVersion, *securityRestrictionsLevel;
BYTE *internetBridgeSupport;
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
	if (accountingVersion != 0)
	    *accountingVersion = receivePacket[63];
	if (VAPVersion != 0)
	    *VAPVersion = receivePacket[64];
	if (queuingVersion != 0)
	    *queuingVersion = receivePacket[65];
	if (printServerVersion != 0)
	    *printServerVersion = receivePacket[66];
	if (virtualConsoleVersion != 0)
	    *virtualConsoleVersion = receivePacket[67];
	if (securityRestrictionsLevel != 0)
	    *securityRestrictionsLevel = receivePacket[68];
	if (internetBridgeSupport != 0)
	    *internetBridgeSupport = receivePacket[69];
     }
    return (ccode);
}
