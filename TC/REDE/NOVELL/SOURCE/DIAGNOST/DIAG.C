/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */

#include <stdio.h>
#include <string.h>

#ifndef BYTE
   #include <nit.h>
#endif

#ifndef DIAG
   #include <diag.h>
#endif

#define RESPONSE_ECBS 4

static SPXHeader responseHeader [RESPONSE_ECBS];
static ECB responseECB [RESPONSE_ECBS];
static BYTE responseData [RESPONSE_ECBS] [sizeof(SPXPacket)];

int GetRemoteSPXSocket( BeginDiagnosticStruct *destination, BYTE *cList )
{
   /* Get remote node's SPX socket #. Place into destination SPXSocket  */
   static      struct {
                  BYTE    exclusionCount;
   } IPXReqstPkt = { (BYTE)NULL };

   static      struct {
                  BYTE    majorVersion;
                  BYTE    minorVersion;
                  WORD    SPXDiagnosticSocket;
                  BYTE    componentCount;
                  BYTE    componentType[53];
   } IPXReplyPkt;

   static      ECB         IPXSendECB,IPXRecvECB;
   static      IPXHeader   IPXSendPkt,IPXRecvPkt;
               WORD        tempSocket,startTicks,endTicks;
               int         tTime;

   tempSocket = (WORD)0x4545;
   if( IPXOpenSocket((BYTE *)&tempSocket, 0) )
      return 1;

   IPXRecvECB.socketNumber = tempSocket;
   IPXRecvECB.fragmentCount = 2;
   IPXGetDataAddress((BYTE *)(&IPXRecvPkt),
                     (WORD *)&IPXRecvECB.fragmentDescriptor[0].address);
   IPXRecvECB.fragmentDescriptor[0].size = sizeof(IPXRecvPkt);
   IPXGetDataAddress((BYTE *)&IPXReplyPkt,
                     (WORD *)&IPXRecvECB.fragmentDescriptor[1].address);
   IPXRecvECB.fragmentDescriptor[1].size = sizeof(IPXReplyPkt);
   IPXGetLocalTarget((BYTE *)destination,
                     (BYTE *)IPXRecvECB.immediateAddress, &tTime);

   IPXListenForPacket(&IPXRecvECB);

   /*--------------------------------------------------------------*/

   IPXSendPkt.destination = *(IPXAddress *)destination;
   *(WORD *)IPXSendPkt.destination.socket = (WORD)0x5604;
   IPXGetInternetworkAddress((BYTE *)&IPXSendPkt.source);
   *(WORD *)IPXSendPkt.source.socket      = tempSocket;
   IPXSendPkt.packetType = 4;
   IPXGetLocalTarget((BYTE *)IPXSendPkt.destination.network,
                     IPXSendECB.immediateAddress, &tTime);
   IPXSendECB.socketNumber = tempSocket;
   IPXSendECB.fragmentCount = 2;
   IPXGetDataAddress((BYTE *) (&IPXSendPkt),
                     (WORD *) &IPXSendECB.fragmentDescriptor[0].address);
   IPXSendECB.fragmentDescriptor[0].size = sizeof(IPXSendPkt);
   IPXGetDataAddress((BYTE *) &IPXReqstPkt,
                     (WORD *) &IPXSendECB.fragmentDescriptor[1].address);
   IPXSendECB.fragmentDescriptor[1].size = sizeof(IPXReqstPkt);
   IPXSendPacket(&IPXSendECB);

   while( IPXSendECB.inUseFlag )
      IPXRelinquishControl();     /* wait for send to complete    */

   startTicks = IPXGetIntervalMarker();    /* read start time      */
   while( IPXRecvECB.inUseFlag ){
      IPXRelinquishControl();     /* wait for response from remote*/
      endTicks = IPXGetIntervalMarker();  /* read end time        */
      if( endTicks - startTicks > 72 )    /* if 4 seconds go by   */
            break;                          /* node isn't responding*/
   }

   IPXCloseSocket( tempSocket );   /* this will cancel receive...  */

   if( IPXSendECB.completionCode != 0 )
      return IPXSendECB.completionCode;

   if( IPXRecvECB.inUseFlag || IPXRecvECB.completionCode )
      return IPXRecvECB.completionCode;   /* no response recv'ed  */

   *(WORD *)destination->socket = IPXReplyPkt.SPXDiagnosticSocket;
   memcpy( cList, (void *)&IPXReplyPkt.componentCount,54);
   return 0;
}

