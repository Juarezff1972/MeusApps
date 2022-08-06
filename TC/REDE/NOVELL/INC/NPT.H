/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */

#ifndef _NPT_H
#define _NPT_H

#ifndef _NIT_H
#include <nit.h>
#endif

#ifndef _NXT_H
#include <nxt.h>
#endif

#ifdef __WATCOMC__
#include <stdarg.h>
#include <stddef.h>
#endif

typedef struct
 {
        BYTE            clientStation;
        BYTE            clientTaskNumber;
        long            clientIDNumber;
        long            targetServerIDNumber;
        BYTE            targetExecutionTime[6];
        BYTE            entryTime[6];
        WORD            jobNumber;
        WORD            formNumber;
        BYTE            jobPosition;
        BYTE            jobControlFlags;
        BYTE            fileName[14];
        BYTE            fileHandle[6];
        BYTE            serverStation;
        BYTE            serverTaskNumber;
        long            serverIDNumber;
        BYTE            jobDescription[50];
        BYTE            versionNumber;
        BYTE            tabSize;
        WORD            numberOfCopies;
        WORD            printControlFlags;
        WORD            maxLinesPerPage;
        WORD            maxCharsPerLine;
        BYTE            formName[16];
        BYTE            reserve[6];
        BYTE            bannerNameField[13];
        BYTE            bannerFileField[13];
        BYTE            headerFileName[14];
        BYTE            directoryPath[80];
 } PRINT_JOB;


/*      Printer configuration file structure               */

typedef struct
 {
        char name[48];            /* Name of printer                        */
        WORD printerSubtype;      /* Subtype of printer                     */
        WORD useInterrupts;       /* Use interrupts or polling?             */
        WORD irqNumber;           /* IRQ number for printer interrupt       */
        WORD serviceMode;         /* Queue service mode                     */
        WORD bufferSize;          /* Buffer size in K                       */
        WORD baudRate;            /* Baud rate (Serial only)                */
        WORD dataBits;            /* Data bits (Serial only)                */
        WORD stopBits;            /* Stop bits (Serial only)                */
        WORD parity;              /* Parity type (Serial only)              */
        WORD useXonXoff;          /* Use X-On/X-Off protocol? (Serial only) */
        WORD currentForm;         /* Currently mounted form                 */
 } PCONFIG;

/*  Print Server information structure returned by PSGetPrintServerInfo */

typedef struct
 {
    BYTE  status;              /* Print server status                     */
    BYTE  numPrinters;         /* Number of attached printers             */
    BYTE  numModes;            /* Number of queue service modes           */
    BYTE  majorVersion;        /* Print server protocol, major version    */
    BYTE  minorVersion;        /* Print server protocol, minor version    */
    BYTE  revision;            /* Print server protocol, revision         */
    BYTE  serialNumber[4];     /* Serial number in BCD                    */
    BYTE  serverType;          /* Print Server Type                       */
                               /* 0 - Unknown                             */
                               /* 1 - Dedicate print server for DOS       */
                               /* 2 - NetWare Loadable Module             */
                               /* 3 - Value added process, in file server */
                               /* 4 - Value added process, in Bridge      */
 } PS_INFO;

/*      Possible values of server status             */

#define RUNNING           0       /* Running                                */
#define GOING_DOWN        1       /* Ready to quit when current jobs finish */
#define DOWN              2       /* Ready to quit                          */
#define INITIALIZING      3       /* Initialization in progress             */

/*      Values of printer status  */

#define PRINTER_RUNNING         0       /* Printer is running               */
#define PRINTER_OFFLINE         1       /* Printer is offline               */
#define PRINTER_PAPER_OUT       2       /* Printer is out of paper          */

/*      Flags used for printControlFlags       */

#define SUPPRESS_FF             0x0800
#define NOTIFY_USER             0x1000
#define TEXT_MODE               0x4000
#define PRINT_BANNER            0x8000

/****************************************************************************/

