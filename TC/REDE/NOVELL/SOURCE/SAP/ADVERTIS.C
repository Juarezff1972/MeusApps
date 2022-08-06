/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */

#include <stdio.h>
#include <string.h>
#include <nxt.h>
#include <sap.h>
#include <nwmisc.h>

#define RESPONSE_ID_PACKET 4

extern void far RespondToLocalQuery( void ) ;

static SAP_BUFFER  periodicSAP, responseSAP ;
static ECB listenECB ;
static SAPQueryPacket querySAP ;
static WORD AdvertiserHasSent = FALSE ;
static WORD thisServer ;
static WORD sapSocket ;

/* --------------------------- AdvertiseService() -------------------------- 

   This routine will do two things for an application, after setting up
   two ECB and associated packets. First one ECB is used with an AES for 
   periodic broadcasting.  Second the other ECB is used for specific 
   queries from a client to a server applicaton when they are on the same
   net. If the two applications are on different nets the bridge takes on the
   responsibility of informing the quering client of the application server
   location, but if the two applications are on the same net it is up to the
   server application to respond for itself.  This routine will listen on the
   SAP socket for a local query and respond on behalf of the server 
   application, so that it is transparent to it.
  ________________________________________________________________________*/

int   AdvertiseService(WORD sType,
                       char *sName, 
                       BYTE *sSocket)
{
    WORD ccode ;

    sapSocket = IntSwap(SAP_SOCKET) ;
    thisServer = IntSwap(sType) ;
    ccode = (BYTE) IPXInitialize();
    if(!ccode)
        ccode = IPXOpenSocket( sSocket, SHORT_LIVED ) ;
    if(!ccode)
        ccode = IPXOpenSocket( (BYTE *)&sapSocket, SHORT_LIVED ) ;
    if(!ccode)
        ccode = SetupRespondPacket( PERIODIC_ID_PACKET, sName, sType, 
                                    (WORD *)sSocket, &periodicSAP );
    if(!ccode)
        ccode = SetupRespondPacket( RESPONSE_ID_PACKET, sName, sType, 
                                    (WORD *)sSocket, &responseSAP );
    if(!ccode)
      ccode = SetupListenForQuery() ;
    if( ccode )
      {
      IPXCloseSocket( *( (WORD *)sSocket) ) ;
      IPXCloseSocket( sapSocket ) ;
      }
    return ccode ;
}

												 
int  SetupListenForQuery() 
{
    listenECB.socketNumber = IntSwap( SAP_SOCKET ) ;
    listenECB.fragmentCount = 1 ;
    listenECB.ESRAddress = (void (far *) () ) RespondToLocalQuery ;
    IPXGetDataAddress((BYTE *) (&querySAP),
                        (WORD *) &listenECB.fragmentDescriptor[0].address)  ;
    listenECB.fragmentDescriptor[0].size = sizeof(SAPQueryPacket) ;

    IPXListenForPacket((ECB *) &listenECB);
    return listenECB.completionCode ;
}

int SetupRespondPacket( WORD sapPacketType,
                        char *serverName,
                        WORD sType,                   
                        WORD *sSocket,
                        SAP_BUFFER *sap )
{
   sap->packet.SAPPacketType = IntSwap( sapPacketType );
   sap->packet.serverType = IntSwap( sType );
   strcpy( sap->packet.serverName, serverName );
   IPXGetInternetworkAddress( sap->packet.serverAddress.network );
   *(WORD *) sap->packet.serverAddress.socket = *sSocket;
   sap->packet.interveningNetworks = IntSwap( 1 );
   sap->packet.packetType = 0;
   
   sap->theECB.fragmentCount = 1;
   sap->theECB.fragmentDescriptor[0].address = &sap->packet;
   sap->theECB.fragmentDescriptor[0].size = sizeof( SAPHeader );
   sap->theECB.inUseFlag = 0 ;
   memset( sap->theECB.immediateAddress, '\xFF', 6 );
    
   if( sapPacketType == PERIODIC_ID_PACKET )
      {

		/* Setup Broadcast Addr for Periodic, Specific Addr done in ESR */

      memset(sap->packet.destination.network, '\x00', 4);
      memset(sap->packet.destination.node, '\xFF', 6);
      *(WORD *)sap->packet.destination.socket = IntSwap(SAP_SOCKET);

      sap->theECB.socketNumber = IntSwap(SAP_SOCKET);
      sap->theECB.ESRAddress = (void (far *)()) Advertiser;

      Advertiser() ;
      }
   else 
      sap->theECB.ESRAddress = (void (far *)()) NULL ;
    return 0 ;
}						 

 /*---------------------------- Advertiser() -------------------------------*/

void far Advertiser()
{
   _LoadDS();
   if(AdvertiserHasSent)
      {
      AdvertiserHasSent = FALSE;
      IPXScheduleSpecialEvent( ONE_MINUTE, &periodicSAP.theECB);
      }
   else
      {
      AdvertiserHasSent = TRUE;
      if( !periodicSAP.theECB.inUseFlag) 
         IPXSendPacket(&periodicSAP.theECB);
      }
}

/* ----------------------- RespondToLocalQuery() --------------------------

   This C function is called as the the ESR Handler.  It checks to see if
   the query is for this server.  If not it just re-posts the ECB.  If it
   is for this server it copies the address from the query packet and 
   places it into the response packet and sends it back.  
	                                                                       */

void far RespondToLocalQuery()
{
   _LoadDS();
   if(querySAP.serverType == thisServer)
      {
      while( responseSAP.theECB.inUseFlag )
         IPXRelinquishControl() ;
      memmove( &responseSAP.packet.destination.network, 
               &querySAP.source.network, 12 ) ;
      IPXSendPacket( &responseSAP.theECB ) ;
      }
   IPXListenForPacket( (ECB *) &listenECB ) ;
}

  /* -------------------------- Shutdown() ----------------------------------*/

int ShutdownSAP()
{
   IPXCancelEvent( &periodicSAP.theECB );
   while( periodicSAP.theECB.inUseFlag )
      IPXRelinquishControl() ;
   if( periodicSAP.theECB.completionCode != IPX_EVENT_CANCELED )
      return( FAILURE );

   periodicSAP.theECB.ESRAddress = (void ( far * )() )0;
   periodicSAP.packet.interveningNetworks = IntSwap( 0x10 );
   IPXSendPacket( &periodicSAP.theECB );
   while( periodicSAP.theECB.inUseFlag ) 
      IPXRelinquishControl() ;

   IPXCloseSocket( sapSocket ) ;

   return( SUCCESSFUL );

}
