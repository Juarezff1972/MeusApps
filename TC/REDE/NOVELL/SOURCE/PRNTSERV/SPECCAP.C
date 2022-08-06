/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwlocal.h>
#include <nwprint.h>


/*_____________________________SpecifyCaptureFile _____________________________
|
| Ouput:		    0  --  SUCCESSFUL
|	   Network Error Code  --  UNSUCCESSFUL
|
| Comments:
|   This routine allows a user to give a specific file name to be used as
|   a spool capture file, rather than the default file usually created in
|   the sys:system directory.  The user must have create, read, and write
|   privileges in the directory where the file is to be created.  If a
|   spool file already existed, it is automatically closed and sent to the
|   print queue.  If the created spool file should not be deleted after
|   the end spool, then the delete flag in the printFlags should not be set.
|   If the file is to be created only, and not printed, the number of copies
|   should be set to 0.
|____________________________________________________________________________*/


int SpecifyCaptureFile(driveHandle, filePath)
BYTE driveHandle;    /* The drive handle that is pointing to the directory
			from which the file path is relative		     */
char *filePath;      /* An absolute path or a path relative to the drive handle
			that specifies the file to be created as a spool
			capture file					     */
{
BYTE	len;
BYTE	sendPacket[261], receivePacket[2];


    len = (BYTE)strlen(filePath);
    *((WORD *)sendPacket) = 3 + len;
    sendPacket[2] = 9;
    sendPacket[3] = driveHandle;
    sendPacket[4] = len;
    memcpy(sendPacket + 5, filePath, (WORD)len);

    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)0xE0,sendPacket,receivePacket));
}
