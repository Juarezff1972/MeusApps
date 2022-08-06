/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef _NWCONSOL_H
#define _NWCONSOL_H
/*_____________________________________________________________________________

   Definitions and structures for the Netware API File Server Enviroment Logic
  ____________________________________________________________________________*/

#ifndef _PROLOG_H
   #include <prolog.h>
#endif

/* NIT buffer IDs */

#define RI_UNDEFINED                        -1
#define RI_GETCONNECTIONSOPENFILES           1
#define RI_GETCONNECTIONSSEMAPHORES          2
#define RI_GETCONNECTIONSTASKINFO            3
#define RI_GETCURRENTACCOUNTSTATUS           4
#define RI_GETLOGICALRECORDINFORMATION       5
#define RI_GETLOGICALRECORDSBYCONNECTIO      6
#define RI_GETPHYSICALRECORDLOCKSBYFILE      7
#define RI_GETPHYSRECLOCKBYCONNECTANDFI      8
#define RI_GETSEMAPHOREINFORMATION           9
#define RI_TTSGETSTATS                      10
#define RI_GETCONNECTIONSUSINGFILE          11

/* Maximum size of NetWare buffers in bytes */

#define MAX_NETWARE_BUFFER_SIZE             512

/* NIT buffer variables */

extern char *NITBufferPtr;
extern char  NITBuffer[];

/* NIT buffer free macro */

#ifdef _VAP_
   #define FreeNITBuffer(buf)
#else
   #define FreeNITBuffer(buf)  free(((char *)buf)-sizeof(int)/sizeof(char))
#endif

typedef struct
 {
    int     openCount;
    char    semaphoreValue;
    BYTE    taskNumber;
    char    semaphoreName[128];     /* ASCIIZ */
 }CONN_SEMAPHORE;

typedef struct
 {
    BYTE    lockStatus;
    BYTE    waitingTaskNumber;  /* valid only if lockStatus != 0 */
    long    beginAddress;       /* valid only if lockStatus == 1 */
    long    endAddress;         /* valid only if lockStatus == 1 */
    BYTE    volumeNumber;       /* valid only if lockStatus == 1 or 2 */
    BYTE    numberOfTasks;
    WORD    directoryEntry;     /* valid only if lockStatus == 1 or 2 */
    char    lockedName[50];     /* valid only if lockStatus != 0
                if lockStatus == 1 or 2, this is a file name
                if lockStatus == 3, this is a record name
                if lockStatus == 4, this is a semaphore name */
    BYTE    taskNumber;
    BYTE    taskState;          /* TState_TTSEXPLICIT, TState_TTSIMPLICIT,
                                   TState_FileSetLock */
 }CONN_TASK_INFO;


typedef struct
 {
    long    systemElapsedTime;
    BYTE    bytesRead[6];
    BYTE    bytesWritten[6];
    long    totalRequestPackets;
 }CONN_USAGE;

typedef struct
 {
    WORD    useCount;
    WORD    openCount;
    WORD    openForReadCount;
    WORD    openForWriteCount;
    WORD    denyReadCount;
    WORD    denyWriteCount;
    WORD    reserved1;
    BYTE    locked;
    BYTE    reserved2;
    WORD    logicalConnNumber;
    BYTE    taskNumber;
    BYTE    lockType;
    BYTE    accessFlags;
    BYTE    lockStatus;
 }CONN_USING_FILE;



typedef struct
 {
    long    systemElapsedTime;
    int     cacheBufferCount;
    int     cacheBufferSize;
    int     dirtyCacheBuffers;
    long    cacheReadRequests;
    long    cacheWriteRequests;
    long    cacheHits;
    long    cacheMisses;
    long    physicalReadRequests;
    long    physicalWriteRequests;
    WORD    physicalReadErrors;
    WORD    physicalWriteErrors;
    long    cacheGetRequests;
    long    cacheFullWriteRequests;
    long    cachePartialWriteRequests;
    long    backgroundDirtyWrites;
    long    backgroundAgedWrites;
    long    totalCacheWrites;
    long    cacheAllocations;
    WORD    thrashingCount;
    WORD    LRUBlockWasDirtyCount;
    WORD    readBeyondWriteCount;
    WORD    fragmentedWriteCount;
    WORD    cacheHitOnUnavailCount;
    WORD    cacheBlockScrappedCount;
 }DISK_CACHE_STATS;



