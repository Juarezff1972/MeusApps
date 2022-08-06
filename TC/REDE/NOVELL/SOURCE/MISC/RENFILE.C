/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwlocal.h>


/*_______________________ RENAME FILE ______________________________________
|
|    Outputs:		      0  --  SUCCESSFUL
|		  Network Error  --  UNSUCCESSFUL
|
|    Comments:	  This function renames a file.
|___________________________________________________________________________*/

int RenameFile(oldFileDirectoryHandle, oldFilePath, searchFileAttributes,
	       newFileDirectoryHandle, newFilePath)

char   oldFileDirectoryHandle;	/* File handle of the file to be renamed     */
char  *oldFilePath;		/* File path of file to be renamed	     */
char   searchFileAttributes;	/* Attributes of the file to be renamed      */
char   newFileDirectoryHandle;	/* File handle of the renamed file	     */
char  *newFilePath;		/* File path of the renamed file	     */
{
    char sendPacket[518];
    int position;

    sendPacket[0] = oldFileDirectoryHandle;
    sendPacket[1] = searchFileAttributes;
    sendPacket[2] = (char)((BYTE)strlen(oldFilePath));
    memmove(sendPacket + 3, oldFilePath, (WORD)sendPacket[2]);
    position = 3 + (int)sendPacket[2];
    sendPacket[position++] = newFileDirectoryHandle;
    sendPacket[position++] = (char)((BYTE)strlen(newFilePath));
    memmove(sendPacket + position, newFilePath, sendPacket[position - 1]);

    return (_FileServiceRequest((BYTE)69, (BYTE *)sendPacket,
				(WORD)(position + sendPacket[position - 1]),
				(BYTE *)NULL, (WORD)0));
}