int BeginDiagnostics( destination, connectionID, componentList )
BeginDiagnosticStruct *destination;
WORD *connectionID;
BYTE *componentList;
{
WORD tempSocket;
int ccode, i, Error = NO_ERRORS;
ECB thisECB;
SPXHeader thisHeader;


   /*  First open a socket then init receive ECB's.  */

   tempSocket = (WORD)0x00;        /* let IPX pick socket number   */
   ccode = IPXOpenSocket((BYTE *)&tempSocket, 0);

   if( (ccode = GetRemoteSPXSocket(destination,componentList)) != 0 )
            Error = COULD_NOT_OPEN_SOCKET;
   else 
      {
      for (i=0; i<RESPONSE_ECBS; i++) 
         {
         responseECB[i].ESRAddress[0]=responseECB[i].ESRAddress[1] = 0;
         responseECB[i].socketNumber = tempSocket;
         responseECB[i].fragmentCount = 2;
         IPXGetDataAddress((BYTE *) &responseHeader[i],
               (WORD *)&responseECB[i].fragmentDescriptor[0].address);
         responseECB[i].fragmentDescriptor[0].size =
                  sizeof(responseHeader[i]);
         IPXGetDataAddress((BYTE *) responseData[i],
               (WORD *)&responseECB[i].fragmentDescriptor[1].address);
         responseECB[i].fragmentDescriptor[1].size =
                  sizeof(responseData[i]);
         SPXListenForSequencedPacket ((ECB *)&responseECB[i]);
         }
      }
   /*---------------------------------------------------------------------
   now that receive ECB's are available, attempt to connect to target node
     ---------------------------------------------------------------------*/
   if (Error == NO_ERRORS) 
      {
      memcpy(&thisHeader.destination, destination,
            sizeof(thisHeader.destination));
      thisECB.ESRAddress[0] = thisECB.ESRAddress[1] = 0;
      thisECB.socketNumber = tempSocket;
      thisECB.fragmentCount = 1;
      IPXGetDataAddress ((BYTE *)&thisHeader,
         (WORD *)&thisECB.fragmentDescriptor[0].address);
      thisECB.fragmentDescriptor[0].size = sizeof(thisHeader);
      ccode = SPXEstablishConnection (0, 0, connectionID, (ECB *)&thisECB);
      if (ccode == 0) 
         {
         while (thisECB.inUseFlag)
            IPXRelinquishControl ();
         if (thisECB.completionCode != 0)
            Error = COULD_NOT_ESTABLISH_CONNECTION;
         }
      else
         Error = COULD_NOT_BEGIN_CONNECTION;
         if (Error != NO_ERRORS)
            IPXCloseSocket (thisECB.socketNumber);
      }
   else
      Error = COULD_NOT_OPEN_SOCKET;

   return (Error);
}

int EndDiagnostics( connectionID )
WORD connectionID;
{
int  returnCode;
ConnectionStatusBlock CSB;
SPXHeader thisHeader;
ECB thisECB;

   /* Terminate the connection to the target and close the socket. */

   returnCode = SPXGetConnectionStatus ( connectionID, 
                                        (CONNECTION_INFO *)&CSB );
   if (returnCode == 0) 
      {
      thisECB.ESRAddress[0] = thisECB.ESRAddress[1] = 0;
      thisECB.fragmentCount = 1;
      IPXGetDataAddress ((BYTE *)&thisHeader,
         (WORD *)&thisECB.fragmentDescriptor[0].address);
      thisECB.fragmentDescriptor[0].size = sizeof (thisHeader);
      SPXTerminateConnection (connectionID, &thisECB);
      while (thisECB.inUseFlag)
            IPXRelinquishControl ();
      if (thisECB.completionCode == 0)
            returnCode = NO_ERRORS;
      else
            returnCode = COULD_NOT_TERMINATE_CONNECTION;
      IPXCloseSocket (CSB.connectionSocket);
      }
   else
      returnCode = BAD_CONNECTION_ID;

   return(returnCode);
}

