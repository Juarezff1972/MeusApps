/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>

/*__________________________________________________________________________

   Output:    0  --  SUCCESSFUL
            198  --  NO_CONSOLE_RIGHTS

   Comments:  This function returns information about a single semaphore.
  _________________________________________________________________________*/


#define MIN(a,b)            ((a) < (b) ? (a) : (b))

static BYTE   receivePacket[514];
static int    i = 8;
static int    ccode;              /* error number */
static int    numberOfRecords;
static int    recordCount = 0;
static char   lastSemaphoreName[ 48 ] = "";


int GetSemaphoreInformation( connectionID, semaphoreName, lastRecord, lastTask,
                             openCount, semaphoreValue, connectionNumber,
                             taskNumber )

WORD    connectionID;           /* Workstation's connection ID of the server
                                   to get information on                      */
char   *semaphoreName;          /* Name of semaphore to get information on    */
int    *lastRecord;             /* Value passed to server for iteration       */
int    *lastTask;               /* Points to next connection/task to return   */
int    *openCount;              /* Number of logical connections that have
                                   the specified semaphore open               */
char   *semaphoreValue;         /* Value of the semaphore (-127 to 128)       */
WORD   *connectionNumber;       /* Logical connection that has the semaphore
                                   open                                       */
BYTE   *taskNumber;             /* Task number within the logical connection
                                   that has the semaphore open                */

{
int    reRead = FALSE;          /* TRUE if new buffer must be read */


    if ( strcmp(lastSemaphoreName, semaphoreName) != 0 )
     {
       *lastRecord = 0;
        strcpy( lastSemaphoreName, semaphoreName );
        reRead = TRUE;
     }
    if( *lastTask == 0 )
     {
        reRead = TRUE;
        i = 8;
        *lastRecord = 0;
        recordCount = 0;
     }

    if ( reRead )
     {
        ccode = _GetSemaphoreInformation( connectionID, semaphoreName,
                                          lastRecord );
        if ( ccode )
           return( ccode );
        else
         {
            *lastRecord = *((WORD *)(receivePacket + 2));
            *openCount = IntSwap( *((WORD *)(receivePacket + 4)) );
            *semaphoreValue = (char)receivePacket[6];
             numberOfRecords = receivePacket[7];
         }
     }

    if ( !ccode )
     {
        if ( recordCount >= numberOfRecords )
         {
            *lastTask         = 0;
             openCount        = 0;
             semaphoreValue   = 0;
             connectionNumber = 0;
            *taskNumber       = 0;
         }
        else
         {
            *connectionNumber = IntSwap( *((WORD *)(receivePacket + i)) );
             i+=2;
            *taskNumber = receivePacket[i];
             i++;
             recordCount++;
            (*lastTask)++;
         }
     }
    else
        *lastTask = 0;

    return( ccode );
}






int _GetSemaphoreInformation( connectionID, semaphoreName, lastRecord )

WORD    connectionID;          /* the workstation's connection ID of the
                                  server for which to get information     */
char   *semaphoreName;         /* name of semaphore to return info about  */
int    *lastRecord;            /* value passed to server for iteration    */

{
WORD   oldConnectionID;        /* the preferred connneciton ID to restore */
BYTE   sendPacket[133];
int    nameLength;


    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );

    nameLength = strlen( semaphoreName );
    *((WORD *)sendPacket) = (WORD)(4 + nameLength);
    sendPacket[2] = 0xE2;
    *((WORD *)(sendPacket + 3)) = *lastRecord;
    sendPacket[5] = (BYTE)nameLength;
    memmove ( sendPacket + 6, semaphoreName, nameLength );

    *((WORD *)receivePacket) = 512;

    ccode = _ShellRequest( (BYTE)0xE3, sendPacket, receivePacket );

    SetPreferredConnectionID( oldConnectionID );
    return( ccode );
}
