/*   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nitq.h>
#include <nwlocal.h>



/*____________________ RestoreQueueServerRights ____________________________
|
|  Output:    0 		  --  SUCCESSFUL
|	      network error code  --  UNSUCCESSFUL
|
|  Comments:  This function restores a server's own identity after it has
|	      assumed its client's rights.  It may be called only by a queue
|	      server that previously changed its identity with the call -
|	      ChangeServerRightsToClientRights.
|____________________________________________________________________________*/

int RestoreQueueServerRights()
{
BYTE  sendPacket[3], receivePacket[2];

      *((WORD *)sendPacket) = 1;
      sendPacket[2] = 117;
      * ((WORD *)receivePacket) = 0;
      return( _ShellRequest((BYTE)227, sendPacket, receivePacket));
}