int SendSPXPacket (connectionID, buffer1, size1)
WORD connectionID;
BYTE *buffer1;
WORD size1;
{
int ccode, returnCode;
ConnectionStatusBlock CSB;
SPXHeader requestHeader;
ECB requestECB;

   /*  set up the packet header and ECB for the request  */

   ccode = SPXGetConnectionStatus(connectionID, (CONNECTION_INFO *)&CSB);
   if (!ccode) 
      {
      requestHeader.connectionControl = 0x00;
      requestHeader.dataStreamType = 0x00;
      requestECB.ESRAddress[0] = requestECB.ESRAddress[1] = 0;
      requestECB.fragmentCount = 2;
      IPXGetDataAddress ((BYTE *)&requestHeader,
         (WORD *)&requestECB.fragmentDescriptor[0].address);
      requestECB.fragmentDescriptor[0].size = sizeof(requestHeader);
      IPXGetDataAddress(buffer1,
         (WORD *)&requestECB.fragmentDescriptor[1].address);
      requestECB.fragmentDescriptor[1].size = size1;
      /*
      now send request to packet
      */
      SPXSendSequencedPacket (connectionID, (ECB *)&requestECB);
      while (requestECB.inUseFlag)
            IPXRelinquishControl();
      if (requestECB.completionCode == 0)
            returnCode = NO_ERRORS;
      else
            returnCode = COULD_NOT_SEND_REQUEST;
      }
   else
      returnCode = BAD_CONNECTION_ID;

   return( returnCode );
}

int GetDiagnosticResponse( connectionID, buffer1, size1, buffer2, size2 )
WORD connectionID;
BYTE *buffer1, *buffer2;
WORD size1, size2;
{
int foundECB = 0;
int ECBPtr;
int returnCode = RECEIVED_REPLY_IN_ERROR;

   /*---------------------------------------------------------------------
   Get a response back from a previously sent request for the target of the
   specified connectionID. Other received packets to other connections,
   if any, left alone.  Valid data will be placed in the two user supplied
   buffers. Then resubmit the receive ECB for continued use.
     --------------------------------------------------------------------*/
   while (!foundECB) 
      {
      for (ECBPtr=0; ECBPtr<RESPONSE_ECBS; (ECBPtr)++) 
         {
         if (!responseECB[ECBPtr].inUseFlag) 
            {
            if (responseHeader[ECBPtr].destConnectionID == connectionID) 
               {
               if (responseECB[ECBPtr].completionCode == 0) 
                  {
                  returnCode = NO_ERRORS;
                  memcpy(buffer1,&responseData[ECBPtr][0],size1);
                  memcpy(buffer2,&responseData[ECBPtr][size1],size2);
                  }
               else
                  returnCode = RECEIVED_REPLY_IN_ERROR;

               foundECB = ~foundECB;
               break;
               }
            }
         }
         IPXRelinquishControl ();
      }

   SPXListenForSequencedPacket ((ECB *)&responseECB[ECBPtr]);
   return (returnCode);
}

int GetDiagnosticStatus (connectionID)
WORD connectionID;
{
int ECBPtr;
int returnCode;

   /*--------------------------------------------------------------------
   Check the status of all outstanding ECB's of the given connectionID
   and return a status if a packet has been received and ready to be read.
    --------------------------------------------------------------------*/
   for (ECBPtr=0; ECBPtr<RESPONSE_ECBS; (ECBPtr)++) 
      {
      if (!responseECB[ECBPtr].inUseFlag) 
         {
         if( responseHeader[ECBPtr].destConnectionID == connectionID ) 
            {
            returnCode = 0;
            break;
            }
         else
            returnCode = 0xFF;
         }
      }
   return (returnCode);
}

