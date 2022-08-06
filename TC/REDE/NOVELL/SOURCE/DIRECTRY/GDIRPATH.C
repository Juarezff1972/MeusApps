/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwdir.h>
#include <nwlocal.h>


/*__________________________ GetDirectoryPath ______________________________
|
| Output:   0		       --  SUCCESSFUL
|	    network error code --  UNSUCCESSFUL
|
| Comments:
|   This routine will return a directory path of a directory handle.  The
|   path name will begin at the volume (it will not include the file server).
|___________________________________________________________________________*/

int GetDirectoryPath(directoryHandle, directoryPath)

BYTE  directoryHandle;  /* A directory path is requested for this
			                  directory handle				                      */
char *directoryPath;	   /* Receives a string containing the path specification
 			                  of directoryHandle (max 255 chars INCLUDING null)*/
{
BYTE	sendPacket[4], receivePacket[258];
int	ccode, len;

    sendPacket[2] = 1;
    sendPacket[3] = directoryHandle;
    *((int *)sendPacket) = 2;
    *((int *)receivePacket) = 256;

    ccode = _ShellRequest((BYTE)226, sendPacket, receivePacket);
    if (ccode)
	return (ccode);

    len = receivePacket[2];
    if (len > 0)
	memmove(directoryPath, receivePacket + 3, len);
    directoryPath[len] = (BYTE)'\0';

    return (0);
}