/*      Queue service modes    */

#define QUEUE_ONLY                 0
#define QUEUE_BEFORE_FORM          1
#define FORM_ONLY                  2
#define FORM_BEFORE_QUEUE          3

typedef struct                    /* Remote printer information structure */
 {
    WORD  printerType;            /* Type of remote printer               */
    WORD  useInterrupts;          /* Should we use interrupts?            */
    WORD  irqNumber;              /* IRQ number for printer               */
    WORD  numBlocks;              /* Number of blocks in buffer           */
    WORD  useXonXoff;             /* Use Xon/Xoff?                        */
    WORD  baudRate;               /* Baud rate                            */
    WORD  dataBits;               /* Number of data bits                  */
    WORD  stopBits;               /* Number of stop bits                  */
    WORD  parity;                 /* Parity type                          */
    WORD  socket;                 /* Socket number for remote printers    */
 } REMOTE_INFO;

/*      Status packet from a remote printer   */

typedef struct
 {
    BYTE  printerNumber;      /* Printer number the request is from        */
    BYTE  needBlocks;         /* Number of new blocks needed to print      */
    BYTE  finishedBlocks;     /* Number of new blocks that are now printed */
    BYTE  status;             /* Printer online/offline or out of paper    */
    BYTE  inSideband;         /* Is remote printer printing sideband?      */
 } REMOTE_STATUS;

/*      Values for serial port control as stored in configuration files  */

#define BAUD_RATE_0300    0               /* Baud rates  */
#define BAUD_RATE_0600    1
#define BAUD_RATE_1200    2
#define BAUD_RATE_2400    3
#define BAUD_RATE_4800    4
#define BAUD_RATE_9600    5

#define STOP_BITS_1             0               /* Stop bits */
#define STOP_BITS_1_5           1
#define STOP_BITS_2             2

#define PARITY_NONE             0               /* Parity type */
#define PARITY_EVEN             1
#define PARITY_ODD              2

/*      Data stream types of data sent to remote printers   */

#define DST_DATA                0   /* Packet contains data to be printed     */
#define DST_FLUSH               1   /* Packet is a flush print buffers packet */
#define DST_PAUSE               2   /* Packet is a pause packet               */
#define DST_START               3   /* Packet is a end pause packet           */
#define DST_SIDEBAND            4   /* Packet is a print sideband packet      */
#define DST_NEW_JOB             5   /* Packet is a starting new job packet    */
#define DST_RELEASE             6   /* Packet is a release printer packet     */
#define DST_RECLAIM             7   /* Packet is a reclaim printer packet     */

/*      Printer types       */

#define P_PAR_1             0       /* Parallel port 1                      */
#define P_PAR_2             1       /* Parallel port 2                      */
#define P_PAR_3             2       /* Parallel port 3                      */
#define P_SER_1             3       /* Serial port 1                        */
#define P_SER_2             4       /* Serial port 2                        */
#define P_SER_3             5       /* Serial port 3                        */
#define P_SER_4             6       /* Serial port 4                        */
#define P_REM_PAR_1         7       /* Remote printer using parallel port 1 */
#define P_REM_PAR_2         8       /* Remote printer using parallel port 2 */
#define P_REM_PAR_3         9       /* Remote printer using parallel port 3 */
#define P_REM_SER_1        10       /* Remote printer using serial port 1   */
#define P_REM_SER_2        11       /* Remote printer using serial port 2   */
#define P_REM_SER_3        12       /* Remote printer using serial port 3   */
#define P_REM_SER_4        13       /* Remote printer using serial port 4   */
#define P_REM_OTHER        14       /* Remote printer of other/unknown type */
#define P_ELSEWHERE        15       /* Printer is defined elsewhere         */

/*      For operator notification purposes, this means notify job owner  */

#define JOB_OWNER               -1

/*      Job outcomes     */

