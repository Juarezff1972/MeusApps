/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include "nwconsol.h"
#include "nwlocal.h"


int SetFileServerDateAndTime(year, month, day, hour, minute, second)
WORD year, month, day, hour, minute, second;
{
BYTE requestPacket[9], replyPacket[2];

    *((WORD *)requestPacket) = 7;
    requestPacket[2] = (BYTE)202;

    if (year > 1899 && year < 2000)
        year -= 1900;
    else if (year > 1999)
        year -= 2000;

    requestPacket[3] = (BYTE)year;
    requestPacket[4] = (BYTE)month;
    requestPacket[5] = (BYTE)day;
    requestPacket[6] = (BYTE)hour;
    requestPacket[7] = (BYTE)minute;
    requestPacket[8] = (BYTE)second;

    *((WORD *)replyPacket) = 0;

    return (_ShellRequest((BYTE)227, requestPacket, replyPacket));
}
