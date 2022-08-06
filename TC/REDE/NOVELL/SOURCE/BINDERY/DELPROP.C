/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwbindry.h>
#include <nwlocal.h>
#include <nwmisc.h>


/*_________________________ DeleteProperty __________________________________
|
| Output:    0			            --  SUCCESSFUL
|            bindery error code  --  UNSUCCESSFUL
|
| Comments:
|   This routine deletes a property from an object in the bindery.  ObjectName
|   and objectType can not contain wild card specifiers.  PropertyName can
|   contain wild cards and if it does all matching properties will be deleted.
|   Only properties that the user has the security access to can be deleted by
|   the user.
|____________________________________________________________________________*/

int DeleteProperty(objectName, objectType, propertyName)

char *objectName;     /* Bindery name of the object		   	     */
char *propertyName;   /* Name of the property to be deleted		     */
WORD  objectType;     /* Type of the bindery object	   		     */
{
BYTE	sendPacket[69], receivePacket[2];
int	offset, len, proplen;

    sendPacket[2] = 58;
    *((int *)(sendPacket + 3)) = IntSwap(objectType);
    len = strlen(objectName);
    sendPacket[5] = len;
    memmove(sendPacket + 6, objectName, len);
    offset = len + 6;
    proplen = strlen(propertyName);
    sendPacket[offset++] = proplen;
    memmove(sendPacket + offset, propertyName, proplen);

    *((int *)sendPacket) = len + proplen + 5;
    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)227, sendPacket, receivePacket));
}
