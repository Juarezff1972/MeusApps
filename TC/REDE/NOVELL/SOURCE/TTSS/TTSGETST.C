/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include "nwconsol.h"
#include "nwtts.h"
#include "nwlocal.h"
#include "nwmisc.h"
#include "nwwrkenv.h"

#define MIN(a,b)            ((a) < (b) ? (a) : (b))

static WORD lastConnectionID = 0xFFFF;
static BYTE   receivePacket[514];
static int    i = 45;
static int    ccode;              /* error number */
static int    numberOfRecords;
static WORD  lastConnectionNumber = 0;

int TTSGetStats( connectionID, taskID, structSize, TTSStats )

WORD    connectionID;           /* the workstation's connection ID of the
                                   server for which to get information */
int    *taskID;                 /* points to the next connection/task to return */
int     structSize;             /* the number of bytes to return in
                                   connectionUsage */
TTS_STATS *TTSStats;

{
int   returnSize;
int   reRead = FALSE;


    returnSize = MIN (sizeof(TTS_STATS), structSize);
    if ( lastConnectionID != connectionID )
     {
        *taskID = 0;
         lastConnectionID = connectionID;
         reRead = TRUE;
     }
    if ( *taskID == 0 )
         reRead = TRUE;

    if ( reRead )
     {
        ccode = _TTSGetStats( connectionID );
        memset( (BYTE *)TTSStats, 0, returnSize );
        if ( ccode )
           return( ccode );
        else
         {
             numberOfRecords = receivePacket[44];
             TTSStats->systemElapsedTime = LongSwap(*((long *)(receivePacket+2)));
             TTSStats->TTS_Supported = receivePacket[6];
             TTSStats->TTS_Enabled = receivePacket[7];
             TTSStats->TTS_VolumeNumber = IntSwap(*((WORD *)(receivePacket+8)));
             TTSStats->TTS_MaxOpenTransactions =
                                        IntSwap(*((WORD *)(receivePacket+10)));
             TTSStats->TTS_MaxTransactionsOpened =
                                        IntSwap(*((WORD *)(receivePacket+12)));
             TTSStats->TTS_CurrTransactionsOpen =
                                        IntSwap(*((WORD *)(receivePacket+14)));
             TTSStats->TTS_TotalTransactions =
                                        LongSwap(*((long *)(receivePacket+16)));
             TTSStats->TTS_TotalWrites =
                                        LongSwap(*((long *)(receivePacket+20)));
             TTSStats->TTS_TotalBackouts =
                                        LongSwap(*((long *)(receivePacket+24)));
             TTSStats->TTS_UnfilledBackouts =
                                        IntSwap(*((WORD *)(receivePacket+28)));
             TTSStats->TTS_DiskBlocksInUse =
                                        IntSwap(*((WORD *)(receivePacket+30)));
             TTSStats->TTS_FATAllocations =
                                        LongSwap(*((long *)(receivePacket+32)));
             TTSStats->TTS_FileSizeChanges =
                                        LongSwap(*((long *)(receivePacket+36)));
             TTSStats->TTS_FilesTruncated =
                                        LongSwap(*((long *)(receivePacket+40)));
             TTSStats->numberOfTransactions = receivePacket[44];
         }
     }
    if ( !ccode )
     {
        if ( *taskID > numberOfRecords )
         {
             memset( (BYTE *)TTSStats, 0, returnSize );
            *taskID = 0;
         }
        else
         {
             TTSStats->connectionNumber = receivePacket[i++];
             TTSStats->taskNumber = receivePacket[i++];
             (*taskID)++;
         }
     }
    else
        *taskID = 0;

    return( ccode );
}






int _TTSGetStats( connectionID )

WORD    connectionID;           /* the workstation's connection ID of the
                                   server for which to get information */
{
WORD  oldConnectionID;         /* the preferred connneciton ID to restore */
BYTE  sendPacket[3];

    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );

    *((WORD *)sendPacket) = 1;
    sendPacket[2] = 0xD5;
    *((WORD *)receivePacket) = 512;

    ccode = _ShellRequest((BYTE)0xE3, sendPacket, receivePacket);

    SetPreferredConnectionID( oldConnectionID );
    return( ccode );
}
