/*	(c) COPYRIGHT 1988 - 1990 Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwfile.h>
#include <nwlocal.h>



/*______________________ RestoreErasedFile _________________________________
|
| Output:   0		       --  SUCCESSFUL
|	    network error code --  UNSUCCESSFUL
|
| Comments:
|   This function restores one file that the requesting workstation has marked
|   for deletion.  It can be used iteratively to restore all erased files.
|   When a workstation deletes a file, or a group of files, they are not
|   completely deleted until the workstation deletes or creates another file
|   on that volume.  This function allows these files to be recovered by the
|   workstation.  Since another workstation could have created a new file by
|   the same name as the deleted file in the directory, the name of the restored
|   file may not have the same name it had before it was deleted.
|_____________________________________________________________________________*/

int RestoreErasedFile(driveHandle, volumeName,
		      erasedFileName, restoredFileName)

BYTE  driveHandle;	 /* Contains the directory handle associated with the
			    volume where the erased file resides	      */
char *volumeName;	 /* String containing the volume where the erased file
			    resides (max 17 chars INCLUDING null)	      */
char *erasedFileName;	 /* Receives a string containing the name of the erased
			    file (max 15 chars INCLUDING null)		      */
char *restoredFileName;  /* Receives a string containing the restored name of
			    the erased file (max 15 chars INCLUDING null)     */
{
    BYTE sendPacket[21], receivePacket[32];
    int  ccode, len;

    sendPacket[2] = 17;
    sendPacket[3] = driveHandle;
    len = strlen(volumeName);
    sendPacket[4] = len;
    memmove(&sendPacket[5], volumeName, len);

    *((int *)sendPacket) = len + 3;
    *((int *)receivePacket) = 30;

    ccode = _ShellRequest((BYTE)226, sendPacket, receivePacket);
    if (!ccode)
     {
	strcpy(erasedFileName, &receivePacket[2]);
	strcpy(restoredFileName, &receivePacket[17]);
     }
    return (ccode);
}
