/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <stdio.h>
#include <string.h>
#include <nwconsol.h>
#include <nwmisc.h>
#include <nwlocal.h>
#include <nwwrkenv.h>


#define MIN(a,b)            ((a) < (b) ? (a) : (b))

static BYTE   receivePacket[514];
static int    i = 5;
static int    ccode;              /* error number */
static int    numberOfRecords;
static WORD  lastConnectionID = 0xFFFF;
static WORD  lastConnectionNumber = 0;


/*__________________________________________________________________________

   Output:    0  --  SUCCESSFUL
            198  --  NO_CONSOLE_RIGHTS

   Comments:  This function returns information about a connection's open
              semaphore.
  _________________________________________________________________________*/

int GetConnectionsSemaphores( connectionID, connectionNumber, lastRecord,
                              taskID, structSize, connectionSemaphores )

WORD    connectionID;           /* the workstation's connection ID of the
                                   server for which to get information */
WORD    connectionNumber;       /* connection for which to return open files */
int    *lastRecord;             /* value passed to server for iteration */
int    *taskID;                 /* points to the next connection/task to return */
int     structSize;             /* the number of bytes to return in
                                   connectionUsage */
CONN_SEMAPHORE *connectionSemaphores;

{
int   returnSize;
int   recordNameLength;
int   reRead = FALSE;


    returnSize = MIN (sizeof(CONN_SEMAPHORE), structSize);
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
        ccode = _GetConnectionsSemaphores( connectionID, connectionNumber,
                                           lastRecord );
        memset( (BYTE *)connectionSemaphores, 0, returnSize );
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
             memset( (BYTE *)connectionSemaphores, 0, returnSize );
            *taskID = 0;
         }
        else
         {
             connectionSemaphores->openCount  =
                                  IntSwap(*((WORD *)(receivePacket +i)) );
             i+=2;
             connectionSemaphores->semaphoreValue = receivePacket[i++];
             connectionSemaphores->taskNumber = receivePacket[i++];
             recordNameLength = receivePacket[i++];
             memmove( connectionSemaphores->semaphoreName, receivePacket + i,
                      recordNameLength );
             connectionSemaphores->semaphoreName[recordNameLength] = '\0';
             i = i + recordNameLength;
             (*taskID)++;
         }
     }
    else
        *taskID = 0;

    return( ccode );
}





int _GetConnectionsSemaphores( connectionID, connectionNumber, lastRecord )

WORD    connectionID;           /* Workstation's connection ID of the server
                                   to get information on                      */
WORD    connectionNumber;       /* Connection for which to return open files  */
int    *lastRecord;             /* Value passed to server for iteration       */
{
WORD    oldConnectionID;
BYTE    sendPacket[7];



    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );

    *((WORD *)sendPacket) = 5;
    sendPacket[2] = 0xE1;
    *((WORD *)(sendPacket + 3)) = IntSwap( connectionNumber );
    *((WORD *)(sendPacket + 5)) = *lastRecord;
    *((WORD *)receivePacket) = 512;

    ccode = _ShellRequest((BYTE)0xE3, sendPacket, receivePacket);

    SetPreferredConnectionID( oldConnectionID );
    return( ccode );
}
