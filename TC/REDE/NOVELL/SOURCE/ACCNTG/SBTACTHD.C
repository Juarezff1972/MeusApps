/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwacct.h>
#include <nwlocal.h>
#include <nwmisc.h>

/*_______________________  SubmitAccountHold __________________________________
|
|  Output:		       0  --  SUCCESSFUL
|	      Network Error Code  --  UNSUCCESSFUL
|
|  Comments:  This function places a hold on a bindery object account.
|_____________________________________________________________________________*/

int SubmitAccountHold(binderyObjectType, binderyObjectName, reserveAmount)
WORD	binderyObjectType;  /*	type of object to get status of */
char   *binderyObjectName;  /*	name of object to get status of */
long	reserveAmount;	    /*	amount of hold			*/
{
BYTE	sendPacket[57];
BYTE	receivePacket[2];
int	nameLen;

    nameLen = strlen(binderyObjectName);
    *((WORD *)sendPacket) = 8 + nameLen;
    sendPacket[9] = (BYTE)nameLen;
    sendPacket[2] = 0x98;
    *((long *)(sendPacket + 3)) = LongSwap(reserveAmount);
    *((WORD *)(sendPacket + 7)) = IntSwap(binderyObjectType);
    memmove( (char *)(sendPacket + 10), binderyObjectName, nameLen);
    return( _ShellRequest((BYTE)0xE3, sendPacket, receivePacket) );
}
