/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwbindry.h>
#include <nwlocal.h>
#include <nwmisc.h>

#define     RECEIVE_PACKET_SIZE     28

/* Note that the receive packet should really be 26 but must be 28 until
   a copy of the bindery fix is installed on the network */

/*___________________________ ScanProperty ___________________________________
|
| Output:    0			           --  SUCCESSFUL
|	          bindery error code --  UNSUCCESSFUL.
|
| Comments:
|   This function is used iteratively to scan the bindery for all properties
|   of the bindery object that match the searchPropertyName.  SequenceNumber
|   should be set to -1 for the first search.  On return, the moreProperties
|   flag will be set if the matched property is not the last property.	If it
|   is not, the sequenceNumber receives a number to be used as the sequence-
|   Number for the next call.
|____________________________________________________________________________*/

int ScanProperty( objectName, objectType, searchPropertyName, sequenceNumber,
		            propertyName, propertyFlags, propertySecurity,
		            propertyHasValue, moreProperties )

char *objectName;	       /* String containing the name of the bindery object 
			                   to search (max 48 characters INCLUDING null the 
			                   NULL terminator) 				                   */
WORD  objectType;	       /* Type of the bindery object (i.e. OT_USER)	      */
char *searchPropertyName;/* String containing the name of the propery to search
			                   for (may contain wildcard characters) (maximum of
			                   16 characters INCLUDING the null)		          */
long *sequenceNumber;	 /* Contains the sequence number from the previous
			                   search (initial search requires a -1) and receives
			                   the sequenceNumber for the matching property      */
char *propertyName;	    /* Receives a string containing the matching property
			                   (maximum 16 characters INCLUDING null terminator*/
char *propertyFlags;	    /* Receives the property flags of the matching
			                   property					                         */
char *propertySecurity;  /* Receives the READ/WRITE security of the matching
			                   property					                         */
char *propertyHasValue;  /* Receives a flag which indicates if the property
			                   has an attached value that may be read	       */
char *moreProperties;	 /* Receives a flag which indicates if the bindery
			                   object has more properties			             */
{
    BYTE sendPacket[73], receivePacket[RECEIVE_PACKET_SIZE];
    int offset, ccode, len, searchlen;

    sendPacket[2] = 60;
    *((int *)(sendPacket + 3)) = IntSwap(objectType);
    len = strlen(objectName);
    sendPacket[5] = len;
    memmove(sendPacket + 6, objectName, len);
    offset = len + 6;
    *((long *)(sendPacket + offset)) = *sequenceNumber;
	/* Note that a LongSwap(-1L) equals -1L on get first sequence number */
    offset += 4;
    searchlen = strlen(searchPropertyName);
    sendPacket[offset++] = searchlen;
    memmove(sendPacket + offset, searchPropertyName, searchlen);

    *((int *)sendPacket) = len + searchlen + 9;
    *((int *)receivePacket) = RECEIVE_PACKET_SIZE - 2;

    ccode =  _ShellRequest((BYTE)227, sendPacket, receivePacket);
    if (ccode)
       return(ccode);

    if (propertyName != (char *)0)
	strcpy(propertyName, (receivePacket + 2));
    if (propertyFlags != (char *)0)
	*propertyFlags = receivePacket[18];
    if (propertySecurity != (char *)0)
	*propertySecurity = receivePacket[19];
    *sequenceNumber = *((long *)(receivePacket + 20));
    if (propertyHasValue != (char *)0)
	*propertyHasValue = receivePacket[24];
    if (moreProperties != (char *)0)
	*moreProperties = receivePacket[25];

    return (ccode);
}
