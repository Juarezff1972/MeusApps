/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwconsol.h>
#include <nwlocal.h>


/*____________________________________________________________________________
  |
  |  Output:    0  --  SUCCESSFUL
  |           198  --  NO_CONSOLE_RIGHTS
  |
  |  Comments:  This function disables all logins to a file server.  It allows
  |             a console operator to restrict new accesses to a file server.
  |             If the requesting workstation loses its connection to the file
  |             server and there are no other objects logged in with console
  |             operator rights, then the file server has to be brought down
  |             from the console physically connected to the file server and
  |             rebooted before new objects can login.  You MUST have console
  |             operator rights to use this call.
  |___________________________________________________________________________*/

int DisableFileServerLogin()
{
int     ccode;
BYTE    requestPacket[3], replyPacket[2];

    *((int *)requestPacket) = 1;
    requestPacket[2] = 203;
    *((int *)replyPacket) = 0;

    ccode = _ShellRequest((BYTE)227, requestPacket, replyPacket);
    return (ccode);

}
