/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwdir.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>


#define MIN_MAJOR_VERSION   2
#define MIN_MINOR_VERSION   0

/*_______________________________________________________________________

    Output:   0  --  SUCCESSFUL
             -1  --  Unable to get path for specified drive


    Comments:

        This function returns the current path that a drive is mapped to.  The
        drive number follows the usual convention of 0 = A:, 1 = B:, etc.  The
        returned path will include the fileserver and volume name if the drive
        is mapped to a network directory, or it will include the drive letter,
        followed by a colon and a backslash if the drive is a local DOS drive.
 ____________________________________________________________________________*/


int GetFullPath(drive, path)
char   drive;    /*  Drive number to get full path for                       */
char   *path;    /*  Receives the full path specification for drive          */
{
int     pathLength, onNetwork;
WORD    connectionID;
BYTE    majorVersion, minorVersion, driveHandle;
char    driveStatus;
int     rcode = 0;
int     ccode;
WORD    oldConnectionID;

    GetFileServerInformation( (char *)NULL, &majorVersion, &minorVersion,
                              (WORD *)NULL, (WORD *)NULL, (WORD *)NULL,
                              (BYTE *)NULL, (BYTE *)NULL, (BYTE *)NULL,
                              (WORD *)NULL );

    if((majorVersion < MIN_MAJOR_VERSION) || (minorVersion < MIN_MINOR_VERSION))
        onNetwork = FALSE;
    else
        onNetwork = TRUE;

    /* Get the current directory path into path */
    if (onNetwork)
        driveStatus = GetDriveInformation( (BYTE)drive, (WORD *)NULL,
                                            &driveHandle );

    if (onNetwork && (driveStatus & DRIVE_NETWORK))
     {
         GetDriveInformation(drive, &connectionID, NULL);
         GetFileServerName(connectionID, path);
         pathLength = strlen(path);
         path[pathLength++] = '/';
         oldConnectionID = GetPreferredConnectionID();
         SetPreferredConnectionID( connectionID );
         ccode = GetDirectoryPath(driveHandle, path + pathLength);
         SetPreferredConnectionID( oldConnectionID);
         if (ccode != 0)
          {
             rcode = -1;
             goto ERROR0;
          }
     }
    else
     {
         path[0] = drive + 'A';
         path[1] = ':';
         path[2] = '\\';
         if (GetCurrentDirectory((char)(drive), path + 3) != 0)
          {
             rcode = -1;
             goto ERROR0;
          }
     }
ERROR0:
         return (rcode);
}                              /*      END OF GetFullPath()      */