#ifdef ANSI
   #pragma pack(1)
#endif

typedef struct
 {
   long      systemElapsedTime;             /* 002 */
   int       channelState;  /* DCS_RUNNING, DCS_BEINGSTOPPED,
                            DCS_STOPPED, DCS_NONFUNCTIONAL */
   int       channelSyncState;              /* 008 */
   BYTE      driverType;                    /* 010 */
   BYTE      driverMajorVersion;            /* 011 */
   BYTE      driverMinorVersion;            /* 012 */
   char      driverDescription[65];         /* 013 */
   WORD      IOAddr1;                       /* 078 */
   WORD      IOAddr1Size;                   /* 080 */
   WORD      IOAddr2;                       /* 082 */
   WORD      IOAddr2Size;                   /* 084 */
   BYTE      sharedMem1Seg[3];       /*hi-low-middle format*/
   WORD      sharedMem1Ofs;                 /* 089 */
   BYTE      sharedMem2Seg[3];       /*hi-low-middle format*/
   WORD      sharedMem2Ofs;                 /* 094 */
   BYTE      interrupt1Used;                /* 096 */
   BYTE      interrupt1;                    /* 097 */
   BYTE      interrupt2Used;                /* 098 */
   BYTE      interrupt2;                    /* 099 */
   BYTE      DMAChannel1Used;               /* 100 */
   BYTE      DMAChannel1;
   BYTE      DMAChannel2Used;
   BYTE      DMAChannel2;
   int       reserved2;
   char      configDescription[80];
 }DISK_CHANNEL_STATS;



#ifdef ANSI
   #pragma pack()
#endif


typedef struct
 {
   long      systemElapsedTime;
   BYTE      SFTLevel;
   BYTE      logicalDriveCount;
   BYTE      physicalDriveCount;
   BYTE      diskChannelTable[5];
   int       pendingIOCommands;
   BYTE      mappingTable[32];
   BYTE      driveMirrorTable[32];
   BYTE      deadMirrorTable[32];
   BYTE      remirroredDrive;
   BYTE      reserved;
   long      remirroredBlock;
   int       SFTErrorTable[60];
 }DRIVE_MAP_TABLE;



typedef struct
 {
   long      systemElapsedTime;
   int       maxOpenFiles;
   int       maxFilesOpened;
   int       currOpenFiles;
   long      totalFilesOpened;
   long      totalReadRequests;
   long      totalWriteRequests;
   int       currChangedFATSectors;
   long      totalChangedFATSectors;
   int       FATWriteErrors;
   int       fatalFATWriteErrors;
   int       FATScanErrors;
   int       maxIndexFilesOpened;
   int       currOpenIndexedFiles;
   int       attachedIndexFiles;
   int       availableIndexFiles;
 }FILE_SYS_STATS;



typedef struct
 {
    BYTE    networkAddress[4];
    BYTE    hostAddress[6];
    BYTE    LANDriverInstalled;
    BYTE    optionNumber;
    char    configurationText1[80];
    char    configurationText2[80];
 }LAN_CONFIG;



typedef struct
 {
    WORD    currentUseCount;
    WORD    shareableLockCount;
    WORD    reserved1;
    BYTE    locked;
    BYTE    reserved2;
    WORD    logicalConnectionNumber;
    BYTE    taskNumber;
    BYTE    lockStatus;
 }LOGICAL_REC_INFO;



typedef struct
 {
    BYTE    taskNumber;
    BYTE    lockStatus;
    char    logicalLockName[102];
 } LOGICAL_RECORD;



