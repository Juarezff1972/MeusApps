/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef SAP_H
#define SAP_H

#ifndef _NXT_H
  #include <nxt.h>
#endif

#ifndef _PROLOG_H
  #include <prolog.h>
#endif

/*____________________________________________________________________________


     Header file that includes the definitions and structures needed for

     the NETWARE API Service Advertising Protocals Logic

 ___________________________________________________________________________*/


   #ifndef TRUE
     #define TRUE                  1
     #define FALSE                 0
   #endif

   #define SAP_SOCKET           0x452
   #define SAP_PACKET_TYPE          2
   #define ONE_MINUTE           60*18
   #define IPX_EVENT_CANCELED    0xFC
   #define SUCCESSFUL            0x00
   #define FAILURE                 -1
   #define MIN(a,b)          (a < b ? a : b)
   #define NOT_SUPPORTED            1
   #define INVALID_QUERY_TYPE       2
   #define PERIODIC_ID_PACKET       2

/*__________________________________________________________________________*/




   typedef struct SAPHeader
    {
       WORD          checksum;             /* high-low 1's complement */
       WORD          length;               /* high-low unsigned int */
       BYTE          transportControl;
       BYTE          packetType;
       IPXAddress    destination;
       IPXAddress    source;
       WORD          SAPPacketType;        /* 2 or 4 */
       WORD          serverType;           /* assigned by Novell */
       BYTE          serverName[48];       /* VAP name */
       IPXAddress    serverAddress;        /* server internetwork address */
       WORD          interveningNetworks;  /* # of networks packet must traverse */
    } SAPHeader;




/*__________________________________________________________________________*/





   typedef struct SAPQueryPacket
    {
       WORD          checksum;             /* high-low 1's complement */
       WORD          length;               /* high-low unsigned int */
       BYTE          transportControl;
       BYTE          packetType;
       IPXAddress    destination;
       IPXAddress    source;
       WORD          queryType;            /* high-low, 1 or 3 */
       WORD          serverType;           /* high-low, assigned by Novell */
    } SAPQueryPacket;



/*__________________________________________________________________________*/


   /* SAP packet */

   typedef struct
    {
       ECB         theECB;
       SAPHeader   packet;
    } SAP_BUFFER;



/*__________________________________________________________________________*/



   typedef struct
    {
       IPXHeader    Header;
       WORD         ResponseType;         /* HI - LO  */
       WORD         ServerType;           /* HI - LO  */
       BYTE         ServerName[48];
       BYTE         Network[4];           /* hi - lo  */
       BYTE         Node[6];              /* hi - lo  */
       BYTE         Socket[2];            /* hi - lo  */
       WORD         InterveningNetworks;  /* hi - lo  */
    } SAP;



/*__________________________________________________________________________*/


   typedef struct
    {
       ECB               theECB;
       SAPQueryPacket    SAPq;
    } SEND_PACKET;


/*__________________________________________________________________________*/



   typedef struct
    {
       ECB   theECB;
       SAP   SB;
    } RECEIVE_PACKET;

/*__________________________________________________________________________*/


   #ifdef PROTOTYPE
      extern void far Advertiser(void);
      extern int  AdvertiseService( WORD, char *, BYTE * );
      extern int  InitializeSAP(void);
      extern void cdecl _LoadDS(void);
      extern int  QueryServices(WORD, WORD, WORD, SAP * );
      extern int  ShutdownSAP(void);
   #else
      extern void far Advertiser();
      extern int  AdvertiseService();
      extern int  InitializeSAP();
      extern void cdecl _LoadDS();
      extern int  QueryServices();
      extern int  ShutdownSAP();
   #endif

#endif
