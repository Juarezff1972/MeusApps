/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwlocal.h>
#include <nwmisc.h>



/*__________________ VerifyNetworkSerialNumber ____________________________
|
| Output:	0      --  Serial Number is correct
|	     NON-ZERO  --  INCORRECT
|
| Comments:
|   This function allows a client to verify that a given number matches a
|   network's serial number.  If the network serial number is correct, the
|   corresponding application number will be returned.	If the network serial
|   is incorrect, the function will return a non-zero completion code and
|   execute a logout of the calling station.
|____________________________________________________________________________*/

int VerifyNetworkSerialNumber(networkSerialNumber, applicationNumber)
long networkSerialNumber;  /* Serial number of the network that the application
			      should be running on			      */
int *applicationNumber;    /* Receives the application number		      */
{
BYTE sendPacket[7], receivePacket[4];
int ccode;

    sendPacket[2] = 12;
    *((long *)(sendPacket + 3)) = LongSwap(networkSerialNumber);
    *((int *)sendPacket) = 5;

    *((int *)receivePacket) = 2;

    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
    if (ccode)
	return (ccode);

    *applicationNumber = IntSwap(*((int *)(receivePacket + 2)));
    return (0);
}
