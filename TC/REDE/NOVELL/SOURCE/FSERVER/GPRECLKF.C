/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>

#define MIN(a,b)            ((a) < (b) ? (a) : (b))

static WORD  lastConnectionID = 0xFFFF;
static char  lastFilePath[ 255 ] = "";
static BYTE  receivePacket[514];
static int   ccode;
static int   i = 6;
static int   numberOfRecords;


/*__________________________________________________________________________

   Output:    0  --  SUCCESSFUL
            198  --  NO_CONSOLE_RIGHTS
            255  --  FILE_NOT_OPEN

   Comments:  This function returns physical records that are locked in a file.
  _________________________________________________________________________*/



int GetPhysicalRecordLocksByFile( connectionID, directoryHandle, filePath,
                                  lastRecord, lastTask, structSize, recordLock )

WORD    connectionID;         /* Workstation's connection ID of the server
                                 to get information on                        */
WORD    directoryHandle;      /* Handle of directory to search                */
char   *filePath;             /* Name of file to return info on               */
int    *lastRecord;           /* Value passed to server for iteration         */
int    *lastTask;             /* Points to the next connection/task to return */
int     structSize;           /* Number of bytes to return in connectionUsage */
PHYS_REC_LOCK *recordLock;

{
int   returnSize;
int   reRead = FALSE;



    returnSize = MIN (sizeof(PHYS_REC_LOCK), structSize);

    if ( (connectionID != lastConnectionID) ||
         (strcmp(lastFilePath, filePath) != 0) )
     {
       *lastRecord = 0;
        lastConnectionID = connectionID;
        strcpy( lastFilePath, filePath );
        reRead = TRUE;
     }
    if ( *lastTask == 0 )
     {
        reRead = TRUE;
        i = 6;
        *lastRecord = 0;
     }

    if ( reRead )
     {
        ccode = _GetPhysicalRecordLocksByFile( connectionID, directoryHandle,
                                               filePath, lastRecord );
        memset( (BYTE *)recordLock, 0, returnSize );
        if ( ccode )
           return( ccode );
        else
         {
            *lastRecord = *((WORD *)(receivePacket + 2));
             recordLock->physicalRecordLockCount = receivePacket[4];
             numberOfRecords = receivePacket[5];
         }
     }
    if ( !ccode )
     {
        if ( *lastTask >= numberOfRecords )
         {
             memset( (BYTE *)recordLock, 0, returnSize );
            *lastTask = 0;
         }
        else
         {
             recordLock->loggedCount = IntSwap( *((WORD *)(receivePacket + i)));
             i+=2;
             recordLock->shareLockCount =
                                       IntSwap( *((WORD *)(receivePacket + i)));
             i+=2;
             recordLock->recordStart = LongSwap(*((long *)(receivePacket + i)));
             i+=4;
             recordLock->recordEnd  = LongSwap( *((long *)(receivePacket + i)));
             i+=4;
             recordLock->connectionNumber =
                                       IntSwap( *((WORD *)(receivePacket + i)));
             i+=2;
             recordLock->taskNumber = receivePacket[i++];
             recordLock->lockType   = receivePacket[i++];
             (*lastTask)++;
         }
     }
    else
        *lastTask = 0;

    return( ccode );
}




int _GetPhysicalRecordLocksByFile( connectionID, directoryHandle, filePath,
                                   lastRecord)

WORD    connectionID;           /* the workstation's connection ID of the
                                   server for which to get information      */
WORD    directoryHandle;        /* handle of directory to search            */
char   *filePath;               /* name of file to return info on           */
int    *lastRecord;             /* value passed to server for iteration     */
{
int    ccode = 0;               /* error number                             */
WORD   oldConnectionID;         /* the preferred connneciton ID to restore  */
BYTE   sendPacket[262];



    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );

    sendPacket[2] = 0xDE;
    *((WORD *)sendPacket + 3) = *lastRecord;
    sendPacket[5] = (BYTE)directoryHandle;
    sendPacket[6] = (BYTE)strlen( filePath );
    strcpy( sendPacket + 7, filePath );
    *((WORD *)sendPacket) = 5 + sendPacket[6];
    *((WORD *)receivePacket) = 512;

    ccode = _ShellRequest((BYTE)0xE3, sendPacket, receivePacket);

    SetPreferredConnectionID( oldConnectionID );
    return( ccode );
}
