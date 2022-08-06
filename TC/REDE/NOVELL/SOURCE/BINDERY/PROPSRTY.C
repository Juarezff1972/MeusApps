/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwbindry.h>
#include <nwlocal.h>
#include <nwmisc.h>


/*_____________________ ChangePropertySecurity ______________________________
|
| Output:   0	            		--  SUCCESSFUL
|     	   bindery error code	--  UNSUCCESSFUL
|
| Comments:
|   This routine changes the access flags of a property in the bindery.  The
|   object name and type and the property name may not contain any wild cards.
|   The newAccessFlags byte should be set with the desired new read and
|   write securities.
|____________________________________________________________________________*/

int ChangePropertySecurity( objectName, objectType,
                  			 propertyName, newAccessFlags )

char *objectName;	      /* Name of the bindery object			                */
WORD  objectType;	      /* Type of the bindery object			                */
char *propertyName;	   /* Property name whose security is to be changed    */
BYTE  newAccessFlags;	/* Indicates the new READ/WRITE access of others
                			   to the property				                      */
{
    BYTE sendPacket[70], receivePacket[2];
    int offset, len, proplen;

    sendPacket[2] = 59;
    *((int *)(sendPacket + 3)) = IntSwap(objectType);
    len = strlen(objectName);
    sendPacket[5] = len;
    memmove(sendPacket + 6, objectName, len);
    offset = len + 6;
    sendPacket[offset++] = newAccessFlags;
    proplen = strlen(propertyName);
    sendPacket[offset++] = proplen;
    memmove(sendPacket + offset, propertyName, proplen);

    *((int *)sendPacket) = len + proplen + 6;
    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)227, sendPacket, receivePacket));
}
