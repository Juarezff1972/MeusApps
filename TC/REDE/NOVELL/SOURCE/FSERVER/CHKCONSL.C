/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwconsol.h>
#include <nwlocal.h>


/*_________________________________________________________________________
  |
  | Output:       0  --  SUCCESSFUL  (The object is a console operator)
  |             198  --  NO_CONSOLE_RIGHTS
  |
  | Comments:   This function determines if the object logged in is a
  |             console operator.  To be a console operator, a bindery
  |             object must be a member of the file server's OPERATOR property.
  |
  |_________________________________________________________________________*/


int CheckConsolePrivileges()
{
        BYTE requestPacket[3], replyPacket[2];

        *((int *)requestPacket) = 1;
        requestPacket[2] = (BYTE)200;
        *((int *)replyPacket) = 0;

        return (_ShellRequest((BYTE)227, requestPacket, replyPacket));
}
