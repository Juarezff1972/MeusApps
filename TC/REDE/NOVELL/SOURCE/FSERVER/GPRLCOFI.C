/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>

#define MIN(a,b)            ((a) < (b) ? (a) : (b))

static WORD  lastConnectionID = 0xFFFF;
static char  lastFilePath[ 255 ] = "";
static int   ccode;           /* error number */
static int   i = 6;
static int   numberOfRecords;

static BYTE receivePacket[514];

/*__________________________________________________________________________

   Output:    0  --  SUCCESSFUL
            198  --  NO_CONSOLE_RIGHTS

   Comments:  This function returns a logical connection's physical record
              locks within a file.
  _________________________________________________________________________*/

int GetPhysRecLockByConnectAndFile( connectionID, connectionNumber,
                                    volumeNumber, directoryHandle, filePath,
                                    lastRecord, lastTask, structSize,
                                    recordLock )

WORD    connectionID;         /* Workstation's connection ID of the server
                                 to get information on                        */
WORD    connectionNumber;     /* Connection number of workstation to get
                                 information on                               */
BYTE    volumeNumber;         /* Volume number of the volume where information
                                 is to be retrieved                           */
WORD    directoryHandle;      /* Directory handle of where information is     */
char   *filePath;             /* File path specification                      */
int    *lastRecord;           /* Value passed to server for iteration         */
int    *lastTask;             /* Points to the next connection/task to return */
int     structSize;           /* Number of bytes to return in connectionUsage */
SHORT_PHYS_REC_LOCK *recordLock;

{
int   reRead = FALSE;
int   returnSize;


    returnSize = MIN (sizeof(SHORT_PHYS_REC_LOCK), structSize);

    if ( (connectionID != lastConnectionID) ||
         (strcmp(lastFilePath, filePath) != 0) )
     {
       *lastRecord = 0;
       *lastTask = 0;
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
        ccode = _GetPhysRecLockByConnectAndFile( connectionID, connectionNumber, volumeNumber, directoryHandle,
                                                 filePath, lastRecord );
        memset( (BYTE *)recordLock, 0, returnSize );
        if ( ccode )
           return( ccode );
        else
         {
            *lastRecord = *((WORD *)(receivePacket + 2));
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
             recordLock->taskNumber = receivePacket[i++];
             recordLock->lockFlag =   receivePacket[i++];
             recordLock->recordStart = LongSwap(*((long *)(receivePacket + i)));
             i+=4;
             recordLock->recordEnd  = LongSwap( *((long *)(receivePacket + i)));
             i+=4;
             (*lastTask)++;
         }
     }
    else
        *lastTask = 0;

    return( ccode );
}






int _GetPhysRecLockByConnectAndFile( connectionID, connectionNumber,
                                     volumeNumber, directoryHandle, fileName,
                                     lastRecord )

WORD    connectionID;           /* Workstation's connection ID of the server
                                   to get information on                     */
WORD    connectionNumber;       /* Connection Number to return logical records
                                   for                                       */
BYTE    volumeNumber;           /* Volume to search for file in              */
WORD    directoryHandle;        /* Directory handle to search                */
char   *fileName;               /* Name of file to search for                */
int    *lastRecord;             /* Value passed to server for iteration      */

{
register int    ccode = 0;
WORD            oldConnectionID;
BYTE            sendPacket[266];

    if ( strlen( fileName ) <= 255 )
     {
        oldConnectionID = GetPreferredConnectionID();
        SetPreferredConnectionID( connectionID );

        *((WORD *)sendPacket) = 264;
        sendPacket[2] = 0xDD;
        *((WORD *)(sendPacket + 3)) = IntSwap( connectionNumber );
        *((WORD *)(sendPacket + 5)) = *lastRecord;
        sendPacket[7] = volumeNumber;
        *((WORD *)(sendPacket + 8)) = IntSwap( directoryHandle );
        strcpy( sendPacket + 10, fileName );
        *((WORD *)receivePacket) = 512;
        ccode = _ShellRequest((BYTE)0xE3, sendPacket, receivePacket);

        SetPreferredConnectionID( oldConnectionID );
     }
    else
        ccode = 0xFE;

    return( ccode );
}
