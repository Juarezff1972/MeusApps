/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconn.h>
#include <nwlocal.h>

/*_______________________ GetInternetAddress _________________________________
|
| Output:   Returns a 0 if successful or a network error code if unsuccessful.
|
| Comments:
|   This routine will return the internet address of a station or other
|   entity on the network.  This address may then be used to send packets
|   directly to that station.
|____________________________________________________________________________*/

int GetInternetAddress(connectionNumber, networkNumber, physicalNodeAddress,
		       socketNumber)

WORD connectionNumber;	   /* Connection number for which an internet address
			      is requested (1..100)			      */
char *networkNumber;	   /* Receives the network number (4 BYTES)	      */
char *physicalNodeAddress; /* Receives the physical node address (6 BYTES)    */
WORD *socketNumber;	   /* Receives the socket number that the shell uses
			      to communicate with the file server	      */
{
int	ccode;
BYTE	sendPacket[4], receivePacket[14];

    sendPacket[2] = 19;
    sendPacket[3] = (BYTE)connectionNumber;
    *((int *)sendPacket) = 2;

    *((int *)receivePacket) = 12;

    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
    if (ccode)
       return(ccode);

    memmove(networkNumber, receivePacket + 2, 4);
    memmove(physicalNodeAddress, receivePacket + 6, 6);
    memmove(socketNumber, receivePacket + 12, 2);

    return (0);
}
