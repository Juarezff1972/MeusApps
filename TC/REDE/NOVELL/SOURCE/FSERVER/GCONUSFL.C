/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>

#define MIN(a,b)            ((a) < (b) ? (a) : (b))

static WORD  lastConnectionID = 0xFFFF;
static BYTE  lastDirectoryHandle = 0xFF;
static char  lastFilePath[ 256 ] = "";

static BYTE         receivePacket[514];


/*__________________________________________________________________________

   Output:    0  --  SUCCESSFUL
            198  --  NO_CONSOLE_RIGHTS

   Comments:  This function returns connections using a file.
  _________________________________________________________________________*/


int GetConnectionsUsingFile( connectionID, lastRecord, taskID, directoryHandle,
                             filePath, structSize, fileUse )

WORD    connectionID;         /* the workstation's connection ID of the
                                 server for which to get information          */
int    *lastRecord;           /* points to the last record retrieved by fs    */
int    *taskID;               /* points to the next connection/task to return */
BYTE    directoryHandle;      /* handle relative to filePath                  */
char   *filePath;             /* pointer to the path of file to look for      */
int     structSize;           /* Number of bytes to return in connectionUsage */
CONN_USING_FILE *fileUse;

{
int   ccode = 0;              /* error number                                 */
int   reRead;                 /* TRUE if new buffer must be read              */



    /*-- Get next buffer full if needed --*/
    reRead = FALSE;
    if ( ( lastConnectionID != connectionID ) ||
         ( lastDirectoryHandle != directoryHandle ) ||
         ( strcmp( lastFilePath, filePath ) ) )
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
        lastDirectoryHandle = directoryHandle;
        strcpy( lastFilePath, filePath );
        ccode = _GetConnectionsUsingFile(connectionID, lastRecord,
                                         directoryHandle, filePath);


        if (ccode)
            return( ccode );
     }
    if ( !ccode )
     {
        structSize = MIN( sizeof(CONN_USING_FILE), structSize );
        if ( *taskID >= *(receivePacket +17))
         {
            memset( (char *)fileUse, 0, structSize );
            *taskID = 0;
         }
        else
         {
            if ( *taskID )
                memmove( (receivePacket + 18 ),
                         (receivePacket + 18 + 6 * (*taskID) ), 6 );
            memmove( fileUse, (receivePacket + 2), structSize );
            if ( ++(*taskID) >= *(receivePacket + 17) )
                 *taskID = 0;
         }
     }
    else
        *taskID = 0;
    return( ccode );
}






int _GetConnectionsUsingFile( connectionID, lastRecord, directoryHandle,
                              filePath)

WORD    connectionID;         /* Workstation's connection ID of the server
                                 to get information on                        */
int    *lastRecord;           /* Points to the last record retrieved by fs    */
BYTE    directoryHandle;      /* filePath is relative to this directoryHandle */
char   *filePath;             /* File path specification                      */

{
int    ccode;
WORD   oldConnectionID;
int    loopCount;
BYTE   sendPacket[136];
int    i;


    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );
    sendPacket[2] = 0xDC;
    *((WORD *)(sendPacket + 3)) = *lastRecord;
    sendPacket[5] = directoryHandle;
    sendPacket[6] = (BYTE)strlen( filePath );
    strcpy( sendPacket + 7, filePath );
    *((WORD *)sendPacket) = 5 + sendPacket[6];
    *((WORD *)receivePacket) = 512;

    ccode = _ShellRequest((BYTE)0xE3, sendPacket, receivePacket);
    if (ccode == 0)
     {
        *lastRecord = *((WORD *)(receivePacket + 14));
        for ( loopCount = 2; loopCount < 15; loopCount+=2 )
            *((WORD *)(receivePacket + loopCount)) =
                             IntSwap(*((WORD *)(receivePacket + loopCount)));
        i = 18;
        for ( loopCount = 0; loopCount < *(receivePacket + 17); loopCount++ )
         {
            *((WORD *)(receivePacket + i)) =
                                     IntSwap(*((WORD *)(receivePacket + i)));
             i += 6;
         }
     }

    SetPreferredConnectionID( oldConnectionID );
    return( ccode );
}
