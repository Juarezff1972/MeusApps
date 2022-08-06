/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <stdio.h>
#include <string.h>
#include <sap.h>
#include <nwmisc.h>

WORD     serverType;
BYTE     socketType;
WORD     queryType;
WORD     sourceSocket;
WORD     bufferSize;
int      TransportTime;

/*-- Declare prototypes for functions inside this code --*/

int InitializeSendPacket( void );
int InitializeReceivePacket( RECEIVE_PACKET * );
int CheckValidityOfResponsePacket( RECEIVE_PACKET *, RECEIVE_PACKET *, SAP *);


int   QueryServices(qType, sType, returnSize, serviceBuffer)

WORD   qType;                   /* Type of query, 1 - General  3 - Nearest  */
WORD   sType;                   /* Type of server, i.e. OT_JOB_SERVER - 5   */
WORD   returnSize;              /* Size in bytes to return in serviceBuffer */
SAP   *serviceBuffer;           /* Receives the query information           */
{
RECEIVE_PACKET   Query, Query2;
int    ccode;


    if (qType == 1)
        return(NOT_SUPPORTED);
    if (qType != 3)
        return(INVALID_QUERY_TYPE);

    queryType = qType;
    serverType = sType;
    sourceSocket = 0x00;
    socketType = SHORT_LIVED;
    bufferSize = returnSize;

    IPXInitialize();
    ccode = IPXOpenSocket( (BYTE *)&sourceSocket, SHORT_LIVED );
    if (ccode)
       return(ccode);

    InitializeReceivePacket(&Query);
    IPXListenForPacket(&Query.theECB);

    InitializeReceivePacket(&Query2);
    IPXListenForPacket(&Query2.theECB);

    InitializeSendPacket();

    ccode = CheckValidityOfResponsePacket(&Query, &Query2, serviceBuffer);
    while (ccode == 2)
       ccode = CheckValidityOfResponsePacket(&Query, &Query2, serviceBuffer);

    IPXCancelEvent(&Query.theECB);
    IPXCancelEvent(&Query2.theECB);
    IPXCloseSocket(sourceSocket);

    return(ccode);
}



 /*-------------------------------------------------------------------------*/




int  InitializeSendPacket()
{
SEND_PACKET   QSP;


   QSP.theECB.fragmentCount = 1;
   QSP.theECB.fragmentDescriptor[0].address = &QSP.SAPq;
   QSP.theECB.fragmentDescriptor[0].size = sizeof(SAPQueryPacket);
   QSP.theECB.inUseFlag = 0;
   QSP.theECB.socketNumber = sourceSocket;
   QSP.theECB.ESRAddress = 0;
   memset(QSP.theECB.immediateAddress, '\xFF', 6);

   QSP.SAPq.packetType = 0;
   memset(QSP.SAPq.destination.network, '\x00', 4);
   memset(QSP.SAPq.destination.node, '\xFF', 6);
   *(WORD *)QSP.SAPq.destination.socket = IntSwap(SAP_SOCKET);
   IPXGetInternetworkAddress(QSP.SAPq.source.network);
   *(WORD *)QSP.SAPq.source.socket = IntSwap(sourceSocket);
   QSP.SAPq.queryType = IntSwap(queryType);
   QSP.SAPq.serverType = IntSwap(serverType);

   IPXSendPacket(&QSP.theECB);
   while (QSP.theECB.inUseFlag)
       IPXRelinquishControl();

   return(QSP.theECB.completionCode);

}


 /*-------------------------------------------------------------------------*/




int  InitializeReceivePacket(Qptr)
RECEIVE_PACKET   *Qptr;
{


   Qptr->theECB.fragmentCount = 1;
   Qptr->theECB.fragmentDescriptor[0].address = &Qptr->SB;
   Qptr->theECB.fragmentDescriptor[0].size = sizeof(Qptr->SB);

   Qptr->theECB.inUseFlag = 0;
   Qptr->theECB.ESRAddress = 0;

   Qptr->theECB.socketNumber = sourceSocket;

   Qptr->SB.Header.packetType = IntSwap(SAP_PACKET_TYPE);
   memset(Qptr->SB.Header.destination.network, '\x00', 4);
   memset(Qptr->SB.Header.destination.node, '\xFF', 6);
   memset(Qptr->theECB.immediateAddress, '\xFF', 6);
   *(WORD *)Qptr->SB.Header.destination.socket = IntSwap(SAP_SOCKET);

   Qptr->SB.Header.length = IntSwap(sizeof(Qptr->SB));
   IPXGetInternetworkAddress(Qptr->SB.Header.source.network);
   *(WORD *)Qptr->SB.Header.source.socket = IntSwap(sourceSocket);

   IPXGetLocalTarget(&Qptr->SB.Header.destination.network[0],
                     &Qptr->theECB.immediateAddress[0], &TransportTime);

   return(0);

}


 /*-------------------------------------------------------------------------*/




int   CheckValidityOfResponsePacket(Qptr, Qptr2, serviceBuffer)
RECEIVE_PACKET    *Qptr, *Qptr2;
SAP               *serviceBuffer;
{
RECEIVE_PACKET    *Qx;
int       Timeout, ReturnVal;
unsigned  StartTime, CurrentTime;


      StartTime = IPXGetIntervalMarker();
      Timeout = FALSE;
      do
       {
          if (!Qptr->theECB.inUseFlag)
           {
               Qx = Qptr;
               break;
           }
          if (!Qptr2->theECB.inUseFlag)
           {
               Qx = Qptr2;
               break;
           }
          CurrentTime = IPXGetIntervalMarker();
          Timeout = ( (CurrentTime - StartTime) > (TransportTime * 5) );

          IPXRelinquishControl();
       }  while (!Timeout);

      if (!Timeout)
       {
          if (Qx->theECB.completionCode)
              ReturnVal = 2;
          else
           {
              if (Qx->SB.ResponseType == IntSwap(4))
               {
                  ReturnVal = 0;

/************************************************************************
                  Qx->SB.ResponseType = IntSwap(Qx->SB.ResponseType);
                  Qx->SB.ServerType =   IntSwap(Qx->SB.ServerType);

                  *((DWORD *)&Qx->SB.Network[0]) =
                                  LongSwap(*((DWORD *)&Qx->SB.Network[0]));
                  *((DWORD *)&Qx->SB.Node[0]) =
                                    LongSwap(*((DWORD *)(&Qx->SB.Node[0])));
                  *((WORD *)(&Qx->SB.Node[0] + 4)) =
                                    IntSwap(*((WORD *)(&Qx->SB.Node[0] + 4)));
                  Qx->SB.Socket =   IntSwap(Qx->SB.Socket);

                  Qx->SB.InterveningNetworks =
                                    IntSwap(Qx->SB.InterveningNetworks);
 ************************************************************************/

                  memmove((BYTE *)&serviceBuffer[0],
                          (BYTE *)&Qx->SB, MIN(bufferSize, 96));
               }
           }
          InitializeReceivePacket(Qx);
          IPXListenForPacket(&Qx->theECB);
       }
      else
          ReturnVal = 1;

      return(ReturnVal);
}
