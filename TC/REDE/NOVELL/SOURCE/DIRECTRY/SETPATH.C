/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <niterror.h>
#include <nwdir.h>
#include <nwmisc.h>
#include <nwwrkenv.h>
#include <ndt.h>


/*___________________________ SetDrivePath __________________________________
 |
 | Input:   BYTE driveNumber;	 The target drive.
 |	    BYTE baseDriveNumber; The drive that the path is relative to.
 |	    char *path; 	The path that the drive should be set to.
 |	    BYTE permenantFlag; Flag to indicate if the drive should be
 |				permenant.
 |
 | Output:  0x00  --  SUCCESSFUL
 |	    0xFB  --  INVALID PARAMETERS
 |	    0xFC  --  UNKNOWN FILE SERVER
 |	    0xF8  --  NO CURRENT CONNECTION TO FILE SERVER
 |	    0xFE  --  BINDERY LOCKED
 |	    0xFF  --  PATH NOT LOCATABLE
 |	    0xF7  --  TARGET DRIVE DOES NOT EXIST
 |	    0xF6  --  TARGET AND BASE DRIVES ARE DIFFERENT
 |
 | Comments:
 |  This routine will take a drive number (0-31), and set it to the given path.
 |  If the given path is not a complete path then it will be assumed to be
 |  relative to the baseDriveNumber (0-31). If the given path is a complete
 |  path, then the baseDriveNumber is not used and may be passed in as 0xff.
 |  The path name should be no longer than 255 characters (and a null). It
 |  may begin with the file server name (server/volume:dir/subdir/...). If
 |  no server name is given, and the baseDriveNumber is 0xff then the server
 |  will be assumed to be the current preferred file server.
 |_________________________________________________________________________*/