#define PLACE_ON_HOLD           0       /* Pause job           */
#define RETURN_TO_QUEUE         1       /* Return job to queue */
#define THROW_AWAY              2       /* Throw job away      */

/*      Remote printer modes   */

#define REMOTE_SHARED   0        /* Remote printer is shared with network    */
#define REMOTE_PRIVATE  1        /* Remote printer is private to workstation */

/****************************************************************************/

/*      Print server request codes       */

#define CMD_LOGIN_TO_PRINT_SERVER               0x01
#define CMD_GET_PRINT_SERVER_INFO               0x02
#define CMD_DOWN                                0x03
#define CMD_CANCEL_DOWN                         0x04
#define CMD_GET_PRINTER_STATUS                  0x05
#define CMD_STOP_PRINTER                        0x06
#define CMD_START_PRINTER                       0x07
#define CMD_MOUNT_FORM                          0x08
#define CMD_REWIND_PRINT_JOB                    0x09
#define CMD_EJECT_PAGE                          0x0A
#define CMD_MARK_PAGE                           0x0B
#define CMD_CHANGE_SERVICE_MODE                 0x0C
#define CMD_GET_JOB_STATUS                      0x0D
#define CMD_ABORT_JOB                           0x0E
#define CMD_SCAN_QUEUE_LIST                     0x0F
#define CMD_CHANGE_QUEUE_PRIORITY               0x10
#define CMD_ADD_QUEUE                           0x11
#define CMD_DELETE_QUEUE                        0x12
#define CMD_GET_PRINTERS_FOR_QUEUE              0x13
#define CMD_SCAN_NOTIFY_LIST                    0x14
#define CMD_CHANGE_NOTIFY                       0x15
#define CMD_ADD_NOTIFY                          0x16
#define CMD_DELETE_NOTIFY                       0x17
#define CMD_ATTACH_TO_FILE_SERVER               0x18
#define CMD_DETACH_FROM_FILE_SERVER             0x19
#define CMD_GET_ATTACHED_SERVERS                0x1A
#define CMD_GET_REMOTE                          0x80
#define CMD_CONNECT_REMOTE                      0x81
#define CMD_SET_REMOTE_MODE                     0x82
#define CMD_LOGOUT                              0xFF

/****************************************************************************/

/*      Printer status codes    */

#define PSTAT_WAITING_FOR_JOB                   0
#define PSTAT_WAITING_FOR_FORM                  1
#define PSTAT_PRINTING_JOB                      2
#define PSTAT_PAUSED                            3
#define PSTAT_STOPPED                           4
#define PSTAT_MARK_EJECT                        5
#define PSTAT_READY_TO_GO_DOWN                  6
#define PSTAT_NOT_CONNECTED                     7
#define PSTAT_PRIVATE                           8

/****************************************************************************/

/*      Print server communication errors    */

#define PSC_NO_AVAILABLE_SPX_CONNECTS                   0x0040
#define PSC_SPX_NOT_INITIALIZED                         0x0041
#define PSC_NO_SUCH_PRINT_SERVER                        0x0042
#define PSC_UNABLE_TO_GET_SERVER_ADDR                   0x0043
#define PSC_UNABLE_TO_CONNECT_TO_SERVER                 0x0044
#define PSC_NO_AVAILABLE_IPX_SOCKETS                    0x0045
#define PSC_ALREADY_ATTACHED_TO_PSERVER                 0x0046
#define PSC_IPX_NOT_INITIALIZED                         0x0047

/*      Print server error codes      */

