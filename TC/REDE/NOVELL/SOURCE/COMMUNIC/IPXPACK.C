/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
/*--------------------------------------------|
 |  Module:     IPXPACK.C                     |
 |                                            |
 |  Functions:  IPXListenForPacket            |
 |              IPXSendPacket                 |
 |              IPXCancelEvent                |
 |___________________________________________*/

#include <nxt.h>
/*________________________________________________________________________

   Output:  None.

   Comments:  This function listens for a packet from IPX.
 _________________________________________________________________________*/

void IPXListenForPacket(eventControlBlock)
ECB  *eventControlBlock;
{
      _IPXPacket(eventControlBlock, (WORD)0x04);
}

/*________________________________________________________________________

   Output:  None.

   Comments:  This function gives a packet to IPX to send.
 _________________________________________________________________________*/

void IPXSendPacket(eventControlBlock)
ECB  *eventControlBlock;
{
      _IPXPacket(eventControlBlock, (WORD)0x03);
}

/*________________________________________________________________________

   Output:  0  --  SUCCESSFUL

   Comments:  This function cancels an IPX event for an ECB in use.
 _________________________________________________________________________*/

int IPXCancelEvent(eventControlBlock)
ECB  *eventControlBlock;
{
int   ccode;

      ccode = _IPXPacket(eventControlBlock, (WORD)0x06);
      return(ccode);
}
