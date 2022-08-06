/*	(c) COPYRIGHT 1988 - 1990 Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwfile.h>
#include <nwlocal.h>
#include <nwmisc.h>



/*______________________ ScanFileInformation _________________________________
|
| Output:   0                  --  SUCCESSFUL
|           network error code --  UNSUCCESSFUL
|
| Comments:
|   This function returns information about a file in a directory.  It requires
|   the requesting workstation have search rights to the specified directory.
|   File names should be uppercase.  The last item in filePath must be a valid
|   filename.  Wild card specifiers may be used in the filename portion of
|   filePath.  This function is provided to facilitate the backup operation of
|   an archive utility.
|___________________________________________________________________________*/

int ScanFileInformation(driveHandle, filePath, searchAttributes, sequenceNumber,
                        fileName, fileAttributes, extendedFileAttributes,
                        fileSize, creationDate, lastAccessDate,
                        lastUpdateDateAndTime, lastArchiveDateAndTime,
                        fileOwnerID)

BYTE  driveHandle;             /* filePath is relative to this directory     */
char *filePath;                /* String containing the path specification of
                                  the file for which information is being
                                  requested (max 256 chars INCLUDING null    */
BYTE  searchAttributes;        /* Indicates the type of file to select       */
int  *sequenceNumber;          /* Receives the sequence number that is used to
                                  obtain all files matching the supplied search
                                  pattern (-1 initially, subsequent calls
                                  should leave the value returned unchanged) */
char *fileName;                /* Receives the matching filename (max 15
                                  bytes INCLUDING null terminator            */
BYTE *fileAttributes;          /* Receives the file's attributes             */
BYTE *extendedFileAttributes;  /* Receives the file's extended attributes    */
long *fileSize;                /* Receives the file's size (in bytes)        */
char *creationDate;            /* Receives the creation date of the file     */
char *lastAccessDate;          /* Receives the last access date of the file  */
char *lastUpdateDateAndTime;   /* Receives the last update date and time of
                                  the file                                   */
char *lastArchiveDateAndTime;  /* Receives the last archived date and time
                                  of the file                                */
long *fileOwnerID;             /* Receives the unique bindery object ID of
                                  the owner of the file                      */
{
    char sendPacket[263], receivePacket[96];
    int ccode, len;

    sendPacket[2] = 15;
    /* Note that -1 equals IntSwap(-1) */
    if (sequenceNumber != (int *)NULL)
        *((int *)(sendPacket + 3)) = IntSwap(*sequenceNumber);
    else
        *((int *)(sendPacket + 3)) = -1;
    sendPacket[5] = driveHandle;
    sendPacket[6] = searchAttributes;
    len = strlen(filePath);
    sendPacket[7] = (BYTE)len;
    memmove(&sendPacket[8], filePath, len);

    *((int *)sendPacket) = len + 6;
    *((int *)receivePacket) = 94;

    ccode = _ShellRequest((BYTE)227, (BYTE *)sendPacket, (BYTE *)receivePacket);
    if (ccode)
       return(ccode);

    if (sequenceNumber != (int *)NULL)
        *sequenceNumber = IntSwap(*((int *)(receivePacket + 2)));
    if (fileName != (char *)NULL)
        strcpy(fileName, receivePacket + 4);
    if (fileAttributes != (BYTE *)NULL)
        *fileAttributes = receivePacket[18];
    if (extendedFileAttributes != (BYTE *)NULL)
        *extendedFileAttributes = receivePacket[19];
    if (fileSize != (long *)NULL)
        *fileSize = LongSwap(*((long *)(receivePacket + 20)));
    if (creationDate != (char *)NULL)
        memmove( creationDate, receivePacket + 24, 2 );
    if (lastAccessDate != (char *)NULL)
        memmove( lastAccessDate, receivePacket + 26, 2 );

    if (lastUpdateDateAndTime != (char *)NULL)
        memmove( lastUpdateDateAndTime, receivePacket + 28, 4 );
    if (fileOwnerID != (long *)NULL)
        *fileOwnerID = LongSwap(*((long *)(receivePacket + 32)));

    if (lastArchiveDateAndTime != (char *)NULL)
        memmove(lastArchiveDateAndTime, receivePacket + 36, 4);


    return (ccode);
}
