/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwconsol.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>


int GetFileServerLoginStatus(loginEnabledFlag)
int *loginEnabledFlag;
{
BYTE    requestPacket[3];
BYTE    replyPacket[3];
int     ccode;

    *((int *)requestPacket) = 1;
    requestPacket[2] = 205;
    *((int *)replyPacket) = 1;

    if ( (ccode = _ShellRequest((BYTE)227, requestPacket, replyPacket)) == 0 )
        *loginEnabledFlag = (replyPacket[2] != (BYTE)0);

    return(ccode);
}
