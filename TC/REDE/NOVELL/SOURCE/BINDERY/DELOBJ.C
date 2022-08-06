/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwbindry.h>
#include <nwlocal.h>
#include <nwmisc.h>


/*_____________________ DeleteBinderyObject __________________________________
|
| Output:    0			           --  SUCCESSFUL
|            bindery error code --  UNSUCCESSFUL
|
| Comments:
|   This routine deletes an object from the bindery.  ObjectName and objectType
|   can not contain wild card specifiers.  Only SUPERVISOR or SUPERVISOR
|   equivalent can delete an object from the bindery.
|_____________________________________________________________________________*/

int DeleteBinderyObject(objectName, objectType)

char *objectName;   /* Name of the bindery object			     */
WORD  objectType;   /* Type of the bindery object			     */
{
    BYTE  sendPacket[53], receivePacket[2];
    int   len;

    sendPacket[2] = 51;
    *((int *)(sendPacket + 3)) = IntSwap(objectType);
    len = strlen(objectName);
    sendPacket[5] = len;
    memmove(sendPacket + 6, objectName, len);

    *((int *)sendPacket) = len + 4;
    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)227, sendPacket, receivePacket));
}
