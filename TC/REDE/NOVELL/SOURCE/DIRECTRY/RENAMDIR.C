/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwdir.h>
#include <nwlocal.h>


/*_________________________ RenameDirectory _________________________________
|
| Output:   0		       --  SUCCESSFUL
|	    network error code --  UNSUCCESSFUL
|
| Comments:
|   This function renames a directory.	To use this function the requesting
|   workstation must have parental and modify rights to the directory's
|   parent.  The standard directories on the SYS volume may not be renamed
|   (i.e. SYSTEM, PUBLIC, MAIL, and LOGIN).
|____________________________________________________________________________*/

int RenameDirectory(directoryHandle, directoryPath, newDirectoryName)

BYTE  directoryHandle;	 /* directoryPath is relative to this directory     */
char *directoryPath;     /* Path specification of the directory to be renamed
                            (maximum 255 characters INCLUDING the null	    */
char *newDirectoryName;  /* New name for the directory (maximum of 15        
                            characters INCLUDING the null)                  */
{
    BYTE sendPacket[275], receivePacket[2];
    int offset, len, newlen;

    len = strlen(directoryPath);
    newlen = strlen(newDirectoryName);
    sendPacket[2] = 15;
    sendPacket[3] = directoryHandle;
    sendPacket[4] = (BYTE)len;
    memmove(sendPacket + 5, directoryPath, len);
    offset = len + 5;
    sendPacket[offset++] = newlen;
    memmove(sendPacket + offset, newDirectoryName, newlen);

    *((int *)sendPacket) = len + newlen + 4;
    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)226, sendPacket, receivePacket));
}