int GetIPXSPXVersion( connectionID, componentNumber, Response, ResponseData )
WORD connectionID;
BYTE componentNumber;
AllResponseData *Response;
IPXSPXVersion *ResponseData;
{
int returnCode;
BYTE Request[2];

   Request[0] = componentNumber;
   Request[1] = 0;
   returnCode = SendSPXPacket( connectionID, Request, sizeof(Request) );
   if( returnCode == NO_ERRORS )
      returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                                sizeof(AllResponseData), (BYTE *)ResponseData, 
                                sizeof(IPXSPXVersion) );
   return( returnCode );
}

int GetIPXStatistics( connectionID, componentNumber, Response, ResponseData )
WORD connectionID;
BYTE componentNumber;
AllResponseData *Response;
IPXStatisticsStruct *ResponseData;
{
int returnCode;
BYTE Request[2];

   Request[0] = componentNumber;
   Request[1] = 1;
   returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
   if (returnCode == NO_ERRORS)
      returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                                sizeof(AllResponseData), (BYTE *)ResponseData, 
                                sizeof(IPXStatisticsStruct) );
   return( returnCode );
}

int GetSPXStatistics( connectionID, componentNumber, Response, ResponseData )
WORD connectionID;
BYTE componentNumber;
AllResponseData *Response;
SPXStatisticsStruct *ResponseData;
{
int returnCode;
BYTE Request[2];

   Request[0] = componentNumber;
   Request[1] = 2;
   returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
   if (returnCode == NO_ERRORS)
      returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                                sizeof(AllResponseData), (BYTE *)ResponseData, 
                                sizeof(SPXStatisticsStruct) );
   return( returnCode );
}

/* ************POINT TO POINT TEST ROUTINES********** */

int StartCountingPkts( connectionID, componentNumber, Response, ResponseData )
WORD connectionID;
BYTE componentNumber;
AllResponseData *Response;
StartCountingPacketsStruct *ResponseData;
{
int returnCode;
BYTE Request[2];

   Request[0] = componentNumber;
   Request[1] = 5;
   returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
   if (returnCode == NO_ERRORS)
      returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                              sizeof(AllResponseData), (BYTE *)ResponseData,
                              sizeof(StartCountingPacketsStruct));
   return( returnCode );
}

int StartSendingPktsTimed( connectionID, componentNumber, RequestData,
                           Response, ResponseData, Ticks )
WORD connectionID;
BYTE componentNumber;
SendPacketsRequestStruct *RequestData;
AllResponseData *Response;
SendPacketsResponseStruct *ResponseData;
WORD Ticks;
{
int returnCode;
BYTE Request[2 + sizeof(SendPacketsRequestStruct)];
ECB Timer;

   Request[0] = componentNumber;
   Request[1] = 3;
   memcpy (&Request[2], RequestData, sizeof(Request));
   returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
   if (returnCode == NO_ERRORS) 
      {
      /* init and start timer */
      memset( &Timer, 0, sizeof(Timer) );
      if( Ticks )
         IPXScheduleSpecialEvent( Ticks, &Timer ); /* start timer */

      while( Timer.inUseFlag && GetDiagnosticStatus( connectionID ) )
         IPXRelinquishControl();
      if (!Timer.inUseFlag) 
         {  /* Timer Expired? */
         AbortSendingPackets (connectionID, componentNumber);
         returnCode = NODE_NOT_FOUND_OR_NO_RESPONSE;
         }
      else 
         {
         if (Ticks)
            IPXCancelEvent (&Timer); /* cancel timer */
         returnCode = GetDiagnosticResponse( connectionID, (BYTE *)Response,
                                           sizeof(AllResponseData), 
                                           (BYTE *)ResponseData,
                                           sizeof(SendPacketsResponseStruct));
         }
      }
   return (returnCode);
}

int AbortSendingPackets( connectionID, componentNumber )
WORD connectionID;
BYTE componentNumber;
{
int returnCode;
BYTE Request[2];

   Request[0] = componentNumber;
   Request[1] = 4;
   returnCode = SendSPXPacket( connectionID, Request, sizeof(Request) );
   return (returnCode); /* no response for this request */
}

