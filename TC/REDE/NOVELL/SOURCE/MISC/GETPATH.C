/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <ctype.h>
#include <string.h>
#include <nwconsol.h>
#include <nwdir.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>


/*__________________________ ConvertNameToFullPath _____________________________
 |
 | Output:  None.
 |
 | Comments:  This function converts a file name or a partial path into a
 |	      full path specification.
 |____________________________________________________________________________*/

int ConvertNameToFullPath(fileName, path)
char *fileName;        /* File name to be converted to a full path	      */
char *path;	       /* Receives the full path specification (max 256 chars */
{
char	server[50];
char	volume[15];
char	directories[256];
int	ccode;

    ccode = ParsePath(fileName, server, volume, directories);
    if (ccode)
	return (ccode);

    /* Assemble the path */
    strcpy(path, volume);
    strcat(path, ":");
    strcat(path, directories);
    return (0);
}






/*________________________ ConvertNameToVolumePath __________________________
 |
 | Output:  None.
 |
 | Comments:  This function converts a file name or a partial path into a
 |	      full path specification.
 |__________________________________________________________________________*/

int ConvertNameToVolumePath(fileName, path)
char *fileName;     /* File name to be converted to a full path 	    */
char *path;	    /* Receives the full path specification (max 256 chars) */
{
char	server[50];
char	volume[15];
char	directories[256];
int	ccode;

    ccode = ParsePath(fileName, server, volume, directories);
    if (ccode)
	return (ccode);

    /* Assemble the path */
    strcpy(path, "\\");
    strcat(path, directories);
    return (0);
}






/*_______________________________ ParsePath ___________________________________
 |
 |  Output:  0 -- SUCCESSFUL
 |
 |  Comments:  This function separates a path specification into the SERVER,
 |	       VOLUME, and DIRECTORIES.
 |____________________________________________________________________________*/

int ParsePath(path, server, volume, directories)