#define PSE_TOO_MANY_FILE_SERVERS                       0x0101
#define PSE_UNKNOWN_FILE_SERVER                         0x0102
#define PSE_BINDERY_LOCKED                              0x0103
#define PSE_SERVER_MAXED_OUT                            0x0104
#define PSE_NO_RESPONSE                                 0x0105
#define PSE_ALREADY_ATTACHED                            0x0106
#define PSE_CANT_ATTACH                                 0x0107
#define PSE_NO_ACCOUNT_BALANCE                          0x0108
#define PSE_NO_CREDIT_LEFT                              0x0109
#define PSE_INTRUDER_DETECTION_LOCK                     0x010A
#define PSE_TOO_MANY_CONNECTIONS                        0x010B
#define PSE_ACCOUNT_DISABLED                            0x010C
#define PSE_UNAUTHORIZED_TIME                           0x010D
#define PSE_UNAUTHORIZED_STATION                        0x010E
#define PSE_NO_MORE_GRACE                               0x010F
#define PSE_LOGIN_DISABLED                              0x0110
#define PSE_ILLEGAL_ACCT_NAME                           0x0111
#define PSE_PASSWORD_HAS_EXPIRED                        0x0112
#define PSE_ACCESS_DENIED                               0x0113
#define PSE_CANT_LOGIN                                  0x0114
#define PSE_NO_SUCH_QUEUE                               0x0200
#define PSE_NOT_AUTHORIZED_FOR_QUEUE                    0x0201
#define PSE_QUEUE_HALTED                                0x0202
#define PSE_UNABLE_TO_ATTACH_TO_QUEUE                   0x0203
#define PSE_TOO_MANY_QUEUE_SERVERS                      0x0204
#define PSE_INVALID_REQUEST                             0x0300
#define PSE_NOT_ENOUGH_MEMORY                           0x0301
#define PSE_NO_SUCH_PRINTER                             0x0302
#define PSE_INVALID_PARAMETER                           0x0303
#define PSE_PRINTER_BUSY                                0x0304
#define PSE_CANT_DETACH_PRIMARY_SERVER                  0x0305
#define PSE_GOING_DOWN                                  0x0306
#define PSE_NOT_CONNECTED                               0x0307
#define PSE_ALREADY_IN_USE                              0x0308
#define PSE_NO_JOB_ACTIVE                               0x0309
#define PSE_NOT_ATTACHED_TO_SERVER                      0x030A
#define PSE_ALREADY_IN_LIST                             0x030B
#define PSE_DOWN                                        0x030C
#define PSE_NOT_IN_LIST                                 0x030D
#define PSE_NO_RIGHTS                                   0x030E
#define PSE_UNABLE_TO_VERIFY_IDENTITY                   0x0400
#define PSE_NOT_REMOTE_PRINTER                          0x0401

/*      Client privilege levels for print server     */

#define PS_LIMITED      0            /* Limited access only */
#define PS_USER         1            /* User access         */
#define PS_OPERATOR     2            /* Operator access     */


/****************************************************************************/

#ifdef PROTOTYPE

extern WORD PrintServerRequest( WORD, void *, int, void *, int );

extern WORD PSGetPrintServerInfo(
                     WORD,        /* SPX Connection number         */
                     PS_INFO *,   /* Server info structure         */
                     WORD );      /* Size of information requested */

extern WORD PSAbortPrintJob(
                     WORD,        /* SPX Connection number */
                     BYTE,        /* Printer number        */
                     BYTE );      /* Job outcome           */

extern WORD PSAddNotifyObject(
                     WORD,        /* SPX Connection number */
                     BYTE,        /* Printer number        */
                     char *,      /* File server name      */
                     char *,      /* Object name           */
                     WORD,        /* Object type           */
                     WORD,        /* First notify delay    */
                     WORD);       /* Notify interval       */

extern WORD PSChangeNotifyInterval(
                     WORD,        /* SPX Connection number */
                     BYTE,        /* Printer number        */
                     char *,      /* File server name      */
                     char *,      /* Object name           */
                     WORD,        /* Object type           */
                     WORD,        /* First notify delay    */
                     WORD);       /* Notify interval       */

extern WORD PSGetNotifyObject(
                     WORD,        /* SPX Connection number           */
                     BYTE,        /* Printer number                  */
                     WORD *,      /* Sequence number.  0 first ti    */
                     char *,      /* File server name                */
                     char *,      /* Object name                     */
                     WORD *,      /* Object type                     */
                     WORD *,      /* First notify delay              */
                     WORD *);     /* Notify interval                 */

