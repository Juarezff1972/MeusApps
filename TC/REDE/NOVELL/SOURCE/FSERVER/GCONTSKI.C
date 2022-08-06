/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>

#define MIN(a,b)            ((a) < (b) ? (a) : (b))

static CONN_TASK_INFO taskInfo;
static char *nextTaskAddress;        /* Address of where next task number and
                                        task state are located                */
static int   numberOfRecords;        /* Number of records to be read          */
static int   ccode;                  /* error number                          */
static WORD  lastConnectionID = 0xFFFF;
static WORD  lastConnectionNumber = 0;

static BYTE   receivePacket[514];

/*__________________________________________________________________________

   Output:    0  --  SUCCESSFUL
            198  --  NO_CONSOLE_RIGHTS

   Comments:  This function returns information about a logical connections
              active tasks.
  _________________________________________________________________________*/

int GetConnectionsTaskInformation( connectionID, connectionNumber, taskPointer,
                                   structSize, connTaskInfo )

WORD    connectionID;         /* the workstation's connection ID of the
                                 server for which to get information          */
WORD    connectionNumber;     /* connection for which to return open files    */
int    *taskPointer;          /* points to the next connection/task to return */
int     structSize;           /* the number of bytes to return in
                                 connectionUsage                              */
CONN_TASK_INFO *connTaskInfo;

{
int   returnSize;
int   bytesToMove;
int   reRead = FALSE;         /* TRUE if new buffer must be read       */


    returnSize = MIN( sizeof(CONN_TASK_INFO), structSize );
    if ( (lastConnectionNumber != connectionNumber) ||
         (lastConnectionID != connectionID) )
     {
        *taskPointer = 0;
         lastConnectionNumber = connectionNumber;
         lastConnectionID = connectionID;
         reRead = TRUE;
     }
    if ( *taskPointer == 0 )
         reRead = TRUE;


    if ( reRead )
     {
        ccode = _GetConnectionsTaskInformation( connectionID, connectionNumber);
        memset( (char *)&taskInfo, 0, sizeof(CONN_TASK_INFO) );
        taskInfo.lockStatus = receivePacket[2];
        numberOfRecords = receivePacket[3];
        switch ( taskInfo.lockStatus )
         {
            case 0:
                   nextTaskAddress = receivePacket + 4;
                   break;


            case 1:
                   taskInfo.waitingTaskNumber = receivePacket[4];
                   taskInfo.beginAddress = *((long *)(receivePacket + 5));
                   taskInfo.endAddress =   *((long *)(receivePacket + 9));
                   taskInfo.volumeNumber = receivePacket[13];
                   taskInfo.directoryEntry =  *((int *)(receivePacket + 14));
                   memmove( taskInfo.lockedName, (receivePacket + 16), 14 );
                   nextTaskAddress = receivePacket + 30;
                   break;

            case 2:
                   taskInfo.waitingTaskNumber = receivePacket[4];
                   taskInfo.volumeNumber =      receivePacket[5];
                   taskInfo.directoryEntry =   *((int *)(receivePacket + 6));
                   memmove( taskInfo.lockedName, (receivePacket + 8), 14 );
                   nextTaskAddress = receivePacket + 22;
                   break;

            case 3:
                    /* Go to case 4 since the logic is the same */

            case 4:
                   taskInfo.waitingTaskNumber = receivePacket[4];
                   bytesToMove = MIN( receivePacket[5], 49);
                   memmove( taskInfo.lockedName,(receivePacket + 6),
                            bytesToMove );
                   taskInfo.lockedName[bytesToMove] = '\0';
                   bytesToMove++;
                   nextTaskAddress = receivePacket + 6 + bytesToMove;
                   break;
         }
     }


    /*--  Get the next task number and task state if needed --*/
    if ( !ccode )
     {
        if ( *taskPointer >= numberOfRecords )
         {
            memset( (char *)connTaskInfo, 0, returnSize );
            *taskPointer = 0;
         }
        else
         {
            taskInfo.taskNumber = *(nextTaskAddress++);
            taskInfo.taskState =  *(nextTaskAddress++);
            (*taskPointer)++;
            memmove( connTaskInfo, &taskInfo, returnSize );
            if ( *taskPointer >= numberOfRecords )
                *taskPointer = 0;
         }
     }
    else
        *taskPointer = 0;

    return( ccode );
}


int _GetConnectionsTaskInformation( connectionID, connectionNumber )

WORD    connectionID;           /* the workstation's connection ID of the
                                   server for which to get information */
WORD    connectionNumber;       /* connection for which to return open files */

{
register WORD  oldConnectionID;   /* the preferred connneciton ID to restore */
BYTE           sendPacket[5];


    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );
    *((WORD *)sendPacket) = 3;
    sendPacket[2] = 0xDA;
    *((WORD *)(sendPacket + 3)) = IntSwap( connectionNumber );
    *((WORD *)receivePacket) = 512;
    ccode = _ShellRequest((BYTE)0xE3, sendPacket, receivePacket);
    switch ( receivePacket[2] )
     {
        case 1:
          *((long *)(receivePacket+5)) = LongSwap(*((long *)(receivePacket+5)));
          *((long *)(receivePacket+9)) = LongSwap(*((long *)(receivePacket+9)));
          *((int *)(receivePacket+14)) = IntSwap(*((int *)(receivePacket+14)));
          break;

        case 2:
          *((int *)(receivePacket+6)) = IntSwap(*((int *)(receivePacket+6)));
     }
    SetPreferredConnectionID( oldConnectionID );
    return( ccode );
}
