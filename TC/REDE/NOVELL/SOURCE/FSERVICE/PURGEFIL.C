/*	(c) COPYRIGHT 1988 - 1990 Novell, Inc.  All Rights Reserved.   */
#include <nwfile.h>
#include <nwlocal.h>


/*________________________ PurgeErasedFiles __________________________________
|
| Output:   0		       --  SUCCESSFUL
|	    network error code --  UNSUCCESSFUL
|
| Comments:
|   This function permanently deletes all files that the requesting workstation
|   has marked for deletion.  It is used to free disk space being used by files
|   marked for deletion.
|____________________________________________________________________________*/

int PurgeErasedFiles()
{
BYTE  sendPacket[3], receivePacket[2];

    sendPacket[2] = 16;
    *((int *)sendPacket) = 1;
    *((int *)receivePacket) = 0;
    return (_ShellRequest((BYTE)226, sendPacket, receivePacket));
}
