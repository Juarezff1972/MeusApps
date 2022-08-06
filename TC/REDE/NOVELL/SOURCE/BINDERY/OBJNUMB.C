/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwbindry.h>
#include <nwlocal.h>
#include <nwmisc.h>

/*________________________ GetBinderyObjectID _______________________________
|
| Output:   0           			--  SUCCESSFUL
|           bindery error code	--  UNSUCCESSFUL
|
| Comments:
|   This function returns the unique bindery object's identification number.
|   ObjectName and objectType must be unique and cannot contain wild card
|   specifiers.  The requesting workstation must be logged into the file
|   server and have READ access to the bindery object.	The object ID number
|   returned is good only on the file server that the call was made to.
|____________________________________________________________________________*/

int GetBinderyObjectID(objectName, objectType, objectID)

char *objectName;   /* String containing the name of the bindery object     */
WORD  objectType;   /* Type of the bindery object			                   */
long *objectID;     /* Receives the unique bindery object ID		          */
{
BYTE	sendPacket[53], receivePacket[56];
int	ccode, len;

    sendPacket[2] = 53;
    *((int *)(sendPacket + 3)) = IntSwap(objectType);
    len = strlen(objectName);
    sendPacket[5] = len;
    memmove(sendPacket + 6, objectName, len);
    *((int *)sendPacket) = len + 4;
    *((int *)receivePacket) = 54;

    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
    if (ccode)
       return (ccode);

    *objectID = LongSwap(*((long *)(receivePacket + 2)));
    return (0);
}