extern WORD PSGetQueuesServiced(
                     WORD,        /* SPX Connection number           */
                     BYTE,        /* Printer number                  */
                     WORD *,      /* Sequence number.  0 first time  */
                     char *,      /* File server name                */
                     char *,      /* Queue name                      */
                     BYTE *);     /* Priority                        */

extern WORD PSAddQueueToPrinter(
                     WORD,        /* SPX Connection number           */
                     BYTE,        /* Printer number                  */
                     char *,      /* File server name                */
                     char *,      /* Queue name                      */
                     BYTE);       /* Priority                        */

extern WORD PSAttachToPrintServer(
                     char *,      /* Print server name               */
                     WORD *);     /* SPX Connection number           */

extern WORD PSDetachFromPrintServer(
                     WORD);       /* SPX Connection number           */

extern WORD PSCancelDownRequest(
                     WORD);       /* SPX Connection number           */

extern WORD PSChangeServiceMode(
                     WORD,        /* SPX Connection number           */
                     BYTE,        /* Printer number                  */
                     BYTE);       /* New service mode                */

extern WORD PSChangeQueuePriority(
                     WORD,        /* SPX Connection number           */
                     BYTE,        /* Printer number                  */
                     char *,      /* File server name                */
                     char *,      /* Queue name                      */
                     BYTE);       /* New priority                    */

extern WORD PSDeleteNotifyObject(
                     WORD,        /* SPX Connection number           */
                     BYTE,        /* Printer number                  */
                     char *,      /* File server name                */
                     char *,      /* Object name                     */
                     WORD);       /* Object type                     */

extern WORD PSDeleteQueueFromPrinter(
                     WORD,        /* SPX Connection number           */
                     BYTE,        /* Printer number                  */
                     char *,      /* File server name                */
                     char *,      /* Queue name                      */
                     BYTE,        /* Detach immediately?             */
                     BYTE);       /* Outcome of current job          */

extern WORD PSDownPrintServer(
                     WORD,        /* SPX Connection number           */
                     BYTE,        /* Go down immediately?            */
                     BYTE);       /* Outcome of current jobs         */

extern WORD PSEjectForm(
                     WORD,        /* SPX Connection number           */
                     BYTE);       /* Printer number                  */

extern WORD PSGetNextRemotePrinter(
                     WORD,        /* SPX Connection number           */
                     BYTE *,      /* Printer number                  */
                     WORD *,      /* Printer type                    */
                     char *);     /* Name of printer                 */

extern WORD PSMarkTopOfForm(
                     WORD,        /* SPX Connection number           */
                     BYTE,        /* Printer number                  */
                     char);       /* Character to mark form with     */

extern WORD PSRequestRemotePrinter(
                     WORD ,            /* SPX Connection number               */
                     BYTE,             /* Printer number                      */
                     REMOTE_INFO *);   /* Remote printer information structure*/

extern WORD PSRewindPrintJob(
                     WORD,        /* SPX Connection number                    */
                     BYTE,        /* Printer number                           */
                     BYTE,        /* Rewind by page?                          */
                     BYTE,        /* Rewind relative to current position?     */
                     WORD,        /* Copy to rewind to (if absolute)          */
                     long);       /* Offset                                   */

extern WORD PSSetMountedForm(
                     WORD,        /* SPX Connection number           */
                     BYTE,        /* Printer number                  */
                     WORD);       /* Form number                     */

extern WORD PSStartPrinter(
                     WORD,       /* SPX Connection number            */
                     BYTE);      /* Printer number                   */

extern WORD PSStopPrinter(
                     WORD,        /* SPX Connection number           */
                     BYTE,        /* Printer number                  */
                     BYTE);       /* Job outcome                     */

