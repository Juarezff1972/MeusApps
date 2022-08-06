/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwbindry.h>
#include <nwlocal.h>
#include <nwmisc.h>

/*_________________ GetBinderyAccessLevel ___________________________________
|
| Output:   0		             --  SUCCESSFUL
|           bindery error code --  UNSUCCESSFUL
|
| Comments:
|   This function returns the security access level the requesting workstation
|   has to the bindery.
|____________________________________________________________________________*/

int GetBinderyAccessLevel(accessLevel, myObjectID)
BYTE *accessLevel;   /* Receives the security access level the requesting
               			workstation has to the bindery		                */
long *myObjectID;    /* Receives the unique ID of the logged bindery object */
{
BYTE	sendPacket[3], receivePacket[7];
int	ccode;

    sendPacket[2] = 70;
    *((int *)sendPacket) = 1;
    *((int *)receivePacket) = 5;

    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
    if (ccode)
       return (ccode);

    if (accessLevel != (BYTE *)0)
	*accessLevel = receivePacket[2];
    if (myObjectID != (long *)0)
	*myObjectID = LongSwap(*((long *)(receivePacket + 3)));

    return (0);
}