int ReturnReceivedPacketCount( connectionID, componentNumber, Response, 
                               ResponseData )
WORD connectionID;
BYTE componentNumber;
AllResponseData *Response;
ReturnReceivedPacketStruct *ResponseData;
{
int returnCode;
BYTE Request[2];

   Request[0] = componentNumber;
   Request[1] = 6;
   returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
   if (returnCode == NO_ERRORS)
      returnCode = GetDiagnosticResponse( connectionID, (BYTE *)Response,
                              sizeof(AllResponseData), (BYTE *)ResponseData,
                              sizeof(ReturnReceivedPacketStruct));
   return (returnCode);
}


/* ************BRIDGE DRIVER COMPONENT FUNCTIONS********** */

int GetBridgeDriverStatus( connectionID, componentNumber, Response, 
                           ResponseData )
WORD connectionID;
BYTE componentNumber;
AllResponseData *Response;
BridgeDriverStatusStruct *ResponseData;
{
        int returnCode;
        BYTE Request[2];

        Request[0] = componentNumber;
        Request[1] = 0;
        returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
        if (returnCode == NO_ERRORS)
                returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                        sizeof(AllResponseData), (BYTE *)ResponseData,
                        sizeof(BridgeDriverStatusStruct));
        return (returnCode);
}

int GetBridgeDriverConfiguration(connectionID, componentNumber,
        LANBoardNumber, Response, ResponseData)
WORD connectionID;
BYTE componentNumber;
BYTE LANBoardNumber;
AllResponseData *Response;
DriverConfigurationStruct *ResponseData;
{
        int returnCode;
        BYTE Request[3];

        Request[0] = componentNumber;
        Request[1] = 1;
        Request[2] = LANBoardNumber;
        returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
        if (returnCode == NO_ERRORS)
                returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                        sizeof(AllResponseData), (BYTE *)ResponseData,
                        sizeof(DriverConfigurationStruct));
        return (returnCode);
}

int GetBridgeDriverStatistics(connectionID, componentNumber, LANBoardNumber,
        Response, ResponseData)
WORD connectionID;
BYTE componentNumber;
BYTE LANBoardNumber;
AllResponseData *Response;
DriverStatisticsStruct *ResponseData;
{
        int returnCode;
        BYTE Request[3];

        Request[0] = componentNumber;
        Request[1] = 2;
        Request[2] = LANBoardNumber;
        returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
        if (returnCode == NO_ERRORS)
                returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                        sizeof(AllResponseData), (BYTE *)ResponseData,
                        sizeof(DriverStatisticsStruct));
        return (returnCode);
}

/* ************SHELL DRIVER COMPONENT FUNCTIONS********** */

int GetShellDriverConfiguration(connectionID, componentNumber, Response,
        ResponseData)
WORD connectionID;
BYTE componentNumber;
AllResponseData *Response;
DriverConfigurationStruct *ResponseData;
{
        int returnCode;
        BYTE Request[2];

        Request[0] = componentNumber;
        Request[1] = 0;
        returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
        if (returnCode == NO_ERRORS)
                returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                        sizeof(AllResponseData), (BYTE *)ResponseData,
                        sizeof(DriverConfigurationStruct));
        return (returnCode);
}

int GetShellDriverStatistics(connectionID, componentNumber, Response, ResponseData)
WORD connectionID;
BYTE componentNumber;
AllResponseData *Response;
DriverStatisticsStruct *ResponseData;
{
        int returnCode;
        BYTE Request[2];

        Request[0] = componentNumber;
        Request[1] = 1;
        returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
        if (returnCode == NO_ERRORS)
                returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                        sizeof(AllResponseData), (BYTE *)ResponseData,
                        sizeof(DriverStatisticsStruct));
        return (returnCode);
}

/* ***************SHELL COMPONENT FUNCTIONS***************** */