typedef struct
 {
    BYTE      taskNumber;
    BYTE      lockStatus;
    BYTE      accessFlag;
    BYTE      lockType;
    BYTE      volumeNumber;
    BYTE      reserved;
    WORD      directoryEntry;           /* THIS IS NOT A DIRECTORY HANDLE */
    char      fileName[15];
 }CON_OPEN_FILES;



typedef struct
 {
   long      systemElapsedTime;
   BYTE      diskChannel;
   BYTE      diskRemovable;
   BYTE      driveType;
   BYTE      controllerDriveNumber;
   BYTE      controllerNumber;
   BYTE      controllerType;
   long      driveSize;          /* in 4096 byte blocks */
   int       driveCylinders;
   BYTE      driveHeads;
   BYTE      sectorsPerTrack;
   int       IOErrorCount;
   long      hotFixStart;        /* only meaningful with SFT I or greater */
   int       hotFixSize;         /* only meaningful with SFT I or greater */
   int       hotFixBlockAvailable;/* only meaningful with SFT I or greater */
   BYTE      hotFixDisabled;     /* only meaningful with SFT I or greater */
 }PHYS_DISK_STATS;



typedef struct
 {
    BYTE    physicalRecordLockCount;
    BYTE    reserved;
    int     loggedCount;
    int     shareLockCount;
    long    recordStart;
    long    recordEnd;
    WORD    connectionNumber;
    BYTE    taskNumber;
    BYTE    lockType;
 }PHYS_REC_LOCK;



typedef struct
 {
   long      systemElapsedTime;
   int       maxRoutingBuffersAvail;
   int       maxRoutingBuffersUsed;
   int       routingBuffersInUse;
   long      totalFileServicePackets;
   int       fileServicePacketsBuffered;
   int       invalidConnPacketCount;
   int       badLogicalConnCount;
   int       packetsRcvdDuringProcCount;
   int       reprocessedRequestCount;
   int       badSequenceNumberPacketCount;
   int       duplicateReplyCount;
   int       acknowledgementsSent;
   int       badRequestTypeCount;
   int       attachDuringProcCount;
   int       attachWhileAttachingCount;
   int       forgedDetachRequestCount;
   int       badConnNumberOnDetachCount;
   int       detachDuringProcCount;
   int       repliesCanceledCount;
   int       hopCountDiscardCount;
   int       unknownNetDiscardCount;
   int       noDGroupBufferDiscardCount;
   int       outPacketNoBufferDiscardCount;
   int       IPXNotMyNetworkCount;
   long      NetBIOSPropagationCount;
   long      totalOtherPackets;
   long      totalRoutedPackets;
 }SERVER_LAN_IO;

typedef struct
 {
   long      systemElapsedTime;
   BYTE      processorType;
   BYTE      reserved;
   BYTE      serviceProcessCount;
   BYTE      serverUtilizationPercent;
   WORD      maxBinderyObjectsAvail;
   WORD      maxBinderyObjectsUsed;
   WORD      binderyObjectsInUse;
   int       serverMemoryInK;
   int       serverWastedMemoryInK;
   int       dynamicAreaCount;
   long      dynamicSpace1;
   long      maxUsedDynamicSpace1;
   long      dynamicSpaceInUse1;
   long      dynamicSpace2;
   long      maxUsedDynamicSpace2;
   long      dynamicSpaceInUse2;
   long      dynamicSpace3;
   long      maxUsedDynamicSpace3;
   long      dynamicSpaceInUse3;
 }SERVER_MISC_INFO;


typedef struct
 {
    char    serverName[48];
    BYTE    netwareVersion;
    BYTE    netwareSubVersion;
    WORD    maxConnectionsSupported;
    WORD    connectionsInUse;
    WORD    maxVolumesSupported;
    BYTE    revisionLevel;
    BYTE    SFTLevel;
    BYTE    TTSLevel;
    WORD    peakConnectionsUsed;
    BYTE    accountingVersion;
    BYTE    VAPversion;
    BYTE    queingVersion;
    BYTE    printServerVersion;
    BYTE    virtualConsoleVersion;
    BYTE    securityRestrictionLevel;
    BYTE    internetBridgeSupport;
  }FILE_SERV_INFO;


