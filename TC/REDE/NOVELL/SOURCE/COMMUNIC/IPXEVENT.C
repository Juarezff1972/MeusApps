/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nxt.h>


void IPXScheduleIPXEvent(timeDelay, eventControlBlock)
WORD   timeDelay;
ECB   *eventControlBlock;
{

       _IPXrequest(timeDelay, eventControlBlock, (WORD)0x05);
}




void IPXScheduleSpecialEvent(timeDelay, eventControlBlock)
WORD   timeDelay;
ECB   *eventControlBlock;
{

       _IPXrequest(timeDelay, eventControlBlock, (WORD)0x07);
}




void IPXDisconnectFromTarget(networkAddress)
BYTE  *networkAddress;
{

       _IPXrequest((WORD)0, (ECB *)networkAddress, (WORD)0x0B);
}




void IPXGetInternetworkAddress(networkAddress)
BYTE  *networkAddress;
{

       _IPXrequest((WORD)0, (ECB *)networkAddress, (WORD)0x09);
}
