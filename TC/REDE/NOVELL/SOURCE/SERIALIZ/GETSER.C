/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwlocal.h>
#include <nwmisc.h>


/*_______________________ GetNetworkSerialNumber ________________________
|
| Output:     0  --  SUCCESSFUL
|	    255  --  UNSUCCESSFUL
|
| Comments:
|   This function returns the serial number and application number of
|   the network that the application is running on.
|________________________________________________________________________*/

int GetNetworkSerialNumber(networkSerialNumber, applicationNumber)

long *networkSerialNumber;  /* Receives the network's serial number      */
int *applicationNumber;     /* Receives the network's application number */
{
BYTE	sendPacket[3], receivePacket[8];
int	ccode;

    sendPacket[2] = 18;
    *((int *)sendPacket) = 1;
    *((int *)receivePacket) = 6;

    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
    if (ccode)
	return (ccode);

    *networkSerialNumber = LongSwap(*((long *)(receivePacket + 2)));
    *applicationNumber = IntSwap(*((int *)(receivePacket + 6)));

    return (0);
}
