/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>

#define MIN(a,b)            ((a) < (b) ? (a) : (b))

static char  oldRecordName[99] = "";
static WORD  oldConnectionID = 0xFFFF;
static BYTE  receivePacket[514];
static int   ccode;
static int   offset = 10;
static int   numberOfRecords;


/*__________________________________________________________________________

   Output:    0  --  SUCCESSFUL
            198  --  NO_CONSOLE_RIGHTS

   Comments:  This function returns information about a logical record.
  _________________________________________________________________________*/



int GetLogicalRecordInformation( connectionID, logicalRecordName, lastRecord,
                                 lastTask, structSize, logicalRecInfo )

WORD    connectionID;           /* the workstation's connection ID of the
                                   server for which to get information */
char   *logicalRecordName;      /* name of record to retrieve information on */
int    *lastRecord;             /* value passed to server for iteration */
int    *lastTask;               /* points to the next connection/task to return */
int     structSize;             /* the number of bytes to return in
                                   connectionUsage */
LOGICAL_REC_INFO *logicalRecInfo;

{
int   returnSize;
int   reRead = FALSE;



    returnSize = MIN (sizeof(LOGICAL_REC_INFO), structSize);
    if ( (connectionID != oldConnectionID) ||
         (strcmp(oldRecordName,logicalRecordName) != 0) )
     {
       *lastRecord = 0;
       *lastTask = 0;
        oldConnectionID = connectionID;
        strcpy( oldRecordName, logicalRecordName );
        reRead = TRUE;
     }
    if ( *lastTask == 0 ) 
     {
        reRead = TRUE;
        offset = 10;
        *lastRecord = 0;
     }

    if ( reRead )
     {
        ccode = _GetLogicalRecordInformation( connectionID, logicalRecordName,
                                              lastRecord );
        memset( (BYTE *)logicalRecInfo, 0, returnSize );
        if ( ccode )
           return( ccode );
        else
         {
             logicalRecInfo->currentUseCount =
                                      IntSwap( *((WORD *)(receivePacket + 2)) );
             logicalRecInfo->shareableLockCount =
                                      IntSwap( *((WORD *)(receivePacket + 4)) );
            *lastRecord = *((WORD *)(receivePacket + 6));
             logicalRecInfo->locked = receivePacket[8];
             numberOfRecords = receivePacket[9];
         }
     }

    if ( !ccode )
     {
        if ( *lastTask >= numberOfRecords )
         {
             memset( (BYTE *)logicalRecInfo, 0, returnSize );
            *lastTask = 0;
         }
        else
         {
             logicalRecInfo->logicalConnectionNumber =
                                 IntSwap( *((WORD *)(receivePacket + offset)) );
             offset+=2;
             logicalRecInfo->taskNumber = receivePacket[offset++];
             logicalRecInfo->lockStatus = receivePacket[offset++];
             (*lastTask)++;
         }
     }
    else
        *lastTask = 0;

    return( ccode );
}







int _GetLogicalRecordInformation( connectionID, logicalRecordName, lastRecord )

WORD    connectionID;           /* The workstation's connection ID of the
                                   server for which to get information        */
char   *logicalRecordName;      /* The name of the logical record to retrieve
                                   information about                          */
int    *lastRecord;             /* Value passed to server for iteration       */

{
int    ccode;               /* Error number                           */
WORD   oldConnectionID;     /* Preferred connneciton ID to restore    */
BYTE   sendPacket[55];
BYTE   recordLength;        /* Length of the record name              */



    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );

    recordLength = (BYTE)strlen( logicalRecordName );
    *(sendPacket + 5) = recordLength;
    *((WORD *)sendPacket) = 4 + recordLength;

    sendPacket[2] = 0xE0;
    *((WORD *)(sendPacket + 3)) = *lastRecord;
    memmove(&sendPacket[6], logicalRecordName, recordLength);
    *((WORD *)receivePacket) = 512;

    ccode = _ShellRequest((BYTE)0xE3, sendPacket, receivePacket);

    SetPreferredConnectionID( oldConnectionID );
    return( ccode );
}
