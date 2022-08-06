/*	(c) COPYRIGHT 1988 - 1990 Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwfile.h>
#include <nwlocal.h>
#include <nwmisc.h>

/*_______________________ SetFileInformation _______________________________
|
| Output:   0		       --  SUCCESSFUL
|	    network error code --  UNSUCCESSFUL
|
| Comments:
|   This function sets file information for a file on the server.  It requires
|   the requesting workstation have modify rights to the directory where the
|   file resides.  The fileOwnerID can only be changed by the SUPERVISOR.  The
|   last item in filePath must be a valid filename specification.  No wild card
|   specifiers are allowed.  This routine is provided to facilitate the restore
|   operation of an archive utility.
|_____________________________________________________________________________*/

int SetFileInformation(driveHandle, filePath, searchAttributes, fileAttributes,
		     extendedFileAttributes, creationDate, lastAccessDate,
		     lastUpdateDateAndTime, lastArchiveDateAndTime, fileOwnerID)

BYTE  driveHandle;		/* filePath is relative to this directory    */
char *filePath; 		/* String containing file path specification
				   (max 256 characters INCLUDING null	     */
BYTE  searchAttributes; 	/* Indicates the type of files to set	     */
BYTE  fileAttributes;		/* Contains the file attributes to be assigned
				   to the file				     */
BYTE  extendedFileAttributes;	/* Contains the extended file attributes to
				   be assigned to the file		     */
char *creationDate;		/* Contains the creation date to be assigned
				   to the file (DOS format)(2 chars)	     */
char *lastAccessDate;		/* Contains the last access date to be
				   assigned to the file (DOS format)(2 chars)*/
char *lastUpdateDateAndTime;	/* Contains the last update date and time to be
				   assigned to the file (DOS format)(4 chars)*/
char *lastArchiveDateAndTime;	/* Contains the last archived date and time to
				   be assigned to the file (DOS format)(4 chars)*/
long  fileOwnerID;		/* Contains the unique bindery object ID to be
				   assigned to the file 		     */
{
    BYTE sendPacket[339], receivePacket[2];
    int  len;

    sendPacket[2] = 16;
    sendPacket[3] = fileAttributes;
    sendPacket[4] = extendedFileAttributes;
    memset(sendPacket + 5, 0, 4);

    *((int *)(sendPacket + 9)) = *(int *)creationDate;
    *((int *)(sendPacket + 11)) = *(int *)lastAccessDate;
    *((long *)(sendPacket + 13)) = *((long *)lastUpdateDateAndTime);
    *((long *)(sendPacket + 17)) = LongSwap(fileOwnerID);
    *((long *)(sendPacket + 21)) = *((long *)lastArchiveDateAndTime);

    memset(sendPacket + 25, 0, 56);
    sendPacket[81] = driveHandle;
    sendPacket[82] = searchAttributes;
    len = strlen(filePath);
    sendPacket[83] = (BYTE)len;
    memmove(&sendPacket[84], filePath, len);

    *((int *)sendPacket) = len + 82;
    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)227, sendPacket, receivePacket));
}
