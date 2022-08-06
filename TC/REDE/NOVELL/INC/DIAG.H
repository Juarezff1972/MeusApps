/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef _DIAG_H
   #define _DIAG_H

   #ifndef _NIT_H
       #include <nit.h>
   #endif

   #ifndef ECB_STRUCTURES_DEFINED
      #define ECB_STRUCTURES_DEFINED
      typedef struct ECBFragment
       {
          #ifndef LC310
              void    far *address;
          #else
              WORD    address[2];             /* offset-segment */
          #endif
              WORD    size;                   /* low-high */
        } ECBFragment;

      typedef struct ECB
        {
              WORD            linkAddress[2];         /* offset-segment */
              WORD            ESRAddress[2];          /* offset-segment */
              BYTE            inUseFlag;
              BYTE            completionCode;
              WORD            socketNumber;            /* high-low */
              BYTE            IPXWorkspace[4];         /* N/A */
              BYTE            driverWorkspace[12];     /* N/A */
              BYTE            immediateAddress[6];     /* high-low */
              WORD            fragmentCount;           /* low-high */
              ECBFragment     fragmentDescriptor[2];
        } ECB;

   #endif

   #ifndef _NXT_H
      #include <nxt.h>
   #endif

   #define MAX_NETWORKS 120
   #define MAX_SERVERS 150
   #define MAX_NODES 120
   #define MAX_EXCLUSIONS 80

   #define MAX_COMPONENTS 10
   #define MAX_LOCAL_NETWORKS 4
   #define MAX_ROUTES 47

   #define MAX_IPX_PACKET_SIZE 576

   #define IPX_SPX_COMPONENT 0
   #define BRIDGE_DRIVER_COMPONENT 1
   #define SHELL_DRIVER_COMPONENT 2
   #define SHELL_COMPONENT 3
   #define VAP_SHELL_COMPONENT 4
   #define BRIDGE_COMPONENT 5
   #define FILE_SERVER_COMPONENT 6
   #define NONDEDICATED_IPX_SPX_COMPONENT 7
   #define IPX_ONLY 8

   #define NO_ERRORS       0x00
   #define COULD_NOT_GET_LOCAL_TARGET      0xFF
   #define COULD_NOT_OPEN_SOCKET  0xFE
   #define COULD_NOT_BEGIN_CONNECTION      0xFD
   #define COULD_NOT_ESTABLISH_CONNECTION  0xFC
   #define COULD_NOT_TERMINATE_CONNECTION  0xFB
   #define BAD_CONNECTION_ID   0xFA
   #define COULD_NOT_SEND_REQUEST  0xF9
   #define RECEIVED_REPLY_IN_ERROR  0xF8
   #define NETWORK_NOT_FOUND       0xEF
   #define NO_RESPONSE_FROM_DESTINATION    0xEE
   #define RESPONSE_COMPLETION_CODE_BAD    0xED
   #define LIST_SIZE_TOO_SMALL     0xEC
   #define INTERNAL_LIST_ERROR     0xEB
   #define NODE_NOT_FOUND_OR_NO_RESPONSE   0xEA
   #define MEMORY_ALLOCATION_ERROR 0xDF
   #define VERSION_DOES_NOT_SUPPORT 0xDE

   typedef struct IPXAddress BeginDiagnosticStruct;

   typedef struct StructIPXPacket
   {
           BYTE    data[MAX_IPX_PACKET_SIZE - sizeof(IPXHeader)];
   } IPXPacket;

   typedef struct StructSPXPacket
   {
           BYTE    data[MAX_IPX_PACKET_SIZE - sizeof(SPXHeader)];
   } SPXPacket;

   typedef struct StructExclusionList
   {
           BYTE    nodeAddress[6];
   }ExclusionListStruct;

   typedef struct StructExclusionPacket
   {
           BYTE                    numberOfExclusions;
           ExclusionListStruct     structureArray[MAX_EXCLUSIONS];
   }ExclusionPacketStructure;

   typedef struct StructAddr
   {
           BYTE    network[4];
           BYTE    node[6];
   } AddrStruct;

   struct StructDriver
   {
           BYTE    localNetworkType;
           BYTE    network[4];
           BYTE    node[6];
   };

   typedef struct StructBridge
   {
           BYTE                    numberOfNets;
           struct StructDriver     bridge[MAX_LOCAL_NETWORKS];
   } BridgeStruct;

   typedef struct StructNode
   {
           char                    name[12];
           BeginDiagnosticStruct   target;
           BYTE                    numberOfComponents;
           BYTE                    componentID[MAX_COMPONENTS];
           BridgeStruct            *bridgePtr;
   } NodeStruct;

   typedef struct StructConfigurationResponse
   {
           AddrStruct      address;
           struct
           {
                   BYTE    majorVersion;
                   BYTE    minorVersion;
                   WORD    SPXDiagnosticSocket;
                   BYTE    numberOfComponents;
                   BYTE    componentStructure[MAX_IPX_PACKET_SIZE  -
                                   sizeof(IPXHeader) - sizeof(AddrStruct) - 3];
           } packet;
   } ConfigurationResponseStruct;

   typedef struct StructAllResp
   {
           BYTE    completionCode;
           long    intervalMarker;
   } AllResponseData;

   typedef struct StructIPXSPXVersion
   {
           BYTE    IPXMajorVersion;
           BYTE    IPXMinorVersion;
           BYTE    SPXMajorVersion;
           BYTE    SPXMinorVersion;
   } IPXSPXVersion;

   typedef struct StructIPXStatistics
   {
           long    sendPacketCount;
           WORD    malformedPacketCount;
           long    getECBRequestCount;
           long    getECBFailureCount;
           long    AESEventCount;
           WORD    postponedAESEventCount;
           WORD    maxConfiguredSocketsCount;
           WORD    maxOpenSocketsCount;
           WORD    openSocketFailureCount;
           long    listenECBCount;
           WORD    ECBCancelFailureCount;
           WORD    findRouteFailureCount;
   } IPXStatisticsStruct;

   typedef struct StructSPXStatistics
   {
           WORD    maxConnectionsCount;
           WORD    maxUsedConnectionsCount;
           WORD    establishConnectionRequest;
           WORD    establishConnectionFailure;
           WORD    listenConnectionRequestCount;
           WORD    listenConnectionFailureCount;
           long    sendPacketCount;
           long    windowChokeCount;
           WORD    badSendPacketCount;
           WORD    sendFailureCount;
           WORD    abortConnectionCount;
           long    listenPacketCount;
           WORD    badListenPacketCount;
           long    incomingPacketCount;
           WORD    badIncomingPacketCount;
           WORD    suppressedPacketCount;
           WORD    noSessionListenECBCount;
           WORD    watchdogDestroySessionCount;
   } SPXStatisticsStruct;

   typedef struct SPReq
   {
           BeginDiagnosticStruct   target;
           BYTE    immediateAddress[6];
           WORD    numberOfPackets;
           BYTE    timerTickInterval;
           BYTE    packetsPerTickInterval;
           WORD    packetSize;
           WORD    changeSize;
   } SendPacketsRequestStruct;

   typedef struct SPResp
   {
           WORD    numberOfTransmitErrors;
   } SendPacketsResponseStruct;

   typedef struct StructStartCountingPackets
   {
           WORD    destinationSocket;
   } StartCountingPacketsStruct;

   typedef struct StructReturnReceivedPacket
   {
           WORD    packetsReceived;
   } ReturnReceivedPacketStruct;

   typedef BYTE status;

   typedef struct StructBridgeDriverStatus
   {
           status  LANBoard[4];
   } BridgeDriverStatusStruct;

   typedef struct StructDriverConf
   {
           BYTE    networkAddress[4];
           BYTE    nodeAddress[6];
           BYTE    LANMode;
           BYTE    nodeAddressType;
           WORD    maxDataSize;
           WORD    reserved1;
           BYTE    LANHardwareID;
           WORD    transportTime;
           BYTE    reserved2[11];
           BYTE    majorVersion;
           BYTE    minorVersion;
           BYTE    ethernetFlagBits;
           BYTE    selectedConfiguration;
           BYTE    LANDescription[80];
           WORD    IOAddress1;
           WORD    IODecodeRange1;
           WORD    IOAddress2;
           WORD    IODecodeRange2;
           BYTE    memoryAddress1[3];
           WORD    memoryDecodeRange1;
           BYTE    memoryAddress2[3];
           WORD    memoryDecodeRange2;
           BYTE    interruptIsUsed1;
           BYTE    interruptLine1;
           BYTE    interruptIsUsed2;
           BYTE    interruptLine2;
           BYTE    DMAIsUsed1;
           BYTE    DMALine1;
           BYTE    DMAIsUsed2;
           BYTE    DMALine2;
           BYTE    microChannelFlagBits;
           BYTE    reserved3;
           BYTE    textDescription[80];
   } DriverConfigurationStruct;

   typedef struct StructDriverStat
   {
           BYTE    driverVersion[2];
           BYTE    statisticsVersion[2];
           long    totalTxPacketCount;
           long    totalRxPacketCount;
           WORD    noECBAvailableCount;
           WORD    packetTxTooBigCount;
           WORD    packetTxTooSmallCount;
           WORD    packetRxOverflowCount;
           WORD    packetRxTooBigCount;
           WORD    packetRxTooSmallCount;
           WORD    packetTxMiscErrorCount;
           WORD    packetRxMiscErrorCount;
           WORD    retryTxCount;
           WORD    checksumErrorCount;
           WORD    hardwareRxMismatchCount;
           WORD    numberOfCustomVariables;
           BYTE    variableData[495];
  /*       BYTE    variableData[1]; */
   } DriverStatisticsStruct;

   typedef struct StructOSVersion
   {
           BYTE    machineID;
           BYTE    versionData[41];
   } OSVersionStruct;

   typedef struct IPXAddress ShellAddressStruct;

   typedef struct StructShellStatistics
   {
           long    shellRequestsCount;
           WORD    operatorAbortsCount;
           WORD    operatorRetriesCount;
           WORD    timeoutsCount;
           WORD    writeErrorCount;
           WORD    invalidReplyHeaderCount;
           WORD    invalidSlotCount;
           WORD    invalidSequenceNumberCount;
           WORD    errorReceivingCount;
           WORD    noRouterFoundCount;
           WORD    beingProcessedCount;
           WORD    unknownErrorCount;
           WORD    invalidServerSlotCount;
           WORD    networkGoneCount;
           WORD    reserved1;
           WORD    allocateCannotFindRouteCount;
           WORD    allocateNoSlotsAvailableCount;
           WORD    allocateServerIsDownCount;
   } ShellStatisticsStruct;

   typedef struct StructAddressTable
   {
           BYTE    serverUsed;
           BYTE    orderNumber;
           BYTE    serverNetwork[4];
           BYTE    serverNode[6];
           WORD    serverSocket;
           WORD    receivedTimeOut;
           BYTE    immediateNode[6];
           BYTE    sequenceNumber;
           BYTE    connectionNumber;
           BYTE    connectionOK;
           WORD    maximumTimeOut;
           BYTE    reserved[5];
   } AddressTableStruct;

   typedef struct StructServerAddressTable
   {
           AddressTableStruct      addressTable[8];
   } ServerAddressTableStruct;

   typedef struct StructNameTable
   {
           BYTE    name[48];
   } NameTableStruct;

   typedef struct StructServerNameTable
   {
           NameTableStruct         nameTable[8];
   } ServerNameTableStruct;

   typedef struct StructPrimaryServer
   {
           BYTE    number;
   } PrimaryServerStruct;

   typedef struct StructShellVersion
   {
           BYTE    minor;
           BYTE    major;
           BYTE    rev;
   } ShellVersionStruct;

   typedef struct StructBridgeStatistics
   {
           WORD    tooManyHopsCount;
           WORD    unknownNetworkCount;
           WORD    noSpaceForServiceCount;
           WORD    noReceiveBuffersCount;
           WORD    notMyNetwork;
           long    netBIOSPropogateCount;
           long    totalPacketsServiced;
           long    totalPacketsRouted;
   } BridgeStatisticsStruct;

   typedef struct StructNumber
   {
           BYTE    number[4];
   } NumberStruct;

   typedef struct StructNodeAddress
   {
           BYTE    address[6];
           BYTE    reserved[2];
   } NodeAddressStruct;

   typedef struct StructLocalTables
   {
           NumberStruct            localNetworkNumber[16];
           NodeAddressStruct       localNodeAddress[16];
   } LocalTablesStruct;

   typedef struct StructNetworkAddress
   {
           BYTE    address[4];
   } NetworkAddressStruct;

   typedef struct StructAllKnownNetworks
   {
           WORD    numberOfNetworkAddresses;
           NetworkAddressStruct    networkAddress[128];
   } AllKnownNetworksStruct;

   typedef struct StructRoutingInfo
   {
           BYTE    routerForwardingAddress[6];
           BYTE    routerBoardNumber;
           BYTE    reserved[2];
           BYTE    routeHops;
           WORD    routeTime;
   } RoutingInfoStruct;

   typedef struct StructSpecificNetInfo
   {
           BYTE    networkAddress[4];
           BYTE    hopsToNet;
           BYTE    reservedA[7];
           WORD    routeTimeToNet;
           WORD    numberOfKnownRouters;
           RoutingInfoStruct       routingInfo[MAX_ROUTES];
   } SpecificNetworkInfoStruct;

   typedef struct StrSrvrInfo
   {
           WORD    serverType;
           BYTE    serverName[48];
   } ServerInfoStruct;

   typedef struct StructAllKnownServers
   {
           WORD    numberOfServers;
           ServerInfoStruct        serverInfo[10];
   } AllKnownServersStruct;

   typedef struct StructRouteSourceInfo
   {
           BYTE    routeSourceAddress[6];
           WORD    routeHopsToSource;
           BYTE    reserved[2];
   } RouteSourceInfoStruct;

   typedef struct StrSpecSrvrInfo
   {
           ServerInfoStruct        serverInfo;
           BYTE    serverAddress[12];
           WORD    hopsToServer;
           BYTE    reserved1[2];
           WORD    numberOfRoutes;
           RouteSourceInfoStruct   routeSourceInfo[MAX_ROUTES];
   } SpecificServerInfoStruct;

   typedef struct StructConnectionStatusBlock
   {
           BYTE    connectionState;
           BYTE    connectionFlags;
           BYTE    sourceConnectionID[2];            /* hi-lo */
           BYTE    destinationConnectionID[2];       /* hi-lo */
           BYTE    sequenceNumber[2];                /* hi-lo */
           BYTE    acknowledgeNumber[2];             /* hi-lo */
           BYTE    allocationNumber[2];              /* hi-lo */
           BYTE    remoteAcknowledgeNumber[2];       /* hi-lo */
           BYTE    remoteAllocationNumber[2];        /* hi-lo */
           WORD    connectionSocket;                 /* hi-lo */
           BYTE    immediateAddress[6];              /* hi-lo */
           struct IPXAddress       destination;
           BYTE    retransmissionCount[2];           /* hi-lo */
           BYTE    estimatedRoundTripDelay[2];       /* hi-lo */
           BYTE    retransmittedPackets[2];          /* hi-lo */
           BYTE    suppressedPackets[2];             /* hi-lo */
   } ConnectionStatusBlock;


   #ifdef PROTOTYPE
     extern int BeginDiagnostics(BeginDiagnosticStruct *,WORD *, BYTE *);
     extern int EndDiagnostics(WORD );
     extern int GetRemoteSPXSocket(BeginDiagnosticStruct *, BYTE *);
     extern int SendSPXPacket(WORD ,BYTE *,WORD );
     extern int GetDiagnosticResponse(WORD ,BYTE *,WORD ,BYTE *,WORD );
     extern int GetDiagnosticStatus(WORD );
     extern int GetIPXSPXVersion(WORD ,BYTE ,AllResponseData *,IPXSPXVersion *);
     extern int GetIPXStatistics(WORD ,BYTE ,AllResponseData *,IPXStatisticsStruct *);
     extern int GetSPXStatistics(WORD ,BYTE ,AllResponseData *,SPXStatisticsStruct *);
     extern int StartCountingPkts(WORD ,BYTE ,AllResponseData *,StartCountingPacketsStruct *);
     extern int StartSendingPktsTimed(WORD ,BYTE ,SendPacketsRequestStruct *,AllResponseData *,SendPacketsResponseStruct *,WORD );
     extern int AbortSendingPackets(WORD ,BYTE );
     extern int ReturnReceivedPacketCount(WORD ,BYTE ,AllResponseData *,ReturnReceivedPacketStruct *);
     extern int GetBridgeDriverStatus(WORD ,BYTE ,AllResponseData *,BridgeDriverStatusStruct *);
     extern int GetBridgeDriverConfiguration(WORD ,BYTE ,BYTE ,AllResponseData *,DriverConfigurationStruct *);
     extern int GetBridgeDriverStatistics(WORD ,BYTE ,BYTE ,AllResponseData *,DriverStatisticsStruct *);
     extern int GetShellDriverConfiguration(WORD ,BYTE ,AllResponseData *,DriverConfigurationStruct *);
     extern int GetShellDriverStatistics(WORD ,BYTE ,AllResponseData *,DriverStatisticsStruct *);
     extern int GetOSVersionInfo(WORD ,BYTE ,AllResponseData *,OSVersionStruct *);
     extern int GetShellAddress(WORD ,BYTE ,AllResponseData *,ShellAddressStruct *);
     extern int GetShellStatistics(WORD ,BYTE ,AllResponseData *,ShellStatisticsStruct *);
     extern int GetServerAddressTable(WORD ,BYTE ,AllResponseData *,ServerAddressTableStruct *);
     extern int GetServerNameTable(WORD ,BYTE ,AllResponseData *,ServerNameTableStruct *);
     extern int GetPrimaryServerNumber(WORD ,BYTE ,AllResponseData *,PrimaryServerStruct *);
     extern int GetShellVersionInfo(WORD ,BYTE ,AllResponseData *,ShellVersionStruct *);
     extern int GetBridgeStatistics(WORD ,BYTE ,AllResponseData *,BridgeStatisticsStruct *);
     extern int GetLocalTables(WORD ,BYTE ,AllResponseData *,LocalTablesStruct *);
     extern int GetAllKnownNetworks(WORD ,BYTE ,WORD ,AllResponseData *,AllKnownNetworksStruct *);
     extern int GetSpecificNetworkInfo(WORD ,BYTE ,BYTE *,AllResponseData *,SpecificNetworkInfoStruct *);
     extern int GetAllKnownServers(WORD ,BYTE ,WORD ,AllResponseData *,AllKnownServersStruct *);
     extern int GetSpecificServerInfo(WORD ,BYTE ,ServerInfoStruct *,AllResponseData *,SpecificServerInfoStruct *);
     extern int ReinitializeRouterTables(WORD ,BYTE ,BYTE *,AllResponseData *);
   #else
     extern int BeginDiagnostics();
     extern int EndDiagnostics();
     extern int GetRemoteSPXSocket();
     extern int SendSPXPacket();
     extern int GetDiagnosticResponse();
     extern int GetDiagnosticStatus();
     extern int GetIPXSPXVersion();
     extern int GetIPXStatistics();
     extern int GetSPXStatistics();
     extern int StartCountingPkts();
     extern int StartSendingPktsTimed();
     extern int AbortSendingPackets();
     extern int ReturnReceivedPacketCount();
     extern int GetBridgeDriverStatus();
     extern int GetBridgeDriverConfiguration();
     extern int GetBridgeDriverStatistics();
     extern int GetShellDriverConfiguration();
     extern int GetShellDriverStatistics();
     extern int GetOSVersionInfo();
     extern int GetShellAddress();
     extern int GetShellStatistics();
     extern int GetServerAddressTable();
     extern int GetServerNameTable();
     extern int GetPrimaryServerNumber();
     extern int GetShellVersionInfo();
     extern int GetBridgeStatistics();
     extern int GetLocalTables();
     extern int GetAllKnownNetworks();
     extern int GetSpecificNetworkInfo();
     extern int GetAllKnownServers();
     extern int GetSpecificServerInfo();
     extern int ReinitializeRouterTables();
    #endif

#endif
