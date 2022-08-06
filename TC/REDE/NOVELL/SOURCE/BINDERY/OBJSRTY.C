/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwbindry.h>
#include <nwlocal.h>
#include <nwmisc.h>


/*___________________ ChangeBinderyObjectSecurity ____________________________
|
| Output:   0	            		--  SUCCESSFUL
|           Bindery error code	--  UNSUCCESSFUL
|
| Comments:
|   This routine changes the access flags of an object in the bindery.	The
|   object name and type may not contain any wild cards.  The newObjectSecurity
|   byte should be set with the desired new read and write securitys.  Only
|   a supervisor can change an objects accessFlags.
|____________________________________________________________________________*/

int ChangeBinderyObjectSecurity(objectName, objectType, newObjectSecurity)

char *objectName;	         /* Bindery name of the object		                */
WORD  objectType;	         /* Bindery type of the object                    */
BYTE newObjectSecurity;    /* Indicates the read-write access of others
               			      to the bindery object	                      */
{
    BYTE sendPacket[54], receivePacket[2];
    int  len, ccode;

    sendPacket[2] = 56;
    sendPacket[3] = newObjectSecurity;
    *((int *)(sendPacket + 4)) = IntSwap(objectType);
    len = strlen(objectName);
    sendPacket[6] = len;
    memmove(sendPacket + 7, objectName, len);

    *((int *)sendPacket) = len + 5;
    *((int *)receivePacket) = 0;

    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
    return(ccode);
}
