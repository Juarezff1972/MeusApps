/*      COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <stdlib.h>
#include <nwdir.h>
#include <nwmisc.h>
#include <nwwrkenv.h>
#include <nat.h>

/*____________________________ AFPDirectoryEntry _____________________________

  Output:   Returns a 0 if a DOS file, 1 if a MAC file, or a Network error.

  Comments:
    Determine if a file or directory is a MAC file.  The Directory Base and
    Path specifications are given in Netware "short name" format.
 ---------------------------------------------------------------------------*/

int AFPDirectoryEntry(connectionID, directoryHandle, pathName)
WORD    connectionID;      /* Connection ID of server to use              */
BYTE    directoryHandle;   /* Directory Handle for pathname               */
char    *pathName;         /* Path relative to directory handle           */
{
long        AFPBaseID;
AFPFILEINFO AFPFileInfo;
int         ccode;
char        directory[128];
int         loop;
char       *ptr;
WORD        saveServer;
char        server[48];
char        volumeName[16];
int         volumeNumber;

    /* Get current server */
    saveServer = GetPreferredConnectionID();
    SetPreferredConnectionID(connectionID);

    /* We got a handle so request the volume name */
    if( directoryHandle != (BYTE)0 )
    {
        ccode = GetVolumeInfoWithHandle( directoryHandle, volumeName, NULL,
                                         NULL, NULL, NULL, NULL, NULL );
        if( ccode )
            goto ERROR;
    }
    /* We don't have a handle so parse the path */
    else
    {
        ccode = ParsePath( pathName, server, volumeName, directory );
        if (ccode)
            goto ERROR;
        /* Local drive, must be a DOS file */
        if( !server[0] )
            goto XDOS;
    }

    /* Map volume name to number */
    ccode = GetVolumeNumber( volumeName, &volumeNumber );
    if( ccode )
        goto ERROR;

    /* Get base ID */
    ptr = StripFileServerFromPath( pathName, server );
    ccode = AFPGetEntryIDFromPathName( GetPreferredConnectionID(),
                                       directoryHandle, ptr, &AFPBaseID );
    if( ccode )
        goto ERROR;

    /* Get file info */
    ccode = AFPGetFileInformation( GetPreferredConnectionID(), 
                                   (BYTE)volumeNumber, AFPBaseID, 0xFFFF, 
                                   "", sizeof(AFPFILEINFO), &AFPFileInfo );
    if( ccode )
        goto ERROR;

     /* Test file to see if it is an AFP file */
    if ( AFPFileInfo.resourceForkLength > 0 )
         goto ISAFP;
    if ( strcmp( AFPFileInfo.longName, AFPFileInfo.shortName ) != 0 )
         goto ISAFP;

/*    if( (long )AFPFileInfo.finderInfo[4] == 0L)
       goto DOS;    */

    if( memicmp( &AFPFileInfo.finderInfo[4], "mdos", 4 ) == 0)
       goto XDOS;

    for ( loop = 0; loop < 16; loop++ )
        if ( ((int *)(AFPFileInfo.finderInfo))[ loop ] )
            goto ISAFP;

    /* Return results */
    XDOS: 
         ccode = 0;
         goto DONE;
    ISAFP: 
         ccode = 1;
    ERROR: 
    DONE: 
        /* Restore server */
    SetPreferredConnectionID( saveServer );

    return( ccode );
}
