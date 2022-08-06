/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwconsol.h>
#include <nwlocal.h>


/*___________________________________________________________________________
  |
  |  Output:     0  --  SUCCESSFUL
  |            198  --  NO_CONSOLE_RIGHTS
  |
  |  Comments:  This function enables logins to a file server.  The requesting
  |             workstation MUST have Console Operator Rights to use this
  |             utility.
  |__________________________________________________________________________*/


int EnableFileServerLogin()
{
int     ccode;
BYTE    requestPacket[3], replyPacket[2];

    *((int *)requestPacket) = 1;
    requestPacket[2] = (BYTE)204;
    *((int *)replyPacket) = 0;

    ccode = _ShellRequest((BYTE)227, requestPacket, replyPacket);
    return (ccode);
}
