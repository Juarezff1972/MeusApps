/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwbindry.h>
#include <nwlocal.h>
#include <nwmisc.h>


/*____________________ WritePropertyValue __________________________________
|
| Outputs:   0			           --  SUCCESSFUL
|	          bindery error code --  UNSUCCESSFUL
|
| Comments:
|   This function writes a value to a property of type ITEM.  ObjectName,
|   objectType, and propertyName can not contain wild card specifiers.
|   To write property data which is more than one segment (128
|   bytes) long, this procedure should be repeatedly called with the
|   moreSegments flag set to 0xFF.  The first call should be made with
|   segment set to 1.	Segment should be incremented by 1 for each subsequent
|   call.   On the last call to WriteProperty the moreSegments flag should
|   be set to 0. (Note that if there is only one segment the moreSegments
|   flag should be set to 0 on the first call). It is recommended that the
|   property values be kept to 128 bytes or less to improve bindery efficiency.
|____________________________________________________________________________*/

int WritePropertyValue( objectName, objectType, propertyName,
		                  segmentNumber, propertyValue, moreSegments )
char *objectName;     /* Name of the bindery object			                */
WORD  objectType;     /* Type of the bindery object			                */
char *propertyName;   /* String containing the name of the property that the
			                data is to be written (max 16 characters INCLUDING
			                null terminator				                         */
int   segmentNumber;  /* Segment number of the data to be written (1 = first
			                segment of the property's value)                   */
BYTE *propertyValue;  /* Contains a segment of the property's value (maximum
			                128 bytes)					                            */
BYTE  moreSegments;   /* Flag indicating whether the property value has more
			                segments after the current segment		             */
{
    BYTE sendPacket[199], receivePacket[2];
    int offset, len, proplen;

    sendPacket[2] = 62;
    *((int *)(sendPacket + 3)) = IntSwap(objectType);
    len = strlen(objectName);
    sendPacket[5] = len;
    memmove(sendPacket + 6, objectName, len);
    offset = len + 6;
    sendPacket[offset++] = (BYTE)segmentNumber;
    sendPacket[offset++] = moreSegments;
    proplen = strlen(propertyName);
    sendPacket[offset++] = proplen;
    memmove(sendPacket + offset, propertyName, proplen);
    offset += proplen;
    memmove(sendPacket + offset, propertyValue, 128);

    *((int *)sendPacket) = len + proplen + 128 + 7;
    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)227, sendPacket, receivePacket));
}
