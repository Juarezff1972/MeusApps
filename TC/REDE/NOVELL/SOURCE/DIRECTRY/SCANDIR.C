/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwdir.h>
#include <nwlocal.h>
#include <nwmisc.h>

/*______________________ ScanDirectoryInformation ____________________________
|
| Output:   0                  --  SUCCESSFUL
|           network error code --  UNSUCCESSFUL
|
| Comments:
|   This routine returns information about subdirectories below a directory.
|   The directory path may contain wild cards in the last subdirectory name.
|   The name of the matched directory is returned with its creation date and
|   time, the object ID of the owner of the directory, the directory, and the
|   directory's maximum rights masks.
|___________________________________________________________________________*/

int ScanDirectoryInformation( directoryHandle, searchDirectoryPath, 
                              sequenceNumber, directoryName, 
                              creationDateAndTime, ownerObjectID,
                              maximumRightsMask )

BYTE directoryHandle;       /* searchDirectoryPath is relative to this
                               directory                                   */
char *searchDirectoryPath;  /* Path specification of the directory looked
                               for (max 255 characters INCLUDING null)     */
int *sequenceNumber;        /* Contains the sequence number of the previous
                               search plus 1 (initial search requires a 0).*/
char *directoryName;        /* Receives the name of the directory that matched
                               the search pattern                          */
BYTE *creationDateAndTime;  /* Receives the date and time the file was created*/
long *ownerObjectID;        /* Receives the unique bindery object ID of the
                               owner of the directory.                     */
BYTE *maximumRightsMask;    /* Receives the maximum rights mask of the
                               directory                                   */
{
    BYTE sendPacket[262], receivePacket[30];
    int ccode, len;

    sendPacket[2] = 2;
    sendPacket[3] = directoryHandle;
    if (sequenceNumber == (int *)0)
        *((int *)(sendPacket + 4)) = IntSwap(1);
    else
        *((int *)(sendPacket + 4)) = IntSwap(*sequenceNumber + 1);
    len = strlen(searchDirectoryPath);
    sendPacket[6] = (BYTE)len;
    memmove(sendPacket + 7, searchDirectoryPath, len);

    *((int *)sendPacket) = len + 5;
    *((int *)receivePacket) = 28;

    ccode = _ShellRequest((BYTE)226, sendPacket, receivePacket);
    if (ccode)
       return(ccode);

    if (directoryName != (char *)0)
        strcpy(directoryName, receivePacket + 2);
    if (creationDateAndTime != (BYTE *)0)
        memmove(creationDateAndTime, receivePacket + 18, 4);
    if (ownerObjectID != (long *)0)
        *ownerObjectID = LongSwap(*((long *)(receivePacket + 22)));
    if (maximumRightsMask != (BYTE *)0)
        *maximumRightsMask = receivePacket[26];
    if (sequenceNumber != (int *)0)
        *sequenceNumber = IntSwap(*((int *)(receivePacket + 28)));

    return (ccode);
}
