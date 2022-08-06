/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwbindry.h>
#include <nwlocal.h>
#include <nwmisc.h>


/*___________________ RenameBinderyObject ___________________________________
|
| Output:   0			            --  SUCCESSFUL
|	         bindery error code	--  UNSUCCESSFUL
|
| Comments:
|   This function renames a bindery object.  Only a SUPERVISOR or SUPERVISOR
|   equivalent can rename a bindery object.  No wild card specifiers are
|   allowed in the objectName and objectType.
|____________________________________________________________________________*/

int RenameBinderyObject( objectName, newObjectName, objectType )

char *objectName;	    /* Name of the currently defined bindery object
			                (maximum 48 characters INCLUDING null)	          */
char *newObjectName;	 /* Name of the new bindery object		                */
WORD  objectType;	    /* Type of the bindery object			                */
{
BYTE  sendPacket[101], receivePacket[2];
int   offset, len, newlen;

    sendPacket[2] = 52;
    *((int *)(sendPacket + 3)) = IntSwap(objectType);
    len = strlen(objectName);
    sendPacket[5] = len;
    memmove(sendPacket + 6, objectName, len);
    offset = len + 6;
    newlen = strlen(newObjectName);
    sendPacket[offset++] = newlen;
    memmove(sendPacket + offset, newObjectName, newlen);

    *((int *)sendPacket) = len + newlen + 5;
    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)227, sendPacket, receivePacket));
}
