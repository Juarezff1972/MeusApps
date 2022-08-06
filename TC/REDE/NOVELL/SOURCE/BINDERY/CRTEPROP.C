/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwbindry.h>
#include <nwlocal.h>
#include <nwmisc.h>


/*____________________________ CreateProperty ______________________________
|
| Output:    0	 (0x00) --  SUCCESSFUL
|   	      150 (0x96) --  SERVER_OUT_OF_MEMORY
|	         237 (0xED) --  PROPERTY_ALREADY_EXISTS
|	         239 (0xEF) --  INVALID_NAME
|
| Comments:
|   This function creates a property for a bindery object.  The objectName
|   and ObjectType must uniquely identify the bindery object and may not
|   contain  wildcard qualifiers.
|_____________________________________________________________________________*/

int CreateProperty( objectName, objectType, propertyName,
			           propertyFlags, propertySecurity )

char *objectName;    	/* Bindery object name (max 48 char. INCLUDING null)*/
char *propertyName;   	/* Property name to be created (max 16 characters
			                  INCLUDING the null terminator)		             */
WORD objectType;	      /* Bindery object type (i.e. OT_USER, OT_GROUP...)  */
BYTE propertyFlags;	   /* Dynamic-Static property flag indicator	          */
BYTE propertySecurity;	/* Indicates read-write access of others to the new
               			   property			                                  */
{
    BYTE sendPacket[71], receivePacket[2];
    int offset, objectlen, propertylen, ccode;

    sendPacket[2] = 57;
    *((int *)(sendPacket + 3)) = IntSwap(objectType);

    objectlen = (BYTE)strlen(objectName);
    sendPacket[5] = (char)objectlen;
    memmove(sendPacket + 6, objectName, objectlen);

    offset = objectlen + 6;
    sendPacket[offset++] = propertyFlags;
    sendPacket[offset++] = propertySecurity;

    propertylen = (BYTE)strlen(propertyName);
    sendPacket[offset++] = (char)propertylen;
    memmove((sendPacket + offset), propertyName, propertylen);

    *((int *)sendPacket) = propertylen + objectlen + 7;
    *((int *)receivePacket) = 0;

    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
    return(ccode);
}
