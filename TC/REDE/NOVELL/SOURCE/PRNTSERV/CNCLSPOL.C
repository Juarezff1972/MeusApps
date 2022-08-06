/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwprint.h>
#include <nwlocal.h>

/*___________________________ CancelCaptureFile ____________________________
  |
  |  Input:    None.
  |
  |  Output:   0 -- successful
  |	       Network Error Code -- unsuccessful
  |
  |  Comments:
  |	  This routine will close the current station's spool capture file and
  |	  delete it.  The file is not given to the networks print queue.
  |___________________________________________________________________________*/

int CancelCaptureFile()
{
    BYTE sendPacket[4], receivePacket[2];

    sendPacket[2] = 1;
    sendPacket[3] = 0xff;
    *((int *)sendPacket) = 2;
    *((int *)receivePacket) = 0;
    return (_ShellRequest((BYTE)0xe0, (BYTE *)sendPacket, (BYTE *)receivePacket));
}
