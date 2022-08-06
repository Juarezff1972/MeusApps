/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwbindry.h>
#include <nwlocal.h>
#include <nwmisc.h>

#define MIN( a, b )   ( (a) < (b) ? (a) : (b) )

/*_______________________ ReadPropertyValue ______________________________
|
| Output:   0		             --  SUCCESSFUL
|	         bindery error code --  UNSUCCESSFUL
|
| Comments:
|   This function returns the value of a bindery object's property.  The
|   objectName, objectType, and propertyName cannot contain wild card
|   specifiers.  To get property data which is more than one segment
|   (128 bytes) long this procedure should be repeatedly called until
|   the moreSegments flag returns false.  On the first call, segment should
|   be set to 1, on each successive call, segment should be incremented by
|   one.  If bit 0 of the propertyFlags is set, then the property is dynamic,
|   if bit 0 is clear, the property is static.	If bit 1 of propertyFlags is
|   set, the property is a set, if bit 1 is clear, the property is an item.
|_____________________________________________________________________________*/

int ReadPropertyValue( objectName, objectType, propertyName, segmentNumber,
		                 propertyValue, moreSegments, propertyFlags )
char *objectName;	     /* Name of the bindery object 		                   */
WORD  objectType;	     /* Type of the bindery object 		                   */
char *propertyName;	  /* Name of the property			                      */
int   segmentNumber;	  /* Segment number of data to be read		             */
BYTE *propertyValue;	  /* Receives a buffer containing a segment of
			                 property's value (128 bytes)                      */
BYTE *moreSegments;	  /* Receives a flag indicating if the property value
			                 has more data segments after the current segment  */
BYTE *propertyFlags;	  /* Receives the property flags of the property       */
{
    BYTE sendPacket[70], receivePacket[132];
    int ccode, offset, len, proplen;

    sendPacket[2] = 61;
    *((int *)(sendPacket + 3)) = IntSwap(objectType);
    len = MIN( strlen(objectName), 47 );
    sendPacket[5] = (BYTE)len;
    memmove(sendPacket + 6, objectName, len);
    offset = len + 6;
    sendPacket[offset++] = (BYTE)segmentNumber;
    proplen = MIN( strlen(propertyName), 15 );
    sendPacket[offset++] = (BYTE)proplen;
    memmove(sendPacket + offset, propertyName, proplen);

    *((int *)sendPacket) = len + proplen + 6;
    *((int *)receivePacket) = 130;

    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
    if (ccode)
       return (ccode);

    memmove(propertyValue, receivePacket + 2, 128);
    if( moreSegments != (BYTE *)0 )
	   *moreSegments = receivePacket[130];
    if( propertyFlags != (BYTE *)0 )
	   *propertyFlags = receivePacket[131];

    return (0);
}