int GetOSVersionInfo(connectionID, componentNumber, Response, ResponseData)
WORD connectionID;
BYTE componentNumber;
AllResponseData *Response;
OSVersionStruct *ResponseData;
{
        int returnCode;
        BYTE Request[2];

        Request[0] = componentNumber;
        Request[1] = 0;
        returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
        if (returnCode == NO_ERRORS)
                returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                        sizeof(AllResponseData), (BYTE *)ResponseData, sizeof(OSVersionStruct));
        return (returnCode);
}

int GetShellAddress(connectionID, componentNumber, Response, ResponseData)
WORD connectionID;
BYTE componentNumber;
AllResponseData *Response;
ShellAddressStruct *ResponseData;
{
        int returnCode;
        BYTE Request[2];

        Request[0] = componentNumber;
        Request[1] = 1;
        returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
        if (returnCode == NO_ERRORS)
                returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                        sizeof(AllResponseData), (BYTE *)ResponseData, sizeof(ShellAddressStruct));
        return (returnCode);
}

int GetShellStatistics(connectionID, componentNumber, Response, ResponseData)
WORD connectionID;
BYTE componentNumber;
AllResponseData *Response;
ShellStatisticsStruct *ResponseData;
{
        int returnCode;
        BYTE Request[2];

        Request[0] = componentNumber;
        Request[1] = 2;
        returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
        if (returnCode == NO_ERRORS)
                returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                        sizeof(AllResponseData), (BYTE *)ResponseData, sizeof(ShellStatisticsStruct));
        return (returnCode);
}

int GetServerAddressTable(connectionID, componentNumber, Response, ResponseData)
WORD connectionID;
BYTE componentNumber;
AllResponseData *Response;
ServerAddressTableStruct *ResponseData;
{
        int returnCode;
        BYTE Request[2];

        Request[0] = componentNumber;
        Request[1] = 3;
        returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
        if (returnCode == NO_ERRORS)
                returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                        sizeof(AllResponseData), (BYTE *)ResponseData,
                        sizeof(ServerAddressTableStruct));
        return (returnCode);
}

int GetServerNameTable(connectionID, componentNumber, Response, ResponseData)
WORD connectionID;
BYTE componentNumber;
AllResponseData *Response;
ServerNameTableStruct *ResponseData;
{
        int returnCode;
        BYTE Request[2];

        Request[0] = componentNumber;
        Request[1] = 4;
        returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
        if (returnCode == NO_ERRORS)
                returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                        sizeof(AllResponseData), (BYTE *)ResponseData, sizeof(ServerNameTableStruct));
        return (returnCode);    /* set up the ECB and packet header for the request */
}

int GetPrimaryServerNumber(connectionID, componentNumber, Response, ResponseData)
WORD connectionID;
BYTE componentNumber;
AllResponseData *Response;
PrimaryServerStruct *ResponseData;
{
        int returnCode;
        BYTE Request[2];

        Request[0] = componentNumber;
        Request[1] = 5;
        returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
        if (returnCode == NO_ERRORS)
                returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                        sizeof(AllResponseData), (BYTE *)ResponseData, sizeof(PrimaryServerStruct));
        return (returnCode);
}

int GetShellVersionInfo(connectionID, componentNumber, Response, ResponseData)
WORD connectionID;
BYTE componentNumber;
AllResponseData *Response;
ShellVersionStruct *ResponseData;
{
        int returnCode;
        BYTE Request[2];

        Request[0] = componentNumber;
        Request[1] = 6;
        returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
        if (returnCode == NO_ERRORS)
                returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                        sizeof(AllResponseData), (BYTE *)ResponseData, sizeof(ShellVersionStruct));
        return (returnCode);
}

/* ******************BRIDGE COMPONENT FUNCTIONS***************** */

int GetBridgeStatistics(connectionID, componentNumber, Response, ResponseData)
WORD connectionID;
BYTE componentNumber;
AllResponseData *Response;
BridgeStatisticsStruct *ResponseData;
{
        int returnCode;
        BYTE Request[2];

        Request[0] = componentNumber;
        Request[1] = 0;
        returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
        if (returnCode == NO_ERRORS)
                returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                        sizeof(AllResponseData), (BYTE *)ResponseData, sizeof(BridgeStatisticsStruct));
        return (returnCode);
}