typedef struct
 {
    BYTE    taskNumber;
    BYTE    lockFlag;
    long    recordStart;
    long    recordEnd;
 }SHORT_PHYS_REC_LOCK;



typedef struct
 {
    long    systemElapsedTime;
    BYTE    TTS_Supported;
    BYTE    TTS_Enabled;
    int     TTS_VolumeNumber;
    int     TTS_MaxOpenTransactions;
    int     TTS_MaxTransactionsOpened;
    int     TTS_CurrTransactionsOpen;
    long    TTS_TotalTransactions;
    long    TTS_TotalWrites;
    long    TTS_TotalBackouts;
    int     TTS_UnfilledBackouts;
    int     TTS_DiskBlocksInUse;
    long    TTS_FATAllocations;
    long    TTS_FileSizeChanges;
    long    TTS_FilesTruncated;
    BYTE    numberOfTransactions;
    BYTE    connectionNumber;
    BYTE    taskNumber;
 }TTS_STATS;


/****************************************************************************/

#ifdef PROTOTYPE
   extern int    CheckConsolePrivileges(void);
   extern int    CheckNetWareVersion(WORD, WORD, WORD, WORD, WORD);
   extern int    ClearConnectionNumber(WORD);
   extern int    DisableFileServerLogin(void);
   extern int    DisableTransactionTracking(void);
   extern int    DownFileServer(int);
   extern int    EnableFileServerLogin(void);
   extern int    EnableTransactionTracking(void);
   extern int    GetBinderyObjectDiskSpaceLeft(WORD, long, long *, long *, int *);
   extern int   _GetConnectionsOpenFiles(WORD, WORD, int *);
   extern int    GetConnectionsOpenFiles(WORD, WORD, int *, int *, int, CON_OPEN_FILES *);
   extern int   _GetConnectionsSemaphores(WORD, WORD, int *);
   extern int    GetConnectionsSemaphores(WORD, WORD, int *, int *, int, CONN_SEMAPHORE *);
   extern int   _GetConnectionsTaskInformation(WORD, WORD);
   extern int    GetConnectionsTaskInformation(WORD, WORD, int *, int, CONN_TASK_INFO *);
   extern int    GetConnectionsUsageStats(WORD, WORD, int, CONN_USAGE *);
   extern int    GetConnectionsUsingFile(WORD, int *, int *, BYTE, char *, int, CONN_USING_FILE *);
   extern int   _GetConnectionsUsingFile(WORD, int *, BYTE, char *);
   extern int    GetDiskCacheStats(WORD, int, DISK_CACHE_STATS *);
   extern int    GetDiskChannelStats(WORD, int, int, DISK_CHANNEL_STATS *);
   extern int    GetDiskUtilization(long, BYTE, LONG *, LONG *, LONG *);
   extern int    GetDriveMappingTable(WORD, int, DRIVE_MAP_TABLE *);
   extern void cdecl  GetFileServerDateAndTime(BYTE *);
   extern int    GetFileServerDescriptionStrings(char *, char *, char *, char *);
   extern int    GetFileServerExtendedInfo(BYTE *, BYTE *, BYTE *, BYTE *, BYTE *, BYTE *, BYTE *);
   extern int    GetFileServerInformation(char *, BYTE *, BYTE *, WORD *, WORD *, WORD *, BYTE *, BYTE *, BYTE *, WORD *);
   extern int    GetFileServerLANIOStats(WORD, int, SERVER_LAN_IO *);
   extern int    GetFileServerLoginStatus(int *);
   extern int    GetFileServerMiscInformation(WORD, int, SERVER_MISC_INFO *);
   extern int    GetFileSystemStats(WORD, int, FILE_SYS_STATS *);
   extern int    GetLANDriverConfigInfo(WORD, BYTE, int, LAN_CONFIG *);
   extern int    GetLogicalRecordInformation(WORD, char *, int *, int *, int, LOGICAL_REC_INFO *);
   extern int   _GetLogicalRecordInformation(WORD, char *, int * );
   extern int    GetLogicalRecordsByConnection( WORD, WORD, int *, int *, int, LOGICAL_RECORD * );
   extern int   _GetLogicalRecordsByConnection( WORD, WORD, int * );
   extern int    GetPathFromDirectoryEntry(WORD, BYTE, WORD, char *);
   extern int    GetPhysicalDiskStats(WORD, BYTE, int, PHYS_DISK_STATS *, char *);
   extern int   _GetPhysicalRecordLocksByFile(WORD, WORD, char *, int *);
   extern int    GetPhysicalRecordLocksByFile(WORD, WORD, char *, int *, int *, int, PHYS_REC_LOCK *);
   extern int   _GetPhysRecLockByConnectAndFile(WORD, WORD, BYTE, WORD, char *, int *);
   extern int    GetPhysRecLockByConnectAndFile(WORD, WORD, BYTE, WORD, char *, int *, int *, int, SHORT_PHYS_REC_LOCK *);
   extern int   _GetSemaphoreInformation(WORD, char *, int *);
   extern int    GetSemaphoreInformation(WORD, char *, int *, int *, int *, char *, WORD *, BYTE *);
   extern int    GetServerInformation(int, FILE_SERV_INFO * );
   extern int    SendConsoleBroadcast(char *, WORD, WORD *);
   extern int    cdecl  _ServerRequest(BYTE, BYTE, BYTE);
   extern int    SetFileServerDateAndTime(WORD, WORD, WORD, WORD, WORD, WORD);