int SetDrivePath(driveNumber, baseDriveNumber, path, permanentFlag)
BYTE driveNumber, baseDriveNumber, permanentFlag;
char *path;
{
char serverName[50], oldDriveStatus;
BYTE baseDirectoryHandle, oldDirectoryHandle;
char baseDriveStatus;
int  ccode;
WORD connectionID = 0;
WORD oldConnectionID, oldDriveConnectionID, curConnectionID;


    if ((driveNumber > (BYTE)31) || ((baseDriveNumber > (BYTE)31) &&
	    (baseDriveNumber != (BYTE)0xff)))
	return (0xfb);

    path = StripFileServerFromPath(path, serverName);
    if ((serverName[0] == '\0') && (path[1] == ':'))
    {
	baseDriveNumber = (BYTE)(path[0] - 'A');
	path += 2;
    }

    /*-- Get the path that the target drive is currently mapped to --*/
    oldDriveStatus = GetDriveInformation(driveNumber, &oldDriveConnectionID,
				&oldDirectoryHandle);

    /*-- If no base drive number was given set the base drive handle to 0 --*/
    /*-- otherwise, get the base drive handle --*/
    if (baseDriveNumber == (BYTE)0xff)
	baseDirectoryHandle = 0;
    else
	baseDriveStatus = GetDriveInformation(baseDriveNumber, NULL,
		&baseDirectoryHandle );

    /*-- Check to see if the path is being set to a local drive --*/
    if ((*serverName == '\0') && (((baseDriveNumber != (BYTE)0xff) &&
	    !(baseDriveStatus & DRIVE_NETWORK)) || ((baseDriveNumber
	    == (BYTE)0xff) && (oldDriveStatus & DRIVE_LOCAL))))

    {
	char dosPath[256];

	/*-- A local drive is being assigned --*/
	if ((baseDriveNumber != (BYTE)0xff) && (baseDriveNumber !=
		driveNumber))
	    /*-- The drives are different, and can not be assigned locally --*/
	    return (0xf6);

	if (path[1] != ':')
	{
	    /*-- The path does not have the volume letter prepended --*/
	    *dosPath = driveNumber + 'A';
	    dosPath[1] = ':';
	    strcpy(dosPath + 2, path);
	    ccode = ChangeDirectory(dosPath);
	}
	else
	    ccode = ChangeDirectory(path);

	if (ccode)
	    return (0xff);
	else
	    return (0);
    }
/*--  else --*/
    /*-- A network drive is being set --*/
    if (*serverName == '\0')
    {
	if (baseDriveNumber == (BYTE)0xff)
	    connectionID = 0;
	else
	    GetDriveInformation(baseDriveNumber, &connectionID, NULL);
    }
    else
    {
	ccode = GetConnectionID(serverName, &connectionID);
	if (ccode)
	    return (ccode);

	GetDriveInformation(baseDriveNumber, &curConnectionID, NULL);
	if (connectionID != curConnectionID)

	/*_____________________________________________________________
	       The server specified in the path is not the same as
	       the server that the base drive is mapped to, so the
	       the base drive can not be used
	  ____________________________________________________________*/

	    baseDirectoryHandle = 0;
    }

    oldConnectionID = GetPreferredConnectionID();


    /*_______________________________________________________________________
       If there was a server specified by the path, or by the base drive
       then set the preferred server to that server, otherwise leave it
       as it is.
       _____________________________________________________________________*/

    if (connectionID != 0)
	SetPreferredConnectionID(connectionID);

    /*-- Create the new drive handle --*/
    if (permanentFlag)
	ccode = AllocPermanentDirectoryHandle( baseDirectoryHandle,
		path, (char)('A' + driveNumber), (BYTE *)NULL, (BYTE *)NULL);
    else
	ccode = AllocTemporaryDirectoryHandle( baseDirectoryHandle,
		path, (char)('A' + driveNumber), (BYTE *)NULL, (BYTE *)NULL);

    if (ccode)
    {
	/*-- The new path could not be created --*/
	SetPreferredConnectionID(oldConnectionID);
	return (ccode);
    }


    /*_______________________________________________________________________
       If the drive was previously mapped to a path, remove the old drive
       handle.	Note one exception, if the old drive was permanent then the
       file server simply reused the old handle for the new path.
      ______________________________________________________________________*/

    GetDriveInformation(driveNumber, &curConnectionID, NULL);
    if ((oldDriveStatus & DRIVE_TEMPORARY) || ((oldDriveStatus &
	    DRIVE_PERMANENT) && (oldDriveConnectionID != curConnectionID)))
    {
	SetPreferredConnectionID((int)oldDriveConnectionID);
	DeallocateDirectoryHandle(oldDirectoryHandle);
    }

    SetPreferredConnectionID(oldConnectionID);
    return (0);
}

/*________________________StripFileServerFromPath ___________________________
 |
 | Input:   char *path; 	The path.
 |	    char server[50];	Buffer where the server name should be placed.
 |
 | Output:  Returns a pointer to the path after the server name.
 |
 | Comments:
 |  This routine copies a server name from the front of a path to a buffer,
 |  if a server name was specified. It returns a pointer to the remainder of
 |  the path.
 |___________________________________________________________________________*/

char *StripFileServerFromPath(path, server)
char *path;			  /* The path				     */
char *server;			  /* Receives the server name		     */
{
char *startOfDirectories, *startOfVolumeName, ch;


    if (server != NULL)
	*server = '\0';

    for (startOfDirectories = path; (*startOfDirectories != '\0') &&
	    (*startOfDirectories != ':'); startOfDirectories++);
    if (*startOfDirectories == '\0')
	return (path);

    for (startOfVolumeName = path; (*startOfVolumeName != ':') &&
	    (*startOfVolumeName != '/') && (*startOfVolumeName != '\\');
	    startOfVolumeName++);
    if (*startOfVolumeName == ':')
	return (path);

    /*-- Copy server name --*/
    if (server != (char *)NULL)
    {
	ch = *startOfVolumeName;
	*startOfVolumeName = '\0';
	strcpy(server, path);
	*startOfVolumeName = ch;
    }

    return (++startOfVolumeName);
}
