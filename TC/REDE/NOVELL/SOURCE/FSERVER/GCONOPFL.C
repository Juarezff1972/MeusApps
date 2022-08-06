/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwmisc.h>
#include <niterror.h>
#include <nwlocal.h>
#include <nwconsol.h>
#include <nwwrkenv.h>


#ifndef MIN
  #define MIN(a,b)            ((a) < (b) ? (a) : (b))
#endif

static WORD lastConnectionID = 0xFFFF;
static WORD lastConnectionNumber = 0xFFFF;

static BYTE   receivePacket[514];

/*__________________________________________________________________________

   Output:    0  --  SUCCESSFUL
            198  --  NO_CONSOLE_RIGHTS

   Comments:  This function returns information about the files a connection
              has open.
  _________________________________________________________________________*/

int GetConnectionsOpenFiles( connectionID, connectionNumber, lastRecord,
                             taskID, structSize, openFiles )

WORD    connectionID;         /* Workstation's connection ID of the server
                                 to get information on                        */
WORD    connectionNumber;     /* Connection for which to return open files    */
int    *lastRecord;           /* Value passed to server for iteration         */
int    *taskID;               /* Points to the next connection/task to return */
int     structSize;           /* Number of bytes to return in connectionUsage */
CON_OPEN_FILES *openFiles;    /* Receives the connection's open files         */

{
int  ccode = 0;               /* error number                                 */
char fileServerName[50];      /* file server name of passed connection        */
int  offset;                  /* offset into buffer for current record        */
int  reRead = FALSE;          /* TRUE if new buffer must be read              */
int  returnSize;


     returnSize = MIN(sizeof(CON_OPEN_FILES), structSize);


/*_________  test connection id ___________*/

    *fileServerName = '\0';
    GetFileServerName( connectionID, fileServerName );
    if ( *fileServerName == '\0' )
     {
        ccode = INVALID_CONNECTION_ID;
        goto ERROR0;
     }


/*--   get next buffer full if needed  --*/

    if ( ( lastConnectionID != connectionID ) ||
         ( lastConnectionNumber != lastConnectionNumber ) )
     {
        *lastRecord = 0;
        *taskID = 0;
         reRead = TRUE;
     }
    if ( *taskID == 0 )
        reRead = TRUE;
    if ( reRead )
     {
        *taskID = 0;
        lastConnectionID = connectionID;
        lastConnectionNumber = connectionNumber;
        ccode = _GetConnectionsOpenFiles( connectionID, connectionNumber,
                                          lastRecord );
     }
    if ( ccode == 0 )
     {
        if ( *taskID >=  receivePacket[4] )
         {
            memset( (char *)openFiles, 0, returnSize );
            *taskID = 0;
         }
        else
         {
            offset = *taskID * 21;
            memmove( openFiles, receivePacket + 5 + offset,
                     MIN( returnSize, 5 ) );
            if ( returnSize > 6 )
                memmove( ((char *)openFiles) + 6,
                        receivePacket + 10 + offset, returnSize - 6 );
            if ( returnSize > 8 )
                ((char *)openFiles)[returnSize - 1] = '\0';
            if ( ++*taskID >=  receivePacket[4] )
                *taskID = 0;
        }
     }
    else
        *taskID = 0;
ERROR0:
    return( ccode );
}






int _GetConnectionsOpenFiles( connectionID, connectionNumber, lastRecord )

WORD     connectionID;           /* the workstation's connection ID of the
                                   server for which to get information        */
WORD     connectionNumber;       /* connection for which to return open files */
int    * lastRecord;             /* value passed to server for iteration      */
{
int      ccode;                  /* error number                              */
WORD     oldConnectionID;        /* the preferred connneciton ID to restore   */
BYTE     sendPacket[7];          /* request buffer                            */



    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );
    *((WORD *)sendPacket) = 5;
    sendPacket[2] = 0xDB;
    *((WORD *)(sendPacket + 3)) = IntSwap( connectionNumber );
    *((WORD *)(sendPacket + 5)) = *lastRecord;
    *((WORD *)receivePacket) = 512;

    ccode = _ShellRequest((BYTE)0xE3, sendPacket, receivePacket);
    if (ccode == 0)
        *lastRecord = *((WORD *)(receivePacket + 2));


    SetPreferredConnectionID( oldConnectionID );
    return( ccode );
}
