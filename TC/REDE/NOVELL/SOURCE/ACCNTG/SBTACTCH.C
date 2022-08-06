/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwacct.h>
#include <nwlocal.h>
#include <nwmisc.h>

#define  MIN( a, b )    ( ( a ) < ( b ) ) ? ( a ) : ( b )

/*____________________________SubmitAccountCharge ___________________________
|
|   Output:		      0  --  SUCCESSFUL
|	     Network Error Code  --  UNSUCCESSFUL
|
|  Comments:  This function charges a bindery object's account.
|___________________________________________________________________________*/


int SubmitAccountCharge(binderyObjectType, binderyObjectName, serviceType,
			chargeAmount, cancelHoldAmount, commentType, comment)

WORD	    binderyObjectType; /* type of object to get status of	          */
char	   *binderyObjectName; /* name of object to get status of	          */
WORD	    serviceType;	     /* type of service for audit file	             */
long	    chargeAmount;	     /* amount of charge (usually in cents)         */
long	    cancelHoldAmount;  /* hold amount placed against account          */
WORD	    commentType;	     /* type of comment for audit file	             */
char	   *comment;		     /* comment for audit file -  256 char including
                                NULL terminator                             */
{
BYTE	sendPacket[321];
BYTE	receivePacket[2];
BYTE	ObjNameLen;
BYTE	CommentLen;

    ObjNameLen = (BYTE)(strlen(binderyObjectName));
    *(sendPacket + 17) =  ObjNameLen;
    CommentLen = MIN( (BYTE)(strlen(comment)), 255 );
    *(sendPacket + 18 + ObjNameLen) = CommentLen;
    *((WORD *)sendPacket) = 17 + ObjNameLen + CommentLen;
    sendPacket[2] = 0x97;
    *((WORD *)(sendPacket + 3)) = IntSwap(serviceType);
    *((long *)(sendPacket + 5)) = LongSwap(chargeAmount);
    *((long *)(sendPacket + 9)) = LongSwap(cancelHoldAmount);
    *((WORD *)(sendPacket + 13)) = IntSwap(binderyObjectType);
    *((WORD *)(sendPacket + 15)) = IntSwap(commentType);
    memmove( (char *)(sendPacket + 18), binderyObjectName, ObjNameLen);
    memmove( (char *)(sendPacket + 19 + ObjNameLen), comment, CommentLen);
    return( _ShellRequest((BYTE)0xE3, sendPacket, receivePacket) );
}
