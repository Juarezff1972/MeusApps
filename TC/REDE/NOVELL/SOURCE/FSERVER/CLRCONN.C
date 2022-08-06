/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwconsol.h>
#include <nwlocal.h>


/*_________________________________________________________________________
  |
  | Output:       0  --  SUCCESSFUL
  |             198  --  NO_CONSOLE_RIGHTS
  |
  | Comments:   This function clears a connection from the file server.
  |             If the workstation calling this function clears its own
  |             connection, it will no longer be able to communicate with
  |             the file server.  When called, all open files are closed
  |             and the connection's file locks are released.  On a TTS
  |             file server this function causes a connection's transactions
  |             to be aborted.  To call this function the requesting work-
  |             station must have security equivalence of a SUPERVISOR.
  |
  |_________________________________________________________________________*/


int ClearConnectionNumber(connectionNumber)

WORD connectionNumber;      /* Connection number to be cleared     */
{
        BYTE requestPacket[4], replyPacket[2];

        *((int *)requestPacket) = 2;
        requestPacket[2] = (BYTE)210;
        requestPacket[3] = (BYTE)connectionNumber;
        *((int *)replyPacket) = 0;

        return (_ShellRequest((BYTE)227, requestPacket, replyPacket));
}
