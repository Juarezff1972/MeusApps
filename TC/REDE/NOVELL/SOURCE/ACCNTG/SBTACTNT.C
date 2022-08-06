/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwacct.h>
#include <nwlocal.h>
#include <nwmisc.h>

#define  MIN( a, b )    ( ( a ) < ( b ) ) ? ( a ) : ( b )

/*____________________________SubmitAccountNote _______________________________
|
|  Output:		     0	--  SUCCESSFUL
|	    Network Error Code	--  UNSUCCESSFUL
|
|
|  Comments:  This function updates the account of a bindery object.
|_____________________________________________________________________________*/

int SubmitAccountNote(binderyObjectType, binderyObjectName, serviceType,
		      commentType, commentLen, comment)
WORD	binderyObjectType;	     /* type of object			*/
char   *binderyObjectName;	     /* name of object			*/
WORD	serviceType;		     /* type of service for audit file	*/
WORD	commentType;		     /* type of comment for audit file	*/
int	commentLen;		     /* length of comment for audit file*/
BYTE   *comment;		     /* comment for audit file		*/
{
BYTE	sendPacket[256];
BYTE	receivePacket[2];
int	nameLen;
int   length;

    nameLen = strlen(binderyObjectName);
    length = MIN( (BYTE)(commentLen), 255 );
    *((WORD *)sendPacket) = (WORD)(10 + nameLen + length);
    sendPacket[9] = (BYTE)(nameLen);
    sendPacket[10 + nameLen] = (BYTE)length;
    sendPacket[2] = 0x99;
    *((WORD *)(sendPacket + 3)) = IntSwap(serviceType);
    *((WORD *)(sendPacket + 5)) = IntSwap(binderyObjectType);
    *((WORD *)(sendPacket + 7)) = IntSwap(commentType);
    memmove( (char *)(sendPacket + 10), binderyObjectName, nameLen);
    memmove( (char *)(sendPacket + 11 + nameLen), comment, length);
    *((WORD *)receivePacket) = 0;
    return( _ShellRequest((BYTE)0xE3, sendPacket, receivePacket));
}