extern WORD PSAttachPrintServerToFileServer(
                     WORD,          /* SPX Connection number         */
                     char *,        /* File server name              */
                     char *);       /* Password                      */

extern WORD PSDetachPrtServerFromFileServer(
                     WORD,        /* SPX Connection number           */
                     char *,      /* File server name                */
                     BYTE,        /* Detach immediately?             */
                     BYTE);       /* Outcome of current jobs         */

extern WORD PSGetPrintersServicingQueue(
                     WORD,        /* SPX Connection number                    */
                     char *,      /* File server name                         */
                     char *,      /* Queue name                               */
                     BYTE,        /* Maximum number of printers to return     */
                     BYTE *,      /* Actual number of printers returned       */
                     BYTE *);     /* Array where printer numbers are returned */

extern WORD PSLoginToPrintServer(
                     WORD,        /* SPX Connection number           */
                     BYTE *);     /* Client's access level           */

extern WORD PSSetRemoteMode(
                     WORD,        /* SPX Connection number           */
                     BYTE,        /* Printer number                  */
                     BYTE);       /* New mode                        */

extern WORD PSGetPrinterStatus(
                     WORD,        /* SPX Connection number           */
                     BYTE,        /* Printer number                  */
                     BYTE *,      /* Printer status                  */
                     BYTE *,      /* On line/Off line/Out of paper   */
                     BYTE *,      /* Printer has an active job       */
                     BYTE *,      /* Queue service mode              */
                     WORD *,      /* Mounted form number             */
                     char *,      /* Mounted form name               */
                     char *);     /* Printer name                    */

extern WORD PSGetPrintJobStatus(
                     WORD,        /* SPX Connection number           */
                     BYTE,        /* Printer number                  */
                     char *,      /* File server name                */
                     char *,      /* Queue name                      */
                     WORD *,      /* Job number                      */
                     char *,      /* Description of job              */
                     WORD *,      /* Number of copies to be printed  */
                     long *,      /* Size of print job               */
                     WORD *,      /* Copies finished                 */
                     long *,      /* Bytes into current copy         */
                     WORD *,      /* Form number for job             */
                     BYTE *);     /* Is job text?                    */

extern WORD PSGetAttachedServers(
                     WORD,        /* SPX Connection number           */
                     WORD *,      /* Sequence number. 0 first time   */
                     char *);     /* File server name                */

extern void cdecl _ReplyReceivedESR( ECB far ** );

#else

extern WORD PrintServerRequest();
extern WORD PSGetPrintServerInfo();
extern WORD PSAbortPrintJob();
extern WORD PSAddNotifyObject();
extern WORD PSChangeNotifyInterval();
extern WORD PSGetNotifyObject();
extern WORD PSGetQueuesServiced();
extern WORD PSAddQueueToPrinter();
extern WORD PSAttachToPrintServer();
extern WORD PSDetachFromPrintServer();
extern WORD PSCancelDownRequest();
extern WORD PSChangeServiceMode();
extern WORD PSChangeQueuePriority();
extern WORD PSDeleteNotifyObject();
extern WORD PSDeleteQueueFromPrinter();
extern WORD PSDownPrintServer();
extern WORD PSEjectForm();
extern WORD PSGetNextRemotePrinter();
extern WORD PSMarkTopOfForm();
extern WORD PSRequestRemotePrinter();
extern WORD PSRewindPrintJob();
extern WORD PSSetMountedForm();
extern WORD PSStartPrinter();
extern WORD PSStopPrinter();
extern WORD PSAttachPrintServerToFileServer();
extern WORD PSDetachPrtServerFromFileServer();
extern WORD PSGetPrintersServicingQueue();
extern WORD PSLoginToPrintServer();
extern WORD PSSetRemoteMode();
extern WORD PSGetPrinterStatus();
extern WORD PSGetPrintJobStatus();
extern WORD PSGetAttachedServers();
extern void cdecl _ReplyReceivedESR();

#endif
#endif