char *path;	     /* Path to be parsed				    */
char *server;	     /* Receives the Server portion of the path 	    */
char *volume;	     /* Receives the Volume portion of the path 	    */
char *directories;   /* Receives the Directories portion of the path	    */
{
int	ccode;
char	ch;
WORD	connectionID;
int	drive;
char   *locationInDirectories;
WORD	oldConnectionID;
int	relativePath;
char   *startOfDirectories;
char   *startOfVolumeName;

    *server = '\0';
    *volume = '\0';
    connectionID = 0;
    relativePath = FALSE;

    for (startOfDirectories = path; (*startOfDirectories != '\0') &&
	    (*startOfDirectories != ':'); startOfDirectories++);
    for (startOfVolumeName = path; (*startOfVolumeName != '\0') &&
	    (*startOfVolumeName != '/') && (*startOfVolumeName != '\\');
	   startOfVolumeName++);

    if (*startOfDirectories != '\0')
    {
	/* There is a volume name and possibly a server name */
	if ((*startOfVolumeName != '\0') && (startOfDirectories >
		startOfVolumeName))
	{
	    /* There is a server name */
	    /* Copy server name */
	    ch = *startOfVolumeName;
	    *startOfVolumeName = '\0';
	    strcpy(server, path);
	    *startOfVolumeName = ch;
	    path = startOfVolumeName + 1;
	}

	if (startOfDirectories == path)
	{
	    /* The path begins with a ':', so it begins at the root */
	    drive = -1;
	    path++;
	}
	else if (startOfDirectories == path + 1)
	{
	    /* There is a drive letter, but no volume name */
	    drive = toupper(*path) - 'A';
	    GetDriveInformation((BYTE)drive, &connectionID, NULL);
	    path = startOfDirectories + 1;
	    if ((*path != '/') && (*path != '\\'))
		relativePath = TRUE;
	    else
		path++;
	}
	else
	{
	    /* The volume name is specified */
	    /* Copy the volume name */
	    ch = *startOfDirectories;
	    *startOfDirectories = '\0';
	    strcpy(volume, path);
	    *startOfDirectories = ch;
	    path = startOfDirectories + 1;
	    drive = -1;
	}
    }
    else
    {
	/* There is no server, volume, or drive information */
	drive = -1;
	if ((*path  != '\\') && (*path != '/'))
	    /* The path is relative */
	    relativePath = TRUE;
	else
	    path++;
    }

    if (connectionID != 0)
    {
	oldConnectionID = GetPreferredConnectionID();
	SetPreferredConnectionID((int)connectionID);
    }

    if (*server == '\0')
    {
	/* No server was specified so use the default server */
	GetFileServerName((WORD)0, server);
    }

    if (*volume == '\0')
    {
	BYTE driveHandle;
	char statusFlags;

	if (drive == -1)
	    drive = GetDefaultDrive();

	statusFlags = GetDriveInformation((BYTE)drive, (WORD *)NULL,
					  &driveHandle);
	if (!(statusFlags & DRIVE_NETWORK))
	{
	    /* The Drive is not a network drive */
	    *server = '\0';
	    *volume = (char)(drive + 'A');
	    volume[1] = '\0';
	}
	else
	{
	    /* It is a network drive */
	    ccode = GetVolumeInfoWithHandle(driveHandle, volume, (WORD *)NULL,
		    (WORD *)NULL, (WORD *)NULL, (WORD *)NULL, (WORD *)NULL,
		    (WORD *)NULL);
	    if (ccode)
		goto Error0;
	}
    }
    locationInDirectories = directories;
    if (volume[1] == '\0')
	/* The directory path is preceeded by a drive letter so begin the */
	/* path with a '/' */
	*(locationInDirectories++) = '/';

    if (!relativePath)
    {
	/* The given path is absolute */
	/* Check to see if the user entered a ":/" */
	if ((*path == '\\') || (*path == '/'))
	    /* Skip the slash */
	    path++;

	/* Copy the remainder of the path into directories */
	while (*path != '\0')
	{
	    if (*path == '\\')
	    {
		*(locationInDirectories++) = '/';
		path++;
	    }
	    else
		*(locationInDirectories++) = *(path++);
	}
	*locationInDirectories = '\0';
    }
    else
    {
	/* The path is a relative path */
	ccode = GetCurrentDirectory((char)(drive), locationInDirectories);
	if (ccode)
	    goto Error0;

	/* Check to see if getcurrentdirectory returned a path */
	if (*locationInDirectories != '\0')
	{
	    /* find the end of the default path */
	    while (*locationInDirectories != '\0')
	    {
		if (*locationInDirectories == '\\')
		    *(locationInDirectories++) = '/';
		else
		    locationInDirectories++;
	    }
	    *(locationInDirectories++) = '/';
	}

	while (*path != '\0')
	{
	    /* Copy the next directory name to directories */
	    if ((*path == '.') && ((path[1] == '.') || (path[1] == '/')
		    || (path[1] == '\\') || (path[1] == '\0')))
	    {
		/* This is backing up to a parent directory */
		path++;
		while ((*path == '.') && (locationInDirectories >
			directories))
		{
		    /* Back up one directory level */
		    if (locationInDirectories > directories)
			locationInDirectories--;
		    while ((*(locationInDirectories - 1) != '/') &&
			    (locationInDirectories > directories))
			locationInDirectories--;
		    path++;
		}
	    }
	    else
	    {
		/* Copy the next directory over */
		while ((*path != '/') && (*path != '\\') && (*path != '\0'))
		    *(locationInDirectories++) = *(path++);
		if (*path != '\0')
		    *(locationInDirectories++) = '/';
	    }
	    if (*path != '\0')
		path++;
	}
	*locationInDirectories = '\0';
    }
    ConvertToUpperCase(server);
    ConvertToUpperCase(volume);
    ConvertToUpperCase(directories);
    ccode = 0;

/* Error Recovery */
Error0:
    if (connectionID != 0)
	SetPreferredConnectionID(oldConnectionID);
    return (ccode);
}