#else
   extern int    CheckConsolePrivileges();
   extern int    CheckNetWareVersion();
   extern int    ClearConnectionNumber();
   extern int    DisableFileServerLogin();
   extern int    DisableTransactionTracking();
   extern int    DownFileServer();
   extern int    EnableFileServerLogin();
   extern int    EnableTransactionTracking();
   extern int    GetBinderyObjectDiskSpaceLeft();
   extern int   _GetConnectionsOpenFiles();
   extern int    GetConnectionsOpenFiles();
   extern int   _GetConnectionsSemaphores();
   extern int    GetConnectionsSemaphores();
   extern int   _GetConnectionsTaskInformation();
   extern int    GetConnectionsTaskInformation();
   extern int    GetConnectionsUsageStats();
   extern int    GetConnectionsUsingFile();
   extern int   _GetConnectionsUsingFile();
   extern int    GetDiskCacheStats();
   extern int    GetDiskChannelStats();
   extern int    GetDiskUtilization();
   extern int    GetDriveMappingTable();
   extern void cdecl  GetFileServerDateAndTime();
   extern int    GetFileServerDescriptionStrings();
   extern int    GetFileServerExtendedInfo();
   extern int    GetFileServerInformation();
   extern int    GetFileServerLANIOStats();
   extern int    GetFileServerLoginStatus();
   extern int    GetFileServerMiscInformation();
   extern int    GetFileSystemStats();
   extern int    GetLANDriverConfigInfo();
   extern int    GetLogicalRecordInformation();
   extern int   _GetLogicalRecordInformation();
   extern int    GetLogicalRecordsByConnection();
   extern int   _GetLogicalRecordsByConnection();
   extern int    GetPathFromDirectoryEntry();
   extern int    GetPhysicalDiskStats();
   extern int   _GetPhysicalRecordLocksByFile();
   extern int    GetPhysicalRecordLocksByFile();
   extern int   _GetPhysRecLockByConnectAndFile();
   extern int    GetPhysRecLockByConnectAndFile();
   extern int   _GetSemaphoreInformation();
   extern int    GetSemaphoreInformation();
   extern int    GetServerInformation();
   extern int    SendConsoleBroadcast();
   extern int    cdecl  _ServerRequest();
   extern int    SetFileServerDateAndTime();
#endif

#endif
