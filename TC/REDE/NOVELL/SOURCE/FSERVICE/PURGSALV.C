/*	(c) COPYRIGHT 1988 - 1990 Novell, Inc.  All Rights Reserved.   */
#include <nwfile.h>
#include <nwlocal.h>



/*__________________________________________________________________________
  |
  |   Output:      0  --  SUCCESSFUL
  |              198  --  NO_CONSOLE_RIGHTS
  |
  |   Comments:
  |       This function permanently deletes all files that are marked for
  |       deletion.  The requesting workstation must have console operator
  |       rights.
  |_______________________________________________________________________*/

int PurgeAllErasedFiles()
{
    BYTE requestPacket[3], replyPacket[2];

    *((int *)requestPacket) = 1;
    requestPacket[2] = (BYTE)206;
    *((int *)replyPacket) = 0;

    return (_ShellRequest((BYTE)227, requestPacket, replyPacket));
}