int GetLocalTables(connectionID, componentNumber, Response, ResponseData)
WORD connectionID;
BYTE componentNumber;
AllResponseData *Response;
LocalTablesStruct *ResponseData;
{
        int returnCode;
        BYTE Request[2];

        Request[0] = componentNumber;
        Request[1] = 1;
        returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
        if (returnCode == NO_ERRORS)
                returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                        sizeof(AllResponseData), (BYTE *)ResponseData, sizeof(LocalTablesStruct));
        return (returnCode);
}

int GetAllKnownNetworks(connectionID, componentNumber, nextNetworkOffset,
        Response, ResponseData)
WORD connectionID;
BYTE componentNumber;
WORD nextNetworkOffset;
AllResponseData *Response;
AllKnownNetworksStruct *ResponseData;
{
        int returnCode;
        BYTE Request[2 + 2];

        Request[0] = componentNumber;
        Request[1] = 2;
        memcpy (&Request[2], &nextNetworkOffset, 2);
        returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
        if (returnCode == NO_ERRORS)
                returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                        sizeof(AllResponseData), (BYTE *)ResponseData, sizeof(AllKnownNetworksStruct));
        return (returnCode);
}

int GetSpecificNetworkInfo(connectionID, componentNumber, networkAddress,
        Response, ResponseData)
WORD connectionID;
BYTE componentNumber;
BYTE *networkAddress;
AllResponseData *Response;
SpecificNetworkInfoStruct *ResponseData;
{
        int returnCode;
        BYTE Request[2 + 4];

        Request[0] = componentNumber;
        Request[1] = 3;
        memcpy (&Request[2], networkAddress, 4);
        returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
        if (returnCode == NO_ERRORS)
                returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                        sizeof(AllResponseData), (BYTE *)ResponseData,
                        sizeof(SpecificNetworkInfoStruct));
        return (returnCode);
}

int GetAllKnownServers(connectionID, componentNumber, numberServersToSkip,
        Response, ResponseData)
WORD connectionID;
BYTE componentNumber;
WORD numberServersToSkip;
AllResponseData *Response;
AllKnownServersStruct *ResponseData;
{
        int returnCode;
        BYTE Request[2 + 2];

        Request[0] = componentNumber;
        Request[1] = 4;
        memcpy (&Request[2], &numberServersToSkip, 2);
        returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
        if (returnCode == NO_ERRORS)
                returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                        sizeof(AllResponseData), (BYTE *)ResponseData, sizeof(AllKnownServersStruct));
        return (returnCode);
}

int GetSpecificServerInfo(connectionID, componentNumber,
                                                Server, Response, ResponseData)
WORD connectionID;
BYTE componentNumber;
ServerInfoStruct *Server;
AllResponseData *Response;
SpecificServerInfoStruct *ResponseData;
{
        int returnCode;
        BYTE Request[2 + sizeof(ServerInfoStruct)];

        Request[0] = componentNumber;
        Request[1] = 5;
        memcpy (&Request[2], Server, sizeof(ServerInfoStruct));
        returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
        if (returnCode == NO_ERRORS)
                returnCode = GetDiagnosticResponse (connectionID, (BYTE *)Response,
                        sizeof(AllResponseData), (BYTE *)ResponseData,
                        sizeof(SpecificServerInfoStruct));
        return (returnCode);
}

int ReinitializeRouterTables(connectionID, componentNumber, authorizationCode,
        Response)
WORD connectionID;
BYTE componentNumber;
BYTE *authorizationCode;
AllResponseData *Response;
{
        int returnCode;
        BYTE Request[2 + 10];

        Request[0] = componentNumber;
        Request[1] = 2;
        memcpy (&Request[2], authorizationCode, 10);
        returnCode = SendSPXPacket (connectionID, Request, sizeof(Request));
        if( returnCode == NO_ERRORS )
           returnCode = GetDiagnosticResponse( connectionID, (BYTE *)Response,
                                               sizeof(AllResponseData), 
                                               (BYTE *)NULL, (WORD)NULL );
        return (returnCode);
}

