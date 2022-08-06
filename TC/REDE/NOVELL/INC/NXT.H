/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef _NXT_H
  #define _NXT_H
  /*__________________________________________________________________________
       Definitions and structures  for the Netware API Communications logic
    _________________________________________________________________________*/

  #ifndef _PROLOG_H
     #include <prolog.h>
  #endif

  /* assigned socket types */

  #define SOC_DIAGNOSTICS
  #define SHORT_LIVED        0x00
  #define LONG_LIVED         0xFF
  #define ENABLE_WATCHDOG    0xFF
  #define DISABLE_WATCHDOG   0x00

  typedef struct IPXAddress
   {
      BYTE    network[4];                    /* high-low */
      BYTE    node[6];                       /* high-low */
      BYTE    socket[2];                     /* high-low */
   }IPXAddress;


  typedef struct IPXHeader
   {
      WORD        checkSum;                  /* high-low */
      WORD        length;                    /* high-low */
      BYTE        transportControl;
      BYTE        packetType;
      IPXAddress  destination;
      IPXAddress  source;
   }IPXHeader;


  typedef struct SPXHeader
   {
      WORD        checksum;               /* high-low 1's complement */
      WORD        length;                 /* high-low unsigned int */
      BYTE        transportControl;
      BYTE        packetType;
      IPXAddress  destination;
      IPXAddress  source;
      BYTE        connectionControl;      /* bit flags */
      BYTE        dataStreamType;
      WORD        sourceConnectionID;     /* high-low unsigned */
      WORD        destConnectionID;       /* high-low unsigned */
      WORD        sequenceNumber;         /* high-low unsigned */
      WORD        acknowledgeNumber;      /* high-low unsigned */
      WORD        allocationNumber;       /* high-low unsigned */
   }SPXHeader;

   typedef struct CONNECTION_INFO
   {
      BYTE    connectionState;
      BYTE    connectionFlags;
      WORD    sourceConnectionID;               /* hi-lo */
      WORD    destinationConnectionID;          /* hi-lo */
      WORD    sequenceNumber;                   /* hi-lo */
      WORD    acknowledgeNumber;                /* hi-lo */
      WORD    allocationNumber;                 /* hi-lo */
      WORD    remoteAcknowledgeNumber;          /* hi-lo */
      WORD    remoteAllocationNumber;           /* hi-lo */
      WORD    connectionSocket;                 /* hi-lo */
      BYTE    immediateAddress[6];
      IPXAddress   destination;
      WORD    retransmissionCount;              /* hi-lo */
      WORD    estimatedRoundTripDelay;          /* hi-lo */
      WORD    retransmittedPackets;             /* hi-lo */
      WORD    suppressedPackets;                /* hi-lo */
   } CONNECTION_INFO;


  #ifndef ECB_STRUCTURES_DEFINED
    #define ECB_STRUCTURES_DEFINED

    typedef struct ECBFragment
     {
        FARDATAPTR(void,address);
        WORD    size;                       /* low-high */
     }ECBFragment;

    typedef struct ECB
     {
        FARDATAPTR(void, linkAddress);
        FARCODEPTR(void, ESRAddress);
        BYTE        inUseFlag;
        BYTE        completionCode;
        WORD        socketNumber;                   /* high-low */
        BYTE        IPXWorkspace[4];                /* N/A */
        BYTE        driverWorkspace[12];            /* N/A */
        BYTE        immediateAddress[6];            /* high-low */
        WORD        fragmentCount;                  /* low-high */
        ECBFragment fragmentDescriptor[2];
     }ECB;
  #endif



  #define SPX_IS_INSTALLED            0xFF
  #define SPX_NOT_INSTALLED           0x00
  #define SPX_CONNECTION_OK           0x00
  #define SPX_CONNECTION_STARTED      0x00
  #define SPX_CONNECTION_ESTABLISHED  0x00
  #define SPX_PACKET_SUCCESSFUL       0x00
  #define SPX_SOCKET_NOT_OPENED       0xFF
  #define SPX_MALFORMED_PACKET        0xFD
  #define SPX_PACKET_OVERFLOW         0xFD
  #define SPX_LISTEN_CANCELED         0xFC
  #define SPX_CONNECTION_TABLE_FULL   0xEF
  #define SPX_INVALID_CONNECTION      0xEE
  #define SPX_NO_ANSWER_FROM_TARGET   0xED
  #define SPX_CONNECTION_FAILED       0xED
  #define SPX_CONNECTION_TERMINATED   0xED
  #define SPX_TERMINATED_POORLY       0xEC

  #ifdef PROTOTYPE
     extern int        IPXCancelEvent(ECB *);
     extern void cdecl IPXCloseSocket(WORD);
     extern void       IPXDisconnectFromTarget(BYTE *);
     extern void cdecl IPXGetDataAddress(BYTE *, WORD *);
     extern void       IPXGetInternetworkAddress(BYTE *);
     extern WORD cdecl IPXGetIntervalMarker(void);
     extern int  cdecl IPXGetLocalTarget(BYTE *, BYTE *, int *);
     extern void cdecl IPXGetProcAddress(WORD *, WORD *);
     extern BYTE cdecl IPXInitialize(void);
     extern void       IPXListenForPacket(ECB *);
     extern int  cdecl IPXOpenSocket(BYTE *, BYTE);
     extern int  cdecl _IPXPacket(ECB *, WORD);
     extern void cdecl _IPXrequest(WORD, ECB *, WORD);
     extern void cdecl IPXRelinquishControl(void);
     extern void       IPXScheduleIPXEvent(WORD, ECB *);
     extern void       IPXScheduleSpecialEvent(WORD, ECB *);
     extern void       IPXSendPacket(ECB *);
     extern void cdecl SPXAbortConnection(WORD);
     extern int  cdecl SPXEstablishConnection(BYTE, BYTE, WORD *, ECB *);
     extern int        SPXGetConnectionStatus(WORD, CONNECTION_INFO *);
     extern int  cdecl SPXInitialize(BYTE *, BYTE *, WORD *, WORD *);
     extern void cdecl SPXListenForConnection(BYTE, BYTE, ECB *);
     extern void cdecl SPXListenForSequencedPacket(ECB *);
     extern int  cdecl _SPXrequest(WORD, ECB *, WORD);
     extern void       SPXSendSequencedPacket(WORD, ECB *);
     extern void       SPXTerminateConnection(WORD, ECB *);
  #else
     extern int        IPXCancelEvent();
     extern void cdecl IPXCloseSocket();
     extern void       IPXDisconnectFromTarget();
     extern void cdecl IPXGetDataAddress();
     extern void       IPXGetInternetworkAddress();
     extern WORD cdecl IPXGetIntervalMarker();
     extern int  cdecl IPXGetLocalTarget();
     extern void cdecl IPXGetProcAddress();
     extern int  cdecl IPXInitialize();
     extern void       IPXListenForPacket();
     extern int  cdecl IPXOpenSocket();
     extern int  cdecl _IPXPacket();
     extern void cdecl _IPXrequest();
     extern void cdecl IPXRelinquishControl();
     extern void       IPXScheduleIPXEvent();
     extern void       IPXScheduleSpecialEvent();
     extern void       IPXSendPacket();
     extern void cdecl SPXAbortConnection();
     extern int  cdecl SPXEstablishConnection();
     extern int        SPXGetConnectionStatus();
     extern int  cdecl SPXInitialize();
     extern void cdecl SPXListenForConnection();
     extern void cdecl SPXListenForSequencedPacket();
     extern int  cdecl _SPXrequest();
     extern void       SPXSendSequencedPacket();
     extern void       SPXTerminateConnection();
  #endif

#endif
