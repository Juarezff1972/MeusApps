/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwlocal.h>


int EnableTransactionTracking()
{
BYTE    requestPacket[3], replyPacket[2];

    *((WORD *)requestPacket) = 1;
    requestPacket[2] = (BYTE)208;
    *((WORD *)replyPacket) = 0;

    return (_ShellRequest((BYTE)227, requestPacket, replyPacket));
}
