/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwconn.h>
#include <nwlocal.h>

/*_____________________________ Logout ____________________________________
|
| Output:   None.
|
| Comments:
|   This function logs an object out of all file servers.  After Logout has
|   been executed all of the tables allocated for the user at the file server
|   are cleared.
|____________________________________________________________________________*/


void Logout()
{
BYTE  sendPacket[2], receivePacket[2];
int ccode;

      *((int *)sendPacket) = 0;
      *((int *)receivePacket) = 0;

      ccode = _ShellRequest( (BYTE)215, sendPacket, receivePacket);
}
