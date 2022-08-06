/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
/*--------------------------------------------|
 |  Module:     SPXCONN.C                     |
 |                                            |
 |  Functions:  SPXSendSequencedPacket        |
 |              SPXTerminateConnection        |
 |              SPXGetConnectionStatus        |
 |___________________________________________*/

#include <nxt.h>

/*________________________________________________________________________

   Output:  None.

   Comments:  This function prepares the ECB and the SPX header of the
              packet for transmission on the specified connection.  If
              a packet is not currently being sent on the connection then
              this packet's send cycle is initiated.  If a previous packet
              is in the process of being sent then this packet is queued
              awaiting its proper send order.
 _________________________________________________________________________*/

void SPXSendSequencedPacket(connectionNumber, eventControlBlock)
WORD   connectionNumber;
ECB   *eventControlBlock;
{
int    ccode;

       ccode = _SPXrequest(connectionNumber, eventControlBlock, (BYTE)0x16);
}

/*________________________________________________________________________

   Output:  None.

   Comments:  This function sends a packet to the connection partner that
              consists only of an SPX header.  The Datastream Type field
              will contain an 0xFE, indicating that the connection is being
              terminated.  This packet will be reliably delivered to the
              connection partner.  When the remote SPX, acting on behalf of
              its client, has acknowledged this packet, both sides will tear
              down the connection link.
  _______________________________________________________________________*/

void SPXTerminateConnection(connectionNumber, eventControlBlock)
WORD   connectionNumber;
ECB   *eventControlBlock;
{
int    ccode;

       ccode = _SPXrequest(connectionNumber, eventControlBlock, (BYTE)0x13);
}

/*_________________________________________________________________________

   Output:    0 --  SPX_CONNECTION_OK
                     (Connection is currently active and the buffer was filled)
            238 --  SPX_INVALID_CONNECTION
                     (No such connection currently exists)

   Comments:  This function allows a program to check on the current status of
              an SPX connection.  If the connection does exist, then buffer
              will be filled with the status information.
  ________________________________________________________________________*/

int SPXGetConnectionStatus(connectionNumber, connectionInfo)
WORD   connectionNumber;
CONNECTION_INFO  *connectionInfo;
{
int    ccode;

    ccode = _SPXrequest(connectionNumber, (ECB *)connectionInfo, (BYTE)0x15);
    return(ccode);
}
