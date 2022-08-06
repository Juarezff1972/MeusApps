/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwconsol.h>
#include <nwlocal.h>


int DisableTransactionTracking()
{
BYTE    requestPacket[3];
BYTE    replyPacket[2];

    *((WORD *)requestPacket) = 1;
    requestPacket[2] = (BYTE)207;
    *((WORD *)replyPacket) = 0;

    return (_ShellRequest((BYTE)227, requestPacket, replyPacket));
}
