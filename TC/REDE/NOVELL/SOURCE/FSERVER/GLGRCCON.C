/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>

#define MIN(a,b)            ((a) < (b) ? (a) : (b))

static BYTE  receivePacket[514];
static int   ccode;
static int   i = 5;
static int   numberOfRecords;
static WORD  lastConnectionID = 0xFFFF;
static WORD  lastConnectionNumber = 0;


/*__________________________________________________________________________

   Output:    0  --  SUCCESSFUL
            198  --  NO_CONSOLE_RIGHTS

   Comments:  This function returns the logical record that a connection
              has logged with a file server.
  _________________________________________________________________________*/


int GetLogicalRecordsByConnection( connectionID, connectionNumber, lastRecord,
                                   taskID, structSize, logicalRecord )

WORD    connectionID;         /* The workstation's connection ID of the
                                 server for which to get information          */
WORD    connectionNumber;     /* server's connection to retrieve info for     */
int    *lastRecord;           /* value passed to server for iteration         */
int    *taskID;               /* points to the next connection/task to return */
int     structSize;           /* Number of bytes to return in logicalRecord   */
LOGICAL_RECORD *logicalRecord;
{
int   returnSize;
int   recordNameLength;
int   reRead = FALSE;


    returnSize = MIN (sizeof(LOGICAL_RECORD), structSize);
    if ( (lastConnectionNumber != connectionNumber) ||
         (lastConnectionID != connectionID) )
     {
        *lastRecord = 0;
        *taskID = 0;
         lastConnectionNumber = connectionNumber;
         lastConnectionID = connectionID;
         reRead = TRUE;
     }

    if ( *taskID == 0 ) 
     {
         reRead = TRUE;
         i = 5;
         *lastRecord = 0;
     }

    if ( reRead )
     {
        ccode = _GetLogicalRecordsByConnection( connectionID, connectionNumber,
                                                lastRecord );
        memset( (BYTE *)logicalRecord, 0, returnSize );
        if ( ccode )
           return( ccode );
        else
         {
            *lastRecord = *((WORD *)(receivePacket + 2));
             numberOfRecords = receivePacket[4];
         }
     }
    if ( !ccode )
     {
        if ( *taskID >= numberOfRecords )
         {
             memset( (BYTE *)logicalRecord, 0, returnSize );
            *taskID = 0;
         }
        else
         {
             logicalRecord->taskNumber = receivePacket[i++];
             logicalRecord->lockStatus = receivePacket[i++];
             recordNameLength = receivePacket[i++];
             memmove( logicalRecord->logicalLockName, receivePacket + i,
                      recordNameLength );
             logicalRecord->logicalLockName[recordNameLength] = '\0';
             i = i + recordNameLength;
             (*taskID)++;
         }
     }
    else
        *taskID = 0;

    return( ccode );
}




int _GetLogicalRecordsByConnection( connectionID, connectionNumber, lastRecord )

WORD    connectionID;           /* Workstation's connection ID of the server
                                   to get information on                     */
WORD    connectionNumber;       /* ConnectionNumber to return logical records
                                   for                                       */
int    *lastRecord;             /* Value passed to server for iteration      */
{
WORD    oldConnectionID;
BYTE    sendPacket[7];


    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );

    *((WORD *)sendPacket) = 5;
    sendPacket[2] = 0xDF;
    *((WORD *)(sendPacket + 3)) = IntSwap( connectionNumber );
    *((WORD *)(sendPacket + 5)) = *lastRecord;
    *((WORD *)receivePacket) = 512;

    ccode = _ShellRequest((BYTE)0xE3, sendPacket, (BYTE *)receivePacket);

    SetPreferredConnectionID( oldConnectionID );
    return( ccode );
}
