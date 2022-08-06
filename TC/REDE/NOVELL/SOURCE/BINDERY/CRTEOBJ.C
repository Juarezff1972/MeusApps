/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwbindry.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <niterror.h>


/*_____________________ CreateBinderyObject __________________________________
|                                            									     |
| Output:   --	0                  if SUCCESSFUL 	                          |
|              bindery error code if UNSUCCESSFUL                            |
| Comments:								                                            |
|   This routine will create a new object in the file server bindery.  The   |
|   objectName and objectType must uniquely identify the bindery object and  |
|   may not contain wildcard specifiers. Only a SUPERVISOR or bindery object |
|   that is security equivalent to SUPERVISOR can create bindery objects.    |
|____________________________________________________________________________*/

int CreateBinderyObject(objectName, objectType, objectFlag, objectSecurity)

char *objectName;     /* Null terminated string containing the name of the
		               	 new bindery object (max 48 characters INCLUDING null */
WORD  objectType;     /* Type of the new bindery object 		      */
BYTE  objectFlag;     /* Indicates if the new bindery object is going to be
               			 dynamic or static (0 = STATIC, 1 = DYNAMIC)	      */
BYTE  objectSecurity; /* Indicates the read and write access of others to the
                			 new bindery object				      */
{
    BYTE sendPacket[55], receivePacket[2];
    int len;


    if ( strlen( objectName) > 47 )
	 return(INVALID_NAME);

    sendPacket[2] = 50;
    sendPacket[3] = objectFlag;
    sendPacket[4] = objectSecurity;
    *((int *)(sendPacket + 5)) = IntSwap(objectType);
    len = strlen(objectName);
    sendPacket[7] = len;
    memmove(sendPacket + 8, objectName, len);

    *((int *)sendPacket) = len + 6;
    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)227, sendPacket, receivePacket));
}
